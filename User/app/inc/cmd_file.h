//--<Head>--
#ifndef CMD_FILE_H
#define CMD_FILE_H

//--<Include>--
#include "bsp.h"

//--<union>--
union dataLong{
    unsigned long data;
    char b[4];
};
union dataInt{
    int data;
    char b[2];
};

//--<TypeDefine>--
typedef struct cmdcache{
    uint8_t cache[48];
    char *psub[3];
    char *suffix;
}cmdcache;

//--<Declaration>--
int cmdStringRece(void);
int cmdStringProcess(void);
//int saveInt( int address, int data );
//int readInt( int address);
//int saveLong( int address, unsigned long data );
//unsigned long readLong( int address);

#endif
