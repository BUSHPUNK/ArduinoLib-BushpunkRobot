Bushpunk Robot - Arduino Library
================================

This library uses Timer 2 to provide background speed control and monitoring of servos, smooth fading between PWM "levels", easy switch-debouncing, and other conveniences.

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
| sonar      | Interfacing with ultrasonic sensors |
| switch     | Monitoring switches (that are wired up using pull-up resistors) |

The general syntax for using this libraray is:
```cpp
#include <BushpunkRobot.h>

BushpunkRobot robot;
thingy headServo;

setup() {
    headServo = robot.addThingy("servo", 4);
    robot.setServoSpeed(headServo, 1);
}

main() {
    robot.moveServo(headServo, 45);
    robot.waitForServo(headServo);
    robot.moveServo(headServo, 135);
    robot.waitForServo(headServo);
}
```

General Methods
---------------
```cpp
thing addThingy("thingy", pinNum, [pin2Num])
```
    thingy:  "analogIn", "buzzer", "digitalIn", "digitalOut", "pwm", "servo", "sonar" or "switch".
    pinNum:  The pin number this thingy is connected to (0 - 13 or A0 - A7).
    pin2Num: This is only used for sonars: pinNum=Trig, pin2Num=Echo.
    Returns: A pointer to a "thing" object.

```cpp
repeat(n) {
    code();
    code();
    andMoreCode();
}
```
    Runs the block of code, n x times.

analogIn:
---------
```cpp
int readValue(thing, [from], [to])
```
    from:    Is optional, it defaults to 0.
    to:      Is optional, it defaults to 1023.
    Returns: The input value (0 - 1023) mapped to within the requested range (from - to).

buzzer:
-------
```cpp
void playBuzzer(thing, duration, times)
```
    duration: Is the length of the buzz in milliseconds (50% on then 50% off).
    times:    Refers to how many sequential buzzes to play.

digitalIn:
----------
```cpp
bool isOn(thing)
```
```cpp
bool isOff(thing)
```

digitalOut:
-------
```cpp
void turnOn(thing)
```
```cpp
void turnOff(thing)
```

pwm:
----
```cpp
void fadePwm(thing, to, speed)
```
```cpp
void setPwm(thing, to)
```
    to:    Sets the duty cycle... 0 = 100% OFF, 127 = 50%/50% (ON/OFF), 255 = 100% ON
    speed: The speed to "fade" from the previously set value to this new one, 1 to 5 (5 = fastest).

servo:
------
```cpp
int  getServoPos(thing)
```
```cpp
void moveServo(thing, to, [speed])
```
```cpp
void restServo(thing)
```
```cpp
void restServos()
```
```cpp
bool servoStopped(thing)
```
```cpp
bool servosStopped()
```
```cpp
void setServoSpeed(thing, speed)
```
```cpp
void stopServo(thing)
```
```cpp
void stopServos()
```
```cpp
void waitForServo(thing)
```
```cpp
void waitForServos()
```
    to:    the angle of the servo horn, between 0 to 180.
    speed: defaults to 3. Sets the default speed of the servo, 1 to 5 (1 = slowest). Specifying the speed in the moveServo() method also changes the default speed for that servo.

sonar:
------
```cpp
int  readSonar(thing, [numOfScans])
```
```cpp
void setSonarRange(thing, maxDist)
```
```cpp
bool sonarSeesSomething(thing, [numOfScans])
```
    numOfScans: defaults to 3. Accuracy is improved by taking multiple readings and averaging them out.
    maxDist:    defaults to 500. This is the maximum distance, in CMs, that will be detected.

switch:
------
```cpp
bool switchIsOn(thing, [millisecs])
```
```cpp
bool switchIsOff(thing, [millisecs])
```
    [millisecs]: defaults to 0. This is useful for dealing with switch bounce, and can be used to check that a switch had been on/off for at least [millisecs] in time.
    A "switch" monitors a pin that is connected with a pull-up resistor (INPUT_PULLUP).
    It returns TRUE when the switch is closed (connected to Gnd) and FALSE when the switch is open (5V via pull-up resister).

