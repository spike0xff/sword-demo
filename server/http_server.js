var http = require('http');
var url = require('url');
var querystring = require('querystring');
var util = require('util');
var fs = require('fs');
var path = require('path');
var spawn = require('child_process').spawn;

http.createServer( function(req,res) {
   // parse the URL into protocol, host, port, pathname, etc.
   var request = url.parse(req.url, true);

   console.log('cwd:' + process.cwd());
   console.log('parsed request:');
   console.log(request);

   if (req.method == 'GET' && request.pathname == '/image.tif') {
      // send the file
      var filepath = path.join(__dirname, request.pathname);
      console.log('serving file: '+filepath);
      var file = fs.readFileSync(filepath);
      var stat = fs.statSync(filepath);
      res.writeHead(200, "OK", {
         'Content-Type':'image/tiff',
         'Content-Length': stat.size
      });
      res.end(file, 'binary');
   } else if (req.method == 'POST' || req.method == 'GET') {
      // Handle POST...
      // Collect the (possible) POST data...
      var body = '';
      req.on('data', function(data) {
         body += data;
      });

      // When the POST or GET has been completely received
      req.on('end', function() {
         // parse the query data
         req.post = querystring.parse(body);
         console.log('parsed query data:');
         console.log(req.post);
         // Spawn the scanner driver.
         // assumes this .js file is in the sword-demo/server directory
         var args =
            ["pixeltype="+req.post.pixelType, "resolution="+req.post.resolution, "html"];
         console.log('driver args:');
         console.log(args);
         var child = spawn(path.join(__dirname, '../driver/driver'),args);
         var output = '';

         // Collect the data on standard in and error...
         child.stdout.on('data', function(data) {
            console.log('stdout: ' + data);
            output += data;
         });
         child.stderr.on('data', function(data) {
            console.log('stderr: ' + data);
            output += 'STDERR: ' + data;
         });

         // When the spawned program closes stdout...
         child.stdout.on('end', function() {
            // take collected output and write the response
            console.log('stdout end');
            res.writeHead(200, "OK", {'Content-Type':'text/html'});
            res.write('SWORD Scanning Server on nodejs\n');
            res.write('<a href="image.tif">scanned TIFF</a>\n');
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

