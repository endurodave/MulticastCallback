#include <stdio.h>
#include "sysdata.h"

// Multicast instances for testing between 2 and 5 arguments
MULTICAST0_DECLARE(MulticastCallback0Args)
MULTICAST0_DEFINE(MulticastCallback0Args, 1)

MULTICAST2_DECLARE(MulticastCallback2Args, int, float)
MULTICAST2_DEFINE(MulticastCallback2Args, int, float, 1)

MULTICAST3_DECLARE(MulticastCallback3Args, int, float, float)
MULTICAST3_DEFINE(MulticastCallback3Args, int, float, float, 1)

MULTICAST4_DECLARE(MulticastCallback4Args, int, float, float, float)
MULTICAST4_DEFINE(MulticastCallback4Args, int, float, float, float, 1)

MULTICAST5_DECLARE(MulticastCallback5Args, int, float, float, float, float)
MULTICAST5_DEFINE(MulticastCallback5Args, int, float, float, float, float, 1)

// Various callback functions
void Callback0Args(void)
{
	printf("Callback0Args:\n");
}

void Callback2Args(int i, float f)
{
	printf("Callback2Args: %d %f\n", i, f);
}

void Callback3Args(int i, float f, float f2)
{
	printf("Callback3Args: %d %f %f\n", i, f, f2);
}

void Callback4Args(int i, float f, float f2, float f3)
{
	printf("Callback4Args: %d %f %f %f\n", i, f, f2, f3);
}

void Callback5Args(int i, float f, float f2, float f3, float f4)
{
	printf("Callback5Args: %d %f %f %f %f\n", i, f, f2, f3, f4);
}

void SysDataCallback1(ModeType mode)
{
	printf("ModeCallback1: %d\n", mode);
}

void SysDataCallback2(ModeType mode)
{
	printf("ModeCallback2: %d\n", mode);
}

int main(void)
{
	// Register with SysData for callbacks
	SysData_SetModeCallback_Register(&SysDataCallback1);
	SysData_SetModeCallback_Register(&SysDataCallback2);

	// Call SysData to change modes
	SysData_SetMode(MODE_STARTING);
	SysData_SetMode(MODE_NORMAL);

	// Unregister callbacks from SysData
	SysData_SetModeCallback_Unregister(&SysDataCallback1);
	SysData_SetModeCallback_Unregister(&SysDataCallback1);

	// Register, callback and unregister 0, 2 through 5 arguments
	MulticastCallback0Args_Register(&Callback0Args);
	MulticastCallback0Args_Invoke();
	MulticastCallback0Args_Unregister(&Callback0Args);

	MulticastCallback2Args_Register(&Callback2Args);
	MulticastCallback2Args_Invoke(123, 1.23f);
	MulticastCallback2Args_Unregister(&Callback2Args);

	MulticastCallback3Args_Register(&Callback3Args);
	MulticastCallback3Args_Invoke(123, 1.23f, 3.21f);
	MulticastCallback3Args_Unregister(&Callback3Args);

	MulticastCallback4Args_Register(&Callback4Args);
	MulticastCallback4Args_Invoke(123, 1.23f, 3.21f, 5.55f);
	MulticastCallback4Args_Unregister(&Callback4Args);

	MulticastCallback5Args_Register(&Callback5Args);
	MulticastCallback5Args_Invoke(123, 1.23f, 3.21f, 5.55f, 9.99f);
	MulticastCallback5Args_Unregister(&Callback5Args);

    return 0;
}

