# Dashboard power manager

![](https://img.shields.io/github/license/oliverabraham/dashboardpowermanager) ![](https://img.shields.io/github/languages/count/oliverabraham/dashboardpowermanager) ![GitHub Repo stars](https://img.shields.io/github/stars/oliverabraham/dashboardpowermanager?label=repo%20stars) ![GitHub Repo stars](https://img.shields.io/github/stars/oliverabraham?label=user%20stars)

## OVERVIEW

Controls a tablet or raspi based dashboard for home automation or monitoring. Add a motion detector to your computer to save energy, switch it on only when somebody's around.


## License

Licensed under Apache licence.
https://www.apache.org/licenses/LICENSE-2.0


## Compatibility

This is source code for ESP8266 based microcontrollers, e.g. Wemos D1 mini or similiar.


## DESCRIPTION

This program for a Wemos D1 mini or similiar ESP8266 controls an info display. 
The purpose is to have a motion sensor attached and wake up the computer or screen saver when motion is detected.

### Setup
I have a tablet computer (I call it display) with "normal" power saving settings: 
Screensaver after some minutes, and go to hibernation after a longer time, i.e. one hour.
I tore out the board out of a cheap computer mouse and attached it to the display. 
Pressing the left mouse button reactivates the display, as long as the display is under power.
I added a relay to my Wemos D1 that connects to the left mouse button. 
This is a simple and cheap method to wake up any computer with a microcontroller and doesn't require USB logic.
Of couse this doesn't work for every display in power off state, because you cannot wake every system by a mouse click.
For this type I've atached a second line to the power button of the display. In my case this line only needs to be pulled low to switch it on.
If needed you can put a relay in between.
The program decides which method to use. "press" the power button or the mouse button.

### Connections
So, the controller has two inputs and two outputs:
- Input 1: signals motion, connect a standard 5 volt IR motion detector to it
- Input 2: signals the power state of the display.
- Output 1: used to switch on the tables. Connect this to the power button or drive a relay and add the contact to the power button
- Output 2: used to "press" the left mouse button, to disable the screen saver and bring back the display.



## INSTALLATION

Compile and burn the ino file to your ESP/Wemos board. Connect the four lines to your display/tablet computer.



## ATTACHING TO THE DISPLAY

### Driving the mouse button
You need to connect a relay to your Wemos. The relay contact needs to be connected parallel to the left button switch in your mouse.
Buy a cheap mouse and drag out the logic board. You will easily locate the mouse button. You only need to try out which two of the three switch pins to use.

If the left mouse click interferes with your display software, you can switch over to the middle mouse button, which often has no negative effects.
A critical step is connecting your Wemos to the power button and find a good source inside your tablet for the power indicator.

### Getting access to the power switch
Opening a tablet computer might be something you don't wanna do, because it requires experience and the knowledge to find the power button 
connectors on the board. You might brick your tablet if you are new. In this case you might forget about this and set your energy settings to not go into hibernation, 
only use the screen saver part. In this case you might connect your motion sensor to a relay that directly drives the mouse button switch.
So you can forget about my program :-)

I used the following method:
I opened the tablet back side carefully, located the battery and pulled out the cable from the battery. 
Then I knew the system has no power and I was able to safely disassemble the logic board. 
The unit only had one board. On the bottom side I soldered one cable to one pin of the power switch (I measured the second pin was connected to 
ground). 
If you are in doubt, just connect two cables to both pins of the power switch and connect them to a relay switch. 
So, use a relay between the wemos and the tablet to "push" the power switch. This is a safe method.

### Getting the power state
When I had the tablet opened but not yet disassembled, I made some tries and measured pins one some switches to get a power signal when
the table is under power and no signal when it's off. I found a pin on the sound volume switch that has 1.7 volts when on.
Another possibility is to locate the micro SD slot that you'll find on most of the models. Google for "micro sd connector pinout"
and locate the VDD or VCC pin on your board. This will have most likely a stable power-on signal.
Another possibility is to put a cable to the power LED or charge LED. In my case the charge LED only lights up when the tablet is off,
so i could also use this and invert the logic in the ino file.

### Powering the Wemos
Logically the Wemos has to be under power permanently. I took the following solution:
Because the tablet is always connecting to the charger, I cut the charger cable and grabbed the power lines.
Sometimes it's a two-pin cable with 19 volts, in my case it's a 5 volts USB cable with a micro USB connector.
In any case I'm attaching a simple step down voltage regulator from Aliexpress that delivers a 3.3 volt output.
I have used the "DC-DC Buck Converter Step Down Modul LM2596", which is robust and adjustable. It doesn't care about the input voltage
and costs under a dollar.
Example: https://de.aliexpress.com/item/1005005911907067.html
This feeds the Wemos.

### The motion sensor
I use the "AM312" round 5V IR motion sensor from aliexpress: https://de.aliexpress.com/item/1005006057413375.html
You could also use the HC-SR501: https://de.aliexpress.com/item/1874954952.html
The output pin can be connected directly to the Wemos input.



## AUTHOR

Oliver Abraham, mail@oliver-abraham.de, https://www.oliver-abraham.de

Please feel free to comment and suggest improvements!



## SOURCE CODE

The source code is hosted at:

https://github.com/OliverAbraham/DashboardPowerManager


# MAKE A DONATION !

If you find this application useful, buy me a coffee!
I would appreciate a small donation on https://www.buymeacoffee.com/oliverabraham

<a href="https://www.buymeacoffee.com/app/oliverabraham" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>

