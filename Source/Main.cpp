#include <cxx/GPIO.h>
#include <cxx/SysTick.h>
#include <cxx/Stream.h>

#include <usbd_cdc_vcp.h>

using namespace STM32;
using namespace CM3;

void SerialUSB_WriteLn(const char *msg) {
	while (*msg)
		if (usb_cdc_putc(*msg++) == -1)
			break;
}

extern void hwInit();

class USBStream: public OwnStream::AbstractStream {
protected:
	virtual void write(char c) {
		usb_cdc_putc(c);
	}

	virtual void write(const char *ptr, int size) {
		usb_cdc_write(ptr, size);
	}
public:
	USBStream() {
		usb_cdc_open();
	}
};

USBStream usb;

int main() {
	SystemTick.init();
	PortB.enable();
	hwInit();
	while (1) {
		usb << "** Led on  ** at" << SystemTick.current_tick() << "\r\n";
		PortB.setBits(GPIO::Pin11);
		SystemTick.delay(250);
		usb << "** Led off ** at" << SystemTick.current_tick() << "\r\n";
		PortB.resetBits(GPIO::Pin11);
		SystemTick.delay(250);
	}
	return 0;
}
