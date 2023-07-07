#include "mqtt.h"
#include "coap.h"
#include "setup_wifi.h"

void connect_to_mqtt()
{
    mqtt_client.setServer(SERVER, PORT);
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
        mqtt_client.connect("ESP32-COAP");
        Serial.println("MQTT connesso");
    }
    mqtt_client.loop();
}