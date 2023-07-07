#include <Arduino.h>
#include "esp_bt_main.h"

#include "esp_bt_device.h"
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void printDeviceAddress()
{

  const uint8_t *point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++)
  {
    char str[3];
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
    if (i < 5)
    {
      Serial.print(":");
    }
  }
}

void setup()
{
  SerialBT.begin("ESP32-Server", 0);
  Serial.begin(115200);

  printDeviceAddress();
}

void loop()
{

  if (SerialBT.available())
  {
    Serial.print("avl: ");
    Serial.println(SerialBT.available());

    String message = SerialBT.readStringUntil('\n');
    message.trim();
    message.replace("\r", "");
    message.replace("\n", "");
    Serial.println(message);

    if (message.equalsIgnoreCase("packetloss"))
    {
      SerialBT.println("packetloss");
    }
    else if (message.equalsIgnoreCase("latency"))
    {
      SerialBT.println("latency");
    }
  }
}
