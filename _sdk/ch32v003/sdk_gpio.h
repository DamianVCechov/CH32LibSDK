
// ****************************************************************************
//
//                                GPIO pins
//
// ****************************************************************************

#ifndef _SDK_GPIO_H
#define _SDK_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

// GPIO pin mode
#define GPIO_MODE_AIN		0x00	// analog input mode (ADC; read input data register will always be 0)
#define GPIO_MODE_IN		0x04	// floating input mode (no pulls)
#define GPIO_MODE_IN_PD		0x28	// input with pull down (set output register to 0)
#define GPIO_MODE_IN_PU		0x48	// input with pull up (set output register to 1)

#define GPIO_MODE_OUT		(0+1)	// push-pull output, medium speed (10 MHz)
#define GPIO_MODE_OUT_SLOW	(0+2)	// push-pull output, slow speed (2 MHz)
#define GPIO_MODE_OUT_FAST	(0+3)	// push-pull output, fast speed (30 MHz)

#define GPIO_MODE_OD		(4+1)	// open-drain output, medium speed (10 MHz)
#define GPIO_MODE_OD_SLOW	(4+2)	// open-drain output, slow speed (2 MHz)
#define GPIO_MODE_OD_FAST	(4+3)	// open-drain output, fast speed (30 MHz)

#define GPIO_MODE_AF		(8+1)	// alternate function output, medium speed (10 MHz)
#define GPIO_MODE_AF_SLOW	(8+2)	// alternate function output, slow speed (2 MHz)
#define GPIO_MODE_AF_FAST	(8+3)	// alternate function output, fast speed (30 MHz)

#define GPIO_MODE_AFOD		(12+1)	// alternate function open-drain output, medium speed (10 MHz)
#define GPIO_MODE_AFOD_SLOW	(12+2)	// alternate function open-drain output, slow speed (2 MHz)
#define GPIO_MODE_AFOD_FAST	(12+3)	// alternate function open-drain output, fast speed (30 MHz)

// GPIO port
typedef struct {
	io32	CFGLR;		// 0x00: configuration register low (4 bits per pin, combination , B0-B1 speed, 
	io32	res;		// 0x04: ...reserved
	io32	INDR;		// 0x08: input data register (8 or 16 pins)
	io32	OUTDR;		// 0x0C: output data register (8 or 16 pins; for pull inputs: 0=pull down, 1=pull up)
	io32	BSHR;		// 0x10: bit set/reset output data register (low 16 bits: set bit, high 16 bits: clear bit)
	io32	BCR;		// 0x14: bit clear output data register (low 16 bits: clear bit)
	io32	LCKR;		// 0x18: configuration lock register (high 16 bits to lock: write 1, write 0, write 1, read 0, read 1)
} GPIO_t;
STATIC_ASSERT(sizeof(GPIO_t) == 0x1C, "Incorrect GPIO_t!");

// Alternate function registers
typedef struct {
	io32	res;		// 0x00: ... reserved
	io32	PCFR1;		// 0x04: Remap register 1
	io32	EXTICR;		// 0x08: External interrupt configuration register
} AFIO_t;
STATIC_ASSERT(sizeof(AFIO_t) == 0x0C, "Incorrect AFIO_t!");

#define GPIOA			((GPIO_t*)GPIOA_BASE)	// PA port 0x40010800
#define GPIOC			((GPIO_t*)GPIOC_BASE)	// PC port 0x40011000
#define GPIOD			((GPIO_t*)GPIOD_BASE)	// PD port 0x40011400
#define AFIO			((AFIO_t*)AFIO_BASE)	// alternative function select 0x40010000

// get port index from GPIO index
INLINE int GPIO_PortInx(int gpio) { return GPIO_PORTINX(gpio); }

// get port base from GPIO index
#define GPIO_PORT(gpio) ((GPIO_t*)(GPIO_PORTINX(gpio)*(GPIOD_BASE-GPIOC_BASE)+GPIOA_BASE))
INLINE GPIO_t* GPIO_Port(int gpio) { return GPIO_PORT(gpio); }

// get port pin from GPIO index
INLINE int GPIO_Pin(int gpio) { return GPIO_PIN(gpio); }

// compose GPIO index
//  port ... port index PORT*_INX
//  pin ... port pin 0..
INLINE int GPIO_Gpio(int port, int pin) { return GPIO_GPIO(port, pin); }

// get input bit from GPIO input port (returns 0 or 1; port = GPIOA..., pin = 0.., gpio = PA0...)
INLINE u8 GPIOx_In(GPIO_t* port, int pin) { return (port->INDR >> pin) & 1; }
INLINE u8 GPIOA_In(int pin) { return GPIOx_In(GPIOA, pin); }
INLINE u8 GPIOC_In(int pin) { return GPIOx_In(GPIOC, pin); }
INLINE u8 GPIOD_In(int pin) { return GPIOx_In(GPIOD, pin); }
INLINE u8 GPIO_In(int gpio) { return GPIOx_In(GPIO_PORT(gpio), GPIO_PIN(gpio)); }

// get all input bits from GPIO input port (returns input bits; port = GPIOA..., gpio = PA0...)
INLINE u8 GPIOx_InAll(GPIO_t* port) { return (u8)port->INDR; }
INLINE u8 GPIOA_InAll(void) { return GPIOx_InAll(GPIOA); }
INLINE u8 GPIOC_InAll(void) { return GPIOx_InAll(GPIOC); }
INLINE u8 GPIOD_InAll(void) { return GPIOx_InAll(GPIOD); }
INLINE u8 GPIO_InAll(int gpio) { return GPIOx_InAll(GPIO_PORT(gpio)); }

// get output from GPIO output port  (returns 0 or 1; port = GPIOA..., pin = 0.., gpio = PA0...)
INLINE u8 GPIOx_GetOut(GPIO_t* port, int pin) { return (port->OUTDR >> pin) & 1; }
INLINE u8 GPIOA_GetOut(int pin) { return GPIOx_GetOut(GPIOA, pin); }
INLINE u8 GPIOC_GetOut(int pin) { return GPIOx_GetOut(GPIOC, pin); }
INLINE u8 GPIOD_GetOut(int pin) { return GPIOx_GetOut(GPIOD, pin); }
INLINE u8 GPIO_GetOut(int gpio) { return GPIOx_GetOut(GPIO_PORT(gpio), GPIO_PIN(gpio)); }

// get all output bits from GPIO output port (returns output bits; port = GPIOA..., gpio = PA0...)
INLINE u8 GPIOx_GetOutAll(GPIO_t* port) { return (u8)port->OUTDR; }
INLINE u8 GPIOA_GetOutAll(void) { return GPIOx_GetOutAll(GPIOA); }
INLINE u8 GPIOC_GetOutAll(void) { return GPIOx_GetOutAll(GPIOC); }
INLINE u8 GPIOD_GetOutAll(void) { return GPIOx_GetOutAll(GPIOD); }
INLINE u8 GPIO_GetOutAll(int gpio) { return GPIOx_GetOutAll(GPIO_PORT(gpio)); }

// output 0 to GPIO output pin (port = GPIOA..., pin = 0.., gpio = PA0...)
//  In GPIO_MODE_IN_* modes this selects pull-down mode.
INLINE void GPIOx_Out0(GPIO_t* port, int pin) { port->BCR = BIT(pin); }
INLINE void GPIOA_Out0(int pin) { GPIOx_Out0(GPIOA, pin); }
INLINE void GPIOC_Out0(int pin) { GPIOx_Out0(GPIOC, pin); }
INLINE void GPIOD_Out0(int pin) { GPIOx_Out0(GPIOD, pin); }
INLINE void GPIO_Out0(int gpio) { GPIOx_Out0(GPIO_PORT(gpio), GPIO_PIN(gpio)); }

// output 1 to GPIO output pin (port = GPIOA..., pin = 0.., gpio = PA0...)
//  In GPIO_MODE_IN_* modes this selects pull-up mode.
INLINE void GPIOx_Out1(GPIO_t* port, int pin) { port->BSHR = BIT(pin); }
INLINE void GPIOA_Out1(int pin) { GPIOx_Out1(GPIOA, pin); }
INLINE void GPIOC_Out1(int pin) { GPIOx_Out1(GPIOC, pin); }
INLINE void GPIOD_Out1(int pin) { GPIOx_Out1(GPIOD, pin); }
INLINE void GPIO_Out1(int gpio) { GPIOx_Out1(GPIO_PORT(gpio), GPIO_PIN(gpio)); }

// output value to GPIO output pin (val = 0 or <>0; port = GPIOA..., pin = 0.., gpio = PA0...)
//  In GPIO_MODE_IN_* modes 0 selects pull-down mode, <>0 selects pull-up mode.
INLINE void GPIOx_Out(GPIO_t* port, int pin, int val) { pin = BIT(pin);
	if (val == 0) pin <<= 16; port->BSHR = pin; }
INLINE void GPIOA_Out(int pin, int val) { GPIOx_Out(GPIOA, pin, val); }
INLINE void GPIOC_Out(int pin, int val) { GPIOx_Out(GPIOC, pin, val); }
INLINE void GPIOD_Out(int pin, int val) { GPIOx_Out(GPIOD, pin, val); }
INLINE void GPIO_Out(int gpio, int val) { GPIOx_Out(GPIO_PORT(gpio), GPIO_PIN(gpio), val); }

// flip GPIO output pin (port = GPIOA..., pin = 0.., gpio = PA0...)
INLINE void GPIOx_Flip(GPIO_t* port, int pin) { pin = BIT(pin);
	if ((port->OUTDR & pin) != 0) pin <<= 16; port->BSHR = pin; }
INLINE void GPIOA_Flip(int pin) { GPIOx_Flip(GPIOA, pin); }
INLINE void GPIOC_Flip(int pin) { GPIOx_Flip(GPIOC, pin); }
INLINE void GPIOD_Flip(int pin) { GPIOx_Flip(GPIOD, pin); }
INLINE void GPIO_Flip(int gpio) { GPIOx_Flip(GPIO_PORT(gpio), GPIO_PIN(gpio)); }

// output all output pins to GPIO output port (port = GPIOA..., gpio = PA0...)
INLINE void GPIOx_OutAll(GPIO_t* port, int value) { port->OUTDR = value; }
INLINE void GPIOA_OutAll(int value) { GPIOx_OutAll(GPIOA, value); }
INLINE void GPIOC_OutAll(int value) { GPIOx_OutAll(GPIOC, value); }
INLINE void GPIOD_OutAll(int value) { GPIOx_OutAll(GPIOD, value); }
INLINE void GPIO_OutAll(int gpio, int value) { GPIOx_OutAll(GPIO_PORT(gpio), value); }

// lock GPIO pin (pin configuration cannot be changed until next reset MCU or reset port; gpio = PA0...)
void GPIO_Lock(int gpio);

// set GPIO pin mode (gpio = PA0..., mode = GPIO_MODE_*)
//  For GPIO_MODE_IN_* modes, the output register will be set accordingly.
void GPIO_Mode(int gpio, int mode);

// get current GPIO pin mode (returns GPIO_MODE_*; gpio = PA0...)
int GPIO_GetMode(int gpio);

// setup external interrupt input pin
//  port ... source port (PORTA_INX,...)
//  pin ... source pin/interrupt pin (0..7)
// - Determines which port pin the external interrupt pin is mapped to.
void GPIO_EXTILine(int port, int pin);

// Remap alternate function with 1 bit, register 1 (pos = bit position, map = remapping index)
INLINE void GPIO_Remap1_1b(int pos, int map) { AFIO->PCFR1 = (AFIO->PCFR1 & ~(1 << pos)) | (map << pos); }

// Remap alternate function with 2 bits, register 1 (pos = bit position, map = remapping index)
INLINE void GPIO_Remap1_2b(int pos, int map) { AFIO->PCFR1 = (AFIO->PCFR1 & ~(3 << pos)) | (map << pos); }

// Remap alternate function with 3 bits, register 1 (pos = bit position, map = remapping index)
INLINE void GPIO_Remap1_3b(int pos, int map) { AFIO->PCFR1 = (AFIO->PCFR1 & ~(7 << pos)) | (map << pos); }

// Remap SPI1 (default 0)
//	0 ... PC1:CS, PC5:SCK, PC7:MISO, PC6:MOSI
//	1 ... PC0:CS, PC5:SCK, PC7:MISO, PC6:MOSI
INLINE void GPIO_Remap_SPI1(int map) { GPIO_Remap1_1b(0, map); }

// Remap I2C1 (default 0)
//	0 ... PC2:SCL, PC1:SDA
//	1 ... PD1:SCL, PD0:SDA
//	2,3 ... PC5:SCL, PC6:SDA
INLINE void GPIO_Remap_I2C1(int map) { AFIO->PCFR1 = (AFIO->PCFR1 & ~(B1|B22)) | ((map & B0) << 1) | ((map >> 1) << 22); }

// Remap USART1 (default 0)
//	0 ... PD5:TX, PD6:RX, PD4:CLK, PD3:CTS, PC2:RTS
//	1 ... PD0:TX, PD1:RX, PD7:CLK, PC3:CTS, PC2:RTS, PD0:SW_RX
//	2 ... PD6:TX, PD5:RX, PD7:CLK, PC6:CTS, PC7:RTS, PD6:SW_RX
//	3 ... PC0:TX, PC1:RX, PC5:CLK, PC6:CTS, PC7:RTS, PC0:SW_RX
INLINE void GPIO_Remap_USART1(int map) { AFIO->PCFR1 = (AFIO->PCFR1 & ~(B2|B21)) | ((map & B0) << 2) | ((map >> 1) << 21); }

// Remap Timer 1 (default 0)
//	0 ... PC5:ETR, PD2:CH1, PA1:CH2, PC3:CH3, PC4:CH4, PC2:BKIN, PD0:CH1N, PA2:CH2N, PD1:CH3N
//	1 ... PC5:ETR, PC6:CH1, PC7:CH2, PC0:CH3, PD3:CH4, PC1:BKIN, PC3:CH1N, PC4:CH2N, PD1:CH3N
//	2 ... PD4:ETR, PD2:CH1, PA1:CH2, PC3:CH3, PC4:CH4, PC2:BKIN, PD0:CH1N, PA2:CH2N, PD1:CH3N
//	3 ... PC2:ETR, PC4:CH1, PC7:CH2, PC5:CH3, PD4:CH4, PC1:BKIN, PC3:CH1N, PD2:CH2N, PC6:CH3N
INLINE void GPIO_Remap_TIM1(int map) { GPIO_Remap1_2b(6, map); }

// Remap Timer 2 (default 0)
//	0 ... PD4:CH1/ETR, PD3:CH2, PC0:CH3, PD7:CH4
//	1 ... PC5:CH1/ETR, PC2:CH2, PD2:CH3, PC1:CH4
//	2 ... PC1:CH1/ETR, PD3:CH2, PC0:CH3, PD7:CH4
//	3 ... PC1:CH1/ETR, PC7:CH2, PD6:CH3, PD5:CH4
INLINE void GPIO_Remap_TIM2(int map) { GPIO_Remap1_2b(8, map); }

// Remap PA1 & PA2 pins to OSC_IN & OSC_OUT (default 0)
//	0 ... pins PA1 & PA2 are used as GPIO ports
//	1 ... pins PA1 & PA2 are used as crystal pins OSC_IN & OSC_OUT
INLINE void GPIO_Remap_PA1PA2(int map) { GPIO_Remap1_1b(15, map); }

// Remap ADC1 external trigger injection conversion (default 0)
//	0 ... ADC1 external trigger injection conversion connected to PD3
//	1 ... ADC1 external trigger injection conversion connected to PC2
INLINE void GPIO_Remap_ADC1INJ(int map) { GPIO_Remap1_1b(17, map); }

// Remap ADC1 external trigger regular conversion (default 0)
//	0 ... ADC1 external trigger regular conversion connected to PD3
//	1 ... ADC1 external trigger regular conversion connected to PC2
INLINE void GPIO_Remap_ADC1REG(int map) { GPIO_Remap1_1b(18, map); }

// Remap Timer 1 Channel 1 (default 0)
//	0 ... timer 1 channel 1 mapped to external pins
//	1 ... timer 1 channel 1 mapped to the LSI internal clock
INLINE void GPIO_Remap_TIM1CH1(int map) { GPIO_Remap1_1b(23, map); }

// Remap SWD debug interface (default 0)
// 	0-3 ... SWD (SDI) enabled
//	4 ... SWD (SDI) disable, pin served as GPIO
// Note: Before switching the SWD pin to GPIO, insert a short delay
// from system startup to allow the chip to be reprogrammed later.
INLINE void GPIO_Remap_SWD(int map) { GPIO_Remap1_3b(24, map); }

// Reset pin to default state (pin must not be locked; does not reset mappings)
void GPIO_PinReset(int gpio);

// Reset GPIO port to default state (use port index PORT*_INX, does not disable clock)
void GPIO_PortReset(int portinx);

// Reset all GPIO ports and all mappings to default state (does not disable clock)
void GPIO_AllReset(void);

#ifdef __cplusplus
}
#endif

#endif // _SDK_GPIO_H
