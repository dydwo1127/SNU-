var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);

var textbox = document.getElementById('txt');
connection.onopen = function () {
    connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
    textbox.value = '' + e;
};
connection.onclose = function(){
    console.log('WebSocket connection closed');
};

function sendText() {
    var text = document.getElementById('textToPrint').value;
    document.getElementById('textToPrint').value = "";
    connection.send(text);
}

function paperOut() {
    connection.send('#paperOut');
}