/*
  spindle_control.c - spindle control methods
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "grbl.h"


#ifdef VARIABLE_SPINDLE
  static FLOAT pwm_gradient; // Precalulated value to speed up rpm to PWM conversions.
#endif


void spindle_init()
{
  #ifdef VARIABLE_SPINDLE

    // Configure variable spindle PWM and enable pin, if requried. On the Uno, PWM and enable are
    // combined unless configured otherwise.
    // Configure as PWM output pin.
    GPIO_SET_PWM_OUTPUTS(SPINDLE_PWM_DDR, (1<<SPINDLE_PWM_BIT));
    // Configure PWM output compare timer
#ifdef AVR
    SPINDLE_TCCRA_REGISTER = SPINDLE_TCCRA_INIT_MASK;
    SPINDLE_TCCRB_REGISTER = SPINDLE_TCCRB_INIT_MASK;
#else
    pwm_hal_setup (SPINDLE_PWM_NUMBER, SPINDLE_PWM_PRESCALER);
#endif
    #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
      // Configure as output pin.
      GPIO_SET_OUTPUTS(SPINDLE_ENABLE_DDR, (1<<SPINDLE_ENABLE_BIT));
    #else
      // Configure as output pin.
      GPIO_SET_OUTPUTS(SPINDLE_DIRECTION_DDR, (1<<SPINDLE_DIRECTION_BIT));
    #endif

    pwm_gradient = SPINDLE_PWM_RANGE/(settings.rpm_max-settings.rpm_min);

  #else

    // Configure no variable spindle and only enable pin.
    // Configure as output pin.
    GPIO_SET_OUTPUTS(SPINDLE_ENABLE_DDR, (1<<SPINDLE_ENABLE_BIT));
    // Configure as output pin.
    GPIO_SET_OUTPUTS(SPINDLE_DIRECTION_DDR, (1<<SPINDLE_DIRECTION_BIT));

  #endif

  spindle_stop();
}


uint8_t spindle_get_state()
{
	#ifdef VARIABLE_SPINDLE
    #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
		  // No spindle direction output pin. 
			#ifdef INVERT_SPINDLE_ENABLE_PIN
			  if (bit_isfalse(SPINDLE_ENABLE_PORT,(1<<SPINDLE_ENABLE_BIT))) { return(SPINDLE_STATE_CW); }
	    #else
	 			if (bit_istrue(SPINDLE_ENABLE_PORT,(1<<SPINDLE_ENABLE_BIT))) { return(SPINDLE_STATE_CW); }
	    #endif
    #else
      // Check if PWM is enabled.
      #ifdef AVR
      if (SPINDLE_TCCRA_REGISTER & (1<<SPINDLE_COMB_BIT)) {
      #else
      if (pwm_hal_is_output_enabled (SPINDLE_PWM_NUMBER)) {
      #endif
        if (SPINDLE_DIRECTION_PORT & (1<<SPINDLE_DIRECTION_BIT)) { return(SPINDLE_STATE_CCW); }
        else { return(SPINDLE_STATE_CW); }
      }
    #endif
	#else
		#ifdef INVERT_SPINDLE_ENABLE_PIN
		  if (bit_isfalse(SPINDLE_ENABLE_PORT,(1<<SPINDLE_ENABLE_BIT))) { 
		#else
		  if (bit_istrue(SPINDLE_ENABLE_PORT,(1<<SPINDLE_ENABLE_BIT))) {
		#endif
      if (SPINDLE_DIRECTION_PORT & (1<<SPINDLE_DIRECTION_BIT)) { return(SPINDLE_STATE_CCW); }
      else { return(SPINDLE_STATE_CW); }
    }
	#endif
	return(SPINDLE_STATE_DISABLE);
}


// Disables the spindle and sets PWM output to zero when PWM variable spindle speed is enabled.
// Called by various main program and ISR routines. Keep routine small, fast, and efficient.
// Called by spindle_init(), spindle_set_speed(), spindle_set_state(), and mc_reset().
void spindle_stop()
{
  #ifdef VARIABLE_SPINDLE
    // Disable PWM. Output voltage is zero.
    #ifdef AVR
      SPINDLE_TCCRA_REGISTER &= ~(1<<SPINDLE_COMB_BIT);
    #else
      pwm_hal_output_disable (SPINDLE_PWM_NUMBER);
    #endif
    #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
      #ifdef INVERT_SPINDLE_ENABLE_PIN
        SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);  // Set pin to high
      #else
        SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT); // Set pin to low
      #endif
    #endif
  #else
    #ifdef INVERT_SPINDLE_ENABLE_PIN
      SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);  // Set pin to high
    #else
      SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT); // Set pin to low
    #endif
  #endif
}


#ifdef VARIABLE_SPINDLE
  // Sets spindle speed PWM output and enable pin, if configured. Called by spindle_set_state()
  // and stepper ISR. Keep routine small and efficient.
  void spindle_set_speed(uint8_t pwm_value)
  {
    SPINDLE_OCR_REGISTER = pwm_value; // Set PWM output level.
    #ifdef SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED
      if (pwm_value == SPINDLE_PWM_OFF_VALUE) {
        spindle_stop();
      } else {
        // Ensure PWM output is enabled.
        #ifdef AVR
          SPINDLE_TCCRA_REGISTER |= (1<<SPINDLE_COMB_BIT);
        #else
          pwm_hal_output_enable (SPINDLE_PWM_NUMBER);
        #endif
        #ifdef INVERT_SPINDLE_ENABLE_PIN
          SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT);
        #else
          SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);
        #endif
      }
    #else
      if (pwm_value == SPINDLE_PWM_OFF_VALUE) {
        // Disable PWM. Output voltage is zero.
        #ifdef AVR
          SPINDLE_TCCRA_REGISTER &= ~(1<<SPINDLE_COMB_BIT);
        #else
          pwm_hal_output_disable (SPINDLE_PWM_NUMBER);
        #endif
      } else {
        // Ensure PWM output is enabled.
        #ifdef AVR
          SPINDLE_TCCRA_REGISTER |= (1<<SPINDLE_COMB_BIT);
        #else
          pwm_hal_output_enable (SPINDLE_PWM_NUMBER);
        #endif
      }
    #endif
  }


  // Called by spindle_set_state() and step segment generator. Keep routine small and efficient.
  uint8_t spindle_compute_pwm_value(FLOAT rpm) // 328p PWM register is 8-bit.
  {
    uint8_t pwm_value;
    rpm *= (0.010*sys.spindle_speed_ovr); // Scale by spindle speed override value.
    // Calculate PWM register value based on rpm max/min settings and programmed rpm.
    if ((settings.rpm_min >= settings.rpm_max) || (rpm >= settings.rpm_max)) {
      // No PWM range possible. Set simple on/off spindle control pin state.
      sys.spindle_speed = settings.rpm_max;
      pwm_value = SPINDLE_PWM_MAX_VALUE;
    } else if (rpm <= settings.rpm_min) {
      if (rpm == 0.0) { // S0 disables spindle
        sys.spindle_speed = 0.0;
        pwm_value = SPINDLE_PWM_OFF_VALUE;
      } else { // Set minimum PWM output
        sys.spindle_speed = settings.rpm_min;
        pwm_value = SPINDLE_PWM_MIN_VALUE;
      }
    } else { 
      // Compute intermediate PWM value with linear spindle speed model.
      // NOTE: A nonlinear model could be installed here, if required, but keep it VERY light-weight.
      sys.spindle_speed = rpm;
      pwm_value = floor((rpm-settings.rpm_min)*pwm_gradient) + SPINDLE_PWM_MIN_VALUE;
    }
    return(pwm_value);
  }
#endif


// Immediately sets spindle running state with direction and spindle rpm via PWM, if enabled.
// Called by g-code parser spindle_sync(), parking retract and restore, g-code program end,
// sleep, and spindle stop override.
#ifdef VARIABLE_SPINDLE
  void spindle_set_state(uint8_t state, FLOAT rpm)
#else
  void _spindle_set_state(uint8_t state)
#endif
{
  if (sys.abort) { return; } // Block during abort.
  if (state == SPINDLE_DISABLE) { // Halt or set spindle direction and rpm.
  
    #ifdef VARIABLE_SPINDLE
      sys.spindle_speed = 0.0;
    #endif
    spindle_stop();
  
  } else {
  
    #ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
      if (state == SPINDLE_ENABLE_CW) {
        SPINDLE_DIRECTION_PORT &= ~(1<<SPINDLE_DIRECTION_BIT);
      } else {
        SPINDLE_DIRECTION_PORT |= (1<<SPINDLE_DIRECTION_BIT);
      }
    #endif
  
    #ifdef VARIABLE_SPINDLE
      // NOTE: Assumes all calls to this function is when Grbl is not moving or must remain off.
      if (settings.flags & BITFLAG_LASER_MODE) { 
        if (state == SPINDLE_ENABLE_CCW) { rpm = 0.0; } // TODO: May need to be rpm_min*(100/MAX_SPINDLE_SPEED_OVERRIDE);
      }
      spindle_set_speed(spindle_compute_pwm_value(rpm));
    #endif
    #if (defined(USE_SPINDLE_DIR_AS_ENABLE_PIN) && \
        !defined(SPINDLE_ENABLE_OFF_WITH_ZERO_SPEED)) || !defined(VARIABLE_SPINDLE)
      // NOTE: Without variable spindle, the enable bit should just turn on or off, regardless
      // if the spindle speed value is zero, as its ignored anyhow.
      #ifdef INVERT_SPINDLE_ENABLE_PIN
        SPINDLE_ENABLE_PORT &= ~(1<<SPINDLE_ENABLE_BIT);
      #else
        SPINDLE_ENABLE_PORT |= (1<<SPINDLE_ENABLE_BIT);
      #endif    
    #endif
  
  }
  
  sys.report_ovr_counter = 0; // Set to report change immediately
}


// G-code parser entry-point for setting spindle state. Forces a planner buffer sync and bails 
// if an abort or check-mode is active.
#ifdef VARIABLE_SPINDLE
  void spindle_sync(uint8_t state, FLOAT rpm)
  {
    if (sys.state == STATE_CHECK_MODE) { return; }
    protocol_buffer_synchronize(); // Empty planner buffer to ensure spindle is set when programmed.
    spindle_set_state(state,rpm);
  }
#else
  void _spindle_sync(uint8_t state)
  {
    if (sys.state == STATE_CHECK_MODE) { return; }
    protocol_buffer_synchronize(); // Empty planner buffer to ensure spindle is set when programmed.
    _spindle_set_state(state);
  }
#endif
