/*------------------------------------------------------------------------------
 * Programmer: Your name
 * Date:
 * Description: .
 -------------------------------------------------------------------------------*/
#ifndef HW2_GEAR_H
#define HW2_GEAR_H

#define TRACK0 0
#define TRACK1 1
#define TRACK2 2
#define TRACK3 3
#define TRACK4 4
#define TRACK0SPEED 100
#define TRACK1SPEED 130
#define TRACK2SPEED 100
#define TRACK3SPEED 150
#define TRACK4SPEED 220

#include <pololu/3pi.h>

class trackSelector {

public:

  int getM_Speed() const
  {
    return m_Speed;
  }

  void setM_Speed(int m_Speed)
  {
    trackSelector::m_Speed = m_Speed;
  }

  int getM_TrackType() const
  {
    return m_TrackType;
  }

  void setM_TrackType(int m_TrackType)
  {
    trackSelector::m_TrackType = m_TrackType;
  }

private:
  int m_Speed{TRACK0SPEED};
  int m_TrackType{TRACK0};

};

#endif //HW2_GEAR_H
