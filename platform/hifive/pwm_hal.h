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


void pwm_hal_setup (uint32_t pwm_index, uint32_t scaling);
void pwm_hal_disable_output (uint32_t pwm_index);
void pwm_hal_enable_output (uint32_t pwm_index);
int pwm_hal_is_enabled (uint32_t pwm_index);


#endif // pwm_hal_h

