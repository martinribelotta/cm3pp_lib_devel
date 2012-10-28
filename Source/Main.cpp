#include <cxx/RTOS.h>
#include <cxx/GPIO.h>
#include <cxx/USBStream.h>
#include <cxx/Functional.h>

#include <cxx/LineReader.h>

#include <usbd_cdc_vcp.h>

#include <cctype>

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

static Stream::LineReader<usb_cdc_getc, usb_cdc_putc> lineReader;

void execute(args_t args) {
	usbup << "argc " << args.count() << "\n";
	for (int i = 0; i < args.count(); i++)
		usbup << '\'' << args[i] << "'\n";
}

RTOS::Task taskUSB(Functional::build([]() {
	while(1) {
		usbup << "ready: ";
		while (!lineReader.read()) {
			RTOS::taskYield();
		}
		if (lineReader.parse()>0) {
			execute(lineReader.args());
		}
	}
}));

int main() {
	RTOS::startRTOS();
	while (1)
		;
	return 0;
}
