/*
  timer_hal.c - timers abstraction layer
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


#include "timer_hal.h"
#include "platform.h"


#define RTC_FREQUENCY 32768


void _delay_ms (uint16_t ms) {
  volatile uint64_t * mtime = (uint64_t*) (CLINT_BASE_ADDR + CLINT_MTIME);
  uint64_t now = *mtime;
  uint64_t then = now + (ms * RTC_FREQUENCY / 1000);
  while (*mtime < then);
}

uint32_t F_CPU;

extern unsigned long get_cpu_freq ();

unsigned long cpu_frequency () {
  if (F_CPU == 0) {
    F_CPU = get_cpu_freq ();
  }
  return F_CPU;
}

