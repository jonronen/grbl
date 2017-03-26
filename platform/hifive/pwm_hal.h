/*
  pwm_hal.h - PWM hardware abstraction layer
  Part of Grbl

  Copyright (c) 2017 Jon Ronen-Drori <jon_ronen@yahoo.com>

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


#ifndef pwm_hal_h
#define pwm_hal_h


#define PWM_PRESCALER_1 1
#define PWM_PRESCALER_8 2
#define PWM_PRESCALER_64 3


typedef void (*callback_fn_t) (void);


void pwm_hal_setup (uint32_t pwm_index, uint32_t scaling);
void pwm_hal_output_disable (uint32_t pwm_index);
void pwm_hal_output_enable (uint32_t pwm_index);
int pwm_hal_is_output_enabled (uint32_t pwm_index);

void pwm_hal_timer_init (uint32_t pwm_index);
void pwm_hal_set_prescaler (uint32_t pwm_index, uint32_t prescaler);
void pwm_hal_set_comparator (uint32_t pwm_index, uint32_t comp_value);
void pwm_hal_start (uint32_t pwm_index);
void pwm_hal_disable (uint32_t pwm_index);
void pwm_hal_interrupt_register (uint32_t pwm_index, callback_fn_t fn);


#endif // pwm_hal_h

