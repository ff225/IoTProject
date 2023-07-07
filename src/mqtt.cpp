#include "mqtt.h"
#include "setup_wifi.h"

unsigned int packet_received = 0;
unsigned int start_time_latency = 0;

void connect_to_mqtt()
{
    mqtt_client.setServer(SERVER, PORT);
    mqtt_client.setCallback(mqtt_callback);
    while (!mqtt_client.connected())
    {
        if (mqtt_client.connect("ESP32"))
        {
            Serial.println("Connesso al broker MQTT");
        }
        else
        {
            Serial.println("Connessione al broker MQTT fallita. Riprovo tra 5 secondi");
            delay(5000);
        }
    }
}

void check_mqtt_connection()
{
    if (!mqtt_client.connected())
    {
        mqtt_client.connect("ESP32");
        subscribe_to_topics();
        Serial.println("MQTT connesso");
    }
    mqtt_client.loop();
}

void subscribe_to_topics()
{
    mqtt_client.subscribe(LATENCY_TOPIC_ACK);
    mqtt_client.subscribe(PACKET_LOSS_ACK);
    Serial.printf("Sottoscritto al topic %s\n", LATENCY_TOPIC_ACK);
    Serial.printf("Sottoscritto al topic %s\n", PACKET_LOSS_ACK);
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    if (strcmp(topic, LATENCY_TOPIC_ACK) == 0)
        latency_callback(topic, payload, length);
    if (strcmp(topic, PACKET_LOSS_ACK) == 0)
        packet_loss_callback(topic, payload, length);
}

void latency_callback(char *topic, byte *payload, unsigned int length)
{
    unsigned long received_time = millis();
    float latency = (received_time - start_time_latency) / 2;
    Serial.print("Latenza: ");
    Serial.print(latency);
    Serial.println(" ms");
    mqtt_client.publish(NODERED_LATENCY, String(latency).c_str(), 1);
}

void packet_loss_callback(char *topic, byte *payload, unsigned int length)
{
    packet_received += 1;
}