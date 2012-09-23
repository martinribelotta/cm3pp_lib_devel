#include <cxx/GPIO.h>
#include <cxx/SysTick.h>
#include <cxx/USBStream.h>

using namespace STM32;
using namespace CM3;
using namespace OwnStream;

extern void hwInit();

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
