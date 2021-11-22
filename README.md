# riego-huerta
Riego automático para una huerta saludable!

## Por hacer
<ol> 
    <li>Features
        <ul>
            <li><del>simple implementacion rapida de un riego de una hora y 11 horas de espera.
            Incia el riego al encender el equipo</del></li>
            <li><del>Eliminar funcion bloqueante delay() mediante uso de Ticker</del></li>
            <li><del>OTA conectado a wifi local</del></li>
            <li>Modo AP con web server captive portal</li>
            <li><a href="https://randomnerdtutorials.com/wifimanager-with-esp8266-autoconnect-custom-parameter-and-manage-your-ssid-and-password/">Wifi config</a></li>
            <li>Usar el RTC interno para generar eventos de encendido apagado</li>
            <li>Guardar registros en .csv en file system interno SPIFFs</li>
            <li>Descarga de datos. Tomar ejemplo ESP32-8266-File-Download</li>
            <a href="https://stackoverflow.com/questions/10659523/how-to-get-the-exact-local-time-of-client">Actualizar tiempo desde los clientes que se conectan</a>   </li>
        </ul>
    </li>
    <li>Mas funcionalidades
        <ul>
            <li><a href="https://randomnerdtutorials.com/esp32-esp8266-plot-chart-web-server/">Graficos</a></li>
        </ul>
    </li>
</ol>

Para editar o bajar archivos: File > Examples > ESP8266WebServer > FSBrowser
### Mas links
<a href="https://makersportal.com/blog/2020/5/26/capacitive-soil-moisture-calibration-with-arduino#:~:text=Capacitive%20soil%20moisture%20sensors%20exploit,find%20more%20specific%20values%20here%5D.">Mas detalles del sensor de humedad y su calibracion</a>

<p><a href="https://tttapa.github.io/ESP8266/Chap16%20-%20Data%20Logging.html">Tutorial completo de ESP8266</a>: web server con SPIFFs conectado a wifi con grafico google charts, entre otras funcionalidades</p>

<a href="https://diyprojects.io/esp8266-web-server-part-1-store-web-interface-spiffs-area-html-css-js/#.X5hU8Ij0lPY"> Tuto webserver para datalogger usando Bootstrap framework</a>


<a href="https://diyprojects.io/esp8266-web-server-fast-development-of-html-js-with-node-js-and-pug/#.X5hiEIj0lPY"> Tuto webserver para datalogger usando node.js</a>
