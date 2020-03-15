BushpunkRobot - Arduino Library
================================

This library uses Timer 2 to provide background speed control and monitoring of servos, smooth fading between PWM "levels", easy switch-debouncing, as well as other conveniences.

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

The general syntax for using this libraray, is along the lines of...
```cpp
#include <BushpunkRobot.h>

BushpunkRobot robot;
thingy tailServo;

setup() {
    tailServo = robot.addThingy("servo", 4);  // Add a servo (that's been connected to pin 4) and call it tailServo
    robot.setServoSpeed(tailServo, 1);        // Set tailServo's default speed to 1 (slowest)
}

main() {
    robot.moveServo(tailServo, 45);           // Move tailServo to 45 degrees
    robot.waitForServo(tailServo);            // Wait until tailServo reaches its destination (45 degrees)
    robot.moveServo(tailServo, 135);          // Move tailServo to 135 degrees
    robot.waitForServo(tailServo);            // Wait until tailServo reaches its destination (135 degrees)
}
```

General Methods
---------------
```cpp
thing addThingy("thingy", pinNum, [pin2Num])
```
Connects a "thingy" to the robot and returns a pointer to it. This method does all the behind-the-scenes work of appropriately setting up the required pin(s), loading default values, etc.
  * thingy:  ("analogIn", "buzzer", "digitalIn", "digitalOut", "pwm", "servo", "sonar" or "switch")
  * pinNum:  (0 - 13 or A0 - A7) The pin number this thingy is connected to.
  * pin2Num: (0 - 13 or A0 - A7) This is only used for sonars (pinNum = Trig, pin2Num = Echo).
  * Returns: (thing) A pointer to the "thingy" that's been created.

```cpp
repeat(n) {
    //code...
}
```
Runs the block of code, n x times.

analogIn:
---------
```cpp
int readValue(thing, [from], [to])
```
Reads the value of an analogue pin, and returns that value mapped to within the requested range - similar to analogRead().
  * from:    (0 - 65535) Is optional, it defaults to 0.
  * to:      (0 - 65535) Is optional, it defaults to 1023.
  * Returns: (0 - 65535) The actual value (0 - 1023) remapped to within the requested range (from - to).

buzzer:
-------
```cpp
void playBuzzer(thing, duration, times)
```
Useful with piezo buzzers where you need audial feedback. Eg: "beep, beep, beep" = job is finished (like a microwave oven), or "dit, dit, dit, dah, dah, dah, dit, dit, dit" (morse code), etc.
  * duration: (0 - 65535) Is the length of the buzz in milliseconds (50% on, and then 50% off).
  * times:    (0 - 255) Refers to how many buzzes to play in a row.

digitalIn:
----------
```cpp
bool isOn(thing)
```
```cpp
bool isOff(thing)
```
Reads the voltage on a digital pin and returns its state - similar to digitalRead().
  * Returns: (true or false) depending on the signal connected to that pin (5V = On, Gnd = Off).

digitalOut:
-------
```cpp
void turnOn(thing)
```
```cpp
void turnOff(thing)
```
Sets the voltage on a digital pin (On = 5V out, Off = 0V out) - similar to digitalWrite().

pwm:
----
```cpp
void fadePwm(thing, to, speed)
```
```cpp
void setPwm(thing, to)
```
This is handy for setting the brightness of things like LEDs, etc.
  * to:    (0 - 255) Sets the duty cycle... 0 = 100% OFF, 127 = 50%/50% (ON/OFF), 255 = 100% ON
  * speed: (1 - 5, optional, default = 3) The speed to "fade" from the previously set value to this new one (5 = fastest).

servo:
------
Controlling and monitoring several servos at the same time, is a large part of this library. Servos are controlled though a background process, which allows their exact speed and position to be easily controlled while the Arduino continues doing other things. This makes coordinating several servos at once (such as in a robotic arm) very simple and straight forward.
```cpp
int  getServoPos(thing)
```
Useful to discover the current physical location a moving servo, that hasn't yet reached it its destination.
    returns: (0 - 180) The current physical position of this servo.
```cpp
void moveServo(thing, to, [speed])
```
Tells a servo to move to a new position.
  * to:    (0 - 180) The position, in degree, to move to.
  * speed: (1 - 5, optional, default = 3) The (new) default speed that this servo moves at (5 = fastest).
```cpp
void restServo(thing)
```
Disconnects the control signal from the servo, which allows it to "rest" (motor turned off = can be moved "by hand", and uses far less power). Using the moveServo() method will automatically reconnect the servo.

```cpp
void restServos()
```
As above, but disconnects the control signal from all of the servos.
```cpp
bool servoStopped(thing)
```
Useful for checking if a servo has reached its final destination yet.
    returns: (true or false)
```cpp
bool servosStopped()
```
As above, but checks if all the servos have reached their final destination yet.
    returns: (true or false)
```cpp
void setServoSpeed(thing, speed)
```
Sets the default speed for this servo.
  * speed: (1 - 5, 5 = fastest).
```cpp
void stopServo(thing)
```
Immediately stops the servo at whatever position it happens to be in at the time.
```cpp
void stopServos()
```
Immediately stops all servos at whatever positions they happen to be in at the time.
```cpp
void waitForServo(thing)
```
Waits for the servo to reach its destination.
```cpp
void waitForServos()
```
Waits for all servos to reach their destination.

sonar:
------
```cpp
int  readSonar(thing, [numOfScans])
```
Take a distance reading from the sonar (ultrasonic sensor).
  * numOfScans: (1-255, defaults to 3). Accuracy is improved by taking multiple readings and averaging them out.
  * Returns:    The average distance, in CMs, of any detected objects. Returns 0 if nothing is detected.
```cpp
void setSonarRange(thing, maxDist)
```
Sets the maximum range that it will detect look to.
  * maxDist: (defaults to 500) This is the maximum distance, in CMs, that will be detected.
```cpp
bool sonarSeesSomething(thing, [numOfScans])
```
Check if the sonar has detected anything within it's preset range.
  * numOfScans: (1-255, defaults to 3). Accuracy is improved by taking multiple readings and averaging them out.
  * maxDist: (defaults to 500) This is the maximum distance, in CMs, that will be detected.

switch:
------
These methods are for use with switches that are wired to an Arduino pin and ground, taking advantage of the Arduino's internal pull-up resistors (INPUT_PULLUP). In this scenerio, when the switch is ON, the pin is connected to ground.
```cpp
void waitForSwitchOff(thing, [millisecs])
```
Wait for the switch to be turned off (contacts open).
  * [millisecs]: defaults to 0. Requires that a switch had been constantly on (or off) for at least [millisecs], which is useful for dealing with switch bounce.
```cpp
void waitForSwitchOn(thing, [millisecs])
```
Wait for the switch to be turned on (contacts closed, so Arduino pin is connected to ground).
  * [millisecs]: defaults to 0. Requires that a switch had been constantly on (or off) for at least [millisecs], which is useful for dealing with switch bounce.
```cpp
bool switchIsOff(thing, [millisecs])
```
Checks if the switch is turned off (contacts open).
  * [millisecs]: defaults to 0. Requires that a switch had been constantly on (or off) for at least [millisecs], which is useful for dealing with switch bounce.
  * Returns: (true or false)
```cpp
bool switchIsOn(thing, [millisecs])
```
Checks if the switch is turned on (contacts closed, so Arduino pin is connected to ground).
  * [millisecs]: defaults to 0. Requires that a switch had been constantly on (or off) for at least [millisecs], which is useful for dealing with switch bounce.
  * Returns: (true or false)

