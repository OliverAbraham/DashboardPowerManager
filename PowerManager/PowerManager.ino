
int motionSensorLastValue = 0;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(D6, INPUT);     // GPIO12 input to detect dashboard power, goes low (0) when dashboard is on
    digitalWrite(D6, HIGH); // pullup on

    pinMode(D8, INPUT);     // GPIO15 input to detect dashboard power, old (cannot boot when high)
    digitalWrite(D8, HIGH); // pullup on

    pinMode(D2, INPUT);     // GPIO4 motion sensor input, goes high (1) when motion is detected
    digitalWrite(D2, HIGH); // pullup on
    
    pinMode(D5, INPUT);     // GPIO14 output connected to the dashboard power button, pull low to switch on
    digitalWrite(D5, HIGH); // pullup on
    
    pinMode(D1, OUTPUT);    // GPIO5 relay output to "press" the left mouse button
    digitalWrite(D1, LOW);  // switch off relay
}

// the loop function runs over and over again forever
void loop()
{
    int powerIsOn = (digitalRead(D6) == 1) ? 0 : 1;
    int motionSensor = digitalRead(D2);

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
    int on = (motionSensor == 1 && motionSensorLastValue == 0);
    motionSensorLastValue = motionSensor;
    return on;
}

// variant for checking the power by inspecting the LED inside the mouse
//int checkIfComputerIsOn()
//{
//    powerIsOn = 0;
//    for (int i = 0; i < 10; i++)
//    {
//        powerIsOn |= digitalRead(D8) ? 0 : 1;
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
