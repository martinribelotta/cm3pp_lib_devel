cm3pp_lib_devel
===============

C++ Code for Cortex-M

Contains:
 * [ARM] CMSIS 3.0 for Cortex-M3 (need more love to include cortex-m4)
 * [ST] stm32f10x standard peripheral library: gradually leave it
 * [ST] stm32 OTG/Device library: For USB Virtual COM port device
 * [ST example reworked] usblib: Wrapper around the CDC ACM Virtual COM Port
 * [ODev based] runtime: Minimal C++ support functions (disabled new/delete)
 * [FreeRTOS team] FreeRTOS: Gradual migration to any other OS (Fall back task switcher eventually)
 * CXX
   * GPIO: Port IO (TODO: Make alternate pin make like GPIO configuration)
   * SysTick: System tick wrapper (stand alone and RTOS supported)
   * RTOS: Real Time OS Wrapper (actually only for FreeRTOS)
   * Functional: Functor and functional programming utilities
   * WriteStream: Abstract print interface. Like iostream but more basic
   * ReadStream: Abstract input parse. Only stub. Need more love
   * USBStream: WriteStream over USB writted over libusb/OTG-Device library
   * Atomic: Atomic operation over arm CM3 (and CM4)
   * LineReader: Command line reader and argc/argv parser based on templates
