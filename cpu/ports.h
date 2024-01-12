#ifndef PORTS_H
#define PORTS_H

unsigned char inportb(unsigned short port);
void outportb(unsigned short port, unsigned char data);

#endif
