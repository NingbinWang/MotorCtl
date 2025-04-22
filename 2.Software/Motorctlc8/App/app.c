#include "app.h"
#include "config.h"
#include "canwork.h"
#include "motor_gm37.h"
#include "motor_gm37hallencoder.h"
#include "ssd1306.h"
#include <string.h>
int i = 0;
void App_Init()
{

	MotorGM37APWMStart();
	MotorGM37_Hallencoder_Start();
	MotorGM37ASetSpeed(0, 1000);
	ssd1306_Init();
}

void App_Show()
{
	char showcount[256];
	sprintf(showcount,"gma %d %d %d",i++,MotorGM37_GetEncodeValue(0),MotorGM37_GetSendWheelSpeed(0));
	if(i == 100)
		i = 0;
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString(showcount,Font_7x10,White);
	ssd1306_UpdateScreen();
}
