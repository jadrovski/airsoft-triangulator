# Triangulator device 

## For military-based games (such as airsoft, paintball, lasertag and so on)

This project is a firmware for military gaming device, originally written for airsoft gaming.

### The main idea

To develop of hearing, map skill, orienteering at night.
For the best results there should be three identical devices (except of some preset consts).

### How it works:

We are dividing people on small teams, giving them paper maps on the region and special usb-keys.
The organisators of event should place devices somewhere in region beforehand in determined positions, using prepared GPS coordinates.
The main objective of teams is to find triangulators, interect with them to receive an integer value from each device, determine triangulators position on the map and use these integer values as radiuses.
Radiuses should be intersected in one point (the precision of this point depends on how accurately triangulators position previously determinated).

The point found in the territory means the end of triangulatorGame and the team that found this point first - wins.

### About the hardware:

It's a simple box with 2 buttons, 4 leds, 1 usb female socket and buzzer. Any described timeline value can be preset in constant.
Every minute buzzer is making a short sound, so the devices can be found, we can hear it in a distance of ~ 50m in woods.
When the usb-key is plugged in, the device doing "handshake" by fast turning on and off leds, and beeping with buzzer.
The next step is to hold both buttons for 1 minute (emulating data download). When holding buttons, leds are blinking like "downloading" progress-bar.
Finally, we get the integer value, and it's not easy to read that value correcly. Each led displays it's own digit of value, blinking this digit N-times.
Every iteration of integer being displayed is divided by pause of several seconds.
In any time when usb-key in unplugged, the devices is turning into standby mode, beeping short sound every minute.
