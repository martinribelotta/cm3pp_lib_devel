#include "builtins.h"

// #include "jimtcl/jim.h"

#include <cxx/USBStream.h>
#include <stdint.h>
#include <unistd.h>
#include <stm32f10x.h>
#include <system_stm32f10x.h>

int cmd_help(int argc, const char* argv[]) {
	extern int _end;

	(void) argc;
	(void) argv;

	char *heap_top = (char*) sbrk(0);
	char *heap_start = (char*) &_end;
	char *stack_top = (char*) __get_MSP();
	Stream::usbup << "STM32 enviroment\n"
			" System clock %d" << uint32_t(SystemCoreClock / 1000000) << "Mhz\n"
			" Mem used " << uint32_t(heap_top - heap_start) << " bytes\n"
			" Mem free " << uint32_t(stack_top - heap_top) << " bytes\n";
	return 0;
}

#ifdef __JIM__H
int jimtcl_main(int argc, const char *argv[]) {
	int retcode;
	Jim_Interp *interp;

	Jim_InitEmbedded(); /* This is the first function embedders should call. */
	interp = Jim_CreateInterp(); /* Create and initialize the interpreter */
	Jim_RegisterCoreCommands(interp);
	Jim_SetVariableStrWithStr(interp, "jim_interactive", "1");
	retcode = Jim_InteractivePrompt(interp);
	Jim_FreeInterp(interp);
	return retcode;
}
#endif
