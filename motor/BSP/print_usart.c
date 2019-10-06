#include "user.h"

void print_usart(const char *format, ...)
{
  static char buf[128];
	va_list ap;
	va_start(ap,format);
	while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX){}
	if(vsprintf(buf,format,ap)>0)
	{
    	HAL_UART_Transmit_DMA(&huart1,(uint8_t *)buf,strlen(buf));  
	}
}
