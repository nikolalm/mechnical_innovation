#include "user.h"
/*

电机PWM,TIM8 CH 1,2,3,4 PI5,6, pc8,9
电机Encode，TIM2,3,4,5
MOTOR1 TIM2,PA1,PA5
MOTOR2 TIM3 PA6,PC7
MOTOR3 TIM4 Pb6,PD13
MOTOR4 TIM5 ,PH11 PA0

电机方向引脚
PD11, PD5
PI3 PB12
PC6,PC2
PC4,PC5


*/
int test_fb=0;
int test_lr=0;
int test_ro=0;

uint8_t AT_INIT[] = "AT+CWMODE=1";
uint8_t AT_RST[] = "AT+RST";
uint8_t AT_WIFI[] = "AT+CWJAP=\"LAP\",\"lm5201314\"";
uint8_t AT_CON[] = "AT+CIPSTART=\"TCP\",\"192.168.134.128\",5000";

//int position_delta_test = 0;
//int pwm_steer =0;

struct Chassis_t chassis;
	
void user_sys_init(void)
{
	//电机编码器定时器开启
	HAL_TIM_Encoder_Start(&TIM_MOTOR1_ENCODE,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&TIM_MOTOR2_ENCODE,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&TIM_MOTOR3_ENCODE,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&TIM_MOTOR4_ENCODE,TIM_CHANNEL_ALL);
	
		HAL_TIM_IC_Start_IT(&htim9,TIM_CHANNEL_1) ;
	//电机pwm定时器开启
	HAL_TIM_PWM_Start(&TIM_MOTOR1_PWM, TIM_CHANNEL_MOTOR1);
	HAL_TIM_PWM_Start(&TIM_MOTOR2_PWM, TIM_CHANNEL_MOTOR2);
	HAL_TIM_PWM_Start(&TIM_MOTOR3_PWM, TIM_CHANNEL_MOTOR3);
	HAL_TIM_PWM_Start(&TIM_MOTOR4_PWM, TIM_CHANNEL_MOTOR4);
	
//	HAL_TIM_PWM_Start(&TIM_STEER, TIM_CHANNEL_1);//舵机pwm定时器开启
	
//	HAL_UART_Receive_DMA(&WIFI_UART, wifi_data, 24u);//串口dma接收开器

	__HAL_UART_ENABLE_IT(&WIFI_UART, UART_IT_IDLE);//设置中断为空闲中断模式
	HAL_UART_Receive_DMA(&WIFI_UART, wifi_data, 26u);//
	
	HAL_UART_Transmit(&WIFI_UART,AT_INIT,sizeof(AT_INIT),0xfffff);
	HAL_UART_Transmit(&WIFI_UART,AT_RST,sizeof(AT_RST),0xfffff);
	HAL_UART_Transmit(&WIFI_UART,AT_WIFI,sizeof(AT_WIFI),0xfffff);
	HAL_UART_Transmit(&WIFI_UART,AT_CON,sizeof(AT_CON),0xfffff);
	
	
	memset(&Motor_1,0,sizeof(Motor_1));       //清空
	memset(&Motor_2,0,sizeof(Motor_2));
	memset(&Motor_3,0,sizeof(Motor_3));
	memset(&Motor_4,0,sizeof(Motor_4));
	
	Motor_1.id = 1;
	Motor_2.id = 2;
	Motor_3.id = 3;
	Motor_4.id = 4;
	
	
	MotorParamInit(&Motor_1,0.5,0.5,0.35,100,0,0,0,0,500);     //初始化1.2 0.3 0 20
	MotorParamInit(&Motor_2,0.5,0.5,0.35,100,0,0,0,0,500);
	MotorParamInit(&Motor_3,0.5,0.5,0.35,100,0,0,0,0,500);//1.3
	MotorParamInit(&Motor_4,0.5,0.5,0.35,100,0,0,0,0,500);
}

void Robot_task(void)
{
	user_sys_init();
	while(1)
	{
/*
		//wifi模块接收底盘速度信息
		data_receive();	
		//	
*/		
		/*
		//位置环
		Motor_1.position_pid.ref = position_delta_test+__HAL_TIM_GET_COUNTER(&TIM_MOTOR1_ENCODE);;  //更新电机的位置环设定值（由结构体infantry决定）
	  Motor_1.position_pid.fdb =  __HAL_TIM_GET_COUNTER(&TIM_MOTOR1_ENCODE);
		PID_Calc(&Motor_1.position_pid);
		//速度环
		Motor_1.speed_pid.ref = Motor_1.position_pid.output;
		Motor_1.speed_pid.fdb = motor_feedback();
		PID_Calc(&Motor_1.speed_pid);//p:10,i:0~0.2
	*/
		if(remote_ok == 1&&remote[5]>2700)
		{
			chassis.FBSpeed = (remote[2]-2250)/12;
			chassis.LRSpeed = (remote[1]-2250)/12;
			chassis.RotateAngle = (remote[4]-2250)/12;	
		}
		else 
		{
   		chassis.FBSpeed = - Remote.FBSpeed;
			chassis.LRSpeed = Remote.LRSpeed;
			//chassis.RotateAngle = Remote.RotateAngle;
		}


//		chassis.FBSpeed = test_fb;
//		chassis.LRSpeed = test_lr;
//		chassis.RotateAngle = test_ro;
		
		Motor_1.speed_pid.ref = chassis.FBSpeed - chassis.LRSpeed + chassis.RotateAngle;
		Motor_2.speed_pid.ref = chassis.FBSpeed + chassis.LRSpeed + chassis.RotateAngle;
		Motor_3.speed_pid.ref = -chassis.FBSpeed + chassis.LRSpeed + chassis.RotateAngle;
		Motor_4.speed_pid.ref = -chassis.FBSpeed - chassis.LRSpeed + chassis.RotateAngle;

//		Motor_4.speed_pid.fdb = motor_feedback();

		Motor_1.speed_pid.fdb = motor_feedback(Motor_1.id,&TIM_MOTOR1_ENCODE);
		Motor_2.speed_pid.fdb = motor_feedback(Motor_2.id,&TIM_MOTOR2_ENCODE);
		Motor_3.speed_pid.fdb = motor_feedback(Motor_3.id,&TIM_MOTOR3_ENCODE);
		Motor_4.speed_pid.fdb = motor_feedback(Motor_4.id,&TIM_MOTOR4_ENCODE);

		PID_Calc(&Motor_1.speed_pid);//p:10,i:0~0.2
		PID_Calc(&Motor_2.speed_pid);//p:10,i:0~0.2
		PID_Calc(&Motor_3.speed_pid);//p:10,i:0~0.2
		PID_Calc(&Motor_4.speed_pid);//p:10,i:0~0.2
		
		//		Motor_1.speed_pid.output = speed_ref_test; //电机输出测试
		
		motor_control(GPIO_MOTOR1_AIN1,GPIO_PIN_MOTOR1_AIN1,GPIO_MOTOR1_AIN2,GPIO_PIN_MOTOR1_AIN2,Motor_1.id, Motor_1.speed_pid.output);
		motor_control(GPIO_MOTOR2_AIN1,GPIO_PIN_MOTOR2_AIN1,GPIO_MOTOR2_AIN2,GPIO_PIN_MOTOR2_AIN2,Motor_2.id, Motor_2.speed_pid.output);
		motor_control(GPIO_MOTOR3_AIN1,GPIO_PIN_MOTOR3_AIN1,GPIO_MOTOR3_AIN2,GPIO_PIN_MOTOR3_AIN2,Motor_3.id, Motor_3.speed_pid.output);
		motor_control(GPIO_MOTOR4_AIN1,GPIO_PIN_MOTOR4_AIN1,GPIO_MOTOR4_AIN2,GPIO_PIN_MOTOR4_AIN2,Motor_4.id, Motor_4.speed_pid.output);
		
		//		steer_control(pwm_steer); //舵机
		HAL_Delay(100);
	}
}

/*
void led_shine(void)
{
	HAL_GPIO_TogglePin(GPIO_LED,GPIO_PIN_LED);
	HAL_Delay(100);
}
*/
