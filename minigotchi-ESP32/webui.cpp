/**
 * webui.cpp: handles creation of network and webUI
 */

#include "webui.h"

bool WebUI::running = false;

// create instances of functions here
DNSServer dnsServer;
AsyncWebServer server(80);

/**
 * Gets first instance of mood class
 */
Mood &WebUI::mood = Mood::getInstance();

// oh the memories this brings, not very good.
const char WebUI::html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Mingotchi Captive Portal</title>
  <style>
      p, b {
          font-family: Arial, Helvetica, sans-serif;
          color: white;
      }
      h1 {
          font-family: Arial, Helvetica, sans-serif;
      }
      html {
          background-color: #C0C0C0;
      }
      footer {
          text-align: center;
          font-size: 10px;
          font-family: Arial, Helvetica, sans-serif;
          margin: 7px;
      }
      .big {
          background-color: #363636;
          border-radius: 16px;
          width: 65%;
          padding: 80px;
          margin: 0 auto;
      }
      .textbox {
          display: block;
          margin: 20px auto;
          padding: 10px;
          width: 60%;
          color: #fff;
          text-align: center;
          border-radius: 16px;
      }
      .title {
          color: #fff;
          padding: 10px;
          text-align: center;
          margin: 0px auto;
          border-radius: 16px;
          width: 60%
      }
      .log-container {
          width: 60%;
          height: 500px;
          border-radius: 16px;
          margin: 20px auto;
          padding: 10px;
          border-color: white;
          border-width: 5px;
          border-style: solid;
          overflow:auto
      }
  </style>
</head>
<body>
  <div class="big">
    <h1 class="title">Welcome to the Captive Portal for the Minigotchi!</h1>
    <div class="textbox">
      <p>This must be done on the first boot to ensure that the Minigotchi runs correctly!</p>
    </div>
  </div>
  <footer>Made by <a href="https://github.com/dj1ch">@dj1ch</a></footer>
</body>
</html>
)rawliteral";

// captive portal class, this isn't the main class though
class CaptivePortalHandler : public AsyncWebHandler {
public:
  CaptivePortalHandler() {}
  virtual ~CaptivePortalHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    return request->url() == "/";
  }

  void handleRequest(AsyncWebServerRequest *request) {
    if (request->method() == HTTP_GET && request->url() == "/") {
      request->send(200, "text/html", WebUI::html);
    } else {
      request->send(200, "text/html", WebUI::html);
    }
  }
};

/**
 * Starts and constructs Web Server
 */
WebUI::WebUI() {
  if(Minigotchi::firstBoot) {
    Serial.println(mood.getIntense() + " Starting Web Server...");
    Display::updateDisplay(mood.getIntense(), "Starting Web Server...");

    dnsServer.start(53, "*", WiFi.softAPIP());

    WiFi.mode(WIFI_AP);
    WiFi.softAP(Config::ssid);

    setupServer();

    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.setTTL(300);
    dnsServer.start(53, "*", WiFi.softAPIP());

    server.begin();
    WebUI::running = true;

    while(running) {
      dnsServer.processNextRequest();
    }
  } else {
  }
}

WebUI::~WebUI() {
  if (running) {
    dnsServer.stop();
    server.end();
    WiFi.softAPdisconnect(true);
    
    running = false;
  }
}


/**
 * Sets up Web Server
 */
void WebUI::setupServer() {
  server.addHandler(new CaptivePortalHandler()).setFilter(ON_AP_FILTER);

  server.onNotFound([&](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });
}
