
module.exports = function(patchText) {
  var patch = {
  };

  var stage = 'meta';
  var operatorIndex;

  var lines = patchText.split('\n');
  lines.forEach(function(line) {
    if (/^LFO/.test(line)) {
      patch.lfo = {};
      stage = 'lfo';
    }
    if (/^Oscillator Key Sync/.test(line)) {
      patch.oscillatorKeySync = /^Oscillator Key Sync: (.*)/.exec(line)[1];
    }
    if (/^Transpose/.test(line)) {
      patch.transpose = /^Transpose: (.*)/.exec(line)[1];
    }
    if (/^Pitch Envelope Generator/.test(line)) {
      stage = 'pitchEG';
      patch.pitchEG = {};
    }
    if (/^Operator: 1/.test(line)) {
      stage = 'operators';
      operatorIndex = 0;
      patch.operators = [];
    }
    if (/^Operator: 2/.test(line)) {
      stage = 'operators';
      operatorIndex = 1;
    }
    if (/^Operator: 3/.test(line)) {
      stage = 'operators';
      operatorIndex = 2;
    }
    if (/^Operator: 4/.test(line)) {
      stage = 'operators';
      operatorIndex = 3;
    }
    if (/^Operator: 5/.test(line)) {
      stage = 'operators';
      operatorIndex = 4;
    }
    if (/^Operator: 6/.test(line)) {
      stage = 'operators';
      operatorIndex = 5;
    }
    if (stage === 'meta') {
      if (/^Name/.test(line)) {
        patch.name = /^Name: (.*)/.exec(line)[1];
      }
      if (/^Algorithm/.test(line)) {
        patch.algorithm = parseInt(/^Algorithm: (.*)/.exec(line)[1]);
      }
      if (/^Feedback/.test(line)) {
        patch.feedback = parseInt(/^Feedback: (.*)/.exec(line)[1]);
      }
    } else if (stage === 'lfo') {
      if (/^  Wave/.test(line)) {
        patch.lfo.wave = /^  Wave: (.*)/.exec(line)[1].toLowerCase();
      }
      if (/^  Speed/.test(line)) {
        patch.lfo.speed = parseInt(/^  Speed: (.*)/.exec(line)[1]);
      }
      if (/^  Delay/.test(line)) {
        patch.lfo.delay = parseInt(/^  Delay: (.*)/.exec(line)[1]);
      }
      if (/^  Pitch Mod Depth/.test(line)) {
        patch.lfo.pitchModDepth = parseInt(/^  Pitch Mod Depth: (.*)/.exec(line)[1]);
      }
      if (/^  AM Depth/.test(line)) {
        patch.lfo.amDepth = /^  AM Depth: (.*)/.exec(line)[1].toLowerCase();
      }
      if (/^  Sync/.test(line)) {
        patch.lfo.sync = /^  Sync: (.*)/.exec(line)[1].toLowerCase();
      }
      if (/^  Pitch Modulation Sensitivity/.test(line)) {
        patch.lfo.pitchModSensitivity = parseInt(/^  Pitch Modulation Sensitivity: (.*)/.exec(line)[1]);
      }
    } else if (stage === 'pitchEG') {
      if (/^  Rate 1/.test(line)) {
        patch.pitchEG.rate1 = parseInt(/^  Rate 1: (.*)/.exec(line)[1]);
      }
      if (/^  Rate 2/.test(line)) {
        patch.pitchEG.rate2 = parseInt(/^  Rate 2: (.*)/.exec(line)[1]);
      }
      if (/^  Rate 3/.test(line)) {
        patch.pitchEG.rate3 = parseInt(/^  Rate 3: (.*)/.exec(line)[1]);
      }
      if (/^  Rate 4/.test(line)) {
        patch.pitchEG.rate4 = parseInt(/^  Rate 4: (.*)/.exec(line)[1]);
      }
      if (/^  Level 1/.test(line)) {
        patch.pitchEG.level1 = parseInt(/^  Level 1: (.*)/.exec(line)[1]);
      }
      if (/^  Level 2/.test(line)) {
        patch.pitchEG.level2 = parseInt(/^  Level 2: (.*)/.exec(line)[1]);
      }
      if (/^  Level 3/.test(line)) {
        patch.pitchEG.level3 = parseInt(/^  Level 3: (.*)/.exec(line)[1]);
      }
      if (/^  Level 4/.test(line)) {
        patch.pitchEG.level4 = parseInt(/^  Level 4: (.*)/.exec(line)[1]);
      }
    } else if (stage === 'operators') {
      if (!patch.operators[operatorIndex]) {
        patch.operators[operatorIndex] = {};
      }
      var op = patch.operators[operatorIndex];
      if (/^  AM Sensitivity/.test(line)) {
        op.amSensitivity = parseInt(/^  AM Sensitivity: (.*)/.exec(line)[1]);
      }
      if (/^  Oscillator Mode:/.test(line)) {
        op.oscillatorMode = /^  Oscillator Mode: (.*)/.exec(line)[1];
        if (/ratio/i.test(op.oscillatorMode)) {
          op.oscillatorMode = "ratio";
        }
        if (/fixed/i.test(op.oscillatorMode)) {
          op.oscillatorMode = "fixed";
        }
      }
      if (/^  Frequency/.test(line)) {
        op.frequency = parseInt(/^  Frequency: (.*)/.exec(line)[1]);
      }
      if (/^  Detune/.test(line)) {
        op.detune = parseInt(/^  Detune: (.*)/.exec(line)[1]);
      }
      if (/^    Rate 1/.test(line)) {
        op.eg = {};
        op.eg.rate1 = parseInt(/^    Rate 1: (.*)/.exec(line)[1]);
      }
      if (/^    Rate 2/.test(line)) {
        op.eg.rate2 = parseInt(/^    Rate 2: (.*)/.exec(line)[1]);
      }
      if (/^    Rate 3/.test(line)) {
        op.eg.rate3 = parseInt(/^    Rate 3: (.*)/.exec(line)[1]);
      }
      if (/^    Rate 4/.test(line)) {
        op.eg.rate4 = parseInt(/^    Rate 4: (.*)/.exec(line)[1]);
      }
      if (/^    Level 1/.test(line)) {
        op.eg.level1 = parseInt(/^    Level 1: (.*)/.exec(line)[1]);
      }
      if (/^    Level 2/.test(line)) {
        op.eg.level2 = parseInt(/^    Level 2: (.*)/.exec(line)[1]);
      }
      if (/^    Level 3/.test(line)) {
        op.eg.level3 = parseInt(/^    Level 3: (.*)/.exec(line)[1]);
      }
      if (/^    Level 3/.test(line)) {
        op.eg.level3 = parseInt(/^    Level 3: (.*)/.exec(line)[1]);
      }
      if (/^    Level 4/.test(line)) {
        op.eg.level4 = parseInt(/^    Level 4: (.*)/.exec(line)[1]);
      }
      if (/^  Keyboard Level Scaling/.test(line)) {
        op.keyboardLevelScaling = {};
      }
      if (/^    Breakpoint/.test(line)) {
        op.keyboardLevelScaling.breakpoint = /^    Breakpoint: (.*)/.exec(line)[1];
      }
      if (/^    Left Curve/.test(line)) {
        op.keyboardLevelScaling.leftCurve = /^    Left Curve: (.*)/.exec(line)[1];
      }
      if (/^    Right Curve/.test(line)) {
        op.keyboardLevelScaling.rightCurve = /^    Right Curve: (.*)/.exec(line)[1];
      }
      if (/^    Left Depth/.test(line)) {
        op.keyboardLevelScaling.leftDepth = parseInt(/^    Left Depth: (.*)/.exec(line)[1]);
      }
      if (/^    Right Depth/.test(line)) {
        op.keyboardLevelScaling.rightDepth = parseInt(/^    Right Depth: (.*)/.exec(line)[1]);
      }
      if (/^  Keyboard Rate Scaling/.test(line)) {
        op.keyboardRateScaling = parseInt(/^  Keyboard Rate Scaling: (.*)/.exec(line)[1]);
      }
      if (/^  Output Level/.test(line)) {
        op.outputLevel = parseInt(/^  Output Level: (.*)/.exec(line)[1]);
      }
      if (/^  Key Velocity Sensitivity/.test(line)) {
        op.keyVelocitySensitivity = parseInt(/^  Key Velocity Sensitivity: (.*)/.exec(line)[1]);
      }
    }
  });
  return patch;
};
