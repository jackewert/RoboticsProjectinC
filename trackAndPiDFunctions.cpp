/*------------------------------------------------------------------------------
 * Programmer: Jedd Lim, Jack Ewert
 * Date: 3/2/2017
 * Description: .Contains all functions pertaining to track movement and motor speed
 -------------------------------------------------------------------------------*/

//#include <w32api/dvbsiparser.h>
#include "robotHelperFunctions.h"

/*Programmer: Jedd Lim, Jack Ewert
 * Date:3/1/2017
 * Description: Function is adapted from pololu documentation for PID motor control. The sensor arrays are read in, the
 *              power differential is calculated, and motor speed set with respect to a maximum speed associated with
 *              the roadsigns. The PID motor speed calculations allow the robot to follow the line and adjust as needed.
 *              In the event of a derail, the robot will use the last known line position and navigate its way back.
 */
// Got it from the examples in pololu documentation and adapted it for homework use/constraints.
void followLine(unsigned int position, pidController &PID, const trackSelector &trackSelect, const bool line) {
    //Checks to make sure it isn't reading in a black line
    if (line == true) {
//Tentative method for keeping bot straight when on line, useful at low speeds.
//       if(PID.getLeftMotorSpeed()>PID.getRightMotorSpeed()){PID.setLeftMotorSpeed(PID.getRightMotorSpeed());}
//       else if (PID.getRightMotorSpeed()>PID.getLeftMotorSpeed()){PID.setRightMotorSpeed(PID.getLeftMotorSpeed());}
    }
    else{
        //Calculates the derivative which is related to current position, 0 is when robot is perfectly centered on line.
        PID.setM_Proportional((int) position - 2000);
        //Derivative (differential) and integral of the robot is calculated. The derivative is the change and the integral
        //is the sum of the position.
        PID.setM_Differential(PID.getM_Proportional() - PID.getM_LastProportional());
        PID.setM_Integral(PID.getM_Integral() + PID.getM_Proportional());
        //Saves last position
        PID.setM_LastProportional(PID.getM_Proportional());
        //Difference in power for each motor is computed here. Positive is a right turn and negative is a left turn.
        PID.setM_powerDifference(
                (PID.getM_Proportional() / PID.getM_Proportional_Control()) +
                (PID.getM_Integral() / PID.getM_Integral_Control()) +
                (PID.getM_Differential() * PID.getM_Differential_Control()));
        // Compute the actual motor settings.  We never set either motor to a negative value.
        // If the value is smaller than negative maxSpeed, it sets the motor to maxSpeed and vice versa for values larger
        // than positive maxSpeed
        if (PID.getM_powerDifference() > trackSelect.getM_Speed()) {
            PID.setM_powerDifference(trackSelect.getM_Speed());
        }
        if (PID.getM_powerDifference() < -trackSelect.getM_Speed()) {
            PID.setM_powerDifference(-trackSelect.getM_Speed());
        }
        //Performs course corrections by increasing the L or R motor
        if (PID.getM_powerDifference() < 0) {
            PID.setLeftMotorSpeed(trackSelect.getM_Speed() + PID.getM_powerDifference());
            PID.setRightMotorSpeed(trackSelect.getM_Speed());
            set_motors(PID.getLeftMotorSpeed(), PID.getRightMotorSpeed());
        }
        else {
            PID.setLeftMotorSpeed(trackSelect.getM_Speed());
            PID.setRightMotorSpeed(trackSelect.getM_Speed() - PID.getM_powerDifference());
            set_motors(PID.getLeftMotorSpeed(), PID.getRightMotorSpeed());
        }
    }
}
/*Programmer: Jedd Lim
 * Date:3/7/2017
 * Description: Function receives the number of lines counted per roadsign and uses the information to set the track type
 *              and speed. It modifies the maxSpeed value that is used in the followLine function. In additon, it also sets
 *              the appropriate pid control values.
 */
void trackSelection(pidController &PID, trackSelector &trackSelect, int &signCount, bool &send, unsigned long &lastDisplayTime, int &whitespaces, int trackNum){
    //Switch statement reads in the roadsign type/number of lines and selects the correct case for it.
    //Once the correct case has been selected, the track type and speed are set appropriately, signCount is reset,
    //and the bool line is set to false in order to follow the track.
        if (send == true) {
            displayTimer(PID, trackSelect, lastDisplayTime);//Call display refresh.
            switch (trackNum) {
                case (1):
                    //Total line count is 1 which is an angular waveform road. Track and speed are set to 2nd highest speed.
                    trackSelect.setM_TrackType(TRACK1);
                    trackSelect.setM_Speed(TRACK1SPEED);
                    PID.setM_Proportional_Control(10);
                    PID.setM_Differential_Control(3/2);
                    PID.setM_Integral_Control(2000);
                    send = false;
                    break;
                case (2):
                    //Total line count is 2 which is a small radius waveform road. Track and speed are set to lowest speed.
                    trackSelect.setM_TrackType(TRACK2);
                    trackSelect.setM_Speed(TRACK2SPEED);
                    PID.setM_Proportional_Control(10);
                    PID.setM_Differential_Control(3/2);
                    PID.setM_Integral_Control(20000);
                    send = false;
                    break;
                case (3):
                    //Total line count is 3 which is a large radius waveform road. Track and speed are set to 2nd lowest
                    // speed.
                    trackSelect.setM_TrackType(TRACK3);
                    trackSelect.setM_Speed(TRACK3SPEED);
                    PID.setM_Proportional_Control(20);
                    PID.setM_Differential_Control(3/2);
                    PID.setM_Integral_Control(10000);
                    send = false;
                    break;
                case (4):
                    //Total line count is 4 which is a straight road. Track and speed are set to highest speed.
                    trackSelect.setM_TrackType(TRACK4);
                    trackSelect.setM_Speed(TRACK4SPEED);
                    PID.setM_Proportional_Control(10);
                    PID.setM_Differential_Control(2);
                    PID.setM_Integral_Control(20000);
                    send = false;
                    break;
                default:
                    /* Used in event that internal lineCount is reset to values below 1 or above 4. This ensures that
                     * the robot is not lost and can be retrieved. Retrieves the previous track settings.
                    */
                    trackSelect.setM_TrackType(trackSelect.getM_TrackType());
                    trackSelect.setM_Speed(trackSelect.getM_Speed());
                    //Allows bot to use the previous values for PID control.
                    PID.setM_Proportional_Control(PID.getM_Proportional_Control());
                    PID.setM_Differential_Control(PID.getM_Differential_Control());
                    PID.setM_Integral_Control(PID.getM_Integral_Control());
                    send = false;
            }
        }
}