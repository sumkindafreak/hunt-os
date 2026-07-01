#include "HuntWebServer.h"

HuntWebServer::HuntWebServer() : server(80) {
  running = false;
}

void HuntWebServer::begin(const char *ssid, const char *password) {
  huntLogService("Web server AP starting: " + String(ssid));
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  huntLogService("Web AP online at http://" + WiFi.softAPIP().toString());
  setupRoutes();
  server.begin();
  running = true;
}

void HuntWebServer::update() {
  if (running) server.handleClient();
}

bool HuntWebServer::isRunning() {
  return running;
}

String HuntWebServer::getIpAddress() {
  return WiFi.softAPIP().toString();
}

void HuntWebServer::setupRoutes() {
  server.on("/", HTTP_GET, [this]() {
    server.send_P(200, "text/html", HUNT_WEB_INDEX_HTML);
  });

  server.on("/style.css", HTTP_GET, [this]() {
    server.send_P(200, "text/css", HUNT_WEB_STYLE_CSS);
  });

  server.on("/app.js", HTTP_GET, [this]() {
    server.send_P(200, "application/javascript", HUNT_WEB_APP_JS);
  });

  server.on("/api/system", HTTP_GET, [this]() {
    String json = "{\"name\":\"HUNT OS\",\"version\":\"0.1\",\"mode\":\"alpha\",\"ip\":\"" + getIpAddress() + "\",\"heap\":" + String(ESP.getFreeHeap()) + "}";
    sendJson(json);
  });

  server.on("/api/game/start", HTTP_POST, [this]() { sendJson("{\"ok\":true,\"action\":\"game_start\"}"); });
  server.on("/api/game/pause", HTTP_POST, [this]() { sendJson("{\"ok\":true,\"action\":\"game_pause\"}"); });
  server.on("/api/game/end", HTTP_POST, [this]() { sendJson("{\"ok\":true,\"action\":\"game_end\"}"); });
  server.on("/api/emergency", HTTP_POST, [this]() { sendJson("{\"ok\":true,\"action\":\"emergency_stop\"}"); });

  server.onNotFound([this]() {
    server.send(404, "application/json", "{\"ok\":false,\"error\":\"not_found\"}");
  });
}

void HuntWebServer::sendJson(const String &json) {
  server.send(200, "application/json", json);
}
