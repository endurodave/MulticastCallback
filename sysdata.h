#ifndef _SYSDATA_H
#define _SYSDATA_H

#include "multicast.h"

typedef enum
{
	MODE_STARTING,
	MODE_NORMAL,
	MODE_ALARM
} ModeType;

// Publisher declares a multicast callback called SysData_ModeCallback. 
// Subscribers register for callbacks with function signature: void MyFunc(ModeType mode)
MULTICAST1_DECLARE(SysData_SetModeCallback, ModeType)

// Set a new system mode and callback any clients registered with SysData_ModeCallback.
void SysData_SetMode(ModeType mode);

#endif // _SYSDATA_H