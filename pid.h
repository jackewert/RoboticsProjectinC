/*------------------------------------------------------------------------------
 * Programmer: Your name
 * Date:
 * Description: .
 -------------------------------------------------------------------------------*/
#ifndef HW2_PID_H
#define HW2_PID_H

#include "trackSelect.h"

class pidController {
public:

  int getM_Proportional() const
  {
    return m_Proportional;
  }

  void setM_Proportional(int m_Proportional)
  {
    pidController::m_Proportional = m_Proportional;
  }

  unsigned int getM_LastProportional() const
  {
    return m_LastProportional;
  }

  void setM_LastProportional(unsigned int m_LastProportional)
  {
    pidController::m_LastProportional = m_LastProportional;
  }

  long int getM_Integral() const
  {
    return m_Integral;
  }

  void setM_Integral(long int m_Integral)
  {
    pidController::m_Integral = m_Integral;
  }

  int getM_Differential() const
  {
    return m_Differential;
  }

  void setM_Differential(int m_Differential)
  {
    pidController::m_Differential = m_Differential;
  }

  int getLeftMotorSpeed() const
  {
    return m_leftMotorSpeed;
  }

  void setLeftMotorSpeed(int leftMotorSpeed)
  {
    pidController::m_leftMotorSpeed = leftMotorSpeed;
  }

  int getRightMotorSpeed() const
  {
    return m_rightMotorSpeed;
  }

  void setRightMotorSpeed(int rightMotorSpeed)
  {
    pidController::m_rightMotorSpeed = rightMotorSpeed;
  }

  int getM_powerDifference() const
  {
    return m_powerDifference;
  }

  void setM_powerDifference(int m_powerDifference)
  {
    pidController::m_powerDifference = m_powerDifference;
  }
//Control algorithms
  int getM_Proportional_Control() const
  {
    return m_Proportional_Control;
  }
  void setM_Proportional_Control(int m_Proportional_Control)
  {
    pidController::m_Proportional_Control = m_Proportional_Control;
  }
  int getM_Integral_Control() const
  {
    return m_Integral_Control;
  }
  void setM_Integral_Control(int m_Integral_Control)
  {
    pidController::m_Integral_Control = m_Integral_Control;
  }
  int getM_Differential_Control() const
  {
    return m_Differential_Control;
  }
  void setM_Differential_Control(int m_Differential_Control)
  {
    pidController::m_Differential_Control = m_Differential_Control;
  }
private:
  int m_Proportional{0};
  unsigned int m_LastProportional{0};
  long int m_Integral{0};
  int m_Differential{0};
  int m_powerDifference{0};
  int m_leftMotorSpeed{0};
  int m_rightMotorSpeed{0};
  int m_Proportional_Control{10};
  int m_Integral_Control{20000};
  int m_Differential_Control{3/2};
};

#endif //HW2_PID_H

