var parsePatch = require('./parsePatch');
var fs = require('fs');
var file = fs.readFileSync('./wurlie.txt', 'binary');
var exec = require('child_process').exec;
var slug = require('slug');

//console.log(parsePatch(file));

var banks = fs.readdirSync('./sysex');

try {
  fs.mkdirSync(__dirname + '/patches');
} catch(e) {
}

banks.forEach(function(bank) {
  exec('./dx7dump -l ./sysex/' + bank, function(err, stdout, stderr) {
    var name = bank.replace('.syx', '');
    if (true || name === 'analog1') {
      try {
        fs.mkdirSync(__dirname + '/patches/' + name);
      } catch(e) {
      }
      var bankPatches = stdout
        .split('-------------------------------------------------')
        .map(function(rawPatch) {
          return rawPatch.split('\n');
        })
        .filter(function(patchLines) {
          return patchLines.length > 10;
        })
        .forEach(function(patchLines) {
          var data = parsePatch(patchLines.join('\n'));
          var patchName = slug(data.name).replace(/(^-|-$)/g,'').toLowerCase();
          try {
            fs.writeFileSync(__dirname + '/patches/' + name + '/' + patchName + '.json', JSON.stringify(data, 0, 2));
          } catch(e) {
            console.log(e);
          }
        });
    }
  });
});
