# arduPilotSensor

This is a inter-connect software of arduino board between a ardupilot fc and an ultrasonic sensor.
To average the value and remove glitch data.
Target sensor: HC-SR04
Target FC: Pixhawk1, FC-10, PWM input for rangesensor, (1us width per 1mm)

The reason is following.
The ultra sonic sensor like HC-SR04 has a gritch value when some vibration and so on.

- buildPulse: generate PWM pulse internally, Mode 0: sweep 0.1m ~ 2m, Mode 1:0.5m, Mode 2:2m
- U2Pulse: handle a ultrasonic sensor, add mode 3:0.2m ~ 4m of the real sensor 
