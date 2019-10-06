#ifndef PID_H_
#define PID_H_

struct PID_t
{
	float KP;
	float KI;
	float KD;
	int error[2];
  int errorMax;
  int error_sum;
	int fdb;
	int ref;
	int output;
	int outputMax;
};

#define DEFAULT_PID \
{0,0,0,{0,0},20,0,0,0,0,100 \
}

void PID_Calc(struct PID_t *pid);

#endif
