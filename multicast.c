#include "multicast.h"
#include <stdbool.h>
#include <assert.h>

void CB_MulticastAddCallback(CB_Data* cbData, size_t cbDataLen, CB_CallbackType callback)
{
    bool success = false;

    if (cbData == NULL|| callback == NULL || cbDataLen == 0)
    {
		assert(0);
        return;
    }

	// TODO - software lock

    // Look for an empty registration within the callback array
    for (size_t idx = 0; idx<cbDataLen; idx++) 
    {
        if (cbData[idx].callback == NULL)
        {
            // Save callback function pointer
            cbData[idx].callback = callback;
            success = true;
            break;
        }
    }

	// TODO - software unlock

    // All registration locations full?
    if (success == false)
    {
		assert(0);
    }
}

void CB_MulticastRemoveCallback(CB_Data* cbData, size_t cbDataLen, CB_CallbackType callback)
{
	if (cbData == NULL || callback == NULL || cbDataLen == 0)
	{
		assert(0);
		return;
	}

	// TODO - software lock

	// Look for an empty registration within the callback array
	for (size_t idx = 0; idx<cbDataLen; idx++)
	{
		if (cbData[idx].callback == callback)
		{
			// Remove callback function pointer
			cbData[idx].callback = NULL;
			break;
		}
	}

	// TODO - software unlock
}

CB_CallbackType CB_MulticastGetCallback(CB_Data* cbData, size_t cbDataLen, size_t idx)
{
	if (cbData == NULL || cbDataLen == 0 || idx >= cbDataLen)
	{
		assert(0);
		return NULL;
	}

	// TODO - software lock

	CB_CallbackType cb = cbData[idx].callback;

	// TODO - software unlock

	return cb;
}



