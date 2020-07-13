#include <FlashRuntimeExtensions.h>
#include <interception.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <windows.h>

#include "Adobetest.h"
#include "ScanCodeValues.h"


void inkeyboard(FREContext ctx);
FREObject ASPassAString(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctionsToSet, const FRENamedFunction** functionsToSet);
void contextFinalizer(FREContext ctx);
int devicearr[5];

enum ScanCode
{
	SCANCODE_X = 0x2D,
	SCANCODE_Y = 0x15,
	SCANCODE_ESC = 0x01
};

__declspec(dllexport) void ExtensionInitializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
{
	*ctxInitializer = &contextInitializer; // The name of function that will intialize the extension context
	*ctxFinalizer = &contextFinalizer; // The name of function that will finalize the extension context
}

__declspec(dllexport) void ExtensionFinalizer(void* extData)
{
	return;
}



void contextInitializer(
	void* extData,
	const uint8_t* ctxType,
	FREContext				   ctx,
	uint32_t* numFunctionsToSet,
	const FRENamedFunction** functionsToSet)
{

	// Create mapping between function names and pointers in an array of FRENamedFunction.
	// These are the functions that you will call from ActionScript -
	// effectively the interface of your native library.
	// Each member of the array contains the following information:
	// { function name as it will be called from ActionScript,
	//   any data that should be passed to the function,
	//   a pointer to the implementation of the function in the native library }
	static FRENamedFunction extensionFunctions[] =
	{
		{ (const uint8_t*)"as_passAString",			NULL, &ASPassAString }
	};

	// Tell AIR how many functions there are in the array:
	*numFunctionsToSet = sizeof(extensionFunctions) / sizeof(FRENamedFunction);

	// Set the output parameter to point to the array we filled in:
	*functionsToSet = extensionFunctions;
}

void contextFinalizer(FREContext ctx)
{
	return;
}

int double_state_check = 0;

FREObject ASPassAString(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
{

	enum
	{
		ARG_STRING_ARGUMENT = 0,

		ARG_COUNT
	};


	FREObject result;
	FREObject stroke_id = NULL;
	FREObject button_state = NULL;
	FREObject device_num = NULL;
	FREObject string_id = NULL;

	FRENewObject((const uint8_t*)"Array", 0, NULL, &result, NULL);
	FRESetArrayLength(result, 4);

	InterceptionContext context;
	InterceptionDevice device;
	InterceptionStroke stroke;

	
	
	char hardware_id[500] = {'\0'};
		
	int raise_process_priority();
		
	context = interception_create_context();
	
	interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_ALL);// | INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP | INTERCEPTION_KEYBOARD(1));


	while (interception_receive(context, device = interception_wait_with_timeout(context,16), &stroke, 1) > 0)
	{
		if (interception_is_keyboard(device))
		{
			
			InterceptionKeyStroke keystroke = *(InterceptionKeyStroke*)&stroke;

			/*if (keystroke.code == 42 || keystroke.state > 1)
			{
				double_state_check = 1;
			}
			else
			{
				if (double_state_check >= 1)
				{
					keystroke.code = keystroke.code + 128;
					if (double_state_check == 2)
					{
						double_state_check = 0;
					}
					else
					{
						double_state_check++;
					}
				}
			}*/

			char buffer[20];
			sprintf_s(buffer,20, "%d|%d|%d",(int)keystroke.code,(int)keystroke.state,device);

			FREDispatchStatusEventAsync(
				ctx,
				(const uint8_t*)buffer,
				(const uint8_t*)scanCodeToStr(keystroke.code));
		

			FRENewObjectFromUint32(keystroke.code, &stroke_id);
  			FRESetArrayElementAt(result, 0, stroke_id);

			FRENewObjectFromUint32(keystroke.state, &button_state);
			FRESetArrayElementAt(result,1, button_state); 
			
			FRENewObjectFromUint32(device, &device_num);
			FRESetArrayElementAt(result, 2, device_num);

			FRENewObjectFromUTF8(15, scanCodeToStr(keystroke.code), &string_id);
			FRESetArrayElementAt(result, 3, string_id);

			

			interception_send(context, device, &stroke, 1);
		}
		size_t length = interception_get_hardware_id(context, device, hardware_id, sizeof(hardware_id));
		
	}
	interception_destroy_context(context);

	return result;
}



	