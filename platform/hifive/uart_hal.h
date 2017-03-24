/*
  uart_hal.h - UART hardware abstraction layer
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


#ifndef uart_hal_h
#define uart_hal_h

typedef void (*callback_fn_t) (void);

void uart_hal_setup (uint32_t baudrate);
void uart_hal_disable_output_irq ();
void uart_hal_enable_output_irq ();
uint8_t uart_hal_get_byte ();
void uart_hal_send_byte (uint8_t data);
void uart_hal_register_tx_interrupt (callback_fn_t tx_handler);
void uart_hal_register_rx_interrupt (callback_fn_t rx_handler);


#endif // uart_hal_h

