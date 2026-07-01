#ifndef HUNT_WEB_SERVER_H
#define HUNT_WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "HuntDebug.h"
#include "web/HuntWebAssets.h"

// =====================================================
// HUNT Web Server v0.1
//
// Hosts HUNT Command Center from PROGMEM on the Base Node.
// =====================================================

class HuntWebServer {
public:
  HuntWebServer();

  void begin(const char *ssid, const char *password);
  void update();
  bool isRunning();
  String getIpAddress();

private:
  WebServer server;
  bool running;

  void setupRoutes();
  void sendJson(const String &json);
};

#endif
