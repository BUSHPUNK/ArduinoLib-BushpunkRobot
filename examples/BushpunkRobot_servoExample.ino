/* BushpunkRobot_servoExample
 *
 * Written by: Darryl Bartlett
 *             bushpunk.art
 * 
 * Scenerio:   A robot dog that has its tail attached directly to a servo.
 *             The dog's tail wags constantly back and forth.
 */


#include <BushpunkRobot.h>

BushpunkRobot robot;                            // Create a BushpunkRobot object, called robot
thingy tailServo;                               // Create a thingy, called tailServo

setup() {
    tailServo = robot.addThingy("servo", 4);    // Add a servo that's connected to pin 4
    robot.setServoSpeed(tailServo, 2);          // Set the servos default speed to 2 (range = 1 to 5)
}

main() {
    robot.moveServo(tailServo, 45);             // Move the servo to 45 degrees
    robot.waitForServo(tailServo);              // Wait until the servo has reached its set position
    robot.moveServo(tailServo, 135);            // Move the servo to 135 degrees
    robot.waitForServo(tailServo);              // Wait until the servo has reached its set position
}
