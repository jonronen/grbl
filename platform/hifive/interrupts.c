/*
  interrupts.c - HiFive interrupts abstraction layer
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


#include "uart_hal.h"
#include <platform.h>
#include <stdint.h>
#include "encoding.h"
#include "interrupts.h"


// Instance data for the PLIC.
plic_instance_t g_plic;


typedef void (*function_ptr_t) (void);
function_ptr_t g_ext_interrupt_handlers[PLIC_NUM_INTERRUPTS];


/*Entry Point for PLIC Interrupt Handler*/
void handle_m_ext_interrupt(){
  plic_source int_num = PLIC_claim_interrupt (&g_plic);
  if ((int_num >= 1) && (int_num < PLIC_NUM_INTERRUPTS)) {
    if (g_ext_interrupt_handlers [int_num]) {
      g_ext_interrupt_handlers [int_num] ();
    }
  }
  else {
    exit(1 + (uintptr_t) int_num);
  }
  PLIC_complete_interrupt(&g_plic, int_num);
}

/*Entry Point for Machine Timer Interrupt Handler*/
void handle_m_time_interrupt(){

  clear_csr(mie, MIP_MTIP);

  // Reset the timer for infinity.
  // This also clears the existing timer interrupt.

  volatile uint64_t * mtimecmp = (uint64_t*) (CLINT_BASE_ADDR + CLINT_MTIMECMP);
  *mtimecmp = 0xffffffff;

  // Re-enable the timer interrupt.
  set_csr(mie, MIP_MTIP);
}

