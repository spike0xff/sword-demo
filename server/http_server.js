var http = require('http');
var util = require('util');
var spawn = require('child_process').spawn;

http.createServer( function(req,res) {
   var currentTime = new Date();
   console.log('Client called at '+currentTime);

   // Handle POST...
   if (req.method == 'POST') {

      // Receive the POST data...
      var body = '';
      req.on('data', function(data) {
         body += data;
      });

      // Handle the response...
      req.on('end', function() {
         // Issue a command...
         var child = spawn('/home/pi/cdrv/sword-demo/driver/driver', ["html"]);
         var output = '';

         // Collect the data on standard in and error...
         child.stdout.on('data', function(data) {
            console.log('stdout: ' + data);
            output += data;
         });
         child.stderr.on('data', function(data) {
            console.log('stderr: ' + data);
         });

         // Write the response...
         child.stdout.on('end', function() {
            console.log('stdout end');
            res.writeHead(200, "OK", {'Content-Type':'text/html'});
            res.write('SWORD Scanning Server on nodejs\n');
            res.write('--------- post body:\n');
            res.write(body+'\n');
            res.write('--------- spawn stdout:\n');
            res.write(output);
            res.end();
         });
      });
   } else {
      res.writeHead(200, "OK", {'Content-Type':'text/plain'});
      res.write('use POST!');
      res.end();
   }

}).listen('80');

