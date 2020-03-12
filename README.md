Bushpunk Robot Library
======================

This library uses interrupt 2 to provide background speed control and monitoring of servos, smooth fading between PWM "levels", easy switch-debouncing, and other conveniences.

It diverges from the regular Arduino coding style, providing a more natural-language style of coding based around the idea of generically adding "thingy's" to your robot, and then interacting with them in a consistent way.

It depends on the Servo library.

Thingy's can be any of the following:

| Thingy     | Used For |
| ---------- | -------- |
| analogIn   | Reading analogue input pins |
| buzzer     | Interfacing with piezo buzzers |
| digitalIn  | Reading digital input pins |
| digitalOut | Controlling digital output pins |
| pwm        | Setting/fading of signal on PWM pins |
| servo      | Control and monitoring of servos |
| sonar      | Interfacing with Ultrasonic sensors |
| switch     | Monitoring switches using pull-up resistors |

General Methods
---------------
```cpp
thing addThingy("thingy", pinNum, [pin2Num])
```
    thingy:  "analogInput", "buzzer", "digitlInput", "output", "pwm", "servo", "sonar" or "switch".
    pinNum:  The pin number this thingy is connected to (0 - 13 or A0 - A7).
    pin2Num: This is only used for sonars: pinNum=Trig, pin2Num=Echo.
    Returns: A pointer to a "thing" object.

```cpp
repeat(n) {
    ...
}
```
    Runs the code inside the { ... }, n x times.

analogInput:
------------
```cpp
int readValue(thing, [from], [to])
```
    from: Is optional, it defaults to 0.
    to:   Is optional, it defaults to 1023.
    These parameters map the input value (0 - 1023) to be within the requested range (from - to).

buzzer:
-------
```cpp
void playBuzzer(thing, duration, times)
```
    duration: Is the length of the buzz in milliseconds (50% on then 50% off).
    times:    Refers to how many buzzes to play in a row.

digitalInput:
------------
```cpp bool isOn(thing)```
```cpp bool isOff(thing)```

output:
-------
```cpp
void turnOn(thing)
void turnOff(thing)
```
pwm:
----
```
void fadePwm(thing, to, speed)
void setPwm(thing, to)
```
    to: Sets the duty cycle.
          0 = 100% off
        127 = 50%/50%
        255 = 100% on
    Speed: 1 - 5 (5 = fastest).

servo:
------
```cpp
int  getServoPos(thing)
void moveServo(thing, to, [speed])
void restServo(thing)
void restServos()
bool servoStopped(thing)
bool servosStopped()
void setServoSpeed(thing, speed)
void stopServo(thing)
void stopServos()
void waitForServo(thing)
void waitForServos()
```
    to:    The angle of the servo horn, between 0 to 180.
    speed: defaults to 3. Sets the speed of the servo, between 1 to 5. Specifying the speed in the moveServo() method changes the general default speed for that servo.

sonar:
------
```cpp
int  readSonar(thing, [numOfScans])
void setSonarRange(thing, maxDist)
bool sonarSeesSomething(thing, [numOfScans])
```
    numOfScans: defaults to 3. Accuracy is improved by taking multiple readings and averaging them out.
    maxDist:    defaults to 500. This is the maximum distance, in CMs, that will be detected.

switch:
------
```cpp
bool switchIsOn(thing, [millisecs])
bool switchIsOff(thing, [millisecs])
```
    A "switch" is designed to monitor pins that are connected with a pull-up resistor to 5V (HIGH), where turning the switch "on" connects the pin to ground (LOW). It returns TRUE is the switch is off/open/disconnected and FALSE if the switch is on/closed/connected.
    Inputs are processed in real time and the time they become .......
    [millisecs] defaults to 0. This parameter is useful for dealing with switch bounce, and can be used to check that an input has been on/off for at least this length of time before it will return as true.
