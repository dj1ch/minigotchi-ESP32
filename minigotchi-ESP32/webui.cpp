/*
 * Minigotchi: An even smaller Pwnagotchi
 * Copyright (C) 2024 dj1ch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
const char WebUI::html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Mingotchi Captive Portal</title>
  <style>
      p, b {
          font-family: Arial, Helvetica, sans-serif;
          color: white;
      }
      h1, h2 {
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
    <div class="textbox">
        <h2>Config.cpp configuration</h2>
        <p>Edit the following values here online then set <i>Config::configured</i> to <i>true</i></p>
        <p></p>
        <p>For now, the only value you can edit besides the boolean mentioned above is your whitelist</p>
        <p>Input the SSIDs like this(10 max): <i>SSID1,SSID2,SSID3</i></p>
        <form action="/get">
            Config::whitelist <input type="text" name="whitelist">
            <input type="submit" value="Submit">
        <p>Make sure to type <i>true</i> in this box when you're done!</p>
        </form><br>
            <form action="/get">
            Config::configured <input type="text" name="config">
            <input type="submit" value="Submit">
        </form><br>
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

  bool canHandle(AsyncWebServerRequest *request) {
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
  Serial.println(mood.getIntense() + " Starting Web Server...");
  Display::updateDisplay(mood.getIntense(), "Starting Web Server...");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(Config::ssid, Config::pass);

  setupServer();

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.setTTL(300);
  dnsServer.start(53, "*", WiFi.softAPIP());

  server.begin();
  WebUI::running = true;

  while (running) {
    dnsServer.processNextRequest();
  }
}

WebUI::~WebUI() {
  // debugging
  Serial.println("WebUI Destructor called");

  // nah fuck it
  dnsServer.stop();
  server.end();
  WiFi.softAPdisconnect(true);

  running = false;
}

/**
 * Sets up Web Server
 */
void WebUI::setupServer() {
  server.addHandler(new CaptivePortalHandler()).setFilter(ON_AP_FILTER);

  // handle whitelist
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("whitelist")) {
      String newWhitelist = request->getParam("whitelist")->value();
      updateWhitelist(newWhitelist);
      request->send(200, "text/html",
                    mood.getHappy() + " Whitelist updated successfully!<br><a "
                                      "href=\"/\">Return to Home Page</a>");
    } else if (request->hasParam("config")) {
      String configValue = request->getParam("config")->value();
      Config::configured = (configValue == "true");
      Config::saveConfig();
      // Serial.println("Config check: " + String(Config::configured ? "true" :
      // "false"));
      request->send(200, "text/html",
                    mood.getHappy() +
                        " Configuration updated! You may exit this tab and "
                        "disconnect from the Wifi AP.<br>");
    } else {
      request->send(200, "text/html",
                    mood.getBroken() + " No <b>valid</b> input received.<br><a "
                                       "href=\"/\">Return to Home Page</a>");
    }
  });

  server.onNotFound([&](AsyncWebServerRequest *request) {
    request->send(200, "text/html", html);
  });
}

/**
 * Update the whitelist with the new values
 * @param newWhiteList new whitelist to use
 */
void WebUI::updateWhitelist(String newWhitelist) {
  Config::whitelist.clear(); // clear existing values
  int start = 0;
  int end = newWhitelist.indexOf(',');

  while (end != -1) {
    Config::whitelist.push_back(newWhitelist.substring(start, end).c_str());
    start = end + 1;
    end = newWhitelist.indexOf(',', start);
  }

  // add last element after last comma
  Config::whitelist.push_back(newWhitelist.substring(start).c_str());
}
