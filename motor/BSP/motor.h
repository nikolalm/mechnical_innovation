#ifndef MOTOR_H_
#define MOTOR_H_

#include "pid.h"
#include "main.h"

struct Motor
{
	uint8_t id;
	struct PID_t position_pid;  //电机位置环PID
	struct PID_t speed_pid;     //电机速度环PID
};

#define DEFAULT_CAN_MOTOR \
{0,DEFAULT_PID, DEFAULT_PID \
}

extern struct Motor Motor_1;
extern struct Motor Motor_2;
extern struct Motor Motor_3;
extern struct Motor Motor_4;

int motor_feedback(uint8_t id,TIM_HandleTypeDef* TIM_MOTOR);
//int motor_feedback(void);
void motor_control(GPIO_TypeDef* GPIOx1, uint16_t GPIO_Pin1,GPIO_TypeDef* GPIOx2, uint16_t GPIO_Pin2,uint8_t id,int pwm_motor);
void steer_control(int pwm_steer);
void MotorParamInit(struct Motor *motor, float speedP, float speedI, float speedD,int speedOutmax, float positionP, float positionI, float positionD, int positionOutmax,int errrormax);

#endif

