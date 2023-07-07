#include <Arduino.h>
#include "utils.h"

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