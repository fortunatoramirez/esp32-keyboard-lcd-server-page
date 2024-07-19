var socket = io.connect();
var explodedValues = [1];


socket.on('DESDE_SERVER_TECLA', function(data){
    //console.log(data);
    //data_JSON = JSON.parse(data)
    var cadena = `<div> <div class="large-text"> <strong> Dato recibido:  <font color="green">` + data + `</strong> </div>`;
    document.getElementById("div_tecla").innerHTML = cadena;
    //explodedValues[2] = parseFloat(data);
    //drawVisualization();
});

socket.on('DESDE_SERVER_SENAL', function(data){
    //console.log(data);
    //data_JSON = JSON.parse(data)
    var cadena = `<div> <div class="large-text"> <strong> Sensor:  <font color="green">` + data + `</strong> </div>`;
    document.getElementById("div_sensor").innerHTML = cadena;
    explodedValues[0] = parseFloat(data);
    drawVisualization();
});

function encender()
{
    socket.emit("desde_cliente","ON");
}

function apagar()
{
    socket.emit("desde_cliente","OFF");
}

function enviar_comando()
{
    var comando = document.getElementById('txt_comando').value;
    socket.emit('desde_cliente',comando);

}


function drawVisualization() {
    // Create and populate the data table from the values received via websocket
    var data = google.visualization.arrayToDataTable([
        ['Tracker', '1',{ role: 'style' }],
        ['Sensor:',   explodedValues[0],'color: green']
    ]);
    
    // use a DataView to 0-out all the values in the data set for the initial draw
    var view = new google.visualization.DataView(data);
    view.setColumns([0, {
        type: 'number',
        label: data.getColumnLabel(1),
        calc: function () {return 0;}
    }]);
    
    // Create and draw the plot
    var chart = new google.visualization.BarChart(document.getElementById('div_grafica'));
    
    var options = {
        title:"Monitoreo",
        width: 1200,
        height: 300,
        bar: { groupWidth: "95%" },
        legend: { position: "none" },
        animation: {
            duration: 0
        },
        hAxis: {
            // set these values to make the initial animation smoother
            minValue: 0,
            maxValue: 4096
        }
    };
    
    var runOnce = google.visualization.events.addListener(chart, 'ready', function () {
        google.visualization.events.removeListener(runOnce);
        chart.draw(data, options);
    });
    
    chart.draw(view, options);
}

google.load('visualization', '1', {packages: ['corechart'], callback: drawVisualization});


