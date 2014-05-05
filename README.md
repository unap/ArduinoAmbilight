ArduinoAmbilight
================

Ambilight clone with 4 zones using arduino

Processing sketch calculates average colors of 4 areas on screen and sends them over serial to an Arduino.
The Arduino then sets the color of 4 RGB-LED strips accordingly using "pseudo-PWM" on digital pins because the Nano doesn't have enough actual PWM-pins.

Demo:
https://www.youtube.com/watch?v=vlL83EJsWs0
