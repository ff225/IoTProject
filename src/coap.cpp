#include <Arduino.h>
#include "mqtt.h"
#include "coap.h"

unsigned int packet_loss_callback = 0;
unsigned int start_time_latency = 0;

void setup_coap()
{
    coap.response(callback);
    coap.start();
}

void callback(CoapPacket &packet, IPAddress ip, int port)
{

    unsigned long received_time = millis();
    char payload[packet.payloadlen + 1];
    memcpy(payload, packet.payload, packet.payloadlen);
    payload[packet.payloadlen] = '\0';
    if (strcmp(payload, URL_COAP_LATENCY) == 0)
    {
        float latency = (received_time - start_time_latency) / 2;
        Serial.print("Latenza Coap: ");
        Serial.print(latency);
        Serial.println(" ms");
        mqtt_client.publish(NODERED_LATENCY_COAP, String(latency).c_str());
    }

    else if (strcmp(payload, URL_PACKET_LOSS) == 0)
    {
        packet_loss_callback++;
    }
}