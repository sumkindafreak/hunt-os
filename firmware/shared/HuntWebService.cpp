#include "HuntWebService.h"

HuntWebService::HuntWebService(const char *ssid, const char *password) {
  apSsid = ssid;
  apPassword = password;
}

void HuntWebService::begin() {
  webServer.begin(apSsid, apPassword);
}

void HuntWebService::update() {
  webServer.update();
}

const char *HuntWebService::name() { return "Web"; }

HuntWebServer *HuntWebService::serverInstance() {
  return &webServer;
}
