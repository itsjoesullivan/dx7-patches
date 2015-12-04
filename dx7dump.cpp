// Yamaha DX7 Sysex Dump
// Copyright 2012, Ted Felix
// License: GPLv3+

// Takes a Yamaha DX7 sysex file and formats it as human readable text.
// The format is also conducive to using diff (or meld) to examine differences
// between patches.

// Based on info from:
// http://homepages.abdn.ac.uk/mth192/pages/dx7/sysex-format.txt

// Build:
//   g++ -o dx7dump dx7dump.cpp

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

// ***************************************************************************

const char *version = "1.00";

// "-l" for long listing
bool longListing = false;

// "-f" to find duplicate patches
bool findDupes = false;

// "-p" to specify patch.  -1 means all.
int patch = -1;

// ***************************************************************************

struct OperatorPacked
{
    unsigned char EG_R1;
    unsigned char EG_R2;
    unsigned char EG_R3;
    unsigned char EG_R4;
    unsigned char EG_L1;
    unsigned char EG_L2;
    unsigned char EG_L3;
    unsigned char EG_L4;
    unsigned char levelScalingBreakPoint;
    unsigned char scaleLeftDepth;
    unsigned char scaleRightDepth;

    unsigned char scaleLeftCurve : 2;  // VERIFIED
    unsigned char scaleRightCurve : 2;  // VERIFIED
    unsigned char : 4;

    unsigned char rateScale : 3;  // VERIFIED
    unsigned char detune : 4;  // VERIFIED
    unsigned : 1;

    unsigned char amplitudeModulationSensitivity : 2;  // VERIFIED
    unsigned char keyVelocitySensitivity : 3;  // VERIFIED
    unsigned : 3;

    unsigned char outputLevel;

    unsigned char oscillatorMode : 1;  // VERIFIED
    unsigned char frequencyCoarse : 5;  // VERIFIED
    unsigned : 2;

    unsigned char frequencyFine;
};

// Yamaha DX7 Voice Bank "Bulk Dump Packed Format"
// See section F of sysex-format.txt for details.
struct VoicePacked
{
    OperatorPacked op[6];

    unsigned char pitchEGR1;
    unsigned char pitchEGR2;
    unsigned char pitchEGR3;
    unsigned char pitchEGR4;
    unsigned char pitchEGL1;
    unsigned char pitchEGL2;
    unsigned char pitchEGL3;
    unsigned char pitchEGL4;

    unsigned char algorithm : 5;  // VERIFIED
    unsigned : 3;

    unsigned char feedback : 3;  // VERIFIED
    unsigned char oscKeySync : 1;  // VERIFIED
    unsigned : 4;

    unsigned char lfoSpeed;
    unsigned char lfoDelay;
    unsigned char lfoPitchModDepth;
    unsigned char lfoAMDepth;

    unsigned char lfoSync : 1;  // VERIFIED
    unsigned char lfoWave : 3;  // Need to see S&H
    unsigned char lfoPitchModSensitivity : 4;  // VERIFIED

    unsigned char transpose;
    unsigned char name[10];
};

struct DX7Sysex
{
    unsigned char sysexBeginF0;  // 0xF0
    unsigned char yamaha43;  // 0x43
    unsigned char subStatusAndChannel;  // 0
    unsigned char format9;  // 9
    unsigned char sizeMSB;  // 7 bits!  0x20
    unsigned char sizeLSB;  // 7 bits!  0x00

    VoicePacked voices[32];

    unsigned char checksum;
    unsigned char sysexEndF7;  // 0xF7
};

// ***************************************************************************

const char *OnOff(unsigned x)
{
    if (x > 1)
        return "*out of range*";
        
    const char *onOff[] = { "Off", "On" };
    return onOff[x];
}

const char *Curve(unsigned x)
{
    if (x > 3)
        return "*out of range*";

    const char *curves[] = { "-LIN", "-EXP", "+EXP", "+LIN" };

    return curves[x];
}

const char *LFOWave(unsigned x)
{
    if (x > 5)
        return "*out of range*";

    const char *waves[] = { "Triangle", "Sawtooth Down", "Sawtooth Up",
                            "Square", "Sine", "Sample and Hold" };

    return waves[x];
}

const char *Mode(unsigned x)
{
    if (x > 1)
        return "*out of range*";

    const char *modes[] = { "Frequency (Ratio)", "Fixed Frequency (Hz)" };
    return modes[x];
}

const char *Note(unsigned x)
{
    x = x % 12;
    const char *notes[] = 
        { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    return notes[x];
}

std::string Transpose(unsigned x)
{
    if (x > 48)
        return "*out of range*";

    // Anything's better than sstream!
    char buffer[10];
    snprintf(buffer, 9, "%s%u", Note(x), x / 12 + 1);

    return buffer;
}

std::string Breakpoint(unsigned x)
{
    if (x > 99)
        return "*out of range*";
        
    char buffer[10];

    // Playing some tricks (add 12 tones, subtract one octave) to avoid
    // the fact that -3/12 rounds to 0 and messes everything up.  Shifting
    // everything up an octave and then subtracting one solves the problem.
    const int octave = ((int)x - 3 + 12) / 12 - 1;
    
    // Anything's better than sstream!
    snprintf(buffer, 9, "%s%d", Note(x + 9), octave);

    return buffer;
}

// ***************************************************************************

int processOpts(int *argc, char ***argv)
{
	struct option opts[] = {
		{ "long", 0, 0, 'l' },
		{ "find-dupes", 0, 0, 'f' },
		{ "patch", 1, 0, 'p' },
		{ "version", 0, 0, 'v' },
		{ "help", 0, 0, 'h' },
		{ NULL, 0, 0, 0 },
	};
	
	for (;;) {
		int i = getopt_long(*argc, *argv, "lfp:vh", opts, NULL);
		if (i == -1)
			break;
			
		switch (i) {
		case 'l':
			longListing = true;
			break;
		case 'f':
			findDupes = true;
			break;
		case 'p':
			patch = strtol(optarg, NULL, 0);
			longListing = true;
			break;
		case 'v':
			printf("dx7dump %s\n", version);
            printf("Yamaha DX7 Sysex Dump\n");
            printf("Copyright 2012, Ted Felix (GPLv3+)\n");
			exit(0);
		case 'h':
			printf("Usage: dx7dump [OPTIONS] filename\n");
            printf("\n");
			printf("Options: -l    long listing\n");
			printf("         -p x  display patch x\n");
			printf("         -v    version info\n");
			printf("         -h    this help\n");
			exit(0);
		default:
		    printf("Unexpected option.  Try -h for help.\n");
		    exit(1);
			break;
		}
	}

    // Bump to the end of the options.
	*argc -= optind;
	*argv += optind;

	return 0;
}

// ***************************************************************************
// Returns 0 if ok.
int Verify(const DX7Sysex *sysex)
{
    // *** Verify Header and Footer ***
    
    if (sysex->sysexBeginF0 != 0xF0)
    {
        printf("Did not find sysex start F0\n");
        return 1;
    }
    if (sysex->yamaha43 != 0x43)
    {
        printf("Did not find Yamaha 0x43\n");
        return 1;
    }
    if (sysex->subStatusAndChannel != 0)
    {
        printf("Did not find substatus 0 and channel 1\n");
        return 1;
    }
    if (sysex->format9 != 0x09)
    {
        printf("Did not find format 9 (32 voices)\n");
        return 1;
    }
    if (sysex->sizeMSB != 0x20  ||  sysex->sizeLSB != 0)
    {
        printf("Did not find size 4096\n");
        return 1;
    }
    if (sysex->sysexEndF7 != 0xF7)
    {
        printf("Did not find sysex end F7\n");
        return 1;
    }


    // **** checksum ****
    // Start of 4096 byte data block.
    const unsigned char *p = (unsigned char *)sysex->voices;
    unsigned char sum = 0;
    for (unsigned i = 0; i < 4096; ++i)
    {
        sum += (p[i] & 0x7F);
    }
    // Two's complement: Flip the bits and add 1
    sum = (~sum) + 1;
    // Mask to 7 bits
    sum &= 0x7F;
    if (sum != sysex->checksum)
    {
        printf("CHECKSUM FAILED: Should have been 0x%X\n", sum);
        return 1;
    }
    
    return 0;
}

// ***************************************************************************

void Format(const DX7Sysex *sysex, const char *filename)
{
    // For each voice.
    for (unsigned voiceNum = 0; voiceNum < 32; ++voiceNum)
    {
        const VoicePacked *voice = &(sysex->voices[voiceNum]);

        char name[11];
        memcpy(name, voice->name, 10);
        name[10] = 0;

        if (!longListing)
        {
            printf("%2d %s\n", voiceNum + 1, name);
            continue;
        }

        // Long Listing

        printf("Filename: %s\n", filename);
        printf("Voice #: %d\n", voiceNum + 1);
        printf("Name: %s\n", name);
        printf("Algorithm: %u\n", voice->algorithm + 1);
        printf("Feedback: %u\n", voice->feedback);

        printf("LFO\n");
        printf("  Wave: %s\n", LFOWave(voice->lfoWave));
        printf("  Speed: %u\n", voice->lfoSpeed);
        printf("  Delay: %u\n", voice->lfoDelay);
        printf("  Pitch Mod Depth: %u\n", voice->lfoPitchModDepth);
        printf("  AM Depth: %u\n", voice->lfoAMDepth);
        printf("  Sync: %s\n", OnOff(voice->lfoSync));  
        printf("  Pitch Modulation Sensitivity: %u\n", 
               voice->lfoPitchModSensitivity);

        printf("Oscillator Key Sync: %s\n", OnOff(voice->oscKeySync));

        printf("Pitch Envelope Generator\n");
        printf("  Rate 1: %u\n", voice->pitchEGR1);
        printf("  Rate 2: %u\n", voice->pitchEGR2);
        printf("  Rate 3: %u\n", voice->pitchEGR3);
        printf("  Rate 4: %u\n", voice->pitchEGR4);
        printf("  Level 1: %u\n", voice->pitchEGL1);
        printf("  Level 2: %u\n", voice->pitchEGL2);
        printf("  Level 3: %u\n", voice->pitchEGL3);
        printf("  Level 4: %u\n", voice->pitchEGL4);

        printf("Transpose: %s\n", Transpose(voice->transpose).c_str());

        // For each operator
        for (unsigned i = 0; i < 6; ++i)
        {
            printf("\n");
            printf("Operator: %u\n", i + 1);
            
            // They're stored in backward order.
            const unsigned j = 5 - i;
            const OperatorPacked &op = voice->op[j];
            
            printf("  AM Sensitivity: %u\n", 
                   op.amplitudeModulationSensitivity);
            printf("  Oscillator Mode: %s\n", Mode(op.oscillatorMode));
            // If ratio mode
            if (op.oscillatorMode == 0)
            {
                double coarse = op.frequencyCoarse;
                if (coarse == 0)
                    coarse = .5;

                const double freq = coarse + 
                    ((double)op.frequencyFine * coarse / 100);
                
                printf("  Frequency: %g\n", freq);
            }
            else  // fixed mode
            {
                const double power = (double)(op.frequencyCoarse % 4) +
                                     (double)op.frequencyFine / 100;
                const double f = pow(10, power);
                printf("  Frequency: %gHz\n", f);
            }
            printf("  Detune: %d\n", op.detune - 7);
            printf("  Envelope Generator\n");
            printf("    Rate 1: %u\n", op.EG_R1);
            printf("    Rate 2: %u\n", op.EG_R2);
            printf("    Rate 3: %u\n", op.EG_R3);
            printf("    Rate 4: %u\n", op.EG_R4);
            printf("    Level 1: %u\n", op.EG_L1);
            printf("    Level 2: %u\n", op.EG_L2);
            printf("    Level 3: %u\n", op.EG_L3);
            printf("    Level 4: %u\n", op.EG_L4);
            printf("  Keyboard Level Scaling\n");
            printf("    Breakpoint: %s\n", 
                   Breakpoint(op.levelScalingBreakPoint).c_str());
            printf("    Left Curve: %s\n", Curve(op.scaleLeftCurve));
            printf("    Right Curve: %s\n", Curve(op.scaleRightCurve));
            printf("    Left Depth: %u\n", op.scaleLeftDepth);
            printf("    Right Depth: %u\n", op.scaleRightDepth);
            printf("  Keyboard Rate Scaling: %u\n", op.rateScale);
            printf("  Output Level: %u\n", op.outputLevel);
            printf("  Key Velocity Sensitivity: %u\n", 
                   op.keyVelocitySensitivity);
        }
        printf("-------------------------------------------------\n");
    }
}

// ***************************************************************************

int FindDupes(const DX7Sysex *sysex)
{
    // For each patch
    for (int i = 0; i < 31; ++i)
    {
        // For each patch after that patch
        for (int j = i + 1; j < 32; ++j)
        {
            // memcmp the patches
            // Subtract 10 to remove the name from the diff.
            int rc = memcmp(&(sysex->voices[i]), &(sysex->voices[j]), 
                            sizeof(VoicePacked) - 10);
            if (rc == 0)
                printf("Found dupe: %d and %d\n", i+1, j+1);
        }
    }
}

// ***************************************************************************

int main(int argc, char *argv[])
{
    processOpts(&argc, &argv);

    if (argc == 0)
    {
        printf("Expecting a filename.\n");
        return 1;
    }

    FILE *file = fopen(argv[0], "r");
    if (file == NULL)
    {
        printf("Can't open the file: %s\n", strerror(errno));
        return 1;
    }

    const unsigned sysexSize = 4104;
    unsigned char buffer[sysexSize];
    const size_t size = fread(buffer, sysexSize, 1, file);
    fclose(file);

    if (size != 1)
    {
        printf("Not enough bytes\n");
        return 1;
    }

    const DX7Sysex *sysex = (DX7Sysex *)buffer;

    // Make sure this is a valid DX7 sysex dump.
    if (Verify(sysex) != 0)
        return 1;

    // Format the bank
    Format(sysex, argv[0]);

    if (findDupes)
        FindDupes(sysex);

    return 0;
}

