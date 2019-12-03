
let express                   = require("express")
const path                    = require('path');
const http                    = require('http');
const bodyParser              = require('body-parser');
// const api                     = require('./server/routes/api');
let { exec, execSync }        = require("child_process")


let port = 1337



let app = express()

const viewerRoot = "/viewer/"

// app.get("/", function(req, res) {
//   console.log(__dirname + viewerRoot + "index.html" )
//   res.sendFile( __dirname + viewerRoot + "index.html" )
// });

app.use(express.static(path.join(__dirname, 'dist')));
// app.use('/api', api);

app.get("/api/regenerate", function( req, res ) {

  console.log( "Call document api" )

  exec('./build/TextIndexer index ' ).stdout.on('data', (data) => {
    console.log("Send query " + "")
    console.log(`stdout: ${data}`);
    res.send( data )
  });

});

app.get("/api/search", function( req, res ) {

  console.log("Call document api", req.query.words )

  exec('./build/TextIndexer query ' + req.query.words ).stdout.on('data', (data) => {
    console.log("Send query " + "")
    console.log(`stdout: ${data}`);
    res.send( data )
  });

});

app.get("/api/document/*", function( req, res ) {
  let urlSplit = req.url.split("/")
  console.log("Call document api")
  let documentId = urlSplit[urlSplit.length-1]
  exec('./build/TextIndexer document_json ' + documentId).stdout.on('data', (data) => {
    console.log("Send document " + documentId)
    console.log(`stdout: ${data}`);
    res.send( data )
  });
})

app.get("/status", function(req, res){
  res.send("ok")
});

app.get('*', (req, res) => {
  res.sendFile(path.join(__dirname, 'dist/index.html'));
});

app.listen(port, function(){
  `Server Text indexing (port ${port})`
})

console.log(`Server Text indexing (port ${port})`)
