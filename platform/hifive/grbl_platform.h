/*
  grbl_platform.h - platform-specific include files
  Part of Grbl

  Copyright (c) 2017-2017 Jon Ronen-Drori <jon_ronen@yahoo.com>

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

#ifndef grbl_platform_h
#define grbl_platform_h


#define PSTR(x) x
#define pgm_read_byte_near(x) *(x)

#define FLOAT double


#include "platform.h"
#include "plic/plic_driver.h"
#include "encoding.h"
#include "stdatomic.h"
#include "pwm_hal.h"
#include "uart_hal.h"
#include "interrupts.h"
#include "timer_hal.h"


#endif


