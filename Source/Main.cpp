#include <cxx/RTOS.h>
#include <cxx/GPIO.h>
#include <cxx/USBStream.h>
#include <cxx/Functional.h>

#include <cxx/LineReader.h>
#include <cxx/CmdLineParser.h>

#include <usbd_cdc_vcp.h>

using namespace STM32;
using namespace Stream;

RTOS::Task taskLed(Functional::build([]() {
	PortB.enable();
	PortB.init(GPIO::PinConfig(11, GPIO::Speed50Mhz, GPIO::OutPP)
			+ GPIO::PinConfig(12, GPIO::Speed50Mhz, GPIO::OutPP)
			+ GPIO::PinConfig(2, GPIO::SpeedIn, GPIO::InPullUp));
	while(1) {
		PortB.setBits(GPIO::Pin11);
		RTOS::taskWait(100);
		PortB.resetBits(GPIO::Pin11);
		RTOS::taskWait(100);
	}
}));

extern int execute(int argc, const char **argv);

Stream::LineReader<usb_cdc_getc, usb_cdc_putc> lineReader;

RTOS::Task taskUSB(Functional::build([]() {
	while(1) {
		usbup << "ready: ";
		lineReader.reset();
		while (!lineReader.poll())
			RTOS::taskYield();
		CmdLineParser<10> args(lineReader.text());
#if 0
		for(int i=0; i<args.argc(); i++)
			usbup << "arg[" <<i<<"] " << args[i] << "\n";
#endif
		execute(args.argc(), args.argv());
	}
}));

int main() {
	RTOS::startRTOS();
	while (1)
		;
	return 0;
}
