//
// Created by Mathews Jacob on 2/21/17.
//

#ifndef HW2_ROBOTHELPERFUNCTIONS_H
#define HW2_ROBOTHELPERFUNCTIONS_H

#include <pololu/3pi.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "pid.h"
#include "trackSelect.h"
#include "trackAndPiDFunctions.h"

void load_custom_characters();

void display_readings(const unsigned int* calibrated_values);

void initializeRobot();

void displayTimer(const pidController PID, const trackSelector trackSelect, unsigned long &lastDisplayTime);

void displayPD_motorVal_signCount(const pidController PID, const trackSelector trackSelect);

//void readSigns(const unsigned int* sensors,int &signCount, int &whitespaces, bool &send, bool&line, unsigned long &lastTime, int &trackNum);
void readSigns(const unsigned int* sensors,int &signCount, int &whitespaces, bool &send, bool &line,
               unsigned long &lastTime, int &trackNum, pidController &PID, trackSelector &trackSelect, unsigned long &lastDisplayTime);
#endif //HW2_ROBOTHELPERFUNCTIONS_H
