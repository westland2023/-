#include "stm32f10x.h"                  
#include "Delay.h"
#include "OLED.h"
#include <Motor.h>
#include <Serial.h>
#include <stdio.h>
#include <stdarg.h>
#include <TCRT5000.h>
#include <COUNTSENSOR.H>
#include <mpu6050.h>
#include <math.h>		

int8_t SpeedL;
int8_t SpeedR;
int8_t GRD;
int32_t x=-1,y=-1;
int32_t x1,y1;
int8_t Mode;
int32_t i=0;
uint8_t ID;
int16_t AX, AY, AZ, GX, GY, GZ;
float  count=0;
uint32_t exit_flag=1,turn_flag=1,turn_start=1,exit9_flag=0;//start if flag==0
uint32_t dt=1010;

struct team
{
   double coordinate1[2];  
   double coordinate2[2];
   double coordinate3[2];
   double coordinate4[2];
	 double coordinate5[2];
}coordiante ;  

void Turnleft(void)
{
  __disable_irq();
	turn_start=0;
  while(turn_flag==1){ OLED_ShowString(3, 2, "turn_flag=1");  }
	if(turn_flag==0)
  {
	  exit_flag=1;
	  OLED_ShowString(4, 2, "turn left");  
   	SpeedL=-30;
    SpeedR=30;	
		Motor_SetSpeedL1(SpeedL);				
	  Motor_SetSpeedR1(SpeedR);
  	Motor_SetSpeedL2(SpeedL);				
	  Motor_SetSpeedR2(SpeedR);
		Delay_ms (300);
  	while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0)
		{
		}
		Delay_ms (300);
   	SpeedL=0;
    SpeedR=0;
	  Motor_SetSpeedL1(SpeedL);				
    Motor_SetSpeedR1(SpeedR);
	  Motor_SetSpeedL2(SpeedL);				
	  Motor_SetSpeedR2(SpeedR);
		exit_flag=0;
		turn_flag=1;
		__enable_irq();
	} 
	turn_start=1;
}	
void Turnright(void)
{
	__disable_irq();
	turn_start=0;
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0&&GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
  { 
	OLED_ShowString(3, 2, "turn_flag=1");  
	}
	if(turn_flag==0)
  {
	  exit_flag=1;
	  OLED_ShowString(4, 2, "turn right");  
   	SpeedL=30;
    SpeedR=-30;	                                                                                         
  	Motor_SetSpeedL1(SpeedL);				
  	Motor_SetSpeedR1(SpeedR);
	  Motor_SetSpeedL2(SpeedL);	 			
	  Motor_SetSpeedR2(SpeedR);
		Delay_ms (300);
	  while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
    {
		}
		Delay_ms (300);
    SpeedL=0;
    SpeedR=0;
	  Motor_SetSpeedL1(SpeedL);				
    Motor_SetSpeedR1(SpeedR);
	  Motor_SetSpeedL2(SpeedL);				
	  Motor_SetSpeedR2(SpeedR);
		exit_flag=0;
		turn_flag=1;
		__enable_irq();
	}
	turn_start=1;
}
void Turnaround(void)
{	
  __disable_irq();
	turn_start=0;
  while(turn_flag==1){ OLED_ShowString(3, 2, "turn_flag=1");  }
	if(turn_flag==0)
  {
	  exit_flag=1;
	  OLED_ShowString(4, 2, "turn around");  
   	SpeedL=-30;
    SpeedR=30;	
	  Motor_SetSpeedL1(SpeedL);				
  	Motor_SetSpeedR1(SpeedR);
	  Motor_SetSpeedL2(SpeedL);				
	  Motor_SetSpeedR2(SpeedR);
		Delay_ms (300);
	  while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0)
		{
		}
		Delay_ms (300);
	  while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0)
	  {
  	}
	 	Delay_ms (300);
   	SpeedL=0;
    SpeedR=0;
	  Motor_SetSpeedL1(SpeedL);				
    Motor_SetSpeedR1(SpeedR);
	  Motor_SetSpeedL2(SpeedL);				
	  Motor_SetSpeedR2(SpeedR);
		exit_flag=0;
		turn_flag=1;
		__enable_irq();
	}
  turn_start=1;
}
void Straight_For_1(void)
{	  
	  
  	exit_flag=0;
	  OLED_ShowString(4, 2, "go straight");  
    SpeedL=40;
    SpeedR=40;		 
	  Motor_SetSpeedL1(SpeedL);				
  	Motor_SetSpeedR1(SpeedR);
	  Motor_SetSpeedL2(SpeedL);				
	  Motor_SetSpeedR2(SpeedR);
	  /*
	  Delay_ms(1000);
	  turn_flag =1;
  	while(turn_flag==1){OLED_ShowString(1, 2, "flag=1,in sf1");}
	  exit_flag=1;
   	SpeedL=0;
    SpeedR=0;	
    Motor_SetSpeedL1(SpeedL);				
  	Motor_SetSpeedR1(SpeedR);
	  Motor_SetSpeedL2(SpeedL);				
	  Motor_SetSpeedR2(SpeedR);        
    */	
    OLED_Clear ();    
		OLED_ShowString(1, 2, "out sf1");
}
void stop (void)
{
	  SpeedL=0;
    SpeedR=0;	
    Motor_SetSpeedL1(SpeedL);				
  	Motor_SetSpeedR1(SpeedR);
	  Motor_SetSpeedL2(SpeedL);				
	  Motor_SetSpeedR2(SpeedR);      
}

void EXTI15_10_IRQHandler(void)
{
	exit9_flag=1;
	if (EXTI_GetITStatus(EXTI_Line11|EXTI_Line12) == SET  && exit_flag==0)
	{
		OLED_ShowString(4,2,"in et1510");
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 1)
		{
			SpeedL=30;
      SpeedR=50;		 
	    Motor_SetSpeedL1(SpeedL);				
    	Motor_SetSpeedR1(SpeedR);
	    Motor_SetSpeedL2(SpeedL);				
	    Motor_SetSpeedR2(SpeedR);
		}
  	else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 1)
		{
			SpeedL=50;
      SpeedR=30;		 
	    Motor_SetSpeedL1(SpeedL);				
    	Motor_SetSpeedR1(SpeedR);
	    Motor_SetSpeedL2(SpeedL);				
	    Motor_SetSpeedR2(SpeedR);
		}
  }	
	EXTI_ClearITPendingBit(EXTI_Line11);
  EXTI_ClearITPendingBit(EXTI_Line12);
	OLED_Clear();
}


void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5|EXTI_Line6) == SET)
	{
		OLED_ShowString(4,2,"in et95");
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0||GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
		{
		  turn_flag=0;
			OLED_ShowString(2, 2, "turn_flag=0");
      if(turn_start==0)  
      {
		    stop(); 
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
  EXTI_ClearITPendingBit(EXTI_Line6);
	OLED_Clear();
}

void coordiante_transform(int32_t coor,double *s[5])
{
	  int32_t m;
		if(*s[coor+1]>*s[coor])
		{ 
		  Turnright();
		  m=*s[coor+1]-*s[coor];
		  while(m--)  Straight_For_1();
			Turnleft();
		}
		else
		{
			Turnleft();
	   	m=*s[coor]-*s[coor+1];
	  	while(m--)  Straight_For_1();
			Turnright();
		}
		
		if(*(s[coor+1]+1)>*(s[coor]+1))  
	  {
	  	m=*(s[coor+1]+1)-*(s[coor]+1);
	  	while(m--)  Straight_For_1();
  	}
  	else   
	  {
	  	Turnaround();
	  	m=*(s[coor]+1)-*(s[coor+1]+1);
	  	while(m--)  Straight_For_1();
			Turnaround();
  	}
}	
	
void Mode1_old(void)
{ 
	OLED_Clear();
	i=3;
	x=-1;	
	y=-1;
	while(i--)
    {
			OLED_Clear();
			OLED_ShowNum(1,8,i,1);
	    OLED_ShowString(1, 2, "Mode1");
			OLED_ShowString(2, 2, "set coordinate");  
			GRD=-1;
			while(x<0)
			{
			  if (Serial_GetRxFlag() == 1)
        {
	       	GRD = Serial_GetRxData(); 
	  	    x=GRD-48;
        }
	  	}
			OLED_ShowNum(3,2,x,1);
			GRD=-1;
      while(y<0)
	  	{
			  if (Serial_GetRxFlag() == 1)
	      {
	       	GRD = Serial_GetRxData(); 
	  	    y=GRD-48;
        }
	   	}
			OLED_ShowNum(3,4,y,1);
			OLED_ShowString(4, 2, "operating       ");
			x1=x;
			y1=y;
			while(x1--)
			{
				Delay_ms (x1*1300);
			  Straight_For_1();
  			OLED_ShowNum(3,2,x1,1);		
			}
			Turnleft();
			while(y1--)
			{
	  		Straight_For_1();
 	  		OLED_ShowNum(3,4,y1,1);
		  }
			
			Delay_ms(2000);
			x1=x;
			y1=y;
	  	OLED_ShowNum(3,2,y,1);
	  	OLED_ShowNum(3,4,x,1);
			Turnaround();
			while(y1--)
			{
				Straight_For_1();
 				OLED_ShowNum(3,2,y1,1);		
			}
				Turnright();
			while(x1--)
			{
				Straight_For_1();
			  OLED_ShowNum(3,4,x1,1);		
			}
			Turnaround();
			OLED_ShowString(4, 2, " operated         ");
			Delay_ms(1000);
		}
		OLED_Clear();
}
void Mode2(void)
{
	i=0;
	while(i<2)
	{
		OLED_ShowString(1, 1, " set coordinate1");
	  if (Serial_GetRxFlag() == 1)
		{
		  GRD=-1;
		  GRD=Serial_GetRxData();
			int32_t tem=GRD-48;
		  if(tem>-1)
	  	{	
				coordiante.coordinate1[i]=tem;
	  		i++;
	   	}  
			else OLED_ShowString(1, 1, "error");
	  }
	}
	i=0;
	while(i<2)
	{
		OLED_ShowString(1, 1, " set coordinate2");
	  if (Serial_GetRxFlag() == 1)
		{
		  GRD=-1;
		  GRD=Serial_GetRxData();
			int32_t tem=GRD-48;
		  if(GRD>-1)
	  	{	
				coordiante.coordinate2[i]=tem;
	  		i++;
	   	}  
			else OLED_ShowString(1, 1, "error");
	  }
	}
	i=0;
	while(i<2)
	{
		OLED_ShowString(1, 1, " set coordinate3");
	  if (Serial_GetRxFlag() == 1)
		{
		  GRD=-1;
		  GRD=Serial_GetRxData();
      int32_t tem=GRD-48;
		  if(GRD>-1)
	  	{	
				coordiante.coordinate3[i]=tem;
	  		i++;
	   	}  
			else OLED_ShowString(1, 1, "error");
	  }
	}
	i=0;
	while(i<2)
	{
		OLED_ShowString(1, 1, " set coordinate4");
	  if (Serial_GetRxFlag() == 1)
		{
		  GRD=-1;
		  GRD=Serial_GetRxData();
			int32_t tem=GRD-48;
		  if(GRD>-1)
	  	{	
				coordiante.coordinate4[i]=tem;
	  		i++;
	   	}  
			else OLED_ShowString(1, 1, "error");
	  }
	}
	i=0;
	while(i<2)
	{
		OLED_ShowString(1, 1, " set coordinate5");
	  if (Serial_GetRxFlag() == 1)
		{
		  GRD=-1;
		  GRD=Serial_GetRxData();
			int32_t tem=GRD-48;
		  if(GRD>-1)
	  	{	
				coordiante.coordinate5[i]=tem;
	  		i++;
	   	}  
			else OLED_ShowString(1, 1, "error");
	  }
	}
	
	double *a=coordiante.coordinate1;
	double *b=coordiante.coordinate2;
	double *c=coordiante.coordinate3;
	double *d=coordiante.coordinate4;
	double *e=coordiante.coordinate5;
	double *l[5]={a,b,c,d,e};
	double h[5];
	h[0]=sqrt(pow(coordiante.coordinate1[0],2)+pow(coordiante.coordinate1[1],2));
	h[1]=sqrt(pow(coordiante.coordinate2[0],2)+pow(coordiante.coordinate2[1],2));
	h[2]=sqrt(pow(coordiante.coordinate3[0],2)+pow(coordiante.coordinate3[1],2));
	h[3]=sqrt(pow(coordiante.coordinate4[0],2)+pow(coordiante.coordinate4[1],2));
	h[4]=sqrt(pow(coordiante.coordinate5[0],2)+pow(coordiante.coordinate5[1],2));
	int32_t p=0;
	i=4;
	while(i--)
	{	
		if(h[p]>h[p+1])
		{
			double z=h[p];
			h[p]=h[p+1];
	    h[p+1]=z;		
			double *w=l[p];
			l[p]=l[p+1];
	    l[p+1]=w;		
	    p++;
		}
	}
	int32_t m=*l[0];
	while(m--)  Straight_For_1();
	Turnleft();
	int32_t n=*(l[0]+1);
  while(n--)  Straight_For_1();
	coordiante_transform(1,l);
	coordiante_transform(2,l);
	coordiante_transform(3,l);
	coordiante_transform(4,l);
	Turnaround();
	m=*(l[4]+1);
	while(m--)  Straight_For_1();
	Turnleft();
	m=*l[4];
	while(m--)  Straight_For_1();
	Turnaround();
}
void Mode3(void)
{
	dt=1150;
	Straight_For_1();
	Delay_ms (dt*4);

	Turnleft ();
	dt=1050;
	Straight_For_1();
	Delay_ms (dt*5);

	dt=1080;
	Turnleft ();
  Straight_For_1();
	Delay_ms (dt*3);
	Turnleft ();
	
	Straight_For_1();
	Delay_ms (dt);
	Turnleft ();
	
	Straight_For_1();
	Delay_ms (dt*2);
	
	Turnright();
	Straight_For_1();
	Delay_ms (dt);
	
	Turnright();
	Straight_For_1();
	Delay_ms (dt*2);
	
	Turnleft ();
	Straight_For_1();
	Delay_ms (dt);
	
	Turnleft ();
	Straight_For_1();
	Delay_ms (dt*2);
	
	Turnright();
	Straight_For_1();
	Delay_ms (dt);
	
	Turnright();
	Straight_For_1();
	Delay_ms (dt*2);
	
	Turnleft();
	Straight_For_1();
	Delay_ms (dt);
	
	Turnright();
	Straight_For_1();
	Delay_ms (dt);
}

void Test1(void)
{
	OLED_Clear();
	OLED_ShowString(1, 2,"Testing");
	while(1)
	{ 
		if (Serial_GetRxFlag() == 1)
		{
			GRD = Serial_GetRxData();
			if(GRD=='1')  Straight_For_1();
			else if(GRD=='2')  Turnleft();
			else if(GRD=='3')  Turnright();
			else if(GRD=='4')  Turnaround();
		}
  }
}

void Test2(void)
{ 
	TCRT5000_Init();
	TCRT5000_ReadDataDef  TCRT5000_ReadData = TCRT5000_read();
	while(1)
	{		   
     OLED_ShowNum(1, 1, GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11), 3);
	   OLED_ShowNum(2, 1, GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12), 3);
     OLED_ShowNum(3, 1, GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5), 3);
     OLED_ShowNum(4, 1, GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6), 3);
	}
}
void Test3()
{

}

void Mode1_new()
{
	OLED_Clear();
	i=3;
	x=-1;	
	y=-1;
	while(i--)
    {
			
			OLED_Clear();
			OLED_ShowNum(1,8,i,1);
	    OLED_ShowString(1, 2, "Mode1");
			OLED_ShowString(2, 2, "set coordinate");  
			GRD=-1;while(x<0)
			{
			  if (Serial_GetRxFlag() == 1)
        {
	       	GRD = Serial_GetRxData(); 
	  	    x=GRD-48;
        }
	  	}
			OLED_ShowNum(3,2,x,1);
			GRD=-1;
      while(y<0)
	  	{
			  if (Serial_GetRxFlag() == 1)
	      {
	       	GRD = Serial_GetRxData(); 
	  	    y=GRD-48;
        }
	   	}
			OLED_ShowNum(3,4,y,1);
			OLED_ShowString(4, 2, "operating       ");
			x1=x;
			y1=y;
			Straight_For_1();
			Delay_ms (x1*dt);
			Turnleft ();
			Straight_For_1();
			Delay_ms (y1*dt);
      Turnaround ();
			Straight_For_1();
			Delay_ms (y1*dt);
			Turnright ();
			Straight_For_1();
			Delay_ms (x1*dt);
      Turnaround ();
			OLED_ShowString(4, 2, "finished       ");
		}
}
int main(void)
{
    CountSensorA_Init();
  	CountSensorB_Init();
	  OLED_Init();		
    MPU6050_Init();	
    MotorAll_Init();
	  Serial_Init();
  	while(1)
		{
			OLED_ShowString(1, 2,"Mode selection");
			if (Serial_GetRxFlag() == 1)			
	  	{
	  		Mode=Serial_GetRxData();
	  		if(Mode=='1')  Mode1_old();
	  		else if(Mode=='2')  Mode2();
	    	else if(Mode=='3')  Mode3();
        else if(Mode=='4')  Test1();	
        else if(Mode=='5')  Test2();		
        else if(Mode=='6')  Test3();		
        else if(Mode=='7')  Mode1_new();						
	  	}	
		}
	}
