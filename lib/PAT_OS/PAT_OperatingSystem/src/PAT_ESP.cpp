#include <Arduino.h>
#include "PAT_ESP.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Class_ESP esp;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Class_ESP::resetReason(void) {
    if (!Serial)
    {
        Serial.begin(115200);
        while (!Serial);
    }
    Serial.println("------------------------------------");

    Serial.print("CPU0 reset reason: ");
    print_reset_reason(rtc_get_reset_reason(0));
    verbose_print_reset_reason(rtc_get_reset_reason(0));

    Serial.print("CPU1 reset reason: ");
    print_reset_reason(rtc_get_reset_reason(1));
    verbose_print_reset_reason(rtc_get_reset_reason(1));

    Serial.println("------------------------------------");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ARDUINO_ISR_ATTR resetModule() {
  ets_printf("ESP was restarted by wdtTimer\n");
  esp_restart();
}
//--------------------------------------------------------------
void Class_ESP::wdgInit(int timeout) {
 wdtTimeout = timeout ;        //time in ms to trigger the watchdog
  if (!Serial)
    {
        Serial.begin(115200);
        while (!Serial);
    }
  wdtTimer = timerBegin(0, 80, true);                  //timer 0, div 80
  timerAttachInterrupt(wdtTimer, &resetModule, true);  //attach callback
  timerAlarmWrite(wdtTimer, wdtTimeout * 1000 , false); //set time in ms
  timerAlarmEnable(wdtTimer);
  this->wdgReset();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Class_ESP::wdgReset(void) {
  timerWrite(wdtTimer, 0);     //reset timer (feed watchdog)
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void print_reset_reason(int reason)
{
  switch ( reason)
  {
    case 1 : Serial.print ("POWERON_RESET -> ");break;          /**<1,  Vbat power on reset*/
    case 3 : Serial.print ("SW_RESET -> ");break;               /**<3,  Software reset digital core*/
    case 4 : Serial.print ("OWDT_RESET -> ");break;             /**<4,  Legacy watch dog reset digital core*/
    case 5 : Serial.print ("DEEPSLEEP_RESET -> ");break;        /**<5,  Deep Sleep reset digital core*/
    case 6 : Serial.print ("SDIO_RESET -> ");break;             /**<6,  Reset by SLC module, reset digital core*/
    case 7 : Serial.print ("TG0WDT_SYS_RESET -> ");break;       /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8 : Serial.print ("TG1WDT_SYS_RESET -> ");break;       /**<8,  Timer Group1 Watch dog reset digital core*/
    case 9 : Serial.print ("RTCWDT_SYS_RESET -> ");break;       /**<9,  RTC Watch dog Reset digital core*/
    case 10 : Serial.print ("INTRUSION_RESET -> ");break;       /**<10, Instrusion tested to reset CPU*/
    case 11 : Serial.print ("TGWDT_CPU_RESET -> ");break;       /**<11, Time Group reset CPU*/
    case 12 : Serial.print ("SW_CPU_RESET -> ");break;          /**<12, Software reset CPU*/
    case 13 : Serial.print ("RTCWDT_CPU_RESET -> ");break;      /**<13, RTC Watch dog Reset CPU*/
    case 14 : Serial.print ("EXT_CPU_RESET -> ");break;         /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : Serial.print ("RTCWDT_BROWN_OUT_RESET -> ");break;/**<15, Reset when the vdd voltage is not stable*/
    case 16 : Serial.print ("RTCWDT_RTC_RESET -> ");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : Serial.print ("NO_MEAN -> ");
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void verbose_print_reset_reason(int reason)
{
  switch ( reason)
  {
    case 1  : Serial.println ("Vbat power on reset");break;
    case 3  : Serial.println ("Software reset digital core");break;
    case 4  : Serial.println ("Legacy watch dog reset digital core");break;
    case 5  : Serial.println ("Deep Sleep reset digital core");break;
    case 6  : Serial.println ("Reset by SLC module, reset digital core");break;
    case 7  : Serial.println ("Timer Group0 Watch dog reset digital core");break;
    case 8  : Serial.println ("Timer Group1 Watch dog reset digital core");break;
    case 9  : Serial.println ("RTC Watch dog Reset digital core");break;
    case 10 : Serial.println ("Instrusion tested to reset CPU");break;
    case 11 : Serial.println ("Time Group reset CPU");break;
    case 12 : Serial.println ("Software reset CPU");break;
    case 13 : Serial.println ("RTC Watch dog Reset CPU");break;
    case 14 : Serial.println ("for APP CPU, reseted by PRO CPU");break;
    case 15 : Serial.println ("Reset when the vdd voltage is not stable");break;
    case 16 : Serial.println ("RTC Watch dog reset digital core and rtc module");break;
    default : Serial.println ("NO_MEAN");
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

