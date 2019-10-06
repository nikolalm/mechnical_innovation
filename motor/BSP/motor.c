#include "user.h"

struct Motor Motor_1 = DEFAULT_CAN_MOTOR;
struct Motor Motor_2 = DEFAULT_CAN_MOTOR;
struct Motor Motor_3 = DEFAULT_CAN_MOTOR;
struct Motor Motor_4 = DEFAULT_CAN_MOTOR;
extern int pwm_motor;
int flag =0;
/*
int32_t tick_last1;
int32_t tick_last2;
int32_t tick_last3;
int32_t tick_last4;
double delta_mins_time;
double rpm = 0;
int32_t tick_inc;
int32_t tick_cur;
uint32_t cur_milli_time;
static uint32_t last_milli_time1;
*/

int test_2 = 0;
int motor_feedback(uint8_t id,TIM_HandleTypeDef* TIM_MOTOR)
{
	int32_t tick_cur;
	
	static int32_t tick_last1;
	static int32_t tick_last2;
	static int32_t tick_last3;
	static int32_t tick_last4;
	
	static int32_t tick_last;
	int32_t tick_inc;
	uint32_t cur_milli_time;
	static uint32_t last_milli_time1;
	static uint32_t last_milli_time2;
	static uint32_t last_milli_time3;
	static uint32_t last_milli_time4;
	
	double delta_mins_time;
	double rpm = 0;
	
		cur_milli_time = HAL_GetTick();
		tick_cur = __HAL_TIM_GET_COUNTER(TIM_MOTOR);

		if(id == 1)
		{
			tick_last = tick_last1;
		}
		else if(id == 2)
		{
			tick_last = tick_last2;
		}
		else if(id == 3)
		{
			tick_last = tick_last3;
		}
		else if(id == 4)
		{
			tick_last = tick_last4;
		}
	
		if(abs(tick_cur-tick_last)>0xffff/2)
		{
			if(tick_cur>tick_last)
			{
				tick_inc = -0xffff+tick_cur-tick_last;
			}
			else 
			{
				tick_inc = 0xffff+tick_cur-tick_last;
			}
		}
		else
		{
			tick_inc = tick_cur-tick_last;
		}
		
		if(id == 1)
		{
			tick_last1 = tick_cur;

			delta_mins_time = (cur_milli_time-last_milli_time1)/60000.0;
			last_milli_time1 = cur_milli_time; 
		}
		else if(id == 2)
		{
			tick_last2 = tick_cur;
			
			delta_mins_time = (cur_milli_time-last_milli_time2)/60000.0;
			last_milli_time2 = cur_milli_time; 
		}
		else if(id == 3)
		{
			tick_last3 = tick_cur;
			
			delta_mins_time = (cur_milli_time-last_milli_time3)/60000.0;
		last_milli_time3 = cur_milli_time; 
		}
		else if(id == 4)
		{
			tick_last4 = tick_cur;
			
			delta_mins_time = (cur_milli_time-last_milli_time4)/60000.0;
		last_milli_time4 = cur_milli_time; 
		}
		
		rpm = tick_inc/1500.0/delta_mins_time;
		return rpm;
//		print_usart("tick %f, %d, %f\r\n " ,rpm,tick_cur,delta_mins_time);//串口输出

		

		
//	motor_control(pwm_motor);
	
}

void motor_control(GPIO_TypeDef* GPIOx1, uint16_t GPIO_Pin1,GPIO_TypeDef* GPIOx2, uint16_t GPIO_Pin2,uint8_t id,int pwm_motor)
{
	if(pwm_motor>0)
	{
		HAL_GPIO_WritePin(GPIOx1,GPIO_Pin1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx2,GPIO_Pin2,GPIO_PIN_SET);
	}
	
	if(pwm_motor<0)
	{
		HAL_GPIO_WritePin(GPIOx2,GPIO_Pin2,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx1,GPIO_Pin1,GPIO_PIN_SET);
		pwm_motor = -pwm_motor;
	}
	if(-10<pwm_motor&&pwm_motor<10)
		pwm_motor = 0;
	if(id == 1)
			TIM8->CCR1 =  pwm_motor*10;
	else if(id == 2)
	{
		TIM8->CCR2 = pwm_motor*10;
	}
	else if(id == 3)
			TIM8->CCR3 = pwm_motor*10;
	else if(id == 4)
			TIM8->CCR4 = pwm_motor*10;
	
}
/*
void steer_control(int pwm_steer)
{
			TIM2->CCR1=pwm_steer;//舵机输出测试
}
*/
void MotorParamInit(struct Motor *motor, float speedP, float speedI, float speedD,int speedOutmax, float positionP, float positionI, float positionD, int positionOutmax,int errormax)
{
	motor->speed_pid.KP = speedP;
	motor->speed_pid.KI = speedI;
	motor->speed_pid.KD = speedD;
	motor->speed_pid.outputMax = speedOutmax;
	motor->position_pid.KP = positionP;
	motor->position_pid.KI = positionI;
	motor->position_pid.KD = positionD;
	motor->position_pid.outputMax = positionOutmax;
	motor->speed_pid.errorMax = errormax;
}


/*
		if(flag == 1)
		{
			HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_10);
			HAL_GPIO_TogglePin(GPIOI,GPIO_PIN_10);
			flag =0;
		}
*/
