/*
 * File:   Encoder.cpp
 * Author: hans
 *
 * Created on 4 de Junho de 2015, 21:39
 */

#include <stdlib.h>
#include "Encoder.h"
#include "Pins.h"
#include <math.h>
#include <sstream>
#include <iostream>

#define STEPS_IN_TURN 40.0
#define S_TO_US 1000000.0

static unsigned int minDeltaT = 5000;
static unsigned int maxDeltaT = 100000;

static volatile int encoderPos;

void count_cb(int gpio, int level, uint32_t tick, void *userdata){
	Encoder *my_enc = (Encoder *) userdata;
	uint32_t lastTick = my_enc->get_lTick(); 
	if (tick - lastTick >= minDeltaT)	{
		my_enc->updateSpeed(tick-lastTick);
		my_enc->count();
		lastTick = tick;
	}
}

Encoder::Encoder() {
	for (int i=0; i<NBUF; i++) {
		buffer[i] = 0;
	}
   b = 0;
}

void Encoder::updateSpeed(uint32_t dt) { //On Edge Interrupt
	float pulseSpeed = (2.0*M_PI*S_TO_US)/(STEPS_IN_TURN*dt); //A complete turn has 40 steps (2*pi/40)

	speed = filter(direction*pulseSpeed);
}

float Encoder::filter(float pulseSpeed) {
	
	//adiciona ao buffer
	buffer[b] = pulseSpeed;
	b = (b+1)%NBUF;

	float sum=0;
	for (int i=0; i<NBUF; i++) {
		sum+=buffer[i];
	}

	return sum/NBUF;
}

float Encoder::getSpeed() {
	if (gpioTick() - lastTick > maxDeltaT) {
		speed = filter(0);
		direction = 0; 
	}
	return speed;
}

void Encoder::setDirection(int direction) {
    if (direction >= -1 && direction <= 1)
        this->direction = direction;
}

int Encoder::setup(int pin, int side) {
	reset();
	this->pin = pin;
	gpioSetMode(pin, PI_INPUT);
	gpioSetPullUpDown(pin, PI_PUD_UP);
	gpioSetISRFuncEx(pin, RISING_EDGE, 0, count_cb, this);
	
	return 1;
}

void Encoder::count() {
    steps+=direction;
}

void Encoder::reset() {
	steps = lastStep = 0;
	direction = 0;
	speed = 0;
	lastTick = gpioTick();
}

int Encoder::getDirection() const {
    return direction;
}

long Encoder::getSteps() const {
    return steps;
}

long Encoder::getDeltaSteps() {
    long delta = steps - lastStep;
    lastStep = steps;
    return delta;
}

float Encoder::getAngle() const { 
    //A complete turn has 40 steps (2*pi/40)
    return steps * 2.0 * M_PI / STEPS_IN_TURN;
}

float Encoder::getDeltaAngle() {
    return getDeltaSteps() * 2.0 * M_PI / STEPS_IN_TURN;
}

Encoder::~Encoder() {
}

