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

enum GPIOMode_t {
	Mode_AIN = 0x0,
	Mode_IN_FLOATING = 0x04,
	Mode_IPD = 0x28,
	Mode_IPU = 0x48,
	Mode_Out_OD = 0x14,
	Mode_Out_PP = 0x10,
	Mode_AF_OD = 0x1C,
	Mode_AF_PP = 0x18
};

enum GPIOSpeed_t {
	Speed_10MHz = 1, Speed_2MHz, Speed_50MHz
};

enum Mode {
	InAnalog = 0x0, //!<
	InFloating = 0x1, //!<
	InPulled = 0x2, //!<
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
			config_high(bit > 7 ? ((speed | (mode << 2)) << ((bit & 7) * 4)) : 0), //
			config_low(bit > 7 ? 0 : ((speed | (mode << 2)) << ((bit & 7) * 4))), //
			mask_high(bit > 7 ? (0xF << ((bit & 7) * 4)) : 0), //
			mask_low(bit > 7 ? 0 : (0xF << ((bit & 7) * 4))) //
	{
	}

	inline const uint32_t CFH() const {
		return config_high;
	}

	inline const uint32_t CFL() const {
		return config_low;
	}

	inline const uint32_t maskH() const {
		return mask_high;
	}

	inline const uint32_t maskL() const {
		return mask_low;
	}

private:
	const uint32_t config_high;
	const uint32_t config_low;
	const uint32_t mask_high;
	const uint32_t mask_low;

	PinConfig(uint32_t cfh, uint32_t cfl, uint32_t mh, uint32_t ml) :
			config_high(cfh), config_low(cfl), mask_high(mh), mask_low(ml) {
	}
};

class Port: GPIO_TypeDef {
public:
	Port() = delete;

	inline void init(uint16_t pins, GPIOMode_t mode, GPIOSpeed_t speed =
			Speed_50MHz) {
		GPIO_InitTypeDef conf;
		conf.GPIO_Pin = pins;
		conf.GPIO_Speed = static_cast<GPIOSpeed_TypeDef>(speed);
		conf.GPIO_Mode = static_cast<GPIOMode_TypeDef>(mode);
		GPIO_Init(this, &conf);
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
