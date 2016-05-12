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
    GizmoTCP();
    void begin(uint8_t *mac_address, uint8_t *ip_address);
    unsigned int PacketReceive(unsigned int len, uint8_t* packet);
};




#endif

