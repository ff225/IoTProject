# Valutazione delle performance delle tecnologie di rete Wi-Fi e Bluetooth per dispositivi IoT

Un dispositivo IoT può utilizzare diverse tecnologie per comunicare. Le tecnologie di rete più comuni utilizzate nei dispositivi IoT includono Wi-Fi e Bluetooth. I dispositivi IoT comunicano tra di loro per consentire lo scambio di informazioni, dati e comandi. Questa comunicazione tra i dispositivi è essenziale per consentire il funzionamento coordinato di un sistema IoT e per consentire il controllo, il monitoraggio e l'automazione delle diverse applicazioni.

## Obiettivi

L'obiettivo di questo progetto è condurre una valutazione delle prestazioni delle tecnologie di rete Wi-Fi e Bluetooth per dispositivi IoT. Le metriche chiave che verranno misurate includono:

- **Latenza:** La misura del tempo di trasmissione dei dati tra dispositivi IoT utilizzando le tecnologie Wi-Fi e Bluetooth.
- **Throughput:** La quantità di dati che può essere trasmessa tra dispositivi IoT utilizzando le tecnologie Wi-Fi e Bluetooth in un determinato intervallo di tempo.
- **Packet Loss:** La percentuale di pacchetti di dati persi durante la trasmissione tra dispositivi IoT utilizzando le tecnologie Wi-Fi e Bluetooth.

## Struttura della repository

Il repository è diviso nei seguenti branch:

- **BluetoothClient**: implementa il client bluetooth utilizzato negli esperimenti;
- **BluetoothServer**: implementa il server bluetooth utilizzato negli esperimenti;
- **WiFiMQTTPerformance**: implementa il client MQTT implementato su ESP32;
- **WiFiCoAPPerforamnce**: implementa il CoAP client implementato su ESP32;
- **VirtualClient**: implementa il secondo client MQTT e il CoAP Server.

