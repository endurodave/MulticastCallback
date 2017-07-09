#include "sysdata.h"

// Define the multicast callback for up to 3 registered clients
MULTICAST1_DEFINE(SysData_SetModeCallback, ModeType, 3)

static ModeType _mode = MODE_STARTING;

void SysData_SetMode(ModeType mode)
{
	// Update the private _mode value
	_mode = mode;

	// Invoke callbacks on all registered clients
	SysData_SetModeCallback_Invoke(_mode);
}
