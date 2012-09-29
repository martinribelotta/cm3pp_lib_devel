#include <cxx/GPIO.h>
#include <cxx/SysTick.h>
#include <cxx/USBStream.h>
#include <cxx/RTOS.h>
#include <cxx/Functional.h>

using namespace STM32;
using namespace CM3;
using namespace Stream;

RTOS::Task taskLed(Functional::build([]() {
	PortB.enable();
	PortB.init(
			GPIO::PinConfig(11, GPIO::Speed50Mhz, GPIO::OutPP)
					+ GPIO::PinConfig(12, GPIO::Speed50Mhz, GPIO::OutPP)
					+ GPIO::PinConfig(2, GPIO::SpeedIn, GPIO::InPullUp));
	while(1) {
		PortB.setBits(GPIO::Pin11);
		RTOS::taskWait(100);
		PortB.resetBits(GPIO::Pin11);
		RTOS::taskWait(100);
		usb << "led loop\r\n";
	}
}));

RTOS::Task taskUSB(Functional::build([]() {
	while(1) {
		usb << "** Led on  ** at " << //
				USBStream::Width(0) << //
				USBStream::FillChar('0') << //
				USBStream::DEC << //
				RTOS::currentTick() << "\r\n";
		usb << " PORTB conf.H 0x" << //
				USBStream::Width(8) << //
				USBStream::FillChar('0') << //
				USBStream::HEX << //
				unsigned(PortB.CRH) << "\r\n";
		usb << " PORTB conf.L 0x" << //
				USBStream::Width(8) << //
				USBStream::FillChar('0') << //
				USBStream::HEX << //
				unsigned(PortB.CRL) << "\r\n";
		usb << " PORTB[2]=" << PortB.readInputDataBit(GPIO::Pin2) << "\r\n";
		RTOS::taskWait(1000);
	}
}));

int main() {
#if 1
	RTOS::startRTOS();
	while(1)
		;
#else
	SystemTick.init();
	PortB.enable();
	PortB.init(
			GPIO::PinConfig(11, GPIO::Speed50Mhz, GPIO::OutPP)
					+ GPIO::PinConfig(12, GPIO::Speed50Mhz, GPIO::OutPP)
					+ GPIO::PinConfig(2, GPIO::SpeedIn, GPIO::InPullUp));
	extern void lambda_test();
	lambda_test();
	while (1) {
		usb << "** Led on  ** at " << //
				USBStream::Width(0) << //
				USBStream::FillChar('0') << //
				USBStream::DEC << //
				SystemTick.current_tick() << "\r\n";
		usb << " PORTB conf.H 0x" << //
				USBStream::Width(8) << //
				USBStream::FillChar('0') << //
				USBStream::HEX << //
				unsigned(PortB.CRH) << "\r\n";
		usb << " PORTB conf.L 0x" << //
				USBStream::Width(8) << //
				USBStream::FillChar('0') << //
				USBStream::HEX << //
				unsigned(PortB.CRL) << "\r\n";
		usb << " PORTB[2]=" << PortB.readInputDataBit(GPIO::Pin2) << "\r\n";
		PortB.setBits(GPIO::Pin11);
		SystemTick.delay(250);
		PortB.resetBits(GPIO::Pin11);
		SystemTick.delay(250);
	}
#endif
	return 0;
}
