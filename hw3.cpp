/*------------------------------------------------------------------------------
 * Programmer: Jedd Lim, Jack Ewert
 * Date: 3/1/2017
 * Description: .This program allows the 3pi robot to follow a black-on-white line and adjust as needed. In addition
 *               should it lose the line, it will find its way back onto the line. The robot also will adjust speed
 *               according to roadsigns that are placed on the track.
 -------------------------------------------------------------------------------*/
#include "pid.h"
//#include "trackSelect.h"
#include "robotHelperFunctions.h"
//#include "trackAndPiDFunctions.h"

//signCounter integer
int signCount = 0;
int trackNum = 0;
int whitespaces = 0;
bool send = false;
bool line = false;
//Initializes the time variables needed for functions
unsigned long lastDisplayTime=0, lastTime = 0;
int main() {
    //Sensor value array
    unsigned int sensors[5] = {0};
    //Initializes the class objects
    pidController PID;
    trackSelector trackSelect;
    // set up the 3pi robot and reset the system clock
    initializeRobot();
    //Resets system time in order to more accurately measure bars and track.
    time_reset();
    while (1) {
        //Reads in and returns weed average value of sensor array.
        unsigned int position = read_line(sensors, IR_EMITTERS_ON);
        //Calls function to orient and follow the black line/track.
        followLine(position, PID, trackSelect, line);
        //Reads in any roadsigns and if so, passes the number to the track selector and display
        readSigns(sensors, signCount, whitespaces, send, line, lastTime, trackNum, PID, trackSelect, lastDisplayTime);
    }
}