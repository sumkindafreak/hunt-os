#include "HuntEspNow.h"

uint8_t HUNT_BROADCAST_ADDRESS[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

String huntLastReceivedPacket = "";
bool huntPacketAvailable = false;

void huntEspNowOnReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len <= 0 || len > HUNT_MAX_PACKET_LENGTH) {
    return;
  }

  char buffer[HUNT_MAX_PACKET_LENGTH + 1];
  memset(buffer, 0, sizeof(buffer));
  memcpy(buffer, incomingData, len);
  buffer[len] = '\0';

  huntLastReceivedPacket = String(buffer);
  huntPacketAvailable = true;

  huntLogPacket("RX", huntLastReceivedPacket);
}

void huntEspNowOnSend(const uint8_t *macAddress, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    huntLog("ESP-NOW send success");
  } else {
    huntLog("ESP-NOW send failed");
  }
}

bool huntEspNowBegin() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  huntLog("Starting ESP-NOW");
  huntLog("Wi-Fi MAC: " + WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    huntLog("ESP-NOW init failed");
    return false;
  }

  esp_now_register_send_cb(huntEspNowOnSend);
  esp_now_register_recv_cb(huntEspNowOnReceive);

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, HUNT_BROADCAST_ADDRESS, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(HUNT_BROADCAST_ADDRESS)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      huntLog("Failed to add ESP-NOW broadcast peer");
      return false;
    }
  }

  huntLog("ESP-NOW ready");
  return true;
}

bool huntEspNowSendBroadcast(const String &packet) {
  if (packet.length() == 0 || packet.length() > HUNT_MAX_PACKET_LENGTH) {
    huntLog("ESP-NOW packet length invalid");
    return false;
  }

  huntLogPacket("TX", packet);

  esp_err_t result = esp_now_send(HUNT_BROADCAST_ADDRESS, (const uint8_t *)packet.c_str(), packet.length());

  if (result == ESP_OK) {
    return true;
  }

  huntLog("ESP-NOW send call failed");
  return false;
}

String huntEspNowReadPacket() {
  if (!huntPacketAvailable) {
    return "";
  }

  huntPacketAvailable = false;
  return huntLastReceivedPacket;
}
