var express = require('express');
var app = express();
var server = require('http').Server(app);
const io = require('socket.io')(server);

app.use(express.static('public'));

io.on('connection', function(socket){
    socket.on('colocar_id_del_mensaje',function(data){
        console.log(data);
        //io.sockets.emit('desde_servidor',data);
    });

    socket.on('DESDE_ESP32_TECLA',function(data){
        console.log(data);
        io.sockets.emit('DESDE_SERVER_TECLA',data);
    });

    socket.on('DESDE_ESP32_SENAL',function(data){
        console.log(data);
        io.sockets.emit('DESDE_SERVER_SENAL',data);
    });

    socket.on('desde_cliente',function(data){
        console.log(data);
        io.sockets.emit('DESDE_SERVER_COMANDO',data);
    });


});

server.listen(5001, function(){
    console.log("Servidor corriendo en el puerto 5001.")
});
