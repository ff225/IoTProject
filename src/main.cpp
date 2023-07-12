#include <Arduino.h>
#include <ESPmDNS.h>
#include "coap.h"
#include "setup_wifi.h"
#include "mqtt.h"

WiFiUDP udp;
Coap coap(udp);
WiFiClient wifi_client;
PubSubClient mqtt_client("MacBook-Pro-di-Francesco-3.local", PORT, wifi_client);
IPAddress ip;
// IPAddress ip(192, 168, 1, 54);

unsigned int start_time_coap_thr = 0;
unsigned int start_time_coap_pl = 0;
unsigned int start_time_pl = 0;
unsigned int messages_sent_pl = 0;
extern unsigned int start_time_latency;
extern unsigned int packet_loss_callback;

void setup()
{
  Serial.begin(115200);

  connect_to_wifi();
  
  while (ip == INADDR_NONE)
  {
    if (!WiFi.hostByName("MacBook-Pro-di-Francesco-3.local", ip))
    {
      Serial.println("DNS resolution failed");
      ip = MDNS.queryHost("MacBook-Pro-di-Francesco-3.local");
      if (ip == INADDR_NONE)
      {
        Serial.println("MDNS resolution failed");
        return;
      }
    }
  }
  connect_to_mqtt();

  setup_coap();

  Serial.println(ip.toString());
  MDNS.begin("Esp32-Client");
}

void loop()
{

  check_mqtt_connection();

  coap.loop();
  if (millis() - start_time_latency > TIMEOUT)
  {
    coap.put(ip, COAP_PORT, URL_COAP_LATENCY, URL_COAP_LATENCY);
    start_time_latency = millis();
  }

  coap.loop();

  unsigned long current_time_coap_thr = millis();
  unsigned long elapsed_time_coap_thr = current_time_coap_thr - start_time_coap_thr;

  if (elapsed_time_coap_thr > TIMEOUT)
  {
    int packet_sent = 0;
    unsigned int t_start_loop = millis();
    while (millis() - t_start_loop < SECONDS)
    {
      coap.put(ip, COAP_PORT, URL_COAP_THROUGHPUT, URL_COAP_THROUGHPUT);
      coap.loop();
      packet_sent += strlen(URL_COAP_THROUGHPUT);
    }
    float throughput = (float)packet_sent / (float)(millis() - t_start_loop) * 1000.0;
    mqtt_client.publish(NODERED_THROUGHPUT_COAP, String(throughput).c_str());
    Serial.print("Throughput Coap: ");
    Serial.print(throughput);
    Serial.println(" B/s");

    start_time_coap_thr = millis();
  }

  coap.loop();
  unsigned long current_time_coap_pl = millis();
  unsigned long elapsed_time_coap_pl = current_time_coap_pl - start_time_coap_pl;

  // Se è trascorso un secondo, calcola il throughput
  if (elapsed_time_coap_pl > TIMEOUT)
  {

    unsigned int t_start_loop = millis();

    while (millis() - t_start_loop < 3000)
    {
      coap.put(ip, COAP_PORT, URL_PACKET_LOSS, URL_PACKET_LOSS);
      delay(10);
      messages_sent_pl++;
      coap.loop();
    }
    packet_loss_callback = packet_loss_callback > messages_sent_pl ? messages_sent_pl : packet_loss_callback;
    float packet_lost = packet_loss_callback == 0 ? 100 : (float)((messages_sent_pl - packet_loss_callback) / (float)messages_sent_pl) * 100;

    Serial.print("Packets Sent: ");
    Serial.println(messages_sent_pl);
    Serial.print("Packets Received: ");
    Serial.println(packet_loss_callback);
    Serial.print("Packets Lost: ");
    Serial.println(packet_lost);

    mqtt_client.publish(NODERED_PACKET_LOSS_COAP, String(packet_lost).c_str());

    start_time_coap_pl = millis();
    messages_sent_pl = 0;
    packet_loss_callback = 0;
  }
}
