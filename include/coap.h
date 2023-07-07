#ifndef COAP_H
#define COAP_H

#include <coap-simple.h>

#define URL_COAP_LATENCY "latency"
#define URL_COAP_THROUGHPUT "throughput"
#define URL_PACKET_LOSS "packetloss"
#define COAP_PORT 5683

extern Coap coap;

void setup_coap();
void callback(CoapPacket &packet, IPAddress ip, int port);

#endif