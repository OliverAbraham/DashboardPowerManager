//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// # Dashboard power manager
// 
// ## OVERVIEW
// 
// Controls a tablet or raspi based dashboard for home automation or monitoring.
// Add a motion detector to your computer to save energy, switch it on only when somebody's around.
// 
// ## License
// 
// Licensed under Apache licence.
// https://www.apache.org/licenses/LICENSE-2.0
// 
// 
// ## Compatibility
// 
// This is source code for ESP8266 based microcontrollers, e.g.Wemos D1 mini or similiar.
// 
// 
// ## DESCRIPTION
// 
// This program for a Wemos D1 mini or similiar ESP8266 controls an info display / dashboard.
// The purpose is to have a motion sensor attached and wake up the computer or screen saver when motion is detected.
// When I'm not in the office, I want the display to be completely off, in power saving.
// My program will detect the motion signal and then either switch on the dashboard (of it's off) or press the mouse button to disable the screesaver.
// 
// 
// ### OPERATION
// 
// The power manager board has two inputs and two outputs.
// Inputs are connected to:
//   1. a typical IR motion sensor, i.e. Aliexpress SR501 or the smaller AM312.
//      If you like a black one, you could also take the D302S.
//   2. a "power on" signal to detect if the tablet is powered on.
// 
// Outputs are connected to:
//   1. the first relay, used to "simulate" a mouse click
//   2. the second relay, to "simulate" the power button press
// 
// 
// ### MATERIAL
// 
// You need:
// - A tablet computer
// - a Wemos D1 mini or similiar board.
// - a random USB mouse
// - two relays
// - two NPN transistors, BC547B or similiar to drive the relays. 
// - two 10k resistors and two diodes, 1N4148 or similiar.
// 
// 
// ### SETUP
// 
//   1. I took a cheap tablet computer set set up these power saving settings:
//      Blank screen screensaver after 5 minutes, and going into hibernation after 30 or 60 minutes.
// 
//   2. Connect the motion sensor output to D2 (GPIO4) of the Wemos D1.
// 
//   3. Connect the mouse and verify that pressing a button will wake up the screensaver.
//      Now disassemble the mouse, inspect the switch below the mouse button and locate the two pins.
//      Connect the mouse again to your dashboard and verify you've got the right pins by bridging them 
//      on the bottom side with a cable or screwdriver.
//      You should see the mouse click on the screen. And when in screensaving, the display should light up now.
//      Connect the output of the first relay to the button pins.
//      Connect Wemos Pin D1 (GPIO5) to the input of the relay.
//      I know this isn't an academic solution, but simple and cheap method to wake up any computer 
//      with a microcontroller and doesn't require USB logic.
//      If your middle mouse button also wakes the screensaver, use this button, because it's doesn't 
//      disturb you when you operate the dashboard and mouse click comes in between.
// 
// If you're able to configure the dashboard that it wakes up from power saving after a mouse click, you're done.
// If not:
// 
//   4. You now need to find a "power on" signal at or in your dashboard.
//      Often the USB sockets have power even in the power off state, so this isn't usable.
//      You might draw the power signal from the mouse itself, the infrared diode in the mouse might also be off in off state.
//      Otherwise you'll need to open the tablet carefully and find a signal, maybe you'll find the power LED 
//      or the display backlight power signal. You'll need a signal of 3,3V. 
//      If it's higher, you need to put a voltage divider in between (only two resistors).
//      Before connecting this to you Wemos, measure the voltage.
//      Connect this signal to D6 (GPIO12) of the Wemos D1.
// 
//   5. Finally you need to find the two connections of the dashboards' power switch.
//      Connect the output of the second relay to them.
//      Connect Wemos Pin D5 (GPIO14) to the input of the relay.
//      In my case, it was enough to pull one of the power switch pins to ground to switch it on.
//      If you have a similiar device, you don't need the second relay and can connect the D5 pin directly to the
//      power switch. But make sure you don't make a mistake here, because you could brick your Wemos D1.
//      If you connect a relay to drive the power switch, set the constant "PowerSwitchOutputActiveHigh" to true.
// 
// 
// ### CONNECTIONS
// 
// So, the controller has two inputs and two outputs :
// -Input D2: signals motion, connect a standard Aliexpress 5 volt IR motion detector to it
// -Input D6: signals the power state of the display.
// -Output D5 : used to switch on the tablet. Connect this to the power button or drive a relay and add the contact to the power button
// -Output D1 : used to "press" the left mouse button, to disable the screen saverand bring back the display.
// 
// 
// ## INSTALLATION
// 
// Compile and burn the ino file to your Wemos D1 board.
// Don't forget to connect the ground between the Wemos and your dashboard.
// Inspect the program in the serial monitor of your Arduino IDE.
// When the motion detector goes "on" (high), you should see "motion detector on" in serial monitor.
// Switch your dashboard on and off. You should see "dashboard power" changing in serial monitor.
// After motion detection, the program will tell you he's clicking the mouse button. You should hear your first relay.
// Same for the the situation when the dashboard is off and motion is detected, you should hear your second relay.
// 
// 
// ## ADD ON
// 
// The power manager tries to connect to your WIFI, to reach a network time server, to get the current time.
// You can configure the time period where the power manager will not react to motion. My default is from 23:00 to 08:00.
// 
// 
// ## AUTHOR
// 
// Oliver Abraham, mail@oliver - abraham.de, https://www.oliver-abraham.de
// Please feel free to comment and suggest improvements!
// 
// 
// ## SOURCE CODE
// 
// The source code is hosted at https://github.com/OliverAbraham/DashboardPowerManager
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <NTPClient.h> // NTP Client library by Rui Santos, Complete project details at https://randomnerdtutorials.com
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef ____________________________________Constants______________________________________________
// Replace with your network credentials
const bool  UseWifi                     = true;
const char* ssid                        = "FritzBox Gastzugang";
const char* password                    = "Abraham2013";
const char* ntpServer                   = "pool.ntp.org";
const long  gmtOffset_sec               = 0;
const int   daylightOffset_sec          = 3600;  // CET plus 1 hour (GMT)
const int   OffFromHour                 = 23;    // Dashboard will be off from 23:00 to 08:00
const int   OffToHour                   = 8;
const bool  PowerSwitchOutputActiveHigh = false;
const bool  MouseButtonOutputActiveHigh = true;
#endif

#ifndef ____________________________________Fields_________________________________________________
WiFiUDP ntpUDP;

int _motionSensorLastValue            = 0;
int _motionCountsBeforeScreenOn       = 2; // to prevent display wakeup at the first motion event
int _inertia                          = 0;
int _inertiaResetCounter              = 0;
int _secondsToWaitBeforeClickingAgain = 120; // 2 minutes between mouse clicks, in case you're trying to operate the monitor. So the mouse clicks won't disturb you too much.
struct tm _time;
#endif


#ifndef ____________________________________Setup__________________________________________________
void setup()
{
    InitHardware();
    PowerUpGreeting();
    ConnectToWifi();
    GetCurrentTimeFromNtpServer();
}
#endif


#ifndef ____________________________________Loop___________________________________________________
void loop()
{
    GetLocalTime();
    if (UseWifi && WeAreConnectedToWifi() && ItsCurrentlyNightTime())
    {
        printf("%04d-%02d-%02d %02d:%02d:%02d | NIGHT TIME (%02d:00 - %02d:00)\n", 
            _time.tm_year + 1900, _time.tm_mon, _time.tm_mday, _time.tm_hour, _time.tm_min, _time.tm_sec,
            OffFromHour, OffToHour);
        delay(1000);
        return;
    }

    int powerIsOn = CheckIfComputerIsOn();
    int motionSensor = CheckTheMotionSensor();

    printf("%04d-%02d-%02d %02d:%02d:%02d | motion sensor: %s    dashboard power: %s  inertia: %d  inertia reset: %d\n",
        _time.tm_year + 1900, _time.tm_mon, _time.tm_mday, _time.tm_hour, _time.tm_min, _time.tm_sec,
        motionSensor ? "ON " : "off", powerIsOn ? "ON " : "off", _inertia, _inertiaResetCounter);

    if (MotionSensorGoesOn())
    {
        if (powerIsOn)
        {
            if (WaitTimeIsOver())
            {
                ReEnableScreen();
                WaitBeforeClickingAgain();
            }
        }
        else
            SwitchOnComputer();
    }

    ResetCounter();
    FlashInternalLed();
    delay(1000);
}
#endif


#ifndef ____________________________________Implementation_________________________________________________
void InitHardware()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // INPUT #1
    pinMode(D6, INPUT);     // GPIO12 input to detect dashboard power, goes low (0) when dashboard is on
    digitalWrite(D6, HIGH); // pullup on

    // INPUT #2
    pinMode(D2, INPUT);     // GPIO4 motion sensor input, goes high (1) when motion is detected
    digitalWrite(D2, HIGH); // pullup on

    // OUTPUT #1
    pinMode(D5, INPUT);     // GPIO14 output connected to the dashboard power button, pull low to switch on
    if (PowerSwitchOutputActiveHigh)
        digitalWrite(D5, LOW);
    else
        digitalWrite(D5, HIGH);

    // OUTPUT #2
    pinMode(D1, OUTPUT);    // GPIO5 relay output to "press" the left mouse button
    if (MouseButtonOutputActiveHigh)
        digitalWrite(D1, LOW);  // switch off relay
    else
        digitalWrite(D1, HIGH); // switch off relay
}

int MotionSensorGoesOn()
{
    int motionSensor = digitalRead(D2);
    int on = (motionSensor == 1 && _motionSensorLastValue == 0); // detect the positive edge of the signal.
    _motionSensorLastValue = motionSensor;
    return on;
}

int CheckTheMotionSensor()
{
    return digitalRead(D2); // return 1 when the motion sensor gives a high value.
}

int CheckIfComputerIsOn()
{
    return (digitalRead(D6) == 1) ? 0 : 1; // invert the signal. Return 1 when the input is low and vica versa.
}

// Variant for checking the power by connecting to the LED inside the mouse
// Such an LED normally blinks very fast when the computer is on.
// We integrate the pulse to have a stable signal.
//int CheckIfComputerIsOn()
//{
//    powerIsOn = 0;
//    for (int i = 0; i < 10; i++)
//    {
//        powerIsOn |= digitalRead(D6) ? 0 : 1;
//        delay(10);
//    }
//    return powerIsOn;
//}

int WaitTimeIsOver()
{
    _inertia++;
    if (_inertia >= _motionCountsBeforeScreenOn)
    {
        printf("motion sensor: %s    dashboard power: %s  inertia: %d  inertia reset: %d\n", true ? "ON " : "off", true ? "ON " : "off", _inertia, _inertiaResetCounter);
        return 1;
    }
    else
    {
        _inertiaResetCounter = 60;
        return 0;
    }
}

void ResetCounter()
{
    if (_inertiaResetCounter > 0)
    {
        _inertiaResetCounter--;
        if (_inertiaResetCounter == 0)
        {
            _inertia = 0;
        }
    }
}

void ReEnableScreen()
{
    // When the motion sensor goes on, press the left mouse button short two times 
    // the second time to remove the lock screen on some computers
    PressLeftMouseButton();
    
    printf("Waiting 3 seconds\n"); 
    delay(3000);

    PressLeftMouseButton();
}

void WaitBeforeClickingAgain()
{
    for (int i = _secondsToWaitBeforeClickingAgain; i > 0; i--)
    {
        printf("Waiting before doing anything %d\n", i);
        delay(1000);
    }
    _inertia = 0;
}

void SwitchOnComputer()
{
    printf("Dashboard is off, switching on by power button\n");
    for (int i = 0; i < 2; i++)
    {
        printf("pressing...\n");
        pinMode(D5, OUTPUT);                // "press" dashboard power button (pull low)
        if (PowerSwitchOutputActiveHigh)
            digitalWrite(D5, HIGH);         // output high
        else
            digitalWrite(D5, LOW);          // output low
        digitalWrite(LED_BUILTIN, LOW);     // turn the LED on by making the voltage LOW
        delay(2000);

        printf("releasing...\n");
        if (PowerSwitchOutputActiveHigh)
            digitalWrite(D5, LOW);          // output low
        else
            digitalWrite(D5, HIGH);         // output high
        digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off (HIGH is the voltage level)
        delay(2000);
    }
    _inertia = 0;
    printf("\n");
}
        
void PressLeftMouseButton()
{
    printf("Pressing left mouse button\n");
    digitalWrite(LED_BUILTIN, LOW);
    if (MouseButtonOutputActiveHigh)
        digitalWrite(D1, HIGH);
    else
        digitalWrite(D1, LOW);
    delay(200);
    
    digitalWrite(LED_BUILTIN, HIGH);
    if (MouseButtonOutputActiveHigh)
        digitalWrite(D1, LOW);
    else
        digitalWrite(D1, HIGH);
}

void PowerUpGreeting()
{
    printf("\n\n\n\n\n\n\n");
    printf("--------------------------------------------------------------\n");
    printf("Dashboard power manager - Version 2024-12-16\n");
    printf("https://github.com/OliverAbraham/DashboardPowerManager\n");
    printf("mail@oliver-abraham.de\n");
    printf("--------------------------------------------------------------\n");
    printf("Night time is between %02d:00 and %02d:00\n", OffFromHour, OffToHour);


    for (int i = 0; i < 20; i++)
    {
        FlashInternalLed();
        delay(100);
    }
}

void FlashInternalLed()
{
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED on by making the voltage LOW
    delay(10);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off (HIGH is the voltage level)
}

void ConnectToWifi()
{
    if (!UseWifi)
        return;

    Serial.printf("Connecting to %s", ssid);
    WiFi.begin(ssid, password);
    int timeout = 60 * 2;
    while (WiFi.status() != WL_CONNECTED && --timeout > 0)
    {
        delay(500);
        Serial.print(".");
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.printf("\nWiFi connection timed out\n");
        return;
    }
    Serial.printf("\nWiFi connected. IP address:\n");
    Serial.println(WiFi.localIP().toString());
}

bool WeAreConnectedToWifi()
{
    return WiFi.status() == WL_CONNECTED;
}

void GetCurrentTimeFromNtpServer()
{
    Serial.printf("Getting current time from network time server...");
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.printf("No WIFI connection, aborting!\n\n");
        return;
    }

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.printf("Done.\n\n");
}

void GetLocalTime()
{
    if (!getLocalTime(&_time)) {
        Serial.printf("Failed to obtain time | ");
        return;
    }
}

bool ItsCurrentlyNightTime()
{
    return _time.tm_hour >= OffFromHour || _time.tm_hour < OffToHour;
}
#endif
