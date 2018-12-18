var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);

var textbox = document.getElementById('txt');
connection.onopen = function () {
   // connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
    var message = e.data;
    //alert(typeof message);
    if(message.indexOf("!") == 0){
        alert(message.substring(1));
    }
    else if(message.indexOf("@") == 0){
        document.getElementById('textsToPrint').value = "";
    }
    else if(message.indexOf("+") == 0){
        if(document.getElementById('textsToPrint').value == ""){
            document.getElementById('textsToPrint').value += message.substring(1);
        }
        else{
            document.getElementById('textsToPrint').value += "\n" + message.substring(1);
        }
    }
};
connection.onclose = function(){
    console.log('WebSocket connection closed');
};

function sendText() {
    var text = document.getElementById('textToPrint').value;
    document.getElementById('textToPrint').value = "";
    connection.send(text);
}
function paperIn(){
    connection.send('#paperIn');
}
function paperOut() {
    connection.send('#paperOut');
}