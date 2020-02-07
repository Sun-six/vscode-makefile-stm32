#include "rtc.h"

//������·�
static int mon_r[12]={31,29,31,30,31,30,31,31,30,31,30,31};
//ƽ����·�
static int mon_p[12]={31,28,31,30,31,30,31,31,30,31,30,31};

//RTC��׼ʱ��ṹ��
struct tm RTC_Time;

//RTC��������
void RTC_Config(void)
{
	//ʹ�ܵ�Դʱ�Ӻͱ�������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE); //ʹ�� RTC �ͺ󱸼Ĵ������� 
	BKP_DeInit();//��λ�������� 
	RCC_LSEConfig(RCC_LSE_ON);// �����ⲿ�������� 
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//�ȴ�ʱ�Ӿ���
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //ѡ�� LSE ��Ϊ RTC ʱ�� 
	RCC_RTCCLKCmd(ENABLE); //ʹ�� RTC ʱ��
	
	RTC_WaitForSynchro();//�ȴ�ͬ��
	RTC_WaitForLastTask();//�ȴ���һ���������
	
	RTC_SetPrescaler(32767);//���÷�Ƶֵ
	RTC_WaitForLastTask();//�ȴ���һ���������
	RTC_SetCounter(0);//���ü�����
	RTC_WaitForLastTask();//�ȴ���һ���������
	RTC_SetAlarm(60);//��������ʱ��
	RTC_WaitForLastTask();//�ȴ���һ���������
}
//RTC�ж�����
void RTC_NVIC_Config(uint32_t PreemptPriority, uint32_t SubPriority ,u8 Alarm_flag)
{
	RTC_ITConfig(RTC_IT_SEC,ENABLE);
	RTC_WaitForLastTask();//�ȴ���һ���������
	if(Alarm_flag)
	{
		RTC_ITConfig(RTC_IT_ALR,ENABLE);
		RTC_WaitForLastTask();//�ȴ���һ���������
	}
	STM32_NVIC_SetPriority(RTC_IRQn,PreemptPriority,SubPriority);
}
//�����ж�
u32 RTC_GetYearState(u32 year)
{
	if((year%4==0 && year%100!=0) || year%400==0)
	{
		return 1;
	}
	return 0;
}
//��ʱ��ת��׼ʱ��
void SecTime_NormTime(u32 time,struct tm *structTime)
{	//ת������
	u32 i;
	structTime->tm_year=1970; //��׼���

	/*1. ���㵱ǰ�����*/
	while(1)
	{
		if(RTC_GetYearState(structTime->tm_year))
		{
			if(time >= 366*24*60*60) //��һ��
			{
				time-=366*24*60*60;
				structTime->tm_year++;
			}
			else break;
		}
		else
		{
			if(time>=365*24*60*60) //��һ��
			{
				time-=365*24*60*60;
				structTime->tm_year++;
			}
			else break;
		}
	}

	/*2. ���㵱ǰ���·�*/

	structTime->tm_mon=1;
#if 0	//��ʱδ�Ż����
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

	/*3. ���㵱ǰ������*/
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

	/*4. ���㵱ǰ��Сʱ*/
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

	/*5. ���㵱ǰ�ķ���*/
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

	/*6. ���㵱ǰ����*/
	structTime->tm_sec=time;
}
#endif
//��׼ʱ��ת��ʱ��
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
//��ȡʱ��
void RTC_GetTime(struct tm *structTime)
{
	u32 Ttime=0;
	Ttime=RTC_GetCounter();
	SecTime_NormTime(Ttime,structTime);
}
//����ʱ��
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


//RTC�жϴ�����
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
