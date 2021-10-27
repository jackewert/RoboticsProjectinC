/*------------------------------------------------------------------------------
 * Programmer: Your name
 * Date:
 * Description: .
 -------------------------------------------------------------------------------*/
#include "pid.h"
#ifndef HW2_TRACKANDPIDFUNCTIONS_H
#define HW2_TRACKANDPIDFUNCTIONS_H


// Prototype of required functions
void followLine(unsigned int position, pidController &PID, const trackSelector &trackSelect, const bool line);
void trackSelection(pidController &PID, trackSelector &trackSelect, int &signCount, bool &send, unsigned long &lastDisplayTime, int &whitespaces, int trackNum);
#endif //HW2_TRACKANDPIDFUNCTIONS_H