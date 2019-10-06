#include "user.h"

/**
* @brief PID计算函数。本PID增量式PID，未设置死区
* @param PID结构体
* @retval None
*/

void PID_Calc(struct PID_t *pid)
{
	pid->error[0] = pid->error[1];
	pid->error[1] = pid->ref - pid->fdb;
  pid->error_sum += pid->error[1];
  if(pid->error_sum > pid->errorMax) pid->error_sum = pid->errorMax;
  if(pid->error_sum < -pid->errorMax) pid->error_sum = -pid->errorMax;

	pid->output = pid->KP*(pid->error[1]) + pid->KI*pid->error_sum+pid->KD*(pid->error[1]-pid->error[0]);
  
  /* 输出上限 */
	if(pid->output > pid->outputMax) pid->output = pid->outputMax;
	if(pid->output < -pid->outputMax) pid->output = -pid->outputMax;
}
