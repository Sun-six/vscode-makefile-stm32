#include "rtc.h"

//闰年的月份
static int mon_r[12]={31,29,31,30,31,30,31,31,30,31,30,31};
//平年的月份
static int mon_p[12]={31,28,31,30,31,30,31,31,30,31,30,31};

//RTC标准时间结构体
struct tm RTC_Time;

//RTC基本配置
void RTC_Config(void)
{
	//使能电源时钟和备份区域时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE); //使能 RTC 和后备寄存器访问 
	BKP_DeInit();//复位备份区域 
	RCC_LSEConfig(RCC_LSE_ON);// 开启外部低速振荡器 
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待时钟就绪
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //选择 LSE 作为 RTC 时钟 
	RCC_RTCCLKCmd(ENABLE); //使能 RTC 时钟
	
	RTC_WaitForSynchro();//等待同步
	RTC_WaitForLastTask();//等待上一步操作完成
	
	RTC_SetPrescaler(32767);//设置分频值
	RTC_WaitForLastTask();//等待上一步操作完成
	RTC_SetCounter(0);//设置计数器
	RTC_WaitForLastTask();//等待上一步操作完成
	RTC_SetAlarm(60);//设置闹钟时间
	RTC_WaitForLastTask();//等待上一步操作完成
}
//RTC中断配置
void RTC_NVIC_Config(uint32_t PreemptPriority, uint32_t SubPriority ,u8 Alarm_flag)
{
	RTC_ITConfig(RTC_IT_SEC,ENABLE);
	RTC_WaitForLastTask();//等待上一步操作完成
	if(Alarm_flag)
	{
		RTC_ITConfig(RTC_IT_ALR,ENABLE);
		RTC_WaitForLastTask();//等待上一步操作完成
	}
	STM32_NVIC_SetPriority(RTC_IRQn,PreemptPriority,SubPriority);
}
//闰年判断
u32 RTC_GetYearState(u32 year)
{
	if((year%4==0 && year%100!=0) || year%400==0)
	{
		return 1;
	}
	return 0;
}
//秒时间转标准时间
void SecTime_NormTime(u32 time,struct tm *structTime)
{	//转换函数
	u32 i;
	structTime->tm_year=1970; //基准年份

	/*1. 计算当前的年份*/
	while(1)
	{
		if(RTC_GetYearState(structTime->tm_year))
		{
			if(time >= 366*24*60*60) //够一年
			{
				time-=366*24*60*60;
				structTime->tm_year++;
			}
			else break;
		}
		else
		{
			if(time>=365*24*60*60) //够一年
			{
				time-=365*24*60*60;
				structTime->tm_year++;
			}
			else break;
		}
	}

	/*2. 计算当前的月份*/

	structTime->tm_mon=1;
#if 0	//暂时未优化完成
	for(i=0;i<12;i++)
	{
		if(time >= mon_r[i]*24*60*60)
		{	
			if(i == 1)
			{
				if(RTC_GetYearState(structTime->tm_year))
				{
					time-=mon_r[i]*24*60*60;
				}
				else
				{
					time-=mon_p[i]*24*60*60;
				}
			}
			structTime->tm_mon++;
		}
		else break;		
	}
#else
	for(i=0;i<12;i++)
	{
		if(RTC_GetYearState(structTime->tm_year))
		{
			if(time >= mon_r[i]*24*60*60)
			{
				time-=mon_r[i]*24*60*60;
				structTime->tm_mon++;
			}
			else break;		
		}
		else
		{
			if(time>=mon_p[i]*24*60*60)
			{
				time-=mon_p[i]*24*60*60;
				structTime->tm_mon++;
			}
			else break;	
		}
	}

	/*3. 计算当前的天数*/
	structTime->tm_mday=1;
	while(1)
	{
		if(time>=24*60*60)
		{
			time-=24*60*60;
			structTime->tm_mday++;
		}
		else break;
	}

	/*4. 计算当前的小时*/
	structTime->tm_hour=0;
	while(1)
	{
		if(time>=60*60)
		{
			time-=60*60;
			structTime->tm_hour++;
		}
		else break;
	}

	/*5. 计算当前的分钟*/
	structTime->tm_min=0;
	while(1)
	{
		if(time>=60)
		{
			time-=60;
			structTime->tm_min++;
		}
		else break;
	}

	/*6. 计算当前的秒*/
	structTime->tm_sec=time;
}
#endif
//标准时间转秒时间
u32 NormTime_SecTime(struct tm *structTime)
{
	u32 sectime;
	u32 mon = structTime->tm_mon, year = structTime->tm_year;
	if(0 >= (int)(mon -= 2))
	{
		mon += 12;
		year -= 1;
	}
	sectime=((((unsigned long)
			(year/4 - year/100 + year/400 + 367*mon/12 + structTime->tm_mday) +
			year*365 - 719499
			)*24 + structTime->tm_hour
		)*60 + structTime->tm_min
	)*60 + structTime->tm_sec;
	return sectime;
}
//获取时间
void RTC_GetTime(struct tm *structTime)
{
	u32 Ttime=0;
	Ttime=RTC_GetCounter();
	SecTime_NormTime(Ttime,structTime);
}
//设置时间
void RTC_SetTime(struct tm *structTime)
{
	RTC_SetCounter( NormTime_SecTime(structTime));
	RTC_WaitForLastTask();
}


void RTC_PutTime(void)
{
	RTC_GetTime(&RTC_Time);
	printf("%4lu-%2u-%2u | %2u:%2u:%2u\n",RTC_Time.tm_year,\
									RTC_Time.tm_mon,\
									RTC_Time.tm_mday,\
									RTC_Time.tm_hour,\
									RTC_Time.tm_min,\
									RTC_Time.tm_sec);
}


//RTC中断处理函数
void RTC_IRQHandler(void)
{
	if(RTC_GetFlagStatus(RTC_FLAG_SEC) != RESET)
	{
		RTC_GetTime(&RTC_Time);
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}	
	if(RTC_GetFlagStatus(RTC_FLAG_ALR) != RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);
	}
}
