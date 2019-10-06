//接收wifi模块的数据
#include "user.h"
#include "string.h"
#include "stdlib.h"
uint8_t rc_data[16];
uint8_t wifi_data[26];
struct RosRemote Remote;
	int index = 0;
		int first_index = 0;
/*
void WIFI_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart2)
  {
//    HAL_UART_Receive_DMA(&WIFI_UART, wifi_data, 24u);
		
    WIFI_RECEIVE();
  }

}
*/
void WIFI_RECEIVE(void)
{
	if(__HAL_UART_GET_FLAG(&WIFI_UART, UART_FLAG_IDLE))     //openMV串口空闲中断
	{
		__HAL_UART_CLEAR_IDLEFLAG(&WIFI_UART);     //清空IDLE标志位
		USART2->SR;      //清空SR寄存器
		USART2->DR;      //清空DR寄存器
		__HAL_DMA_CLEAR_FLAG(&WIFI_UART, DMA_FLAG_TCIF1_5);    //清空DMA传输完成标志位
		HAL_UART_DMAStop(&WIFI_UART);
		RemoteReceiveHandle();
		for(int i =0;i<26;i++)
			wifi_data[i] = 0;
		HAL_UART_Receive_DMA(&WIFI_UART,wifi_data,26u);		//再次使能接收，NDTR重载
	}
}


//uint8_t。		if(GPS_begin == '$')
void RemoteReceiveHandle(void)
{

		for(int i = 0; i<26;i++)
		{
			if(wifi_data[i] == 64)
			{
					first_index = i;
			}		
		}
		rc_data[0] = wifi_data[first_index];	
		index = first_index;
		for(int i = 1; i<16;i++)
		{
			index ++;
			if(index == 26)
				index =0;
			
			rc_data[i] = wifi_data[index];
		}
		
	if(rc_data[0] == 64)
	{			 
//		Remote.FBSpeed = stringTOint(rc_data+2);
		for(int i = 0; i<16;i++)
		{
			if (rc_data[i] == 'x')
				 Remote.LRSpeed= -stringTOint(rc_data+i);
			else if(rc_data[i] == 'y')
				Remote.FBSpeed = stringTOint(rc_data+i);
			else if(rc_data[i] == 'r')
				Remote.RotateAngle = stringTOint(rc_data+i);
		}
		
		
	}

	
}

int stringTOint(uint8_t *str)
{
	int i=0,j=0,length=0,number=0,signal = 0;
	for(i=0;i<10;i++)
	{
		if((str[i]>=48&&str[i]<=57)||(str[i] == '-'&&(str[i+1]>=48&&str[i+1]<=57)))
		{
			signal = i;
			break;
		}
	}
	for(i=signal;;i++)
	{
		if(i==signal&&str[i]=='-')
		{
			j=1;
			continue;
		}
		if(str[i]>=48&&str[i]<=57)
			length++;
		else
			break;
	}
	for(i=signal+j;i<signal+length+j;i++)
		number = number * 10 + (str[i]-48);
	if(j==1)
		number = -number;
	
	return number;
}

