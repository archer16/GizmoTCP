#ifndef GIZMOTCP_H
#define GIZMOTCP_H

#include "Arduino.h"
#include <inttypes.h>
#include "enc28j60.h"
#include "ip_arp_udp_tcp.h"
#include "net.h"

class GizmoTCP
{
  public:
    unsigned int buffer_length;
    uint8_t *buffer;
    
    uint8_t *dest_ip;
    uint8_t *dest_mac;
  
    GizmoTCP(uint8_t* buf,unsigned int len);
    void init(uint8_t *mac_address, uint8_t *ip_address);
    void blink_led();
    unsigned int PacketReceive();
    void setDestMAC(uint8_t *destmac);
    void setDestIP(uint8_t *destip);
    void setDestination(uint8_t *destip);
    void sendUDP(char *data,uint8_t datalen, uint16_t dport,uint16_t sport);
};

void arp_callback(uint8_t *ip __attribute__((unused)),uint8_t reference_number,uint8_t *mac);


#endif

