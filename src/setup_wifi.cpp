#include "setup_wifi.h"
#include "secrets.h"

void connect_to_wifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PWD);

    unsigned long timeout = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connessione al WiFi in corso...");
        if ((millis() - timeout) > TIMEOUT)
        {
            Serial.println("Connessione al WiFi fallita. Riavvio in corso...");
            ESP.restart();
        }
    }

    Serial.println("Connesso al WiFi");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.RSSI());
}
