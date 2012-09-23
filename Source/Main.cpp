#include <cxx/GPIO.h>
#include <cxx/SysTick.h>
#include <string>
#include <usbd_cdc_vcp.h>

using namespace STM32;
using namespace CM3;

void SerialUSB_WriteLn(const char *msg) {
	while (*msg)
		if (usb_cdc_putc(*msg++) == -1)
			break;
}

extern void hwInit();

int main() {
	usb_cdc_open();
	SystemTick.init();
	PortB.enable();
	hwInit();
	while (1) {
		SerialUSB_WriteLn("** LED On  **\r\n");
		PortB.setBits(GPIO::Pin11);
		SystemTick.delay(250);
		SerialUSB_WriteLn("-- LED Off --\r\n");
		PortB.resetBits(GPIO::Pin11);
		SystemTick.delay(250);
	}
	return 0;
}
