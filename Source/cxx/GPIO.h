/*
 * GPIO.h
 *
 *  Created on: 08/09/2012
 *      Author: user001
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>

namespace STM32 {

namespace GPIO {

enum Pin_t {
	Pin0 = (1 << 0),
	Pin1 = (1 << 1),
	Pin2 = (1 << 2),
	Pin3 = (1 << 3),
	Pin4 = (1 << 4),
	Pin5 = (1 << 5),
	Pin6 = (1 << 6),
	Pin7 = (1 << 7),
	Pin8 = (1 << 8),
	Pin9 = (1 << 9),
	Pin10 = (1 << 10),
	Pin11 = (1 << 11),
	Pin12 = (1 << 12),
	Pin13 = (1 << 13),
	Pin14 = (1 << 14),
	Pin15 = (1 << 15)
};

enum Mode {
	InAnalog = 0x0, //!<
	InFloating = 0x1, //!<
	InPullUp = 0x12, //!<
	InPullDown = 0x12, //!<
	OutPP = 0x0, //!<
	OutOD = 0x1, //!<
	AltPP = 0x2, //!<
	AltOD = 0x3 //!<
};

enum Speed {
	SpeedIn = 0x0, //!<
	Speed10Mhz = 0x1, //!<
	Speed2Mhz = 0x2, //!<
	Speed50Mhz = 0x3 //!<
};

class PinConfig {
public:
	PinConfig(int bit, Speed speed, Mode mode) :
			config_h(bit > 7 ? make_config(bit, speed, mode) : 0), //
			config_l(bit > 7 ? 0 : make_config(bit, speed, mode)), //
			mask_h(bit > 7 ? (0xF << ((bit & 7) * 4)) : 0), //
			mask_l(bit > 7 ? 0 : (0xF << ((bit & 7) * 4))), //
			pull_up(mode == InPullUp ? (1 << bit) : 0), //
			pull_down(mode == InPullDown ? (1 << bit) : 0) //
	{
	}

	PinConfig() :
			config_h(0x44444444), config_l(0x44444444), //
			mask_h(0xffffffff), mask_l(0xffffffff), //
			pull_up(0), pull_down(0) //
	{
	}

	inline const uint32_t CRH() const {
		return config_h;
	}

	inline const uint32_t CRL() const {
		return config_l;
	}

	inline const uint32_t maskH() const {
		return mask_h;
	}

	inline const uint32_t maskL() const {
		return mask_l;
	}

	inline const uint32_t pullUpMask() const {
		return pull_up;
	}

	inline const uint32_t pullDownMask() const {
		return pull_down;
	}

	inline const PinConfig operator+(const PinConfig& other) const {
		return PinConfig( //
				config_h | other.config_h, //
				config_l | other.config_l, //
				mask_h | other.mask_h, //
				mask_l | other.mask_l, //
				pull_up | other.pull_up, //
				pull_down | other.pull_down //
						);
	}

private:
	const uint32_t config_h;
	const uint32_t config_l;
	const uint32_t mask_h;
	const uint32_t mask_l;
	const uint32_t pull_up;
	const uint32_t pull_down;

	PinConfig(uint32_t cfh, uint32_t cfl, uint32_t mh, uint32_t ml, //
			uint32_t pu, uint32_t pd) :
			config_h(cfh), config_l(cfl), mask_h(mh), mask_l(ml), //
			pull_up(pu), pull_down(pd) //
	{
	}

	static inline uint32_t make_config(int bit, Speed speed, Mode mode) {
		return ((speed | ((mode & 3) << 2)) << ((bit & 7) * 4));
	}
};

class Port: GPIO_TypeDef {
public:
	Port() = delete;

	inline void init(const PinConfig& config) {
		this->CRH = (PinConfig().CRH() & ~config.maskH()) | config.CRH();
		this->CRL = (PinConfig().CRL() & ~config.maskL()) | config.CRL();
		this->BRR = config.pullUpMask();
		this->BSRR = config.pullDownMask();
	}

	uint16_t readInputData() const {
		return static_cast<uint16_t>(this->IDR);
	}

	bool readInputDataBit(uint16_t pins) const {
		return static_cast<bool>(readInputData() & pins);
	}

	uint16_t readOutputData() const {
		return static_cast<uint16_t>(this->ODR);
	}

	bool readOutputDataBit(uint16_t pins) const {
		return static_cast<bool>(readOutputData() & pins);
	}

	void setBits(uint16_t pins) {
		this->BSRR = pins;
	}

	void resetBits(uint16_t pins) {
		this->BRR = pins;
	}

	void writeBit(uint16_t pins, bool value) {
		value ? setBits(pins) : resetBits(pins);
	}

	void write(Pin_t value) {
		this->ODR = static_cast<uint16_t>(value);
	}

	void enable() const {
		switch (reinterpret_cast<uint32_t>(this)) {
			case GPIOA_BASE:
			RCC ->APB2ENR |= RCC_APB2Periph_GPIOA;
			break;
			case GPIOB_BASE:
			RCC ->APB2ENR |= RCC_APB2Periph_GPIOB;
			break;
			case GPIOC_BASE:
			RCC ->APB2ENR |= RCC_APB2Periph_GPIOC;
			break;
			case GPIOD_BASE:
			RCC ->APB2ENR |= RCC_APB2Periph_GPIOD;
			break;
			case GPIOE_BASE:
			RCC ->APB2ENR |= RCC_APB2Periph_GPIOE;
			break;
			case GPIOF_BASE:
			RCC ->APB2ENR |= RCC_APB2Periph_GPIOF;
			break;
			case GPIOG_BASE:
			RCC ->APB2ENR |= RCC_APB2Periph_GPIOG;
			break;
		}
	}

	void disable() const {
		switch (reinterpret_cast<uint32_t>(this)) {
			case GPIOA_BASE:
			RCC ->APB2ENR &= ~RCC_APB2Periph_GPIOA;
			break;
			case GPIOB_BASE:
			RCC ->APB2ENR &= ~RCC_APB2Periph_GPIOB;
			break;
			case GPIOC_BASE:
			RCC ->APB2ENR &= ~RCC_APB2Periph_GPIOC;
			break;
			case GPIOD_BASE:
			RCC ->APB2ENR &= ~RCC_APB2Periph_GPIOD;
			break;
			case GPIOE_BASE:
			RCC ->APB2ENR &= ~RCC_APB2Periph_GPIOE;
			break;
			case GPIOF_BASE:
			RCC ->APB2ENR &= ~RCC_APB2Periph_GPIOF;
			break;
			case GPIOG_BASE:
			RCC ->APB2ENR &= ~RCC_APB2Periph_GPIOG;
			break;
		}
	}
};

}

static GPIO::Port& PortA = *reinterpret_cast<GPIO::Port*>(GPIOA_BASE);
static GPIO::Port& PortB = *reinterpret_cast<GPIO::Port*>(GPIOB_BASE);
static GPIO::Port& PortC = *reinterpret_cast<GPIO::Port*>(GPIOC_BASE);
static GPIO::Port& PortD = *reinterpret_cast<GPIO::Port*>(GPIOD_BASE);
static GPIO::Port& PortE = *reinterpret_cast<GPIO::Port*>(GPIOE_BASE);
static GPIO::Port& PortF = *reinterpret_cast<GPIO::Port*>(GPIOF_BASE);
static GPIO::Port& PortG = *reinterpret_cast<GPIO::Port*>(GPIOG_BASE);

} /* namespace STM32 */

#endif /* GPIO_H_ */
