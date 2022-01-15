console.log("Riego JS script");

var segundos = 0;

// JavaScritp loop
var intervalID = setInterval(function() {

    segundos++;
/*     var now = new Date();
    console.log(now.getFullYear());
    console.log(now.getMonth());
    console.log(now.getDate());
    console.log(now.getDay());
    console.log(now.getHours());
    console.log(now.getMinutes());
    console.log(now.getSeconds());
    console.log(now.getTime()); */

    // document.getElementById("lastUpdate").innerHTML = `Segundos: ${segundos} s`;

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
