/*
  uart_hal.c - UART hardware abstraction layer
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
#include "timer_hal.h"
#include <platform.h>
#include <stdint.h>
#include "encoding.h"
#include "interrupts.h"


function_ptr_t g_tx_handler;
function_ptr_t g_rx_handler;


static void uart_interrupt_handler (void) {

  // disable interrupts first to enable reading the registers without changing
  // the interrupt status in between register reads
  clear_csr(mie, MIP_MEIP);

  // the interrupts pending depend on the IP register
  uint32_t uart_ip_reg = UART0_REG(UART_REG_IP);
  // mask the interrupts that were not enabled in the first place. this covers
  // cases where an old status bit was left in the IP register
  uart_ip_reg &= UART0_REG(UART_REG_IE);

  // now handle each one of the interrupts
  if (uart_ip_reg & UART_IP_RXWM) {
    if (g_rx_handler) g_rx_handler ();
  }
  if (uart_ip_reg & UART_IP_TXWM) {
    if (g_tx_handler) g_tx_handler ();
  }

  // restore interrupts
  set_csr(mie, MIP_MEIP);
}


void uart_hal_setup (uint32_t baudrate) {

  // TODO: move the PLIC_init to a better place
  PLIC_init(
    &g_plic,
    PLIC_BASE_ADDR,
    PLIC_NUM_INTERRUPTS,
    PLIC_NUM_PRIORITIES
  );

  // Disable the machine & timer interrupts until setup is done.

  clear_csr(mie, MIP_MEIP);

  UART0_REG(UART_REG_DIV) = cpu_frequency () / baudrate + 1;
  UART0_REG(UART_REG_RXCTRL) = UART_RXEN;
  UART0_REG(UART_REG_TXCTRL) = UART_TXWM(8) | UART_TXEN;
  UART0_REG(UART_REG_IE) = UART_IP_RXWM | UART_IP_TXWM;
  g_ext_interrupt_handlers [INT_UART0_BASE] = uart_interrupt_handler;
  PLIC_set_priority(&g_plic, INT_UART0_BASE, 1);
  PLIC_enable_interrupt (&g_plic, INT_UART0_BASE);

  set_csr(mie, MIP_MEIP);
}

void uart_hal_disable_output_irq () {
  UART0_REG (UART_REG_IE) &= ~UART_IP_TXWM;
}

void uart_hal_enable_output_irq () {
  UART0_REG (UART_REG_IE) |= UART_IP_TXWM;
}

uint8_t uart_hal_get_byte () {
  uint32_t rx_val = UART0_REG (UART_REG_RXFIFO);
  if ((rx_val & 0x80000000) == 0) {
    return (uint8_t)rx_val;
  }
  // 0xff means no data
  return 0xff;
}

void uart_hal_send_byte (uint8_t data) {
  UART0_REG (UART_REG_TXFIFO) = data;
}

void uart_hal_register_tx_interrupt (function_ptr_t tx_handler) {
  g_tx_handler = tx_handler;
}

void uart_hal_register_rx_interrupt (function_ptr_t rx_handler) {
  g_rx_handler = rx_handler;
}

