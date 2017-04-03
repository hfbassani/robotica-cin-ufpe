/*
 * File:   Encoder.h
 * Author: hans
 *
 * Created on 4 de Junho de 2015, 21:39
 */

#ifndef ENCODER_H
#define	ENCODER_H

#define LEFT_SIDE 0
#define RIGHT_SIDE 1
#define NBUF 4

#include <pigpio.h>

class Encoder {

	int pin;
	int direction;
	float speed;

	long steps;
	long lastStep;
	float buffer[NBUF]; //buffer circular
	int b;					// posicao no buffer
	uint32_t lastTick;

public:
	Encoder();
	virtual ~Encoder();
	int setup(int pin, int side);
	void setDirection(int direction);
	void reset();	
	void updateSpeed(uint32_t dt);
    void count();
	int getDirection() const;
	float getSpeed();
	long getSteps() const;
	long getDeltaSteps();
	float getAngle() const;
	float getDeltaAngle();
	uint32_t get_lTick(){ return lastTick;}

private:
	float filter(float pulseSpeed);	
};

#endif	/* ENCODER_H */

