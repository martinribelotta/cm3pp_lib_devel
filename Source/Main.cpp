#include <cxx/GPIO.h>
#include <cxx/SysTick.h>
#include <cxx/USBStream.h>

using namespace STM32;
using namespace CM3;
using namespace OwnStream;

int main() {
	SystemTick.init();
	PortB.enable();
	PortB.init(
			GPIO::PinConfig(11, GPIO::Speed50Mhz, GPIO::OutPP)
					+ GPIO::PinConfig(12, GPIO::Speed50Mhz, GPIO::OutPP)
					+ GPIO::PinConfig(2, GPIO::SpeedIn, GPIO::InPullUp));
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
	return 0;
}
