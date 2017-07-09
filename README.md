# Type-Safe Multicast Callbacks in C
A type-safe multicast callback library used for anonymous function invocation implemented in C.

Originally published on CodeProject at: <a href="http://www.codeproject.com/Articles/1169105/Cplusplus-std-thread-Event-Loop-with-Message-Queue"><strong>Type-Safe Multicast Callbacks in C</strong></a>

<h2>Introduction</h2>

<p>Function pointers are used to reduce coupling between two pieces of code. A <em>publisher </em>defines a callback function signature and allows anonymous registration of a function pointer. A <em>subscriber </em>creates a function implementation conforming to the publisher&rsquo;s callback signature and registers a pointer to the function with the publisher at runtime. The publisher code knows nothing about the subscriber code &ndash; the registration and callback invocation is anonymous.</p>

<p>Multicast callbacks permit two or more subscribers to register for notification via a callback. When a publisher invokes the callback, all registered subscriber functions get sequentially invoked.</p>

<p>This article provides a simple, type-safe multicast callback module implemented in C.</p>

<h2>Callbacks Background</h2>

<p>Software systems are organized into different software modules. A module&rsquo;s incoming interface is declared within an interface file. An outgoing interface can be expressed using function pointers registered and invoked at runtime. The publisher module notifies subscribers by invoking anonymous functions via a function pointer. &ldquo;Anonymous&rdquo; means the publisher code doesn&rsquo;t include any subscriber header files.<span style="margin: 0px"> </span>The subscriber knows about the publisher, but not vice versa. In this way, the publisher code module doesn&rsquo;t change when a new subscriber wants to receive callback notifications.</p>

<p>For instance, say our system has an alarm module. It&rsquo;s responsible for handling detected alarms. Now, other modules within the system may be interested in receiving alarm notifications. Maybe the GUI needs to display the alarm to the user. A logging module might save the alarm to a persistent storage. And an actuators module may need to stop motor movements. Ideally, the alarm module should not know about the GUI, logger or actuator modules. Instead, subscribers register with the alarm module for notifications so that when an alarm occurs, each subscriber alarm handler function is called.</p>

<h2>Using the Code</h2>

<p>I&rsquo;ll first present how to use the code, and then get into the implementation details.</p>

<p>The <code>MULTICASTX_DECLARE</code> macro exposes a multicast callback interface within a publisher header file. The &ldquo;<code>X</code>&rdquo; is a numeric value indicating the number of function arguments within the callback. For instance, use <code>MULTICAST2_DECLARE</code> if the callback function has two function arguments. From 0 to 5 arguments are supported by the library.</p>

<p>The first argument is the callback name. The other arguments are the function argument types. The macro below defines a function signature of <code>void MyFunc(int, float).</code></p>

<pre lang="C++">
MULTICAST2_DECLARE(MyCallback, int, float)</pre>

<p>The <code>MULTICASTX_DEFINE </code>macro implements the multicast callback interface within a publisher source file. The macro is placed at file scope. The macro arguments are the callback name, function argument types, and the maximum number of allowed registrars. In the example below, up to 5 function pointers can be registered.</p>

<pre lang="C++">
MULTICAST2_DEFINE(MyCallback, int, float, 5)</pre>

<p>The two macros fully implement a type-safe multicast callback interface. The macros automatically create three functions based on the macro arguments provided.</p>

<pre lang="C++">
void MyCallback_Register(MyCallbackCallbackType callback);
void MyCallback_Unregister(MyCallbackCallbackType callback);
void MyCallback_Invoke(int val1, float val2);</pre>

<p>A subscriber registers a callback using the <code>Register() </code>function:</p>

<pre lang="C++">
MyCallback_Register(&amp;NotificationCallback);</pre>

<p>Similarly a subscriber unregisters using the <code>Unregister() </code>function:</p>

<pre lang="C++">
MyCallback_Unregister(&amp;NotificationCallback);</pre>

<p>The publisher sequentially invokes all registered callbacks using the <code>Invoke() </code>function.</p>

<pre lang="C++">
MyCallback_Invoke(123, 3.21f);</pre>

<h2>SysData Example</h2>

<p><code>SysData </code>is a simple module showing how to expose an outgoing multicast callback interface. <code>SysData </code>stores system data and provided subscriber notification when the mode changes. The <code>interface</code> is shown below:</p>

<pre lang="C++">
#ifndef _SYSDATA_H
#define _SYSDATA_H

#include &quot;multicast.h&quot;

typedef enum
{
   MODE_STARTING,
   MODE_NORMAL,
   MODE_ALARM
} ModeType;

// Publisher declares a multicast callback called SysData_SetModeCallback.
// Subscribers register for callbacks with function signature: void MyFunc(ModeType mode)
MULTICAST1_DECLARE(SysData_SetModeCallback, ModeType)

// Set a new system mode and callback any clients registered with SysData_SetModeCallback.
void SysData_SetMode(ModeType mode);

#endif // _SYSDATA_H</pre>

<p>The <code>SysData </code>implementation is shown below:</p>

<pre lang="C++">
#include &quot;sysdata.h&quot;

// Define the multicast callback for up to 3 registered clients
MULTICAST1_DEFINE(SysData_SetModeCallback, ModeType, 3)

static ModeType _mode = MODE_STARTING;

void SysData_SetMode(ModeType mode)
{
   // Update the private _mode value
   _mode = mode;
   
   // Invoke callbacks on all registered clients
   SysData_SetModeCallback_Invoke(_mode);
}</pre>

<p>A subscriber connects to <code>SysData </code>by creating a callback function and registering the function pointer at runtime.</p>

<pre lang="C++">
void SysDataCallback1(ModeType mode)
{
   printf(&quot;ModeCallback1: %d\n&quot;, mode);
}

int main(void)
{
   // Register with SysData for callbacks
   SysData_SetModeCallback_Register(&amp;SysDataCallback1);

   // Call SysData to change modes
   SysData_SetMode(MODE_STARTING);
   SysData_SetMode(MODE_NORMAL);

   return 0;
}</pre>

<p>Notice that <code>SysDataCallback1() </code>is called each time <code>SysData_SetMode() </code>is called. Also note that <code>SysData </code>doesn&rsquo;t know about the subscriber as the registration is anonymous.</p>

<h2>Implementation</h2>

<p>The implementation uses macros and token pasting to provide a type-safe interface for using multicast callbacks. The token pasting operator (<code>##</code>) is used to merge two tokens when the preprocessor expands the macro. The <code>MULTICAST1_DECLARE </code>macro is shown below.</p>

<pre lang="C++">
#define MULTICAST1_DECLARE(name, arg1) \
    typedef void(*name##CallbackType)(arg1 val1); \
    void name##_Register(name##CallbackType callback); \
    void name##_Unregister(name##CallbackType callback);</pre>

<p>In the <code>SysData </code>example used above, macro expands to:</p>

<pre lang="C++">
typedef void(*SysData_SetModeCallbackCallbackType)(ModeType val1);
void SysData_SetModeCallback_Register(SysData_SetModeCallbackCallbackType callback);
void SysData_SetModeCallback_Unregister(SysData_SetModeCallbackCallbackType callback);</pre>

<p>Notice every <code>name## </code>location is replaced by the macro name argument, in this case, being <code>SysData_SetModeCallback</code> from the declaration below.</p>

<pre lang="C++">
MULTICAST1_DECLARE(SysData_SetModeCallback, ModeType)</pre>

<p>The implementation macro is shown below:</p>

<pre lang="C++">
#define MULTICAST1_DEFINE(name, arg1, max) \
   static CB_Data name##Multicast[max]; \
      void name##_Register(name##CallbackType callback) { \
   CB_MulticastAddCallback(&amp;name##Multicast[0], max, (CB_CallbackType)callback); } \
      void name##_Unregister(name##CallbackType callback) { \
   CB_MulticastRemoveCallback(&amp;name##Multicast[0], max, (CB_CallbackType)callback); } \
       void name##_Invoke(arg1 val1) { \
           for (size_t idx=0; idx&lt;max; idx++) { \
               name##CallbackType callback = (name##CallbackType)CB_MulticastGetCallback
                                             (&amp;name##Multicast[0], max, idx); \
               if (callback != NULL) \
                   callback(val1); } }</pre>

<p>The expanded <code>MULTICAST1_DEFINE </code>from the <code>SysData </code>example results in:</p>

<pre lang="C++">
static CB_Data SysData_SetModeCallbackMulticast[3];

void SysData_SetModeCallback_Register(SysData_SetModeCallbackCallbackType callback)
{
   CB_MulticastAddCallback(&amp;SysData_SetModeCallbackMulticast[0], 3, (CB_CallbackType)callback);
}

void SysData_SetModeCallback_Unregister(SysData_SetModeCallbackCallbackType callback)
{
   CB_MulticastRemoveCallback(&amp;SysData_SetModeCallbackMulticast[0], 3, (CB_CallbackType)callback);
}

void SysData_SetModeCallback_Invoke(ModeType val1)
{
   for (size_t idx=0; idx&lt;3; idx++)
   {
       SysData_SetModeCallbackCallbackType callback =
          (SysData_SetModeCallbackCallbackType)CB_MulticastGetCallback
                               (&amp;SysData_SetModeCallbackMulticast[0], 3);
   
       if (callback != NULL)
           callback(val1);
   }
}</pre>

<p>Notice the macro provides a thin, type-safe wrapper around <code>CB_MulticastAddCallback() </code>and <code>CB_MulticastRemoveCallback()</code> functions. If the wrong function signature is registered, the compiler generates an error or warning. The macros automate the monotonous, boilerplate code that you&rsquo;d normally write by hand.</p>

<p>The callback functions are simply stored in an array. The <code>Invoke() </code>function just iterates over the callback function array and invokes any element that is not <code>NULL</code>.</p>

<p>The callback add/remove functions just store a generic <code>CB_CallbackType </code>to be extracted and used by the boilerplate macro code.</p>

<pre lang="C++">
#include &quot;multicast.h&quot;
#include &lt;stdbool.h&gt;
#include &lt;assert.h&gt;

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
    for (size_t idx = 0; idx&lt;cbDataLen; idx++)
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
    for (size_t idx = 0; idx&lt;cbDataLen; idx++)
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
    if (cbData == NULL || cbDataLen == 0 || idx &gt;= cbDataLen)
    {
       assert(0);
       return NULL;
    }

    // TODO - software lock

    CB_CallbackType cb = cbData[idx].callback;

    // TODO - software unlock

    return cb;
}</pre>

<h2>Multithreading</h2>

<p>The multicast library can be used on a single or multithreaded systems. If used on a system with operating system threads, it can be made thread-safe. The <em>multicast.c</em> file contains comments on where to place software locks. Once the locks are in place, the multicast API and macros are safe to deploy on a multithreaded system where subscribers that execute on another thread may register and unregister.</p>

<p>On a multithreaded system, if the subscriber code executes on a separate thread, then typically the subscriber&#39;s callback function implementation posts a message to a thread queue to be handled asynchronously. In this way, the publisher thread generating the callbacks doesn&rsquo;t get blocked and the subscriber code will be able to handle the notification asynchronously via an OS message queue.</p>

<h2>Conclusion</h2>

<p>Multicast callbacks eliminates unnecessary code coupling between modules. Registering multiple callback function pointers offers a convenient notification system. The article has demonstrated one possible design capable of being uniformly deployed on a system. The implementation was kept to a minimum facilitating usage on any system embedded or otherwise. A small amount of macro code automates boilerplate code and gives the C library type-safety.</p>




