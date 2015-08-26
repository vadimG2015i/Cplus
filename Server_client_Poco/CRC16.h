#ifndef __CRC16_H__
#define __CRC16_H__
typedef unsigned int uint;
uint calcCRC16 ( const byte * buf, int len );
WORD calcCRC16_MB ( const byte * buf, int len );

#endif //__CRC16_H__
