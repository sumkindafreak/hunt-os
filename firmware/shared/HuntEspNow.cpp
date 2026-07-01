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

void huntEspNowOnSend(const esp_now_send_info_t *tx_info, esp_now_send_status_t status) {
  (void)tx_info;
  if (status == ESP_NOW_SEND_SUCCESS) {
    huntLogDebugTag("NET", "ESP-NOW send acknowledged");
  } else {
    huntLogWarnTag("NET", "ESP-NOW send failed");
  }
}

bool huntEspNowBegin() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  huntLogNet("Starting ESP-NOW");
  huntLogNet("Wi-Fi MAC: " + WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    huntLogErrorTag("NET", "ESP-NOW init failed");
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
      huntLogErrorTag("NET", "Failed to add ESP-NOW broadcast peer");
      return false;
    }
  }

  huntLogNet("ESP-NOW ready (broadcast peer added)");
  return true;
}

bool huntEspNowSendBroadcast(const String &packet) {
  if (packet.length() == 0 || packet.length() > HUNT_MAX_PACKET_LENGTH) {
    huntLogWarnTag("NET", "Packet length invalid: " + String(packet.length()));
    return false;
  }

  huntLogPacket("TX", packet);

  esp_err_t result = esp_now_send(HUNT_BROADCAST_ADDRESS, (const uint8_t *)packet.c_str(), packet.length());

  if (result == ESP_OK) {
    return true;
  }

  huntLogErrorTag("NET", "esp_now_send() rejected packet");
  return false;
}

String huntEspNowReadPacket() {
  if (!huntPacketAvailable) {
    return "";
  }

  huntPacketAvailable = false;
  return huntLastReceivedPacket;
}
