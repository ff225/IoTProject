#include <Arduino.h>
#include <ESPmDNS.h>
#include <BluetoothSerial.h>
#include "setup_wifi.h"
#include "mqtt.h"
#include "utils.h"

BluetoothSerial SerialBT;
WiFiClient wifi_client;
PubSubClient mqtt_client("MacBook-Pro-di-Francesco-3.local", PORT, wifi_client);

void setup()
{
  SerialBT.begin("ESP32-Client", 1);
  Serial.begin(115200);

  connect_to_wifi();

  connect_to_mqtt();
  MDNS.begin("Esp32-Client");
  printDeviceAddress();
}

unsigned int start_time_latency = 0;
unsigned int start_time_thr = 0;
unsigned int start_time_pl = 0;

void loop()
{
  if (!SerialBT.connect("ESP32-Server"))
  {
    Serial.println("Failed to connect to server");
    delay(1000);
  }

  if (millis() - start_time_latency > TIMEOUT)
  {
    SerialBT.println("latency");
    unsigned int t_start_loop = millis();
    while (millis() - t_start_loop < TIMEOUT)
    {
      if (SerialBT.available())
      {
        String message = SerialBT.readStringUntil('\n');
        message.trim();
        message.replace("\r", "");
        message.replace("\n", "");
        if (message.equalsIgnoreCase("latency"))
        {
          float latency = (millis() - t_start_loop) / 2;
          Serial.println(latency);
          mqtt_client.publish(NODERED_LATENCY_BL, String(latency).c_str());
          break;
        }
      }
    }
    start_time_latency = millis();
  }

  unsigned long current_time_bl_thr = millis();
  unsigned long elapsed_time_bl_thr = current_time_bl_thr - start_time_thr;

  if (elapsed_time_bl_thr > TIMEOUT)
  {
    unsigned int packet_sent = 0;
    unsigned int t_start_loop = millis();
    while (millis() - t_start_loop < SECONDS)
    {
      SerialBT.println("throughput");
      delay(5);
      packet_sent += strlen("throughput");
    }
    float throughput = (float)packet_sent / (float)(millis() - t_start_loop) * 1000.0;
    mqtt_client.publish(NODERED_THROUGHPUT_BL, String(throughput).c_str());
    Serial.print("throughput: ");
    Serial.println(throughput);
    current_time_bl_thr = millis();
  }

  unsigned long current_time_bl_pl = millis();
  unsigned long elapsed_time_bl_pl = current_time_bl_pl - start_time_pl;

  if (elapsed_time_bl_pl > TIMEOUT)
  {
    int packet_sent = 0;
    int packet_received = 0;
    unsigned int t_start_loop = millis();
    while (millis() - t_start_loop < SECONDS)
    {
      SerialBT.println("packetloss");
      packet_sent++;
      int wait = millis();
      while (millis() - wait < 100)
      {
        if (SerialBT.available())
        {
          String message = SerialBT.readStringUntil('\n');
          message.trim();
          message.replace("\r", "");
          message.replace("\n", "");
          if (message.equalsIgnoreCase("packetloss"))
          {
            packet_received++;
            break;
          }
        }
      }
    }
    Serial.print("Packet sent: ");
    Serial.println(packet_sent);
    Serial.print("Packet received: ");
    Serial.println(packet_received);

    float packet_loss = packet_received == 0 ? 100 : (float)(packet_sent - packet_received) / (float)packet_sent * 100;
    mqtt_client.publish(NODERED_PACKET_LOSS_BL, String(packet_loss).c_str());

    packet_received = 0;
    current_time_bl_pl = millis();
  }
}