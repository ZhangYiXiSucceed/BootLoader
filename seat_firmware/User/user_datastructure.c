#include "user_datastructure.h"



uint8_t CheckSum(uint8_t * dataBuf,uint16_t bufLength)
{
	uint8_t sum=0;
	uint16_t index;
	
	for(index=0;index<bufLength;index++)
	{
	   sum+=*(dataBuf+index);
		 sum=sum&0xFF;	
	}
	
	return sum;
}

