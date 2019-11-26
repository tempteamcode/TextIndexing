
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

app.get("/api/document/4259932", function( req, res ) {
  console.log("Call document api")
  exec("./build/TextIndexer", (error, stdout, stderr) => {
    if (error) {
      console.error(`exec error: ${error}`);
      return;
    }
    data = stdout;
    console.log(`stdout: ${stdout}`);
  });
  exec('./build/TextIndexer document_json 4259932').stdout.on('data', (data) => {
    console.log("iogfeoagfeaygfeaogy")
    console.log(`stdout: ${data}`);
    res.send( stdout )
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
