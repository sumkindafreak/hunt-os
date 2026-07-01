#ifndef HUNT_ESPNOW_H
#define HUNT_ESPNOW_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "HuntDebug.h"
#include "HuntTypes.h"

// Broadcast MAC address sends packets to all nearby ESP-NOW devices.
extern uint8_t HUNT_BROADCAST_ADDRESS[6];

// The latest packet received by ESP-NOW.
extern String huntLastReceivedPacket;
extern bool huntPacketAvailable;

// Starts Wi-Fi station mode and initialises ESP-NOW.
bool huntEspNowBegin();

// Sends a text packet using ESP-NOW broadcast.
bool huntEspNowSendBroadcast(const String &packet);

// Returns the latest received packet and clears the available flag.
String huntEspNowReadPacket();

#endif
