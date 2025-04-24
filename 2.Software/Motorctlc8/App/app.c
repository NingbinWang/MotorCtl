#include "app.h"
#include "config.h"
#include "canwork.h"
#include "motor_gm37.h"
#include "motor_gm37hallencoder.h"
#include "ssd1306_conf.h"
#include "lsm6ds3tr_conf.h"
#include <string.h>

void App_IMU_Init()
{

}


void App_Init()
{

	MotorGM37APWMStart();
	MotorGM37_Hallencoder_Start();
	MotorGM37ASetSpeed(1, 1000);
	lsm6dstr_Init();
#if SSD1306_ENABLE
	ssd1306_Init();
#endif
}

void App_Show()
{
#if SSD1306_ENABLE
	char showcount[256];
	ssd1306_SetCursor(0,0);
	sprintf(showcount,"speed %d %d",MotorGM37_GetSendWheelSpeed(0),MotorGM37_GetSendWheelSpeed(1));
	ssd1306_WriteString(showcount,Font_7x10,White);
	ssd1306_UpdateScreen();
#endif
}



void App_Func()
{
	MotorGM37_UpdateSpeed();
	App_Show();
}




