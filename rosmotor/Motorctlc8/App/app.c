#include "app.h"
#include "config.h"
#include "canwork.h"
#include  "motor_gm37.h"
#include <string.h>

void App_Init()
{
	MotorGM37ASetSpeed(0, 500);
	//Wifi_UserInit();
}

