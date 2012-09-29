#ifndef GPIO_H_
#define GPIO_H_

#include <stm32f10x.h>

namespace STM32 {

namespace GPIO {

/**
 * @brief Pin mask for set/reset simultaneous bits on port
 * @see STM32::GPIO::Port
 */
enum Pin_t {
	Pin0 = (1 << 0),  //!< Pin0
	Pin1 = (1 << 1),  //!< Pin1
	Pin2 = (1 << 2),  //!< Pin2
	Pin3 = (1 << 3),  //!< Pin3
	Pin4 = (1 << 4),  //!< Pin4
	Pin5 = (1 << 5),  //!< Pin5
	Pin6 = (1 << 6),  //!< Pin6
	Pin7 = (1 << 7),  //!< Pin7
	Pin8 = (1 << 8),  //!< Pin8
	Pin9 = (1 << 9),  //!< Pin9
	Pin10 = (1 << 10),  //!< Pin10
	Pin11 = (1 << 11),  //!< Pin11
	Pin12 = (1 << 12),  //!< Pin12
	Pin13 = (1 << 13),  //!< Pin13
	Pin14 = (1 << 14),  //!< Pin14
	Pin15 = (1 << 15) //!< Pin15
};

/**
 * @brief Mode of pin
 * @see STM32::GPIO::PinConfig
 */
enum Mode {
	InAnalog = 0x0, //!< Analog input
	InFloating = 0x1, //!< Digital floating input
	InPullUp = 0x12, //!< Digital pull up input
	InPullDown = 0x12, //!< Digital pull down input
	OutPP = 0x0, //!< Digital output push pull
	OutOD = 0x1, //!< Digital output open drain
	AltPP = 0x2, //!< Alternate function push pull
	AltOD = 0x3, //!< Alternate function open drain
};

/**
 * @brief Seed specification for pin
 * @see STM32::GPIO::PinConfig
 */
enum Speed {
	SpeedIn = 0x0, //!< Input functions
	Speed10Mhz = 0x1, //!< 10Mhz output
	Speed2Mhz = 0x2, //!< 2Mhz output
	Speed50Mhz = 0x3, //!< 50Mhz output
};

/**
 * @brief configuration of pin
 *
 * Define the configuration over one pin of GPIO.
 *
 * @see STM32::GPIO::Port::init
 */
class PinConfig {
public:
	/**
	 * @brief Create configuration of pin <i>bit</i>
	 * @param bit Numer of bit of GPIO pin. Value for 0 to 15
	 * @param speed Speed of GPIO or Input mode
	 * @param mode Mode of operation of GPIO
	 */
	PinConfig(int bit, Speed speed, Mode mode) :
			config_h(bit > 7 ? make_config(bit, speed, mode) : 0), //
			config_l(bit > 7 ? 0 : make_config(bit, speed, mode)), //
			mask_h(bit > 7 ? (0xF << ((bit & 7) * 4)) : 0), //
			mask_l(bit > 7 ? 0 : (0xF << ((bit & 7) * 4))), //
			pull_up(mode == InPullUp ? (1 << bit) : 0), //
			pull_down(mode == InPullDown ? (1 << bit) : 0) //
	{
	}

	/**
	 * @brief Default constructor. Put all pins on RESET mode
	 */
	PinConfig() :
			config_h(0x44444444), config_l(0x44444444), //
			mask_h(0xffffffff), mask_l(0xffffffff), //
			pull_up(0), pull_down(0) //
	{
	}

	/**
	 * @return CRH register computed value
	 */
	inline const uint32_t CRH() const {
		return config_h;
	}

	/**
	 * @return CRL register computed value
	 */
	inline const uint32_t CRL() const {
		return config_l;
	}

	/**
	 * @return Mask of CRH register computed value
	 */
	inline const uint32_t maskH() const {
		return mask_h;
	}

	/**
	 * @return Mask of CRL register computed value
	 */
	inline const uint32_t maskL() const {
		return mask_l;
	}

	/**
	 * @return Pull-up (set bits) computed values for input modes
	 */
	inline const uint32_t pullUpMask() const {
		return pull_up;
	}

	/**
	 * @return Pull-down (clear bits) computed values for input modes
	 */
	inline const uint32_t pullDownMask() const {
		return pull_down;
	}

	/**
	 * @brief Combine tow PinConfig configurations in one.
	 * Used for create a complete port configuration combining various
	 * PinConfig values
	 */
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

/**
 * @brief Wrapper for GPIO
 *
 * Abstract the GPIO hardware
 *
 */
class Port: public GPIO_TypeDef {
public:
	/**
	 * Disable the creation of Port directly.
	 *
	 * This class only can wrap the existing addresses of hardware PORTS defined as \ref GPIO_STM32
	 */
	Port() = delete;

	/**
	 * @brief Configure the port with a combination of #STM32::GPIO::PinConfig class
	 *
	 * @code // Init PortB pins 11, 12 and 2
	 * PortB.init(GPIO::PinConfig(11, GPIO::Speed50Mhz, GPIO::OutPP) +
	 *            GPIO::PinConfig(12, GPIO::Speed50Mhz, GPIO::OutPP) +
	 *            GPIO::PinConfig(2,  GPIO::SpeedIn,    GPIO::InPullUp));
	 * @endcode
	 */
	inline void init(const PinConfig& config) {
		this->CRH = (PinConfig().CRH() & ~config.maskH()) | config.CRH();
		this->CRL = (PinConfig().CRL() & ~config.maskL()) | config.CRL();
		this->BRR = config.pullUpMask();
		this->BSRR = config.pullDownMask();
	}

	/**
	 * @brief Read the 16 bits of input port
	 * @return Input port value
	 */
	uint16_t readInputData() const {
		return static_cast<uint16_t>(this->IDR);
	}

	/**
	 * @brief Read an specific bit of port
	 * @param pins Pin of port. Value of PinX defined as Pin_t
	 * @return True if pin is high or false if pin is low
	 * @see Pin_t
	 */
	bool readInputDataBit(uint16_t pins) const {
		return static_cast<bool>(readInputData() & pins);
	}

	/**
	 * @brief Read output latch of GPIO port.
	 * This value can be different of \ref readInputData() depending of impedance of pin
	 * @return The 16 bits of output latch of port
	 */
	uint16_t readOutputData() const {
		return static_cast<uint16_t>(this->ODR);
	}

	/**
	 * @brief Read an specific bit of output data register
	 * This value can be different of \ref readInputDataBit() depending of impedance of pin
	 * @param pins Pin of port. Value of PinX defined as Pin_t
	 * @return True if bit is '1' or false if bit is '0'
	 * @see Pin_t
	 */
	bool readOutputDataBit(uint16_t pins) const {
		return static_cast<bool>(readOutputData() & pins);
	}

	/**
	 * @brief Put on high the bits specific ant <i>pins</i> parameter
	 * @param pins Mask of one or more Pin_t values
	 * @see Pin_t
	 */
	void setBits(uint16_t pins) {
		this->BSRR = pins;
	}

	/**
	 * @brief Put on low the bits specific ant <i>pins</i> parameter
	 * @param pins Mask of one or more Pin_t values
	 * @see Pin_t
	 */
	void resetBits(uint16_t pins) {
		this->BRR = pins;
	}

	/**
	 * @brief Set value of the bits specific ant <i>pins</i> parameter
	 * @param pins Mask of one or more Pin_t values
	 * @param value Value to set (true) or reset (false) the bits of `pins` mask
	 * @see Pin_t
	 */
	void writeBit(uint16_t pins, bool value) {
		value ? setBits(pins) : resetBits(pins);
	}

	/**
	 * @brief Write value of output register
	 * @param value Value to write of register
	 */
	void write(uint16_t value) {
		this->ODR = static_cast<uint16_t>(value);
	}

	/**
	 * @brief Enable clock of GPIO
	 * This function must be called first if you want to use the corresponding port
	 */
	const void enable() const {
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

	/**
	 * @brief Disable the clock of GPIO
	 * Without clock, the port does not consume any power but can't operate with it
	 */
	const void disable() const {
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

} /* namespace GPIO */

/**
 * @defgroup GPIO Port related class and variables
 *
 * Port related utilities.
 *   - @ref GPIO::Port Port class access wrapper
 *   - @ref GPIO::PinConfig Pin configuration class
 *
 * ### To use a GPIO port you can:
 *   - Enable clock of port with GPIO::Port::enable `PortA.enable();`
 *   - Configure ins with #STM32::GPIO::Port::init()
 *   - Use \ref GPIO_STM32 as PortX.[some function]
 * @{
 */

/**
 * @defgroup GPIO_STM32 Predefined GPIO Ports of STM32 platform
 * @{
 */
static GPIO::Port& PortA = *reinterpret_cast<GPIO::Port*>(GPIOA_BASE); //!< GPIO Port A
static GPIO::Port& PortB = *reinterpret_cast<GPIO::Port*>(GPIOB_BASE); //!< GPIO Port B
static GPIO::Port& PortC = *reinterpret_cast<GPIO::Port*>(GPIOC_BASE); //!< GPIO Port C
static GPIO::Port& PortD = *reinterpret_cast<GPIO::Port*>(GPIOD_BASE); //!< GPIO Port D
static GPIO::Port& PortE = *reinterpret_cast<GPIO::Port*>(GPIOE_BASE); //!< GPIO Port E
static GPIO::Port& PortF = *reinterpret_cast<GPIO::Port*>(GPIOF_BASE); //!< GPIO Port F
static GPIO::Port& PortG = *reinterpret_cast<GPIO::Port*>(GPIOG_BASE); //!< GPIO Port G
		/** @} */
		/** @} */

		} /* namespace STM32 */

#endif /* GPIO_H_ */
