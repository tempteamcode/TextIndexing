// var http = require('http')
//
//
let port = 1337
//
// http.createServer(function(request, response) {
//     response.writeHead(
//       200,
//       {
//         'Content-type': 'text/plain'
//       });
//       response.write("Hello world!")
//       response.end();
//       console.log( "Node js running on port " + port );
//
// }).listen(port);


let express = require("express")
let { exec } = require("child_process")

let app = express()

const viewerRoot = "/../viewer/dist"

app.get("/", function(req, res) {
  res.sendFile( viewerRoot + "/index.html" )
});

app.get("/status", function(req, res){
  res.send("ok")
});

function rebuild() {
  exec(`cp ${viewerRoot} ./viewer`)
}

rebuild();

app.listen(port, function(){
  `Server Text indexing (port ${port})`
})
