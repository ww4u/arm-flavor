
//--<#define>--

#include <stdio.h>
#include "ConfigFile.h"
#include "StringFile.h"
#include "FunctionFile.h"

//--<Global&Cache>--



//--<mainLoop>--
unsigned char indexLoop = 0;
void loop()
{
    if ( Serial.available()>0 )
    {
//        delay(50);        //48Bytes
        if( !SerialStringRece() ){
            SerialStringProcess();
        }
        else{
            Serial.println( "CmdError" );
        }
    }
    pumpTimeCheck();
    if(indexLoop % 64 == 0){
        minutesTimeCheck();
    }
    indexLoop++;
    delay(20);    
}
