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


extern unsigned long get_cpu_freq ();


void uart_hal_setup (uint32_t baudrate) {
  UART0_REG(UART_REG_DIV) = get_cpu_freq() / baudrate + 1;
  g_ext_interrupt_handlers [INT_UART0_BASE] = uart_interrupt_handler;
  PLIC_set_priority(&g_plic, INT_UART0_BASE, 1);
  PLIC_enable_interrupt (&g_plic, INT_UART0_BASE);
  UART0_REG(UART_REG_RXCTRL) = UART_RXEN;
  UART0_REG(UART_REG_TXCTRL) = UART_TXWM(8) | UART_TXEN;
  UART0_REG(UART_REG_IE) = UART_IP_RXWM;
}

void uart_hal_disable_output_irq () {
}

void uart_hal_enable_output_irq () {
}

uint8_t uart_hal_get_byte () {
}

void uart_hal_send_byte (uint8_t data) {
}

void uart_hal_register_tx_interrupt (void (*)(void) tx_handler) {
}

void uart_hal_register_rx_interrupt (void (*)(void) rx_handler) {
}






