// Type-safe multicast callback library capable of calling back any function with up to 5 arguments.

#ifndef _MULTICAST_H
#define _MULTICAST_H

#include <stddef.h>

#if !defined(NULL)
#define NULL ((void*)0)
#endif

// Generic callback function pointer type
typedef void(*CB_CallbackType)(void*);

// Private callback data structure
typedef struct
{
	CB_CallbackType callback;
} CB_Data;

// Generic Add/Remove functions for multicast callback registration and unregistration. 
// The multiline macros call these functions. Do not call these functions directly. 
void CB_MulticastAddCallback(CB_Data* cbData, size_t cbDataLen, CB_CallbackType callback);
void CB_MulticastRemoveCallback(CB_Data* cbData, size_t cbDataLen, CB_CallbackType callback);
CB_CallbackType CB_MulticastGetCallback(CB_Data* cbData, size_t cbDataLen, size_t idx);

// Type-safe multiple client callback support declaration. Macro typically used in publisher header file. 
// 
//		name - the name of the multicast callback (e.g. MyCallback)
//		arg1 - the callback data type for the argument (e.g. int)
// 
// For example: MULTICAST1_DECLARE(MyCallback, int)
#define MULTICAST1_DECLARE(name, arg1) \
	typedef void(*name##CallbackType)(arg1 val1); \
    void name##_Register(name##CallbackType callback); \
	void name##_Unregister(name##CallbackType callback);

// Type-safe muliple client callback support definition. Macro typically used in publisher implementation file.
//
//		name - the name of the multicast callback (e.g. MyCallback)
//		arg1 - the callback data type for the argument (e.g. int)
//		max - the maximum number of callbacks allowed to registered (e.g. 5)
//
// For example: MULTICAST1_DEFINE(MyCallback, int, 5)
//
// Three user functions are created for each multicast callback:
//
//		<name>_Register() - subscriber registers a callback function pointer
//		<name>_Unregister() - subscriber unregisters a callback function pointer
//		<name>_Invoke(arg1 val) - invoke all registered function pointers using the supplied argument
#define MULTICAST1_DEFINE(name, arg1, max) \
	static CB_Data name##Multicast[max]; \
	void name##_Register(name##CallbackType callback) { \
		CB_MulticastAddCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Unregister(name##CallbackType callback) { \
		CB_MulticastRemoveCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Invoke(arg1 val1) { \
        for (size_t idx=0; idx<max; idx++) { \
            name##CallbackType callback = (name##CallbackType)CB_MulticastGetCallback(&name##Multicast[0], max, idx); \
            if (callback != NULL) \
                callback(val1); } }


#define MULTICAST2_DECLARE(name, arg1, arg2) \
	typedef void(*name##CallbackType)(arg1 val1, arg2 val2); \
    void name##_Register(name##CallbackType callback); \
	void name##_Unregister(name##CallbackType callback);

#define MULTICAST2_DEFINE(name, arg1, arg2, max) \
	static CB_Data name##Multicast[max]; \
	void name##_Register(name##CallbackType callback) { \
		CB_MulticastAddCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Unregister(name##CallbackType callback) { \
		CB_MulticastRemoveCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Invoke(arg1 val1, arg2 val2) { \
        for (size_t idx=0; idx<max; idx++) { \
            name##CallbackType callback = (name##CallbackType)CB_MulticastGetCallback(&name##Multicast[0], max, idx); \
            if (callback != NULL) \
                callback(val1, val2); } }

#define MULTICAST3_DECLARE(name, arg1, arg2, arg3) \
	typedef void(*name##CallbackType)(arg1 val1, arg2 val2, arg3 val3); \
    void name##_Register(name##CallbackType callback); \
	void name##_Unregister(name##CallbackType callback);

#define MULTICAST3_DEFINE(name, arg1, arg2, arg3, max) \
	static CB_Data name##Multicast[max]; \
	void name##_Register(name##CallbackType callback) { \
		CB_MulticastAddCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Unregister(name##CallbackType callback) { \
		CB_MulticastRemoveCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Invoke(arg1 val1, arg2 val2, arg3 val3) { \
        for (size_t idx=0; idx<max; idx++) { \
            name##CallbackType callback = (name##CallbackType)CB_MulticastGetCallback(&name##Multicast[0], max, idx); \
            if (callback != NULL) \
                callback(val1, val2, val3); } }

#define MULTICAST4_DECLARE(name, arg1, arg2, arg3, arg4) \
	typedef void(*name##CallbackType)(arg1 val1, arg2 val2, arg3 val3, arg4 val4); \
    void name##_Register(name##CallbackType callback); \
	void name##_Unregister(name##CallbackType callback);

#define MULTICAST4_DEFINE(name, arg1, arg2, arg3, arg4, max) \
	static CB_Data name##Multicast[max]; \
	void name##_Register(name##CallbackType callback) { \
		CB_MulticastAddCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Unregister(name##CallbackType callback) { \
		CB_MulticastRemoveCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Invoke(arg1 val1, arg2 val2, arg3 val3, arg4 val4) { \
        for (size_t idx=0; idx<max; idx++) { \
            name##CallbackType callback = (name##CallbackType)CB_MulticastGetCallback(&name##Multicast[0], max, idx); \
            if (callback != NULL) \
                callback(val1, val2, val3, val4); } }

#define MULTICAST5_DECLARE(name, arg1, arg2, arg3, arg4, arg5) \
	typedef void(*name##CallbackType)(arg1 val1, arg2 val2, arg3 val3, arg4 val4, arg5 val5); \
    void name##_Register(name##CallbackType callback); \
	void name##_Unregister(name##CallbackType callback);

#define MULTICAST5_DEFINE(name, arg1, arg2, arg3, arg4, arg5, max) \
	static CB_Data name##Multicast[max]; \
	void name##_Register(name##CallbackType callback) { \
		CB_MulticastAddCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Unregister(name##CallbackType callback) { \
		CB_MulticastRemoveCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Invoke(arg1 val1, arg2 val2, arg3 val3, arg4 val4, arg5 val5) { \
        for (size_t idx=0; idx<max; idx++) { \
            name##CallbackType callback = (name##CallbackType)CB_MulticastGetCallback(&name##Multicast[0], max, idx); \
            if (callback != NULL) \
                callback(val1, val2, val3, val4, val5); } }

#define MULTICAST0_DECLARE(name) \
	typedef void(*name##CallbackType)(void); \
    void name##_Register(name##CallbackType callback); \
	void name##_Unregister(name##CallbackType callback);

#define MULTICAST0_DEFINE(name, max) \
	static CB_Data name##Multicast[max]; \
	void name##_Register(name##CallbackType callback) { \
		CB_MulticastAddCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Unregister(name##CallbackType callback) { \
		CB_MulticastRemoveCallback(&name##Multicast[0], max, (CB_CallbackType)callback); } \
	void name##_Invoke(void) { \
        for (size_t idx=0; idx<max; idx++) { \
            name##CallbackType callback = (name##CallbackType)CB_MulticastGetCallback(&name##Multicast[0], max, idx); \
            if (callback != NULL) \
                callback(); } }

#endif // _MULTICAST_H