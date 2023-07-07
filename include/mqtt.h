#ifndef MQTT_H
#define MQTT_H

#include <WiFiClient.h>
#include <PubSubClient.h>

// #define SERVER "192.168.1.54"
#define SERVER "192.168.254.38"
#define PORT 1883
#define NODERED_LATENCY_BL "nodered/latency_bl"
#define NODERED_PACKET_LOSS_BL "nodered/packet_loss_bl"
#define NODERED_THROUGHPUT_BL "nodered/throughput_bl"

extern PubSubClient mqtt_client;

void connect_to_mqtt();
void check_mqtt_connection();

#endif