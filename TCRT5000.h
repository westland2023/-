#ifndef __TCRT5000_H
#define __TCRT5000_H

typedef enum
{
    TCRT5000_Error_Left8 = -13,
    TCRT5000_Error_Left7 = -11,
    TCRT5000_Error_Left6 = -9,
    TCRT5000_Error_Left5 = -7,
    TCRT5000_Error_Left4 = -5,
    TCRT5000_Error_Left3 = -3,
    TCRT5000_Error_Left2 = -2,
    TCRT5000_Error_Left1 = -1,
    TCRT5000_Error_Center = 0,
    TCRT5000_Error_Right1 = 1,
    TCRT5000_Error_Right2 = 2,
    TCRT5000_Error_Right3 = 3,
    TCRT5000_Error_Right4 = 5,
    TCRT5000_Error_Right5 = 7,
    TCRT5000_Error_Right6 = 9,
    TCRT5000_Error_Right7 = 11,
    TCRT5000_Error_Right8 = 13,
		TCRT5000_Error_All = 14
} TCRT5000_ErrorTypeDef;

typedef struct
{
    uint8_t TCRT5000_DATA[9];
    TCRT5000_ErrorTypeDef ERROR;
} TCRT5000_ReadDataDef;

void TCRT5000_Init(void);
TCRT5000_ReadDataDef TCRT5000_read(void);
void TCRT5000_ErrorCalc(TCRT5000_ReadDataDef* TCRT5000_ReadData);
char* TCRT5000_ErrorString(TCRT5000_ErrorTypeDef TCRT5000_ErrorType);

#endif
