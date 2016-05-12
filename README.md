# GizmoTCP
Arduino TCP stack for enc28j60 modules

Based on the following projects:
 * [tuxgraphics stack v5.10](http://tuxgraphics.org/electronics/200606/article06061.shtml)
 * [etherShield](https://github.com/jonoxer/etherShield)

This library aims to be as small as possible while providing a complete TCP stack supporting the following protocols:
  * ARP
  * DHCP
  * IP
  * UDP
  * TCP
  * DNS
  * DHCP
  * HTTP (client)

For now the GizmoTCP stack is just a really thin wrapper class around the tuxgraphics stack. This might change later when I
have more time.

This is still in development as part of the [Gensoip project](http://www.sega-16.com/forum/showthread.php?26831-Gensoip) (not on github)

