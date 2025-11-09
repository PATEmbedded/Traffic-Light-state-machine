#include <Arduino.h>
#include "PAT_OS.h"

//                State Machine Chart for Traffic Light Simulation
//
//                                                             +----------------------------+
//       __ __ __ __ __ __ __ __             - < - < - < - < - |                            |
//      |                       v           v                  |    Event  ──[ ■■■ ]──      |
//      ^                       v           v                  |                            |
//      |      +-----------------------------------+           |       Passerby Button      |
//      |      |          (Stop)                   |           |   Transition to RED STATE  |
//      |      |         RED STATE                 |           +----------------------------+
//      |      |                                   |
//      |      |    ╔═══════╗                      |
//      ^      |    ║ 🔴🔴 ║  // Red light ON     |
//      |      |    ║       ║  // Yellow light OFF |
//      |      |    ║       ║  // Green light OFF  |
//      |      |    ╚═══════╝                      |
//      |      |                                   |
//      |      +-----------------------------------+
//      |                       |
//      ^                       v (Timer Expired)
//      |                       |
//      |      +-----------------------------------+
//      |      |        (Caution)                  |
//      |      |      YELLOW STATE                 |
//      |      |                                   |
//      |      |    ╔═══════╗                      |
//      ^      |    ║       ║  // Red light OFF    |
//      |      |    ║ 🟡🟡 ║  // Yellow light ON  |
//      |      |    ║       ║  // Green light OFF  |
//      |      |    ╚═══════╝                      |
//      |      |                                   |
//      |      +-----------------------------------+
//      |                       |
//      ^                       v(Timer Expired)
//      |                       |
//      |      +-----------------------------------+
//      |      |           (Go)                    |
//      |      |       GREEN STATE                 |
//      |      |                                   |
//      |      |    ╔═══════╗                      |
//      ^      |    ║       ║  // Red light OFF    |
//      |      |    ║       ║  // Yellow light OFF |
//      |      |    ║ 🟢🟢 ║  // Green light ON   |
//      |      |    ╚═══════╝                      |
//      |      |                                   |
//      |      +-----------------------------------+
//      |                       |
//      ^                       v(Timer Expired)
//      |__ __ __ __ __ __ __ __|

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool botton_is_Pushed(void);
void print(String state);
void println(String state);
void setup(void)
{
      os.init();
      //========================================================================================================
      os.sm["trafficLight"].StartState("Red", []()
                                       {
                                             //-----
                                             println("Red");
                                             delay(3000);
                                             os.sm["trafficLight"].setNextState("Yellow");
                                             //-----
                                       });
      //========================================================================================================
      os.sm["trafficLight"].addState("Yellow", []()
                                     {
                                           //-----
                                           if (os.sm["trafficLight"].getState(Q_1) == "Red")
                                           {
                                                 for (int i = 0; i < 3; i++)
                                                 {
                                                       print("Yellow");
                                                       delay(200);
                                                       print("Off");
                                                       delay(200);
                                                 }
                                                 println("Yellow");

                                                 os.sm["trafficLight"].setNextState("Green");
                                           }
                                           else if (os.sm["trafficLight"].getState(Q_1) == "Green")
                                           {
                                                 println("Yellow");
                                                 delay(1000);
                                                 os.sm["trafficLight"].setNextState("Red");
                                           }
                                           //-----
                                     });
      //========================================================================================================
      os.sm["trafficLight"].addState("Green", []()
                                     {
                                           //-----
                                           println("Green");
                                           delay(3000);
                                           os.sm["trafficLight"].setNextState("Yellow");
                                           //-----
                                     });
      //========================================================================================================
      os.sm["trafficLight"].onEnterState([](const String &stateName)
                                         {
                                               //-----
                                               Serial.println("\nvvvvvvvvvvvvvvv Entering state: " + stateName + " vvvvvvvvvvvvvvv");
                                               //-----
                                         });

      os.sm["trafficLight"].onExitState([](const String &stateName)
                                        {
                                              //-----
                                              Serial.println("--------------- Exiting state: " + stateName + " ---------------\n");

                                              //-----
                                        });

      os.sm["trafficLight"].setErrorHandler([](const String &errorMessage)
                                            {
                                                  //-----
                                                  Serial.println("Error: " + errorMessage);
                                                  //-----
                                            });
      //========================================================================================================
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void)
{
      //--------------------- Event Botton-----------------
      // Event Button
      if (botton_is_Pushed())
      {
            os.sm["trafficLight"].setNextState("Red");
      }
      //---------------------------------------------------
      os.sm["trafficLight"].update();
      delay(100);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool botton_is_Pushed(void)
{
      String input = "";

      if (Serial.available() > 0) // Read serial input
      {
            input = Serial.readStringUntil('\n'); // Read input until newline
      }

      return (input == ("1")); // retuen true if scan = "1" char 
}

void print(String state)
{
      if (state == "Red")
      {
            Serial.print(" 🔴 ");
      }
      else if (state == "Yellow")
      {
            Serial.print(" 🟡 ");
      }
      else if (state == "Green")
      {
            Serial.print(" 🟢 ");
      }
      else
      {
            Serial.print(" ⚫ ");
      }
}
void println(String state)
{
      if (state == "Red")
      {
            Serial.println(" 🔴 ");
      }
      else if (state == "Yellow")
      {
            Serial.println(" 🟡 ");
      }
      else if (state == "Green")
      {
            Serial.println(" 🟢 ");
      }
      else
      {
            Serial.println(" ⚫ ");
      }
}