/*
  gpio_hal.c - GPIO hardware abstraction layer
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


#include "gpio_hal.h"
#include <platform.h>
#include <stdint.h>
#include "encoding.h"
#include "interrupts.h"


void gpio_interrupt_rise_disable (uint32_t pin_mask) {
  // TODO
}

void gpio_interrupt_rise_enable (uint32_t pin_mask) {
  // TODO
}

void gpio_interrupt_fall_disable (uint32_t pin_mask) {
  // TODO
}

void gpio_interrupt_fall_enable (uint32_t pin_mask) {
  // TODO
}

void gpio_interrupt_register (uint32_t pin_mask, function_ptr_t handler) {
  // TODO
}

