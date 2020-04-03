/*
 *	BushpunkRobot.cpp - Bushpunk Robot Library
 *
 *	Darryl Bartlett (BUSHPUNK), 4th November 2016
 *	Email: bushpunker@gmail.com
 *	License: Copyright 2016 - 2020, All rights reserved
 *
 *  Updated: 13/03/2020
 *  Version: 2.0
 */

#include "BushpunkRobot.h"

volatile uint8_t  _pwms[PWMS_MAX][4];			// [0]=pinNum, [1]=brightness, [2]=to, [3]=speed
volatile uint8_t  _servoSpecs[SERVOS_MAX][4];	// [0]=pinNum, [1]speed, [2]=pos, [3]=dest
volatile uint32_t _switches[INPUTS_MAX][2];		// [0]=pinNum, [1]=1st-On-time

uint8_t  _analogInputs[ANALOGINPUTS_MAX];
uint8_t  _buzzers[BUZZERS_MAX];
uint8_t  _inputs[INPUTS_MAX];
uint8_t  _outputs[OUTPUTS_MAX];
uint16_t _sonars[SONARS_MAX][3];				// [0]=triggerPin, [1]=echoPin, [3]=maxDistance

uint8_t	_numOfAnalogInputs = 0;
uint8_t	_numOfBuzzers = 0;
uint8_t	_numOfInputs = 0;
uint8_t	_numOfOutputs = 0;
uint8_t	_numOfPwms = 0;
uint8_t	_numOfServos = 0;
uint8_t	_numOfSonars = 0;
uint8_t	_numOfSwitches = 0;
Servo	_servos[SERVOS_MAX];

void _isrPwmFade() {
	uint8_t from, to, speed, inc, newVal;
	int8_t dir = 1;
	for (uint8_t i = 0; i < _numOfPwms; i++) {
		from = _pwms[i][1];
		to = _pwms[i][2];
		if (from != to) {
			speed = _pwms[i][3];
			inc = 1 << (speed - 1);
			if (to - inc < from and from < to + inc) inc = 1;
			if (from > to) dir = -1;
			if (speed == 0)
				newVal = to;
			else
				newVal = from + (inc * dir);
			_pwms[i][1] = newVal;
			analogWrite(_pwms[i][0], newVal);
		}
	}
}

void _isrServoMove() {
	uint8_t pos, dest, inc, newVal;
	for (uint8_t i = 0; i < _numOfServos; i++) {
		pos = _servoSpecs[i][2];
		dest = _servoSpecs[i][3];
		if (pos != dest) {
			inc = 1 << (_servoSpecs[i][1] - 1);
			if (dest - inc < pos and pos < dest + inc) inc = abs(pos - dest);
			if (pos > dest) inc *= -1;
			newVal = pos + inc;
			_servoSpecs[i][2] = newVal;
			_servos[i].write(newVal);
		}
	}
}

void _isrCheckEvents() {
	for (uint8_t i = 0; i < _numOfSwitches; i++) {
		if (digitalRead(_switches[i][0]) == LOW) {
			if (_switches[i][1] == 0) _switches[i][1] = millis();
		} else {
			_switches[i][1] = 0;
		}
	}
}

ISR(TIMER2_OVF_vect) {
	static uint8_t count = 0;
	TCNT2 = TCNT2_VAL;
	if (++count >= 25) {
		count -= 25;
		_isrServoMove();
		_isrCheckEvents();
		_isrPwmFade();
	}
}


// General methods...

BushpunkRobot::BushpunkRobot() {
	randomSeed(analogRead(0)+analogRead(1)+analogRead(2)+analogRead(3)+analogRead(4)+analogRead(5));
	_timer2Setup();
}

thingy BushpunkRobot::addThingy(const char *component, uint8_t pinNum, uint8_t pin2Num) {
	int8_t result = -1;
	if (strcmp(component, "analogIn") == 0) {
		if (_numOfAnalogInputs < ANALOGINPUTS_MAX) {
			_analogInputs[_numOfAnalogInputs] = pinNum;
			result = _numOfAnalogInputs++;
		}
	}
	else if (strcmp(component, "buzzer") == 0) {
		if (_numOfBuzzers < BUZZERS_MAX) {
			_buzzers[_numOfBuzzers] = pinNum;
			digitalWrite(pinNum, LOW);
			pinMode(pinNum, OUTPUT);
			result = _numOfBuzzers++;
		}
	} else if (strcmp(component, "digitalIn") == 0) {
		if (_numOfInputs < INPUTS_MAX) {
			_inputs[_numOfInputs] = pinNum;
			pinMode(pinNum, INPUT);
			result = _numOfInputs++;
		}
	} else if (strcmp(component, "digitalOut") == 0) {
		if (_numOfOutputs < OUTPUTS_MAX) {
			_outputs[_numOfOutputs] = pinNum;
			digitalWrite(pinNum, LOW);
			pinMode(pinNum, OUTPUT);
			result = _numOfOutputs++;
		}

	} else if (strcmp(component, "pwm") == 0) {
		if (_numOfPwms < PWMS_MAX) {
			_pwms[_numOfPwms][0] = pinNum;
			_pwms[_numOfPwms][1] = 0;
			_pwms[_numOfPwms][2] = 0;
			_pwms[_numOfPwms][3] = 0;
			analogWrite(pinNum, 0);
			result = _numOfPwms++;
		}
	} else if (strcmp(component, "servo") == 0) {
		if (_numOfServos < SERVOS_MAX) {
			_servos[_numOfServos].attach(pinNum);
			_servos[_numOfServos].write(90);
			_servoSpecs[_numOfServos][0] = pinNum;
			_servoSpecs[_numOfServos][1] = 3;
			_servoSpecs[_numOfServos][2] = 90;
			_servoSpecs[_numOfServos][3] = 90;
			result = _numOfServos++;
		}
	} else if (strcmp(component, "sonar") == 0) {
		if (_numOfSonars < SONARS_MAX) {
			_sonars[_numOfSonars][0] = pinNum;
			_sonars[_numOfSonars][1] = pin2Num;
			_sonars[_numOfSonars][2] = 100;
			pinMode(pinNum, OUTPUT);
			pinMode(pin2Num, INPUT);
			digitalWrite(pinNum, LOW);
			result = _numOfSonars++;
		}
	} else if (strcmp(component, "switch") == 0) {
		if (_numOfSwitches < SWITCHES_MAX) {
			_switches[_numOfSwitches][0] = pinNum;
			_switches[_numOfSwitches][1] = 0;
			pinMode(pinNum, INPUT_PULLUP);
			result = _numOfSwitches++;
		}
	}
	return result;
}


// AnalogIn methods...

int16_t	BushpunkRobot::readValue(uint8_t thing, uint16_t from, uint16_t to) {
	return (map(analogRead(_analogInputs[thing]), 0, 1023, from, to));
}


// Buzzer methods...

void BushpunkRobot::playBuzzer(thingy thing, uint16_t duration, uint8_t times) {
	for (int i = 0; i < times; i++) {
		digitalWrite(_buzzers[thing], HIGH);
		delay(duration);
		digitalWrite(_buzzers[thing], LOW);
		delay(duration);
	}
}


// digitalIn methods...

bool BushpunkRobot::isOff(thingy thing) {
	return !isOn(thing);
}

bool BushpunkRobot::isOn(thingy thing) {
	if (digitalRead(_inputs[thing]) == HIGH)
		return true;
	else
		return false;
}


// digitalOut methods...

void BushpunkRobot::turnOff(thingy thing) {
	digitalWrite(_outputs[thing], LOW);
}

void BushpunkRobot::turnOn(thingy thing) {
	digitalWrite(_outputs[thing], HIGH);
}


// PWM methods...

void BushpunkRobot::fadePwm(thingy thing, uint8_t to, uint8_t speed) {
	if (speed <= 5) {
		_pwms[thing][2] = to;
		_pwms[thing][3] = speed;
	}
}

void BushpunkRobot::setPwm(thingy thing, uint8_t to) {
	_pwms[thing][2] = to;
	_pwms[thing][3] = 0;
}


// Servo methods...

uint16_t BushpunkRobot::getServoPos(thingy thing) {
	return _servoSpecs[thing][2];
}

void BushpunkRobot::moveServo(thingy thing, uint8_t to, uint8_t speed) {
	if (speed != 0)
		setServoSpeed(thing, speed);
	if (_servos[thing].attached() == false)
		_servos[thing].attach(_servoSpecs[thing][0]);
	_servoSpecs[thing][3] = to;
}

void BushpunkRobot::restServo(thingy thing) {
	waitForServo(thing);
	_servos[thing].detach();
}

void BushpunkRobot::restServos() {
	for (uint8_t i = 0; i < _numOfServos; i++) {
		restServo(i);
	}
}

bool BushpunkRobot::servoStopped(thingy thing) {
	if (getServoPos(thing) == _servoSpecs[thing][3])
		return true;
	else
		return false;
}

bool BushpunkRobot::servosStopped() {
	for (uint8_t i = 0; i < _numOfServos; i++) {
		if (not servoStopped(i))
			return false;
	}
	return true;
}

void BushpunkRobot::setServoSpeed(thingy thing, uint8_t speed) {
	if (0 < speed and speed <= 5)
		_servoSpecs[thing][1] = speed;
}

void BushpunkRobot::setServoSpeeds(uint8_t speed) {
	for (uint8_t i = 0; i < _numOfServos; i++) {
		setServoSpeed(i, speed);
	}
}

void BushpunkRobot::stopServo(thingy thing) {
	moveServo(thing, getServoPos(thing));
}

void BushpunkRobot::stopServos() {
	for (uint8_t i = 0; i < _numOfServos; i++) {
		stopServo(i);
	}
}

void BushpunkRobot::waitForServo(thingy thing) {
	while (not servoStopped(thing)) {}
}

void BushpunkRobot::waitForServos() {
	for (uint8_t i = 0; i < _numOfServos; i++) {
		waitForServo(i);
	}
}


// Sonar methods...

uint16_t BushpunkRobot::readSonar(thingy thing, uint8_t numOfScans) {
	uint16_t cm=0, dist=0;
	uint8_t validReadings=0;
	for (int i = 0; i < numOfScans; i++) {
		digitalWrite(_sonars[thing][0], HIGH);
		delayMicroseconds(3);
		digitalWrite(_sonars[thing][0], LOW);
		cm = pulseIn(_sonars[thing][1], HIGH, _sonars[thing][2]/0.012) * 0.017;
		delayMicroseconds(3);
		if (cm > 0 and cm <= _sonars[thing][2]) {
			dist += cm;
			validReadings++;
		}
	}
	if (validReadings > 0)
		dist /= validReadings;
	return dist;
}

void BushpunkRobot::setSonarRange(thingy thing, uint16_t maxDist) {
	if (0 < maxDist and maxDist <= 500)
		_sonars[thing][2] = maxDist;
}

bool BushpunkRobot::sonarSeesSomething(thingy thing, uint8_t numOfScans) {
	if (readSonar(thing, numOfScans) == 0)
		return false;
	else
		return true;
}


// switch methods...

void BushpunkRobot::waitForSwitchOff(thingy thing, uint16_t millisecs) {
	while (switchIsOn(thing, millisecs)) {}
}

void BushpunkRobot::waitForSwitchOn(thingy thing, uint16_t millisecs) {
	while (switchIsOff(thing, millisecs)) {}
}

bool BushpunkRobot::switchIsOff(thingy thing, uint16_t millisecs) {
	return !switchIsOn(thing, millisecs);
}

bool BushpunkRobot::switchIsOn(thingy thing, uint16_t millisecs) {
	if (_switches[thing][1] != 0 and millis() - _switches[thing][1] >= millisecs)
		return true;
	else
		return false;
}


// Timer methods...

void BushpunkRobot::_timer2Setup() {
	cli();
	TIMSK2 &= ~(1 << TOIE2);
	TCCR2A &= ~((1 << WGM21) | (1 << WGM20));
	TCCR2B &= ~(1 << WGM22);
	ASSR   &= ~(1 << AS2);
	TIMSK2 &= ~(1 << OCIE2A);
	TCCR2B |= (1 << CS22);
	TCCR2B &= ~((1 << CS22) | (1 << CS20));
	TCNT2  =  TCNT2_VAL;
	TIMSK2 |= (1 << TOIE2);
	sei();
}
