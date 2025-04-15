#ifndef _CANWORKCONF_H_
#define _CANWORKCONF_H_

#define CANWORK_ENABLE 1
#if (CANWORK_ENABLE)
#include "can.h"
#define canworkhcan &hcan
#endif

#endif
