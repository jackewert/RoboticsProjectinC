//
// Created by Mathews Jacob on 2/21/17.
//

#include "robotHelperFunctions.h"

// Introductory messages.  The "PROGMEM" identifier causes the data to
// go into program space.
const char welcome_line1[] PROGMEM = " Pololu";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "PID Line";
const char demo_name_line2[] PROGMEM = "follower";

// A couple of simple tunes, stored in program space.
const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";
const char beep[] PROGMEM = ">C8";
// Data for generating the characters used in load_custom_characters
// and display_readings.  By reading levels[] starting at various
// offsets, we can generate all of the 7 extra characters needed for a
// bargraph.  This is also stored in program space.
const char levels[] PROGMEM = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

// This function loads custom characters into the LCD.  Up to 8
// characters can be loaded; we use them for 7 levels of a bar graph.
void load_custom_characters()
{
  lcd_load_custom_character(levels+0, 0); // no offset, e.g. one bar
  lcd_load_custom_character(levels+1, 1); // two bars
  lcd_load_custom_character(levels+2, 2); // etc...
  lcd_load_custom_character(levels+3, 3);
  lcd_load_custom_character(levels+4, 4);
  lcd_load_custom_character(levels+5, 5);
  lcd_load_custom_character(levels+6, 6);
  clear(); // the LCD must be cleared for the characters to take effect
}

// This function displays the sensor readings using a bar graph.
void display_readings(const unsigned int* calibrated_values) {
  unsigned char i;

  for (i = 0; i<5; i++) {
    // Initialize the array of characters that we will use for the
    // graph.  Using the space, an extra copy of the one-bar
    // character, and character 255 (a full black box), we get 10
    // characters in the array.
    const char display_characters[10] = {' ', 0, 0, 1, 2, 3, 4, 5, 6, 255};
    // The variable c will have values from 0 to 9, since
    // calibrated values are in the range of 0 to 1000, and
    // 1000/101 is 9 with integer math.
    char c = display_characters[calibrated_values[i]/101];
    // Display the bar graph character.
    print_character(c);
  }
}

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void initializeRobot()
{
  unsigned int counter; // used as a simple timer
  unsigned int sensors[5]; // an array to hold sensor values

  // This must be called at the beginning of 3pi code, to set up the
  // sensors.  We use a value of 2000 for the timeout, which
  // corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
  pololu_3pi_init(2000);
  load_custom_characters(); // load the custom characters

  // Play welcome music and display a message
  print_from_program_space(welcome_line1);
  lcd_goto_xy(0, 1);
  print_from_program_space(welcome_line2);
  play_from_program_space(welcome);
  delay_ms(1000);

  clear();
  print_from_program_space(demo_name_line1);
  lcd_goto_xy(0, 1);
  print_from_program_space(demo_name_line2);
  delay_ms(1000);

  // Display battery voltage and wait for button press
  while (!button_is_pressed(BUTTON_B)) {
    int bat = read_battery_millivolts();

    clear();
    print_long(bat);
    print("mV");
    lcd_goto_xy(0, 1);
    print("Press B");

    delay_ms(100);
  }

  // Always wait for the button to be released so that 3pi doesn't
  // start moving until your hand is away from it.
  wait_for_button_release(BUTTON_B);
  delay_ms(1000);

  // Auto-calibration: turn right and left while calibrating the
  // sensors.
  for (counter = 0; counter<80; counter++) {
    if (counter<20 || counter>=60)
      set_motors(40, -40);
    else
      set_motors(-40, 40);

    // This function records a set of sensor readings and keeps
    // track of the minimum and maximum values encountered.  The
    // IR_EMITTERS_ON argument means that the IR LEDs will be
    // turned on during the reading, which is usually what you
    // want.
    calibrate_line_sensors(IR_EMITTERS_ON);

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay_ms(20);
  }
  set_motors(0, 0);

  // Display calibrated values as a bar graph.
  while (!button_is_pressed(BUTTON_B)) {
    // Read the sensor values and get the position measurement.
    unsigned int position = read_line(sensors, IR_EMITTERS_ON);

    // Display the position measurement, which will go from 0
    // (when the leftmost sensor is over the line) to 4000 (when
    // the rightmost sensor is over the line) on the 3pi, along
    // with a bar graph of the sensor readings.  This allows you
    // to make sure the robot is ready to go.
    clear();
    print_long(position);
    lcd_goto_xy(0, 1);
    display_readings(sensors);
    lcd_init_printf();
    delay_ms(100);
  }
  wait_for_button_release(BUTTON_B);
  clear();
  print("Go!");

  // Play music and wait for it to finish before we start driving.
  play_from_program_space(go);
  while (is_playing());
}
/*Programmer: Jedd Lim
 * Date: 3/8/2017
 * Description: This function, when called, checks the time then calls the function that prints the motor bar graphs,
 *              power difference, and track number.
 */
void displayTimer(const pidController PID, const trackSelector trackSelect, unsigned long &lastDisplayTime) {
    //Calls current time
    unsigned long currentDisplayTime = get_ms();
    //If the display time since last display time call is greater than 1 second, it refreshes the screen.
    if ((currentDisplayTime-lastDisplayTime) > 1000) {
        clear();    //Clears the screen
        displayPD_motorVal_signCount(PID, trackSelect); //Calls the display values
        lastDisplayTime=currentDisplayTime; //Sets/saves the display time for future comparison
    }
}
/*Programmers: Jedd Lim, Jack Ewert
 * Date:3/6/2017
 * Description: This function displays the power_difference as an integer value and each motor's speed as a bar graph. We
 *              adapted the code from the display_readings function. In order to function, it calls and only reads the
 *              PID class and signCount. From the class, motor and power difference values are extracted. Then the values
 *              are printed to the screen.
 */
void displayPD_motorVal_signCount(const pidController PID, const trackSelector trackSelect) {
    /* Initialize the array of characters that we will use for the graph.  Using the space, an extra copy of the one-bar
     * character, and character 255 (a full black box), we get 10
     * characters in the array.
    */
    const char display_characters[10] = {' ', 0, 0, 1, 2, 3, 4, 5, 6, 255};
    // The variable c will have values from 0 to 9, since
    // calibrated values are in the range of 0 to 255, we divide 255 by 28 to get 9.
    char L = display_characters[PID.getLeftMotorSpeed()/28];
    char R = display_characters[PID.getRightMotorSpeed()/28];
    //Prints the power difference values in the upper left corner of the LCD screen
    lcd_goto_xy(0,0);
    print_long(PID.getM_powerDifference());
    lcd_goto_xy(5,0);
    print("T#");    //Prints the label for the track number
    lcd_goto_xy(7,0);
    print_long(trackSelect.getM_TrackType());//Prints the current track number (or signCount) in the upper right corner of the LCD screen.
    lcd_goto_xy(0,1);
    print_character(L);//Prints the bar graph for the left motor in the lower left corner of the LCD screen
    lcd_goto_xy(7,1);
    print_character(R);//Prints the bar graph for the right motor in the lower right corner of the LCD screen
}

/*Programmer: Jack Ewert, Jedd Lim
 * Date: 3/9/2017
 * Description: Reads the roadsigns and returns a total value of lines counted per sign. Sets bool send to true in
 *              order to allow track switching to occur.
 */
//void readSigns(const unsigned int* sensors,int &signCount, int &whitespaces, bool &send, bool &line,
//               unsigned long &lastTime, int &trackNum) {
void readSigns(const unsigned int* sensors,int &signCount, int &whitespaces, bool &send, bool &line,
               unsigned long &lastTime, int &trackNum, pidController &PID, trackSelector &trackSelect, unsigned long &lastDisplayTime) {
    //Gets current time
    unsigned long currentTime = get_ms();
    if (sensors[0]>=800 && sensors[4]>=800){//Check if its black
        if (line!=true) {//makes it so it counts the black "bar" once basically a flag that this bar has been counted already
            //count your black bar
            signCount++;//counts the sign
            play_from_program_space(beep);
            line = true;//will always be true on black bars, thus won't double count
            send = false;
            lastTime=currentTime;//Sets last time to current time
        }
        lastTime=currentTime;//Sets last time to current time
    }
    else{ //not on black bar
            // time difference since function was last called is greater than 2 seconds, allows the signCount to be sent.
            if((currentTime - lastTime)>150){
                send = true;//Will be true when no more bars are read in
                trackNum = signCount;//Sets track number equal to total signs counted
                signCount = 0;  //Resets the sign counter
                //Sets the track type and speed while resetting the signCount and calling display function
                trackSelection(PID, trackSelect, signCount,send,lastDisplayTime,whitespaces,trackNum);
            }
            line = false;
        }
}

