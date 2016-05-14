extern "C" {
  #include "enc28j60.h"
  #include "ip_arp_udp_tcp.h"
 
}
#include "GizmoTCP.h"

#define GIZMO_ARP_WAITING  1
#define GIZMO_ARP_RESOLVED 2
static uint8_t arpmac[6];
static uint8_t arpstate;

  GizmoTCP::GizmoTCP(uint8_t* buf,unsigned int len)
  {
    buffer = buf;
    buffer_length = len;
  }
  
  
  void GizmoTCP::blink_led()
  {
    // 0x880 is PHLCON LEDB=on, LEDA=on
    // enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
    enc28j60PhyWrite (PHLCON, 0x880);
    delay (500);
  
    // 0x990 is PHLCON LEDB=off, LEDA=off
    // enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
    enc28j60PhyWrite (PHLCON, 0x990);
    delay (500);
  
    // 0x880 is PHLCON LEDB=on, LEDA=on
    // enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
    enc28j60PhyWrite (PHLCON, 0x880);
    delay (500);
  
    // 0x990 is PHLCON LEDB=off, LEDA=off
    //enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
    enc28j60PhyWrite (PHLCON, 0x990);
    delay (500);
  
    // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
    // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
    enc28j60PhyWrite (PHLCON, 0x476);
    delay (100);
  }
  
  void GizmoTCP::init(uint8_t *mac_address, uint8_t *ip_address)
  {
    enc28j60Init(mac_address);
  
    // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
    enc28j60PhyWrite(PHLCON,0b0000010001110110);
    //enc28j60PhyWrite(PHLCON,0x476);
  
  
    init_udp_or_www_server(mac_address,ip_address);
  }
  
  unsigned int GizmoTCP::PacketReceive()
  {
    unsigned int plen = enc28j60PacketReceive(buffer_length, buffer);
    
    
    plen = packetloop_arp_icmp_tcp(buffer,plen);
  
    /*
    if (plen != 0){
      
      if (eth_type_is_arp_and_my_ip (buffer,plen)) {
        make_arp_answer_from_request (buffer);
      }
  
      // check if ip packets (icmp or udp) are for us:
      if (eth_type_is_ip_and_my_ip (buffer,plen)!=0) {
        if (buffer[IP_PROTO_P] == IP_PROTO_ICMP_V && buffer[ICMP_TYPE_P] == ICMP_TYPE_ECHOREQUEST_V) {
          // a ping packet, let's send pong
          make_echo_reply_from_request (buffer, plen);
        }
      }
  
      return 0;
    }
    */ 
    
    return plen;
  }
  
  void GizmoTCP::setDestMAC(uint8_t *destmac)
  {
    dest_mac = destmac;
  }
  
  void GizmoTCP::setDestIP(uint8_t *destip)
  {
    dest_ip = destip;
  }
  
  void GizmoTCP::setDestination(uint8_t *destip)
  {
    arpstate = GIZMO_ARP_WAITING;
    get_mac_with_arp(destip, 1, &arp_callback);
    
    while(arpstate != GIZMO_ARP_RESOLVED) {GizmoTCP::PacketReceive();}
    
    dest_ip  = destip;
    dest_mac = arpmac;
  }
  
  void GizmoTCP::sendUDP(char *data,uint8_t datalen, uint16_t dport,uint16_t sport)
  {
    send_udp(buffer,data,datalen,sport,dest_ip,dport,dest_mac);
  }

  
  void arp_callback(uint8_t *ip __attribute__((unused)),uint8_t reference_number,uint8_t *mac)
  {
    memcpy(arpmac, mac, 6);
    arpstate = GIZMO_ARP_RESOLVED;
  }

