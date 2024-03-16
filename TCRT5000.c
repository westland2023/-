#include "stm32f10x.h"                  // Device header
#include "TCRT5000.h"


void TCRT5000_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB的时钟

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;                       //GPIO输入模式为浮空输入，可读取寻迹模块输入的高低电平，引脚不存在悬空。
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12; //PA2-PA7引脚IO口
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;                                                //引脚输出速率
    GPIO_Init(GPIOA,&GPIO_InitStruct);																													//初始化PA2-PA7引脚，用于读取寻迹模块输入的高低电平

    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;                                            //GPIO输入模式为浮空输入，可读取寻迹模块输入的高低电平，引脚不存在悬空。
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;                                 //PB0、PB1、PB10引脚IO口
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;																								//引脚输出速率
    GPIO_Init(GPIOB,&GPIO_InitStruct);																													//初始化B0、PB1、PB10引脚，用于读取寻迹模块输入的高低电平
}

//红外传感器识别到黑线返回数字信号低电平0，未识别到黑线返回高电平1
TCRT5000_ReadDataDef TCRT5000_read(void)
{
    TCRT5000_ReadDataDef TCRT5000_ReadData;
    TCRT5000_ReadData.TCRT5000_DATA[0] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11); //左四寻迹模块
    TCRT5000_ReadData.TCRT5000_DATA[1] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12); //左三寻迹模块
    TCRT5000_ReadData.TCRT5000_DATA[2] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5); //左二寻迹模块
    TCRT5000_ReadData.TCRT5000_DATA[3] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6); //左一寻迹模块

    TCRT5000_ErrorCalc(&TCRT5000_ReadData);

    return TCRT5000_ReadData;
}

void TCRT5000_ErrorCalc(TCRT5000_ReadDataDef* TCRT5000_ReadData)
{
    //中间态
    if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Center;
    }
    //左一态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Left1;
    }
    //左二态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Left2;
    }
    //左三态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Left3;
    }
    //左四态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Left4;
    }
    //左五态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Left5;
    }
    //左六态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Left6;
    }
    //左七态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Left7;
    }
    //左八态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Left8;
    }
    //右一态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Right1;
    }
    //右二态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Right2;
    }
    //右三态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Right3;
    }
    //右四态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Right4;
    }
    //右五态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Right5;
    }
    //右六态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==1)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Right6;
    }
    //右七态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==0
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==0)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Right7;
    }
    //右八态
    else if(TCRT5000_ReadData->TCRT5000_DATA[0]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[1]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[2]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[3]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[4]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[5]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[6]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[7]==1
            &&TCRT5000_ReadData->TCRT5000_DATA[8]==0)
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_Right8;
    }
    else
    {
        TCRT5000_ReadData->ERROR  = TCRT5000_Error_All;
    }
}

char* TCRT5000_ErrorString(TCRT5000_ErrorTypeDef TCRT5000_ErrorType)
{
		if(TCRT5000_ErrorType == TCRT5000_Error_All)
		{
				return "All   ";
		}		
		else if(TCRT5000_ErrorType == TCRT5000_Error_Center)
		{
			  return "Center";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Left1)
		{
				return "Left1 ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Left2)
		{
				return "Left2 ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Left3)
		{
				return "Left3 ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Left4)
		{
				return "Left4   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Left5)
		{
				return "Left5   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Left6)
		{
				return "Left6   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Left7)
		{
				return "Left7   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Left8)
		{
				return "Left8   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Right1)
		{
				return "Right1   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Right2)
		{
				return "Right2   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Right3)
		{
				return "Right3   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Right4)
		{
				return "Right4   ";
		}
		else if(TCRT5000_ErrorType == TCRT5000_Error_Right5)
		{
				return "Right5   ";
		}	
		else if(TCRT5000_ErrorType == TCRT5000_Error_Right6)
		{
				return "Right6   ";
		}	
		else if(TCRT5000_ErrorType == TCRT5000_Error_Right7)
		{
				return "Right7   ";
		}	
		else if(TCRT5000_ErrorType == TCRT5000_Error_Right8)
		{
				return "Right8   ";
		}	
		return "All   ";
}


