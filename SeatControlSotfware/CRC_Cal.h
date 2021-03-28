#ifndef CRC_CAL_H
#define CRC_CAL_H

#include "QByteArray"
class CRC_Cal
{
public:
    CRC_Cal();
private:
    unsigned long crc_val;
    unsigned long  Reflect(unsigned long ref,unsigned char ch);
public:

    void crc32(const QByteArray *data, unsigned long len);
    unsigned long getCrc32();
};

#endif // CRC_CAL_H
