#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// for the server 
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>

// library for StaticJsonBuffer
#include <ArduinoJson.h>

// multi ssid for wifi connections
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#include "credentials.h"
/* "credentials.h" only contains the SSID and password of the WiFi network.
 * It is not included in the repository because it contains sensitive data.
 * If you want to use this code, you need to create your own "credentials.h"
 * file with your own SSID and password, copy and paste the contents of:

#ifndef STASSID_1
#define STASSID_1 "your-ssid-1"
#define STAPSK_1  "your-password-1"
#define STASSID_2 "your-ssid-2"
#define STAPSK_2  "your-password-2"
#endif
 */

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 25000;

// wifi Access Point
const byte DNS_PORT = 53;
const char *APssid = "Riego Huerta";        // The name of the Wi-Fi network that will be created
const char *APpassword = "TakeControl";     // The password required to connect to it, leave blank for an open network
IPAddress apIP(192, 168, 1, 1);

////////////////////// SERVER //////////////////////

DNSServer dnsServer;
ESP8266WebServer webServer(80);

File fsUploadFile; // a File variable to temporarily store the received file

/*__________________________________________________________HELPER_FUNCTIONS__________________________________________________________*/

String formatBytes(size_t bytes)
{ // convert sizes in bytes to KB and MB
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
}

String getContentType(String filename)
{
  if (webServer.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

String tiempoRestante(){
    String msg = "";
    if (get_minutos_restantes() == 0 && get_horas_restantes() == 0)      {
      msg = String(get_segundos_restantes()) + " segundos"; 
    }      else if (get_horas_restantes() == 0)      {
      msg = String(get_minutos_restantes()) + " minutos y " + String(get_segundos_restantes()) + " segundos";
    }      else      {
      msg = String(get_horas_restantes()) + " horas, " + String(get_minutos_restantes()) + " minutos y " + String(get_segundos_restantes()) + " segundos";
    }
    return msg;
}

/*__________________________________________________________SERVER_HANDLERS__________________________________________________________*/

// HTML GET
bool handleFileRead(String path)
{ 
  // send the right file to the client (if it exists)
  // Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
    path += "index.html";                    // If a folder is requested, send the index file
  String contentType = getContentType(path); // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
  {                                                        // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                       // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = webServer.streamFile(file, contentType); // Send it to the client
    file.close();                                          // Close the file again
    // Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path); // If the file doesn't exist, return false
  return false;
}

/*__________________________________________________________SETUP_FUNCTIONS__________________________________________________________*/

void SPIFFS_start()
{                 // Start the SPIFFS and list all contents
  SPIFFS.begin(); // Start the SPI Flash File System (SPIFFS)
  Serial.println("SPIFFS started. Contents:");
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    { // List the file system contents
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
}

void APMode_Start()
{
  delay(10);
  Serial.println('\n');

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(APssid, APpassword); // Start the access point
  Serial.print("Access Point \"");
  Serial.print(APssid);
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP()); // Send the IP address of the ESP8266 to the computer
  
  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  Serial.println("\" iniciado con captive portal");
  dnsServer.start(DNS_PORT, "*", apIP);
}

void server_start()
{
  //redirect all traffic to index.html
  webServer.onNotFound([]() {
    if (!handleFileRead(webServer.uri()))
    {
      const char *metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"0; url=http://192.168.1.1/index.html\" /></head><body><p>redirecting...</p></body>";
      webServer.send(200, "text/html", metaRefreshStr);
    }
  });
  // Manual ON / OFF riego control
  webServer.on("/riegoToggle", []() {
    if(riego_state){
      turn_off_riego();
      Serial.println("Riego apagado manualmente");
      webServer.send(200, "text/json", "{\"riego_state\":\"OFF\"}");
    }
    else{
      turn_on_riego();
      Serial.println("Riego encendido manualmente");
      webServer.send(200, "text/json", "{\"riego_state\":\"ON\"}");
    }
  });
  // send time remain riego state
  webServer.on("/riegoTime", []() {
    String msg = "Mensaje de riegoTime vacio";
    if (riego_state)
    {
      msg = String("Regando... tiempo restante: " + tiempoRestante());
      webServer.send(200, "text/json", "{\"riego_state\":\"ON\",\"riego_time_remain\":\"" + msg + "\"}");
    }
    else
    {
      msg = "Esperando para el proximo riego que comienza en: " + tiempoRestante();
      webServer.send(200, "text/json", "{\"riego_state\":\"OFF\",\"riego_time_remain\":\"" + msg + "\"}");
    }   
      Serial.println(msg);
  });

  // get config times from front-end in JSON format
  // post from front-end in JSON format
  // JSON format: "horaEncendido", "minEncendido", "segEncendido",
  // "horaApagado", "minApagado", "segApagado"
  webServer.on("/riegoConfig", []() {
    String msg = "Mensaje de riegoConfig vacio";
    if (webServer.method() == HTTP_POST)
    {
      // receibe JSON from front-end
      String json = webServer.arg("plain");
      Serial.println(json);
      StaticJsonDocument<200> jsonBuffer;
      deserializeJson(jsonBuffer, json);
      int horaEncendido = jsonBuffer["horaEncendido"];
      int minEncendido = jsonBuffer["minEncendido"];
      int segEncendido = jsonBuffer["segEncendido"];
      int horaApagado = jsonBuffer["horaApagado"];
      int minApagado = jsonBuffer["minApagado"];
      int segApagado = jsonBuffer["segApagado"];
      
      set_riego_config(horaEncendido, minEncendido, segEncendido, 
        horaApagado, minApagado, segApagado);
      msg = "Configuracion actualizada";
      Serial.println(msg);
      webServer.send(200, "text/json", "{\"riego_config\":\"" + msg + "\"}");
    }
    else
    {
      msg = "Configuracion actual: " + riego_config_str();
      Serial.println(msg);
      webServer.send(200, "text/json", "{\"riego_config\":\"" + msg + "\"}");
    }
  });


  webServer.begin();
  Serial.println("HTTP server started.");
}

void OTA_setup()
{
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("Riego-Huerta-esp8266");

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
                     {
                       String type;
                       if (ArduinoOTA.getCommand() == U_FLASH)
                       {
                         type = "sketch";
                       }
                       else
                       { // U_FS
                         type = "filesystem";
                       }

                       // NOTE: if updating FS this would be the place to unmount FS using FS.end()
                       Serial.println("Start updating " + type);
                     });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
                       Serial.printf("Error[%u]: ", error);
                       if (error == OTA_AUTH_ERROR)
                       {
                         Serial.println("Auth Failed");
                       }
                       else if (error == OTA_BEGIN_ERROR)
                       {
                         Serial.println("Begin Failed");
                       }
                       else if (error == OTA_CONNECT_ERROR)
                       {
                         Serial.println("Connect Failed");
                       }
                       else if (error == OTA_RECEIVE_ERROR)
                       {
                         Serial.println("Receive Failed");
                       }
                       else if (error == OTA_END_ERROR)
                       {
                         Serial.println("End Failed");
                       }
                     });
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/////////////////// WIFI REFRESH ///////////////////
void wifi_refresh()
{
  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED)
  {
    ArduinoOTA.handle();
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
}

/////////////////// WIFI SETUP ///////////////////
void wifi_setup()
{
  Serial.begin(115200);
  Serial.println("Booting, connecting to WiFi...");
  wifiMulti.addAP(STASSID_1, STAPSK_1);
  wifiMulti.addAP(STASSID_2, STAPSK_2);

  bool wifi_connected = true;
  while (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Starting AP MODE...");
    APMode_Start();
    wifi_connected = false;
  }

  if (wifi_connected)
  {
    Serial.print("WiFi connected: ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.localIP());
  }

  SPIFFS_start();
  server_start();
  OTA_setup();
}