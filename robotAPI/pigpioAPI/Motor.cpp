/* 
 * File:   Motor.cpp
 * Author: hans
 * 
 * Created on 4 de Junho de 2015, 23:22
 */

#include "Motor.h"
#include "Pins.h"
#include <pigpio.h>
#include <iostream>
#include <math.h>

#define max(a,b) (a)>(b)?(a):(b)
#define min(a,b) (a)<(b)?(a):(b)

Motor::Motor() {
}


Motor::~Motor() {
}

int Motor::setup(int pinA, int pinB, int pinE, Encoder *encoder) {

	this->pinA = pinA;
   this->pinB = pinB;
   this->pinE = pinE;
	this->encoder = encoder;

	gpioSetMode(pinA, PI_OUTPUT);
	gpioSetMode(pinB, PI_OUTPUT);
	gpioSetPWMfrequency(pinE, 10000);//10kHz
	gpioPWM(pinE, 0);

	setTargetSpeed(0);
	this->speed = 0;
	this->power = 0;
	this->minPower = 1;
	
	pid.setParam(1.5, 5, 0.1);
   	pid.setHiLoInput(100,-100);
	pid.setHiLoOutput(100,-100);

	this->lastAngle = 0;

	return 0;
}

void Motor::setTargetSpeed(float targetSpeed) {
	this->targetSpeed = targetSpeed;
	pid.setpoint(targetSpeed);
}

float Motor::getTargetSpeed() {
	return this->targetSpeed;
}

void Motor::setPower(float power) {
	this->power = power;
}

void Motor::writePower() {
	if (power>100) power = 100;
	else if (power<-100) power = -100;

	if (power>minPower) {
		 if (encoder->getDirection()!=1) {
		     gpioWrite(pinA, 0);
		     gpioWrite(pinB, 1);
		     encoder->setDirection(1);
		 }
		 gpioPWM(pinE, rangePower(power, pinE));
	}
	else
	if (power<-minPower) {
		if (encoder->getDirection()!=-1) {
		     gpioWrite(pinA, 1);
		     gpioWrite(pinB, 0);
		     encoder->setDirection(-1);
		 }
		 gpioPWM(pinE, rangePower(-power, pinE));  
	}
	else{
     gpioPWM(pinE, rangePower(power, pinE));
     encoder->setDirection(0);
  }
}

void Motor::setMinPower(float minPower) {
	this->minPower = minPower;
}

float Motor::getPower() {
	return power;
}


float Motor::getSpeed() {
	return speed;
}

void Motor::stop() {
	power = 0;
    gpioPWM(pinE, rangePower(power, pinE));
}

float Motor::controlSpeed() {
	speed = encoder->getSpeed();
	this->power += pid.input(speed);
        writePower();
	return speed;
}

int Motor::rangePower(float power, int pin) {
   int pwm_range = gpioGetPWMrange(pin);
   float pwr = power*pwm_range/100.0;

   if (pwr < 0) pwr = 0;
   else if (pwr > pwm_range)
        pwr = pwm_range;

	return pwr;
}


