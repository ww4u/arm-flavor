#include "main.h"

cmdcache cmdBuff;
extern pumpPara pump[PUMP_LIMIT];

int cmdStringRece(void)
{       
    comGetString(0, cmdBuff.cache);     
 
    cmdBuff.psub[0] = strtok((char*)cmdBuff.cache, " ");        
    cmdBuff.suffix = strtok(NULL, "\n");

    cmdBuff.psub[1] = strtok(cmdBuff.psub[0], ":");
    cmdBuff.psub[2] = strtok(NULL, ":");
    //int i = 2;
    //while( cmdBuff.psub[i] = strtok(NULL, ":") ){ i++; } 

    if( strcmp( cmdBuff.psub[1], "SYRUP" ) != 0 ){
        return -1;    //设备不符      
    }
    return 0;
}

int workStatusBack( int index )
{
	u8 _temp = 0;
	_temp = pumpStatusGet(index);
    switch( _temp ){
        case 0:
            printf( "IDLE\n" );break;
        case 1:
            printf( "BUSY\n" );break;
        case 2:
            printf( "CLEAN\n" );break;
        case 3:
            printf( "CALIBRATE\n" );break;
        case 15:
            printf( "ERROR\n" );break;
        default:
            printf( "BUG\n" );break;  
    }
    return _temp;
}

int cmdStringProcess(void)
{
    int index = 0;
    unsigned long num = 0;
    do{
		if( strcmp(cmdBuff.psub[2], "STATUS?" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index);
			if( index == PUMP_ALL ){
				for(int i=1; i<PUMP_LIMIT; i++){
					 workStatusBack(i);
				}
			}
			else{
				workStatusBack(index);
			}
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "WEIGHT" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d,%ld", &index, &num );
			if( index == 0 ){ return -1; }
			pump[index].weightTatol += num;
			bspJumpWrite(&pump[index].weightTatolAddr, 
						  pump[index].weightTatol,
						  TOTAL_ADDR + (index-1)*FLASH_PAGE,
						  500);
			pumpWork( index, num*pump[index].pumpRatio );
			pumpStatusSet(index, 1);
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "TIME" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d,%ld", &index, &num );     
			if( index == 0 ){ return -1; }
			pumpSecond( index, num );
			pumpStatusSet(index, 1);
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "RATIO?" ) == 0 ){
			for(int i=1; i<PUMP_LIMIT; i++){
				printf( "%d\n", pump[i].pumpRatio );
			}
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "RATIO" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d,%ld", &index, &num );
			pump[index].pumpRatio = num;
			pumpParaSave();
			break;
		}
		if( strcmp( cmdBuff.psub[2], "CALIBRATE" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index);
			if( index == 0 ){ return -1; }
			pumpSecond( index, 10);
			pumpStatusSet(index, 3);
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "MEASUREMENTS" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d,%ld", &index, &num );

			if( index == 0 ){ return -1; }
			pump[index].pumpRatio = (int)( 10000000/num );
			pumpParaSave();
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "TOTAL?" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index );
			if( index == PUMP_ALL ){
				for(int i=1; i<PUMP_LIMIT; i++){
					printf( "%d\n", pump[i].weightTatol );
				}
			}
			else{        
				printf( "%d\n", pump[index].weightTatol );
			}
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "CLEARACC" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index );
			if( index == 0 ){ return -1; }
			pump[index].weightTatol = 0;
			bspJumpEarse(TOTAL_ADDR + (index-1)*FLASH_PAGE);			
			pump[index].storageHour = bsp_hourReturn();	
			//bsp_hourCheck();			
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "STORAGE?" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index );
			if( index == PUMP_ALL ){
				for(int i=1; i<PUMP_LIMIT; i++){
					 printf( "%d\n", bsp_hourReturn()-pump[i].storageHour);
				}
			}
			else{
				printf( "%d\n", bsp_hourReturn()-pump[index].storageHour);
			}
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "MINUTE" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d,%ld", &index, &num );
			if( index == 0 ){ return -1; }
			pumpMinute( index, num );
			pumpStatusSet(index, 1);
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "CLEAN" ) == 0 ){
			pumpClean();
			for( int i=1; i<PUMP_LIMIT; i++){
				pumpStatusSet(i, 2);
			}
			pump[0].storageSecond = bsp_hourReturn();
			pumpParaSave();
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "RECORD?" ) == 0 ){
			printf("%d\n", bsp_hourReturn()-pump[0].storageSecond);
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "FILL" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index );
			if( index == 0 ){ return -1; }
			pumpWork( index, pump[index].pumpLength );
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "LENGTH?" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index);
			if( index == 0 ){ return -1; }
			printf( "%d\n", pump[index].pumpLength );
			//for( int i=1; i<PUMP_LIMIT; i++){
			//	printf( "%d\n", pump[i].pumpLength );
			//}
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "LENGTH" ) == 0 ){
			sscanf( cmdBuff.suffix, "%ld", &num );
			//if( index == 0 ){ return -1; }
			for( int i=1; i<PUMP_LIMIT; i++){
				pump[i].pumpLength = num;
				pumpParaSave();
			}
			break;
		}
		
		if( strcmp( cmdBuff.psub[2], "START" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index);
			pumpRun(index, TRUE);
			pumpStatusSet(index, 1);
			break;
		}
		
		if( strcmp(cmdBuff.psub[2], "STOP" ) == 0 ){
			sscanf( cmdBuff.suffix, "%d", &index);
			//if( index == 0 ){ return -1; }
			if( index == PUMP_ALL ){ 
				for( int i=1; i<PUMP_LIMIT; i++){
					pumpRun(i, FALSE);
					pumpStatusSet(i, 0);
				}
			}
			else{
				pumpRun(index, FALSE);
				pumpStatusSet(index, 0);
			}       
			break;
		}
		
		if( strcmp(cmdBuff.psub[2], "VERSION?" ) == 0 ){
			printf( "00.01\n" );
			break;
		}
	}while(0);
	
	memset(cmdBuff.cache,0,48);
	for(uint8_t i=0;i<3;i++){		
		if(cmdBuff.psub[0]!=NULL) memset(cmdBuff.psub[0],0,strlen(cmdBuff.psub[0]));
	}
	if(cmdBuff.suffix!=NULL) memset(cmdBuff.suffix,0,strlen(cmdBuff.suffix));
	
    return 0;
}


