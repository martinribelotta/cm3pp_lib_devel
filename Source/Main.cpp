#include <cxx/RTOS.h>
#include <cxx/GPIO.h>
#include <cxx/USBStream.h>
#include <cxx/Functional.h>

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
		// It is bad!!! (not reentrant code of streams)
		// But not crash for two days.
		// Anyway... a cat is fine too
		usb << "led loop\r\n";
	}
}));

RTOS::Task taskUSB(Functional::build([]() {
	while(1) {
		usb << USBStream::Config(0, '0', USBStream::DEC);
		usb << "SYSTICK: " << RTOS::currentTick() << "\r\n";
		usb << USBStream::Config(8, '0', USBStream::HEX);
		usb << " PORTB conf.H 0x" << unsigned(PortB.CRH) << "\r\n";
		usb << " PORTB conf.L 0x" << unsigned(PortB.CRL) << "\r\n";
		usb << " PORTB[2]=" << PortB.readInputDataBit(GPIO::Pin2) << "\r\n";
		RTOS::taskWait(1000);
	}
}));

int main() {
	RTOS::startRTOS();
	while (1)
		;
	return 0;
}
