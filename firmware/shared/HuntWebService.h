#ifndef HUNT_WEB_SERVICE_H
#define HUNT_WEB_SERVICE_H

#include <Arduino.h>
#include "HuntService.h"
#include "HuntWebServer.h"

class HuntWebService : public HuntService {
public:
  HuntWebService(const char *ssid, const char *password);

  void begin() override;
  void update() override;
  const char *name() override;

  HuntWebServer *serverInstance();

private:
  const char *apSsid;
  const char *apPassword;
  HuntWebServer webServer;
};

#endif
