console.log("Riego JS script");
var segundos = 0;

// load tiempos de riego y espera to config form
loadConfig();

// JavaScritp loop
var intervalID = setInterval(function() {

    // TODO: evitar llamados al servidor cada segundo
    //get remain time of riego state and write it in titleRiego
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var response = JSON.parse(this.responseText);
            document.getElementById("titleRiego").innerHTML = `${response.riego_time_remain}`;
            toggleRiegoButton(response.riego_state);
        }
    };
    xhttp.open("GET", "/riegoTime", true);
    xhttp.send();

}, 1000);

// riegoButton set on/off in toggleRiego() sending ajax request
function toggleRiego() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var response = JSON.parse(this.responseText);
            toggleRiegoButton(response.riego_state);
        }
    };
    xhttp.open("GET", "/riegoToggle", true);
    xhttp.send();
}

// toggle riegoButton
function toggleRiegoButton(state) {
    if (state == "ON") {
        document.getElementById("riegoButton").innerHTML = "Apagar";
        document.getElementById("riegoButton").style.backgroundColor = "red";
    } else {
        document.getElementById("riegoButton").innerHTML = "Regar";
        document.getElementById("riegoButton").style.backgroundColor = "#0040ff";
    }
}

// save configuration times from
// horaEncendido, minEncendido, segEncendido,
// horaApagado, minApagado, segApagado
// send post request to /riegoConfig with ajax
function saveConfig() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var response = JSON.parse(this.responseText);
            console.log(response.riego_config);
            // write responde on messageSaveConfig
            document.getElementById("messageSaveConfig").innerHTML = response.riego_config;
        }
    };
    xhttp.open("POST", "/riegoConfig", true);
    xhttp.setRequestHeader("Content-type", "application/json");
    xhttp.send(JSON.stringify({
        "horaEncendido": document.getElementById("horaEncendido").value,
        "minEncendido": document.getElementById("minEncendido").value,
        "segEncendido": document.getElementById("segEncendido").value,
        "horaApagado": document.getElementById("horaApagado").value,
        "minApagado": document.getElementById("minApagado").value,
        "segApagado": document.getElementById("segApagado").value
    }));
}

function loadConfig() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var response = JSON.parse(this.responseText);
            console.log(response.riego_config);
            // response.riego_config is in format: Encendido: 00:00:00 Apagado: 00:00:00
            var config = response.riego_config.split(" ");
            var encendido = config[3].split(":");
            var apagado = config[5].split(":");
            document.getElementById("horaEncendido").value = encendido[0];
            document.getElementById("minEncendido").value = encendido[1];
            document.getElementById("segEncendido").value = encendido[2];
            document.getElementById("horaApagado").value = apagado[0];
            document.getElementById("minApagado").value = apagado[1];
            document.getElementById("segApagado").value = apagado[2];
        }
    };
    xhttp.open("GET", "/riegoConfig", true);
    xhttp.send();
}