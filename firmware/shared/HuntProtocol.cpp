#include "HuntProtocol.h"

String huntBuildPacket(const String &type, const String &source, const String &target, const String &payload) {
  String packet = "HUNT";
  packet += "|";
  packet += HUNT_PROTOCOL_VERSION;
  packet += "|";
  packet += type;
  packet += "|";
  packet += source;
  packet += "|";
  packet += target;
  packet += "|";
  packet += payload;
  return packet;
}

HuntPacket huntParsePacket(const String &rawPacket) {
  HuntPacket packet;
  packet.valid = false;
  packet.version = "";
  packet.type = "";
  packet.source = "";
  packet.target = "";
  packet.payload = "";

  if (rawPacket.length() == 0 || rawPacket.length() > HUNT_MAX_PACKET_LENGTH) {
    return packet;
  }

  int first = rawPacket.indexOf('|');
  int second = rawPacket.indexOf('|', first + 1);
  int third = rawPacket.indexOf('|', second + 1);
  int fourth = rawPacket.indexOf('|', third + 1);
  int fifth = rawPacket.indexOf('|', fourth + 1);

  if (first < 0 || second < 0 || third < 0 || fourth < 0 || fifth < 0) {
    return packet;
  }

  String prefix = rawPacket.substring(0, first);
  if (prefix != "HUNT") {
    return packet;
  }

  packet.version = rawPacket.substring(first + 1, second);
  packet.type = rawPacket.substring(second + 1, third);
  packet.source = rawPacket.substring(third + 1, fourth);
  packet.target = rawPacket.substring(fourth + 1, fifth);
  packet.payload = rawPacket.substring(fifth + 1);

  if (packet.version != HUNT_PROTOCOL_VERSION) {
    return packet;
  }

  if (packet.type.length() == 0 || packet.source.length() == 0 || packet.target.length() == 0) {
    return packet;
  }

  packet.valid = true;
  return packet;
}

bool huntIsPacketForDevice(const HuntPacket &packet, const String &deviceId) {
  if (!packet.valid) {
    return false;
  }

  if (packet.target == "ALL") {
    return true;
  }

  return packet.target == deviceId;
}
