/*
  interrupts.h - HiFive interrupts abstraction layer
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


#ifndef interrupts_hal_h
#define interrupts_hal_h


#include <stdlib.h>
#include "plic/plic_driver.h"


// Instance data for the PLIC.
extern plic_instance_t g_plic;

typedef void (*function_ptr_t) (void);
extern function_ptr_t g_ext_interrupt_handlers[PLIC_NUM_INTERRUPTS];


#define interrupts_enable() \
  set_csr(mstatus, MSTATUS_MIE)

#define interrupts_disable() \
  clear_csr(mstatus, MSTATUS_MIE)


#define GET_SYSTEM_STATUS() \
  read_csr(mstatus)

#define SET_SYSTEM_STATUS(val) \
  write_csr(mstatus, (val))


#endif //interrupts_hal_h

