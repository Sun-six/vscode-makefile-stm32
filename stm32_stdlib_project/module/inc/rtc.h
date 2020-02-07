#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"
#include "sys.h"
#include <stdio.h>
#include "led.h"

struct tm
{
  uint8_t tm_sec;	//��
  uint8_t tm_min;	//��
  uint8_t tm_hour;	//ʱ
  uint8_t tm_mday;	//��
  uint8_t tm_mon;	//��
  uint32_t tm_year;	//��
};
extern struct tm RTC_Time;
void RTC_NVIC_Config(uint32_t PreemptPriority, uint32_t SubPriority,u8 Alarm_flag);
void RTC_Config(void);


u32 NormTime_SecTime(struct tm *structTime);
void SecTime_NormTime(u32 time,struct tm *structTime);
void RTC_GetTime(struct tm *structTime);
void RTC_SetTime(struct tm *structTime);
void RTC_PutTime(void);

#endif
