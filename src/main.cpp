#include <Arduino.h>
#include "setup_wifi.h"
#include "mqtt.h"

WiFiClient wifi_client;
PubSubClient mqtt_client(SERVER, PORT, wifi_client);

unsigned int start_time_thr = 0;
unsigned int start_time_pl = 0;
unsigned int messages_sent_pl = 0;
extern unsigned int packet_received;
extern unsigned long start_time_latency;

void setup()
{
  Serial.begin(115200);

  connect_to_wifi();

  connect_to_mqtt();
  subscribe_to_topics();
}

void loop()
{

  check_mqtt_connection();

  mqtt_client.loop();
  if (millis() - start_time_latency > TIMEOUT)
  {
    start_time_latency = millis();
    mqtt_client.publish(LATENCY_TOPIC, "latency", 1);
  }

  mqtt_client.loop();

  unsigned long current_time = millis();
  unsigned long elapsed_time = current_time - start_time_thr;
  int packet_sent = 0;
  if (elapsed_time > TIMEOUT)
  {
    unsigned int t_start_loop = millis();
    while (millis() - t_start_loop < SECONDS)
    {
      mqtt_client.publish(THROUGHPUT, "throughput", 1);
      packet_sent += strlen("throughput");
    }
    float throughput = (float)packet_sent / (float)(millis() - t_start_loop) * 1000.0;
    mqtt_client.publish(NODERED_THROUGHPUT, String(throughput).c_str(), 1);
    Serial.print("Throughput MQTT: ");
    Serial.print(throughput);
    Serial.println(" B/s");

    start_time_thr = millis();
  }

  mqtt_client.loop();

  unsigned long current_time_pl = millis();
  unsigned long elapsed_time_pl = current_time_pl - start_time_pl;

  if (elapsed_time_pl > TIMEOUT)
  {
    unsigned int t_start_loop = millis();
    while (millis() - t_start_loop < SECONDS)
    {
      mqtt_client.publish(PACKET_LOSS, "packetloss", 1);
      messages_sent_pl++;
      mqtt_client.loop();
    }

    packet_received = packet_received > messages_sent_pl ? messages_sent_pl : packet_received;
    float packet_loss = packet_received == 0 ? 100 : ((float)(messages_sent_pl - packet_received) / (float)messages_sent_pl) * 100;

    Serial.print("Packets Sent: ");
    Serial.println(messages_sent_pl);
    Serial.print("Packets Received: ");
    Serial.println(packet_received);
    Serial.print("Packets Lost: ");
    Serial.println(packet_loss);

    mqtt_client.publish(NODERED_PACKET_LOSS, String(packet_loss).c_str(), 1);

    messages_sent_pl = 0;
    packet_received = 0;
    start_time_pl = millis();
  }
}
