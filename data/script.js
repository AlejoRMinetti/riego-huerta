console.log("Riego JS script");

var segundos = 0;

// JavaScritp loop
var intervalID = setInterval(function() {
    // communication over local file
    // TODO: communication AJAX with server

    segundos++;
    var now = new Date();
    console.log(now.getFullYear());
    console.log(now.getMonth());
    console.log(now.getDate());
    console.log(now.getDay());
    console.log(now.getHours());
    console.log(now.getMinutes());
    console.log(now.getSeconds());
    console.log(now.getTime());

    document.getElementById("lastUpdate").innerHTML = `Segundos: ${segundos} s`;

}, 1000);