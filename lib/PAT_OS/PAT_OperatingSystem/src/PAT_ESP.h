#ifndef __PAT_ESP_h__
#define __PAT_ESP_h__
#include <Arduino.h>
#include <Esp.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ESP_IDF_VERSION_MAJOR // IDF 4+
#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
#include "esp32/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/rtc.h"
#else 
#error Target CONFIG_IDF_TARGET is not supported
#endif
#else // ESP32 Before IDF 4.0
#include "rom/rtc.h"
#endif
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void print_reset_reason(int reason);
void verbose_print_reset_reason(int reason);
static void ARDUINO_ISR_ATTR resetModule();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Class_ESP : public EspClass
{
private:
  int wdtTimeout=10000 ;        //time in ms to trigger the watchdog
  hw_timer_t* wdtTimer = NULL;
public:
  Class_ESP() { }
  void wdgReset(void);
  void wdgInit(int timeout = 15000);
  void resetReason();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*
  Example Serial Log:
  ====================

rst:0x10 (RTCWDT_RTC_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0x00
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0008,len:8
load:0x3fff0010,len:160
load:0x40078000,len:10632
load:0x40080000,len:252
entry 0x40080034
CPU0 reset reason:
RTCWDT_RTC_RESET
RTC Watch dog reset digital core and rtc module
CPU1 reset reason:
EXT_CPU_RESET
for APP CPU, reseted by PRO CPU
Going to sleep
ets Jun  8 2016 00:22:57

rst:0x5 (DEEPSLEEP_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0x00
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0008,len:8
load:0x3fff0010,len:160
load:0x40078000,len:10632
load:0x40080000,len:252
entry 0x40080034
CPU0 reset reason:
DEEPSLEEP_RESET
Deep Sleep reset digital core
CPU1 reset reason:
EXT_CPU_RESET
for APP CPU, reseted by PRO CPU
Going to sleep
ets Jun  8 2016 00:22:57

rst:0x5 (DEEPSLEEP_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0x00
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0008,len:8
load:0x3fff0010,len:160
load:0x40078000,len:10632
load:0x40080000,len:252
entry 0x40080034
CPU0 reset reason:
DEEPSLEEP_RESET
Deep Sleep reset digital core
CPU1 reset reason:
EXT_CPU_RESET
for APP CPU, reseted by PRO CPU
Going to sleep
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif  // __PAT_ESP_h__