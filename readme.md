## dx7-patches

JSON representation of a collection of Yamaha DX7 patches compiled by Glenn Scott

For the original readme: ftp://ftp.ucsd.edu/midi/patches/DX7/README.1ST

## History

These files have been around for a while. They're currently hosted on a [ucsd website](ftp://ftp.ucsd.edu/midi/patches/DX7/). However, I retrieved them from [Dave Benson's website](https://homepages.abdn.ac.uk/mth192/pages/html/dx7.html#patches). [Ted Felix's dx7 utility](http://tedfelix.com/yamaha-dx7/index.html) then helped convert the sysex information into something I could read, which I then parsed with a file included in a different branch of this repository.

## Organization

The original files came in banks of 32 patches each. Each top-level directory here represents one bank, containing the bank's 32 patches as json files.

## Usage

`npm install --save dx7-patches`

```javascript
var ePianoPatch = require('dx7-patches/rhodes1/epiano-1');
var FictionalDX7 = require('fictionary-dx7');

var synth = new FictionaryDX7();
synth.fictionalSetPathMethod(ePianoPatch);

midi.connect(synth); // fictional?
```

## Format

I imagine the formatting, as well as the available patches, will change over time. But below is an example patch:

```json
{
  "name": "E.PIANO 1 ",
  "algorithm": 5,
  "feedback": 6,
  "lfo": {
    "wave": "sine",
    "speed": 15,
    "delay": 33,
    "pitchModDepth": 0,
    "amDepth": "0",
    "sync": "off",
    "pitchModSensitivity": 2
  },
  "oscillatorKeySync": "Off",
  "pitchEG": {
    "rate1": 94,
    "rate2": 67,
    "rate3": 95,
    "rate4": 60,
    "level1": 50,
    "level2": 50,
    "level3": 50,
    "level4": 50
  },
  "transpose": "C3",
  "operators": [
    {
      "amSensitivity": 0,
      "oscillatorMode": "ratio",
      "frequency": 1,
      "detune": 3,
      "eg": {
        "rate1": 96,
        "rate2": 25,
        "rate3": 25,
        "rate4": 67,
        "level1": 99,
        "level2": 75,
        "level3": 0,
        "level4": 0
      },
      "keyboardLevelScaling": {
        "breakpoint": "A-1",
        "leftCurve": "-LIN",
        "rightCurve": "-LIN",
        "leftDepth": 0,
        "rightDepth": 0
      },
      "keyboardRateScaling": 3,
      "outputLevel": 99,
      "keyVelocitySensitivity": 2
    },
    {
      "amSensitivity": 0,
      "oscillatorMode": "ratio",
      "frequency": 14,
      "detune": 0,
      "eg": {
        "rate1": 95,
        "rate2": 50,
        "rate3": 35,
        "rate4": 78,
        "level1": 99,
        "level2": 75,
        "level3": 0,
        "level4": 0
      },
      "keyboardLevelScaling": {
        "breakpoint": "A-1",
        "leftCurve": "-LIN",
        "rightCurve": "-LIN",
        "leftDepth": 0,
        "rightDepth": 0
      },
      "keyboardRateScaling": 3,
      "outputLevel": 58,
      "keyVelocitySensitivity": 7
    },
    {
      "amSensitivity": 0,
      "oscillatorMode": "ratio",
      "frequency": 1,
      "detune": 0,
      "eg": {
        "rate1": 95,
        "rate2": 20,
        "rate3": 20,
        "rate4": 50,
        "level1": 99,
        "level2": 95,
        "level3": 0,
        "level4": 0
      },
      "keyboardLevelScaling": {
        "breakpoint": "A-1",
        "leftCurve": "-LIN",
        "rightCurve": "-LIN",
        "leftDepth": 0,
        "rightDepth": 0
      },
      "keyboardRateScaling": 3,
      "outputLevel": 99,
      "keyVelocitySensitivity": 2
    },
    {
      "amSensitivity": 0,
      "oscillatorMode": "ratio",
      "frequency": 1,
      "detune": 0,
      "eg": {
        "rate1": 95,
        "rate2": 29,
        "rate3": 20,
        "rate4": 50,
        "level1": 99,
        "level2": 95,
        "level3": 0,
        "level4": 0
      },
      "keyboardLevelScaling": {
        "breakpoint": "A-1",
        "leftCurve": "-LIN",
        "rightCurve": "-LIN",
        "leftDepth": 0,
        "rightDepth": 0
      },
      "keyboardRateScaling": 3,
      "outputLevel": 89,
      "keyVelocitySensitivity": 6
    },
    {
      "amSensitivity": 0,
      "oscillatorMode": "ratio",
      "frequency": 1,
      "detune": -7,
      "eg": {
        "rate1": 95,
        "rate2": 20,
        "rate3": 20,
        "rate4": 50,
        "level1": 99,
        "level2": 95,
        "level3": 0,
        "level4": 0
      },
      "keyboardLevelScaling": {
        "breakpoint": "A-1",
        "leftCurve": "-LIN",
        "rightCurve": "-LIN",
        "leftDepth": 0,
        "rightDepth": 0
      },
      "keyboardRateScaling": 3,
      "outputLevel": 99,
      "keyVelocitySensitivity": 0
    },
    {
      "amSensitivity": 0,
      "oscillatorMode": "ratio",
      "frequency": 1,
      "detune": 7,
      "eg": {
        "rate1": 95,
        "rate2": 29,
        "rate3": 20,
        "rate4": 50,
        "level1": 99,
        "level2": 95,
        "level3": 0,
        "level4": 0
      },
      "keyboardLevelScaling": {
        "breakpoint": "D3",
        "leftCurve": "-LIN",
        "rightCurve": "-LIN",
        "leftDepth": 0,
        "rightDepth": 19
      },
      "keyboardRateScaling": 3,
      "outputLevel": 79,
      "keyVelocitySensitivity": 6
    }
  ]
}
```
