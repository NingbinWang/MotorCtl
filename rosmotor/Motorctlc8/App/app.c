#include "app.h"
#include "config.h"
#include "canwork.h"
#include  "motor_gm37.h"
#include  "ssd1315_oled.h"
#include   "ssd1306.h"
#include <string.h>

void App_Init()
{
	MotorGM37APWMStart();
	MotorGM37ASetSpeed(0, 1000);
	ssd1306_Init();
	ssd1306_WriteString("hello world",Font_7x10,White);
	ssd1306_UpdateScreen();
	//Wifi_UserInit();
}

