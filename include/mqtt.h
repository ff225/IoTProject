#ifndef MQTT_H
#define MQTT_H

#include <WiFiClient.h>
#include <PubSubClient.h>

// #define SERVER "192.168.1.54"
#define SERVER "192.168.254.38"
#define PORT 1883
#define LATENCY_TOPIC "room/devices/esp32/latency"
#define LATENCY_TOPIC_ACK "room/devices/esp32/latency_ack"
#define THROUGHPUT "room/devices/esp32/throughput"
#define PACKET_LOSS "room/devices/esp32/packet_loss"
#define PACKET_LOSS_ACK "room/devices/esp32/packet_loss_ack"
#define NODERED_LATENCY "nodered/latency"
#define NODERED_PACKET_LOSS "nodered/packet_loss"
#define NODERED_THROUGHPUT "nodered/throughput"

extern PubSubClient mqtt_client;

void connect_to_mqtt();
void check_mqtt_connection();
void mqtt_callback(char *topic, byte *payload, unsigned int length);
void latency_callback(char *topic, byte *payload, unsigned int length);
void packet_loss_callback(char *topic, byte *payload, unsigned int length);
void subscribe_to_topics();

#endif