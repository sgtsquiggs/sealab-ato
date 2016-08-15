# Sealab ATO

A water level controller I hacked together in a weekend.

* Powered by an [attiny85](http://www.atmel.com/images/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf).
* [MCP23008](http://ww1.microchip.com/downloads/en/DeviceDoc/21919e.pdf) I/O expander with I²C interface. Because the attiny85 only has so many free pins.
* Two sensors - high level and low level. When both sensors are HIGH (water is below bobber), it pumps water until both sensors are LOW, or until 10 minutes has passed. My pump is especially slow; it hits the 10 minute mark before the sensors stop it.
* Three LEDs to indicate level: red (both sensors are HIGH), yellow (one sensor is HIGH), green (both sensors are LOW).


This is a break out box that will eventually report data to my aquarium controller once finished.

Don't blame me if this floods your place :)
