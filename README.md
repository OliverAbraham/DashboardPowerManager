# Dashboard power manager

![](https://img.shields.io/github/license/oliverabraham/dashboardpowermanager) ![](https://img.shields.io/github/languages/count/oliverabraham/dashboardpowermanager) ![GitHub Repo stars](https://img.shields.io/github/stars/oliverabraham/dashboardpowermanager?label=repo%20stars) ![GitHub Repo stars](https://img.shields.io/github/stars/oliverabraham?label=user%20stars)

## OVERVIEW

Controls a tablet or raspi based dashboard for home automation or monitoring. Add a motion detector to your computer to save energy, switch it on only when somebody's around.


## License

Licensed under Apache licence.
https://www.apache.org/licenses/LICENSE-2.0


## DESCRIPTION

This program for a Wemos D1 mini or similiar ESP8266 controls an info display / dashboard.
The purpose is to have a motion sensor attached and wake up the computer or screen saver when motion is detected.
When I'm not in the office, I want the display to be completely off, in power saving.
My program will detect the motion signal and then either switch on the dashboard (of it's off) or press the mouse button to disable the screesaver.


## OPERATION

The power manager board has two inputs and two outputs.
Inputs are connected to:
  1. a typical IR motion sensor, i.e. Aliexpress SR501 or the smaller AM312.
     If you like a black one, you could also take the D302S.
  2. a "power on" signal to detect if the tablet is powered on.

Outputs are connected to:
  1. the first relay, used to "simulate" a mouse click
  2. the second relay, to "simulate" the power button press


## MATERIAL

You need:
- A tablet computer
- a Wemos D1 mini or similiar board.
- a random USB mouse
- two relays
- two NPN transistors, BC547B or similiar to drive the relays. 
- two 10k resistors and two diodes, 1N4148 or similiar.


## SETUP

  1. I took a cheap tablet computer set set up these power saving settings:
     Blank screen screensaver after 5 minutes, and going into hibernation after 30 or 60 minutes.

  2. Connect the motion sensor output to D2 (GPIO4) of the Wemos D1.

  3. Connect the mouse and verify that pressing a button will wake up the screensaver.
     Now disassemble the mouse, inspect the switch below the mouse button and locate the two pins.
     Connect the mouse again to your dashboard and verify you've got the right pins by bridging them 
     on the bottom side with a cable or screwdriver.
     You should see the mouse click on the screen. And when in screensaving, the display should light up now.
     Connect the output of the first relay to the button pins.
     Connect Wemos Pin D1 (GPIO5) to the input of the relay.
     I know this isn't an academic solution, but simple and cheap method to wake up any computer 
     with a microcontroller and doesn't require USB logic.
     If your middle mouse button also wakes the screensaver, use this button, because it's doesn't 
     disturb you when you operate the dashboard and mouse click comes in between.

If you're able to configure the dashboard that it wakes up from power saving after a mouse click, you're done.
If not:

  4. You now need to find a "power on" signal at or in your dashboard.
     Often the USB sockets have power even in the power off state, so this isn't usable.
     You might draw the power signal from the mouse itself, the infrared diode in the mouse might also be off in off state.
     Otherwise you'll need to open the tablet carefully and find a signal, maybe you'll find the power LED 
     or the display backlight power signal. You'll need a signal of 3,3V. 
     If it's higher, you need to put a voltage divider in between (only two resistors).
     Before connecting this to you Wemos, measure the voltage.
     Connect this signal to D6 (GPIO12) of the Wemos D1.

  5. Finally you need to find the two connections of the dashboards' power switch.
     Connect the output of the second relay to them.
     Connect Wemos Pin D5 (GPIO14) to the input of the relay.
     In my case, it was enough to pull one of the power switch pins to ground to switch it on.
     If you have a similiar device, you don't need the second relay and can connect the D5 pin directly to the
     power switch. But make sure you don't make a mistake here, because you could brick your Wemos D1.
     If you connect a relay to drive the power switch, set the constant "PowerSwitchOutputActiveHigh" to true.


## CONNECTIONS

So, the controller has two inputs and two outputs :
-Input D2: signals motion, connect a standard Aliexpress 5 volt IR motion detector to it
-Input D6: signals the power state of the display.
-Output D5 : used to switch on the tablet. Connect this to the power button or drive a relay and add the contact to the power button
-Output D1 : used to "press" the left mouse button, to disable the screen saverand bring back the display.


## INSTALLATION

Compile and burn the ino file to your Wemos D1 board.
Don't forget to connect the ground between the Wemos and your dashboard.
Inspect the program in the serial monitor of your Arduino IDE.
When the motion detector goes "on" (high), you should see "motion detector on" in serial monitor.
Switch your dashboard on and off. You should see "dashboard power" changing in serial monitor.
After motion detection, the program will tell you he's clicking the mouse button. You should hear your first relay.
Same for the the situation when the dashboard is off and motion is detected, you should hear your second relay.


## ADD ON

The power manager tries to connect to your WIFI, to reach a network time server, to get the current time.
You can configure the time period where the power manager will not react to motion. My default is from 23:00 to 08:00.


## AUTHOR

Oliver Abraham, mail@oliver - abraham.de, https://www.oliver-abraham.de
Please feel free to comment and suggest improvements!


## SOURCE CODE

The source code is hosted at https://github.com/OliverAbraham/DashboardPowerManager


# MAKE A DONATION !

If you find this application useful, buy me a coffee!
I would appreciate a small donation on https://www.buymeacoffee.com/oliverabraham

<a href="https://www.buymeacoffee.com/app/oliverabraham" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>

