/*
 *	BushpunkRobot.h - Bushpunk Robot Library
 *
 *	Darryl Bartlett (BUSHPUNK), 4th November 2016
 *	Email: bushpunker@gmail.com
 *	License: Copyright 2016 - 2020, All rights reserved
 *
 *	Updated: 13/03/2020
 *	Version: 2.0
 */

#ifndef BushpunkRobot_h

#define BushpunkRobot_h
#define thingy		uint8_t
#define repeat(n)	for(int ii = 0; ii < n; ii++)

#include <Arduino.h>
#include <Servo.h>

const uint8_t ANALOGINPUTS_MAX	= 12;
const uint8_t BUZZERS_MAX		= 12;
const uint8_t INPUTS_MAX		= 12;
const uint8_t OUTPUTS_MAX		= 12;
const uint8_t PWMS_MAX			= 12;
const uint8_t SERVOS_MAX		= 12;
const uint8_t SONARS_MAX		= 12;
const uint8_t SWITCHES_MAX		= 12;
const uint8_t TCNT2_VAL			= 255;


class BushpunkRobot {
	public:
		BushpunkRobot();
		thingy		addThingy(const char *component, uint8_t pinNum, uint8_t pin2Num=0);

		// analogIn methods...
		int16_t		readValue(thingy thing, uint16_t from=0, uint16_t to=1023);

		// buzzer methods...
		void		playBuzzer(thingy thing, uint16_t duration, uint8_t times=1);

		// digitalIn methods...
		bool		isOff(thingy thing);
		bool		isOn(thingy thing);

		// digitalOut methods
		void		turnOff(thingy thing);
		void		turnOn(thingy thing);

		// pwm methods...
		void		fadePwm(thingy thing, uint8_t to, uint8_t speed=5);
		void		setPwm(thingy thing, uint8_t to);

		// servo methods...
		uint16_t	getServoPos(thingy thing);
		void		moveServo(thingy thing, uint8_t to, uint8_t speed=0);
		void		restServo(thingy thing);
		void		restServos();
		bool		servoStopped(thingy thing);
		bool		servosStopped();
		void		setServoSpeed(thingy thing, uint8_t speed);
		void		stopServo(thingy thing);
		void		stopServos();
		void		waitForServo(thingy thing);
		void		waitForServos();

		// sonar methods...
		uint16_t	readSonar(thingy thing, uint8_t numOfScans=3);
		void		setSonarRange(thingy thing, uint16_t maxDist);
		bool		sonarSeesSomething(thingy thing, uint8_t numOfScans=3);

		// switch methods...
		void		waitForSwitchOff(thingy thing, uint16_t millisecs=0);
		void		waitForSwitchOn(thingy thing, uint16_t millisecs=0);
		bool		switchIsOff(thingy thing, uint16_t millisecs=0);
		bool		switchIsOn(thingy thing, uint16_t millisecs=0);

	private:
		void	_timer2Setup();
};

#endif
