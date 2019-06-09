#include "PID.h"
#include <iostream>

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;

  p_error = 0;
  i_error = 0;
  d_error = 0;
  
  dKp = 1;
  dKi = 1;
  dKd = 1;
  pid_state = 0;
  twiddle_state = 0;
  twiddle_cnt = 0;
  accum_err = 0;
  best_err = 0;
 }

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  d_error = cte - p_error;
  p_error = cte;
  i_error += cte;
  std::cout << "p_error: " << p_error << " i_error: " << i_error << " d_error: " << d_error  
            << std::endl;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return ((Kp*p_error + Ki*i_error + Kd*d_error)*-1);  // TODO: Add your total error calc here!
}

#define TWIDDLE_CYCLE 10
void PID::Twiddle(double cte ,double tol) {

  double err = 0;
  double K = 0;
  double dK = 0;
  bool flag = false;

  if((dKp + dKi + dKd) > tol) {
    twiddle_cnt++;
    if((twiddle_cnt % TWIDDLE_CYCLE) >= (TWIDDLE_CYCLE/2)) {
      accum_err += cte*cte;
    }

    if((twiddle_cnt % TWIDDLE_CYCLE) == 0) {
      err = accum_err/(TWIDDLE_CYCLE/2);
      accum_err = 0;

      if(twiddle_cnt == TWIDDLE_CYCLE) {
          best_err = err;
          pid_state = 0;
          twiddle_state = 0;
          Kp += dKp;
      }else {
        if(pid_state == 0) {
          K = Kp;
          dK = dKp;
        }else if(pid_state == 1) {
          K = Kd;
          dK = dKd;
        }else if(pid_state == 2) {
          K = Ki;
          dK = dKi;
        }else {
          ;
        }

        if(err < best_err)
        {
            best_err = err;
            dK *= 1.1;
            flag = true;
        }
        else
        {
            if(twiddle_state == 0) {
              K -= 2 * dK;
              twiddle_state = 1;
            }
            else{
                K += dK;
                dK *= 0.9;
                flag = true;
            }
        }

        if(pid_state == 0) {
          Kp = K;
          dKp = dK;
        }else if(pid_state == 1) {
          Kd = K;
          dKd = dK;
        }else if(pid_state == 2) {
          Ki = K;
          dKi = dK;
        }else {
          ;
        }

        if(flag == true) {
          if(pid_state == 0) {
            pid_state = 1;
            twiddle_state = 0;
            Kd += dKd;
          }else if(pid_state == 1) {
            pid_state = 2;
            twiddle_state = 0;
            Ki += dKi;
          }else if(pid_state == 2) {
            pid_state = 0;
            twiddle_state = 0;
            Kp += dKp;
          }else {
            ;
          }
        }
      }

      std::cout << "best err: " << best_err << " Kp: " << Kp << " Ki: " << Ki << " Kd: " << Kd 
                << std::endl;
      std::cout << "twiddle count: " << twiddle_cnt << " pid state: " << pid_state << " twiddle state: " << twiddle_state 
                << " dKp: " << dKp << " dKi: " << dKi << " dKd: " << dKd << std::endl;
    }
  }else {
    std::cout << "Twiddle end! "  
              << std::endl;
    std::cout << "best err: " << best_err << " Kp: " << Kp << " Ki: " << Ki << " Kd: " << Kd 
              << std::endl;
    std::cout << "twiddle count: " << twiddle_cnt << " pid state: " << pid_state << " twiddle state: " << twiddle_state 
              << " dKp: " << dKp << " dKi: " << dKi << " dKd: " << dKd << std::endl;
  }
}