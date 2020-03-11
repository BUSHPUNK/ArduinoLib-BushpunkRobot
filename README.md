Bushpunk Robot Library
======================

General Methods
---------------

thing addThingy("thingy", pinNum, [pin2Num])

    thingy:  "analogInput", "buzzer", "input", "output", "pwm", "servo" or "sonar".
    pinNum:  The pin number this thingy is connected to (0 - 13 or A0 - A7).
    pin2Num: This is only used for sonars: pinNum=Trig, pin2Num=Echo.

analogInput:
------------

int readValue(thing, [from], [to])

    from: Defaults to 0.
    to:   Defaults to 1023.
    These parameters map the input value to be within the required range.

buzzer:
-------

void playBuzzer(thing, duration, times)

    duration: Is the length of the buzz in milliseconds (50% on then 50% off).
    times:    Refers to how many buzzes to play in a row.

input:
------

bool isOn(thing, [millisecs])
bool isOff(thing, [millisecs])

    [millisecs] defaults to 0.
    This parameter is useful for dealing with switch bounce, and can be used to
    check that an input has been on or off for at least this length of time
    before it will return as true.

output:
-------

void turnOn(thing)

void turnOff(thing)

pwm:
----

void fadePwm(thing, to, speed)

void setPwm(thing, to)

    to: Sets the duty cycle.
          0 = 100% off
        127 = 50%/50%
        255 = 100% on
    Speed: 1 - 5 (5 = fastest).

servo:
------

int  getServoPos(thing)
void moveServo(thing, to)
void restServo(thing)
void restServos()
bool servoStopped(thing)
bool servosStopped()
void stopServo(thing)
void stopServos()
void waitForServo(thing)
void waitForServos()

    to: This is the angle of the servo horn, in degrees, between 0 to 180.

sonar:
------

int  readSonar(thing, [numOfScans])
void setSonarRange(thing, maxDist)
bool sonarSeesSomething(thing, [numOfScans])

    numOfScans: defaults to 3.
    Accuracy is improved by taking multiple readings and averaging them out.
    maxDist: defaults to 500.
    This is the maximum distance, in CMs, that will be detected.

