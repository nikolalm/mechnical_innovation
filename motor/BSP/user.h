#ifndef USER_H_
#define USER_H_
/*引脚分配
电机PWM,TIM1
电机Encode，TIM3
舵机PWM,TIM2
*/
#include "main.h"

/*user lib*/
#include "motor.h"
#include "print_usart.h"
#include "pid.h"
#include "wifi_rec.h"

/*c lib*/
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "math.h"

extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;//串口2wifi模块串口
extern int remote[9];
extern uint8_t wifi_data[26];

extern uint8_t remote_ok;

#define WIFI_UART huart2

//端口重命名
#define GPIO_LED GPIOH
#define GPIO_PIN_LED GPIO_PIN_12

//编码器
#define TIM_MOTOR1_ENCODE htim2
#define TIM_MOTOR2_ENCODE htim3
#define TIM_MOTOR3_ENCODE htim4
#define TIM_MOTOR4_ENCODE htim5

#define TIM_MOTOR1_PWM htim8
#define TIM_MOTOR2_PWM htim8
#define TIM_MOTOR3_PWM htim8
#define TIM_MOTOR4_PWM htim8

#define TIM_CHANNEL_MOTOR1 TIM_CHANNEL_1
#define TIM_CHANNEL_MOTOR2 TIM_CHANNEL_2
#define TIM_CHANNEL_MOTOR3 TIM_CHANNEL_3
#define TIM_CHANNEL_MOTOR4 TIM_CHANNEL_4

//#define TIM_STEER htim2

#define GPIO_MOTOR1_AIN1 			GPIOD
#define GPIO_MOTOR1_AIN2 			GPIOD
#define GPIO_PIN_MOTOR1_AIN1	GPIO_PIN_11
#define GPIO_PIN_MOTOR1_AIN2  GPIO_PIN_5

#define GPIO_MOTOR2_AIN1 			GPIOI
#define GPIO_MOTOR2_AIN2 			GPIOB
#define GPIO_PIN_MOTOR2_AIN1	GPIO_PIN_3
#define GPIO_PIN_MOTOR2_AIN2  GPIO_PIN_12

#define GPIO_MOTOR3_AIN1 			GPIOC
#define GPIO_MOTOR3_AIN2 			GPIOC
#define GPIO_PIN_MOTOR3_AIN1	GPIO_PIN_6
#define GPIO_PIN_MOTOR3_AIN2  GPIO_PIN_2

#define GPIO_MOTOR4_AIN1 			GPIOC
#define GPIO_MOTOR4_AIN2			GPIOC
#define GPIO_PIN_MOTOR4_AIN1	GPIO_PIN_4
#define GPIO_PIN_MOTOR4_AIN2  GPIO_PIN_5

struct Chassis_t
{
	int FBSpeed;
	int LRSpeed;
	int RotateAngle;
};

void user_sys_init(void);

void led_shine(void);
void Robot_task(void);

extern struct Chassis_t chassis;
#endif


