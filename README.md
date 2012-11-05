cm3pp lib devel
===============

C++ Code for Cortex-M

Contains:

 * _[ARM]_ __CMSIS 3.0 for Cortex-M3__: Need inclution of CM-4 headers
 * _[ST]_ __stm32f10x standard peripheral library__: gradually leave it
 * _[ST]_ __stm32 OTG/Device library__: For USB Virtual COM port device
 * _[ST example reworked]_ __usblib__: Wrapper around the CDC ACM Virtual COM Port
 * _[ODev based]_ __C++ runtime__: Minimal C++ support functions (disabled new/delete)
 * _[FreeRTOS team]_ __FreeRTOS__: Gradual migration to any other OS (Fall back task switcher eventually)
 * _[Own]_ __CXX__
   * __GPIO__: Port IO (TODO: Make alternate pin make like GPIO configuration)
   * __SysTick__: System tick wrapper (stand alone and RTOS supported)
   * __RTOS__: Real Time OS Wrapper (actually only for FreeRTOS)
   * __Functional__: Functor and functional programming utilities
   * __WriteStream__: Abstract print interface. Like iostream but more basic
   * __ReadStream__: Abstract input parse. Only stub. Need more love
   * __USBStream__: WriteStream over USB writted over libusb/OTG-Device library
   * __Atomic__: Atomic operation over arm CM3 (and CM4)
   * __LineReader__: Command line reader and argc/argv parser based on templates
