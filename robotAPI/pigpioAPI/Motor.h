/* 
 * File:   Motor.h
 * Author: hans
 *
 * Created on 4 de Junho de 2015, 23:22
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include "Encoder.h"
#include "PIDController.h"

class Motor {
    int pinA;
    int pinB;
    int pinE;

	float targetSpeed;
	float speed;
	float power;
	float minPower;
	float lastAngle;
	int rangePower(float power, int pin);

public:

	Encoder *encoder;
	PIDController pid;

	Motor();
	virtual ~Motor();
	int setup(int pinA, int pinB, int pinE, Encoder *encoder);
	void setTargetSpeed(float targetSpeed);
	float getTargetSpeed();
	void setMinPower(float minPower);
	void setPower(float power);
	void writePower();
	float getPower();
	float getSpeed();
	void stop();
	float controlSpeed();
private:

};

#endif	/* MOTOR_H */
