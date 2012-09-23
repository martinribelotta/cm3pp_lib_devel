#include <cxx/GPIO.h>

using namespace STM32;

void hwInit() {
	PortB.init(
			GPIO::PinConfig(11, GPIO::Speed50Mhz, GPIO::OutPP) +
			GPIO::PinConfig(12, GPIO::Speed50Mhz, GPIO::OutPP) +
			GPIO::PinConfig(2, GPIO::SpeedIn, GPIO::InPullUp)
	);
}
