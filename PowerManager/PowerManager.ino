// # Dashboard power manager
// 
// ## OVERVIEW
// Controls a tablet or raspi based dashboard for home automation or monitoring.Add a motion detector to your computer to save energy, switch it on only when somebody's around.
// 
// ## License
// Licensed under Apache licence.
// https://www.apache.org/licenses/LICENSE-2.0
// 
// 
// ## Compatibility
// This is source code for ESP8266 based microcontrollers, e.g.Wemos D1 mini or similiar.
// 
// 
// ## DESCRIPTION
// This program for a Wemos D1 mini or similiar ESP8266 controls an info display.
// The purpose is to have a motion sensor attached and wake up the computer or screen saver when motion is detected.
// 
// ### Setup
// I have a tablet computer(I call it display) with "normal" power saving settings :
// Screensaver after some minutes, and go to hibernation after a longer time, i.e.one hour.
// I tore out the board out of a cheap computer mouseand attached it to the display.
// Pressing the left mouse button reactivates the display, as long as the display is under power.
// I added a relay to my Wemos D1 that connects to the left mouse button.
// This is a simpleand cheap method to wake up any computer with a microcontrollerand doesn't require USB logic.
// Of couse this doesn't work for every display in power off state, because you cannot wake every system by a mouse click.
// For this type I've atached a second line to the power button of the display. In my case this line only needs to be pulled low to switch it on.
// If needed you can put a relay in between.
// The program decides which method to use. "press" the power button or the mouse button.
// 
// ### Connections
// So, the controller has two inputsand two outputs :
// -Input 1 : signals motion, connect a standard 5 volt IR motion detector to it
// -Input 2 : signals the power state of the display.
// -Output 1 : used to switch on the tables.Connect this to the power button or drive a relay and add the contact to the power button
// -Output 2 : used to "press" the left mouse button, to disable the screen saverand bring back the display.
// 
// 
// ## INSTALLATION
// compile and burn the ino file to your ESP / Wemos board.Connect the four lines to your display / tablet computer.
// 
// 
// ## AUTHOR
// Oliver Abraham, mail@oliver - abraham.de, https://www.oliver-abraham.de
// Please feel free to commentand suggest improvements!
// 
// 
// ## SOURCE CODE
// The source code is hosted at :
// https://github.com/OliverAbraham/DashboardPowerManager



int motionSensorLastValue = 0;

void setup()
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
    digitalWrite(D5, HIGH); // pullup on
    
    // OUTPUT #2
    pinMode(D1, OUTPUT);    // GPIO5 relay output to "press" the left mouse button
    digitalWrite(D1, LOW);  // switch off relay
}

// the loop function runs over and over again forever
void loop()
{
    int powerIsOn = CheckIfComputerIsOn();
    int motionSensor = CheckTheMotionSensor();

    printf("motion sensor: %s    dashboard power is: %s\n", motionSensor ? "ON" : "off", powerIsOn ? "ON" : "off");

    if (MotionSensorGoesOn())
    {
       if (powerIsOn)
           ReEnableScreen();
       else
           SwitchOnComputer();
    }

    FlashInternalLed();
    delay(1000);
}

int MotionSensorGoesOn()
{
    int motionSensor = digitalRead(D2);
    int on = (motionSensor == 1 && motionSensorLastValue == 0); // detect the positive edge of the signal.
    motionSensorLastValue = motionSensor;
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

void ReEnableScreen()
{
    // When the motion sensor goes on, press the left mouse button short two times 
    // the second time to remove the lock screen on some computers
    PressLeftMouseButton();
    delay(1000);
    PressLeftMouseButton();
    delay(1000);
}

void SwitchOnComputer()
{
    printf("Dashboard is off, switching on by power button ");
    for (int i = 0; i < 2; i++)
    {
        printf("pressing...");
        pinMode(D5, OUTPUT);                // "press" dashboard power button (pull low)
        digitalWrite(D5, LOW);              // output low
        digitalWrite(LED_BUILTIN, LOW);     // turn the LED on by making the voltage LOW
        delay(2000);

        printf("releasing...");
        pinMode(D5, INPUT);                 // "press" dashboard power button (pull low)
        digitalWrite(D5, HIGH);             // pullup on
        digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off (HIGH is the voltage level)
        delay(2000);
    }
    printf("\n");
}
        
void PressLeftMouseButton()
{
    printf("Pressing left mouse button\n");
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(D1, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(D1, LOW);
}

void FlashInternalLed()
{
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED on by making the voltage LOW
    delay(10);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off (HIGH is the voltage level)
}
