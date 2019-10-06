#ifndef WIFI_REC_H_
#define WIFI_REC_H_

#include "user.h"
#include "stm32f4xx.h"

struct RosRemote 
{
	int16_t FBSpeed;
	int16_t LRSpeed;
	int16_t RotateAngle;
	
	//debug 其他控制如舵机标志
	//char steer;
	
};

extern uint8_t rc_data[16];
extern struct RosRemote Remote;
void RemoteReceiveHandle(void);
int stringTOint(uint8_t *str);
void WIFI_RECEIVE(void);
#endif 
