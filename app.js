'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

// Import library
var svg = ffi.Library('./libsvgparser.so', {
  'createSVGlogs': ['string', ['string']],
  'fillSVGViewPanel': ['string', ['string']],
  'imgTitle': ['string', ['string']],
  'imgDescription': ['string', ['string']],
  'editTitleDesc': ['void', ['string', 'string', 'string']],
  'createNewSVGfromInput': ['int', ['string', 'string']]
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

//File Log Panel
app.get('/updateFileLogs', function(req, res) {
  let svgServer = [];
  let error = 0;
  fs.readdir('uploads/', (err, files) => {
    files.forEach(file => {
      let fileName = ('uploads/' + file);
      // use fs.stat to get info such as file size
      let fileInfo = fs.statSync(fileName);
      // console.log(fileName);
      let fileStr = svg.createSVGlogs(fileName);
      let fileJSON = JSON.parse(fileStr);
      if (fileJSON != null) {
        let fileJSON = JSON.parse(fileStr);
        var fileData = {
          name: file,
          fileSize: Math.round(fileInfo.size / 1000),
          numRect: fileJSON.numRect,
          numCirc: fileJSON.numCirc,
          numPath: fileJSON.numPaths,
          numGroup: fileJSON.numGroups
        }
        svgServer.push(fileData);
      } else {
        error = 0;
      }
    });
    if (error == 0) {
      res.send({
        files: svgServer,
        message: "success"
      });
    } else {
      console.log("Error occurred while loading Log Panel Files");
          res.send({
              message: "error",
              files: svgServer
          });
      }
  });

  app.get('/updateSVG/:name', function(req , res){
    let file = req.params.name;
    // console.log("filename is: " + file);

    let json = svg.fillSVGViewPanel('uploads/'+file);
    var title = svg.imgTitle('uploads/'+file);
    var description = svg.imgDescription('uploads/'+file);

    let svgPanel = {
      jsonSVG: json,
      title: title,
      desc: description
    }
    res.send({
      file: svgPanel,
      message: "success"
    });
  });
  
  // Edit title and description
  app.get('/editTD/', function(req , res){
    console.log("Edit Title/Description Received:");
  
  
    let editTitleDesc = svg.editTitleDesc(req.query.file, req.query.title, req.query.description);
  
    res.send("Successfully Added");
  });

  // Create an SVG image
  app.get('/createSVGimg', function(req , res){
    try{
      let file = req.query.fileName;
      let title = req.query.title;
      let description = req.query.description;

      // console.log("\n\n File Name received is: " + file + "\n\n");
      // console.log("\n\n Title = " + title + " and description = " + description);

      let val = svg.createNewSVGfromInput(file, title, description);
      console.log(val);

      if (val == 1) {
        res.send({
          message: "success"
        });
      }
      // console.log("\n\n" + newSVG +"yoooo"+ "\n\n");
    } catch(err) {
      res.send("Could not successfully create SVG");
    }
  });
});


