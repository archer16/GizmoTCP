extern "C" {
  #include "enc28j60.h"
  #include "ip_arp_udp_tcp.h"
 
}
#include "GizmoTCP.h"



GizmoTCP::GizmoTCP() {}

void GizmoTCP::begin(uint8_t *mac_address, uint8_t *ip_address)
{
  enc28j60Init(mac_address);

  // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
  enc28j60PhyWrite(PHLCON,0b0000010001110110);
  //enc28j60PhyWrite(PHLCON,0x476);


  init_udp_or_www_server(mac_address,ip_address);
}

unsigned int GizmoTCP::PacketReceive(unsigned int len, uint8_t* packet)
{
  unsigned int plen = enc28j60PacketReceive(len, packet);

  if (plen != 0){
    
    if (eth_type_is_arp_and_my_ip (packet,plen)) {
      make_arp_answer_from_request (packet);
    }

    // check if ip packets (icmp or udp) are for us:
    if (eth_type_is_ip_and_my_ip (packet,plen)!=0) {
      if (packet[IP_PROTO_P] == IP_PROTO_ICMP_V && packet[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V) {
        // a ping packet, let's send pong
        make_echo_reply_from_request (packet, plen);
      }
    }

    return 0;
  }
  
  return plen;
}

