#include <BushpunkRobot.h>

BushpunkRobot robot;				// Create a BushpunkRobot object, called robot
thingy headServo;				// Create a thingy, called headServo

setup() {
    headServo = robot.addThingy("servo", 4);	// Add a servo that's connected to pin 4
    robot.setServoSpeed(headServo, 2);		// Set the servos default speed to 2 (range = 1 to 5)
}

main() {
    robot.moveServo(headServo, 45);		// Move the servo to 45 degrees
    robot.waitForServo(headServo);		// Wait until the servo has reached its set position
    robot.moveServo(headServo, 135);		// Move the servo to 135 degrees
    robot.waitForServo(headServo);		// Wait until the servo has reached its set position
}
