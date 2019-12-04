
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

  console.log( "Call document api, regnerate" )

  exec('./build/TextIndexer json index ' ).stdout.on('data', (data) => {
    console.log("Send query " + "")
    console.log(`stdout: ${data}`);
    res.send( data )
  });

});

app.get("/api/search", function( req, res ) {

  console.log("Call document api, search", req.query.words )
  let sentOnce = false;

  exec('./build/TextIndexer json query ' + req.query.words ).stdout.on('data', (data) => {
    console.log("Send query " + req.query.words )
    if( data[0] != "{" )
      data = '{"error":"no query"}'
    console.log(`stdout: ${data}`)
    sentOnce = true;
    res.send( data )
  });

});

app.get("/api/document/*", function( req, res ) {

  let urlSplit = req.url.split("/")
  console.log("Call document api, document")
  let documentId = urlSplit[urlSplit.length-1]
  let sentOnce = false;

  exec('./build/TextIndexer json document ' + documentId).stdout.on('data', (data) => {
    if( sentOnce ) return;
    console.log("Send document " + documentId)
    if( data[0] != "{" )
      data = '{"error":"no document"}'
    console.log(`stdout: ${data}`)
    sentOnce = true;
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
