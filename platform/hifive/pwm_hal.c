/*
  pwm_hal.c - PWM hardware abstraction layer
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


#include "pwm_hal.h"
#include "interrupts.h"
#include "platform.h"
#include "plic/plic_driver.h"
#include "encoding.h"


#define PWM_INDEX_COUNT 3


static function_ptr_t g_pwm_handlers [PWM_INDEX_COUNT];
static uint32_t g_prescalers [PWM_INDEX_COUNT];

static void pwm1_interrupt_handler (void) {
  clear_csr(mie, MIP_MEIP);
  PWM1_REG(PWM_CFG) &= ~PWM_CFG_CMP0IP;
  if (g_pwm_handlers [1]) {
    g_pwm_handlers [1] ();
  }
  set_csr(mie, MIP_MEIP);
}

static void pwm2_interrupt_handler (void) {
  clear_csr(mie, MIP_MEIP);
  PWM2_REG(PWM_CFG) &= ~PWM_CFG_CMP0IP;
  if (g_pwm_handlers [2]) {
    g_pwm_handlers [2] ();
  }
  set_csr(mie, MIP_MEIP);
}


void pwm_hal_setup (uint32_t pwm_index, uint32_t scaling) {
  if (pwm_index >= PWM_INDEX_COUNT) return;
  g_prescalers [pwm_index] = scaling;
  // for now we only support PWM0
  if (pwm_index != 0) return;
  PWM0_REG(PWM_CFG) = 0;
  PWM0_REG(PWM_COUNT) = 0;
  PWM0_REG(PWM_CMP0) = 0;
  PWM0_REG(PWM_CMP1) = 0;
  PWM0_REG(PWM_CMP2) = 0;
  PWM0_REG(PWM_CMP3) = 0xff;
}

void pwm_hal_output_disable (uint32_t pwm_index) {
  // for now we only support PWM0
  if (pwm_index != 0) return;
  PWM0_REG(PWM_CFG) = 0;
  PWM0_REG(PWM_COUNT) = 0;
  PWM0_REG(PWM_CMP3) = 0xff;
}

void pwm_hal_output_enable (uint32_t pwm_index) {
  // for now we only support PWM0
  if (pwm_index != 0) return;
  PWM0_REG(PWM_COUNT) = 0;
  PWM0_REG(PWM_CFG) = PWM_CFG_ENALWAYS;
}

int pwm_hal_is_output_enabled (uint32_t pwm_index) {
  // for now we only support PWM0
  if (pwm_index != 0) return 0;
  return PWM0_REG(PWM_CFG) != 0;
}


void pwm_hal_timer_init (uint32_t pwm_index) {
  if (pwm_index >= PWM_INDEX_COUNT) return;
  switch (pwm_index) {
    case 1:
      PWM1_REG(PWM_CFG) = PWM_CFG_STICKY | PWM_CFG_ZEROCMP;
      PWM1_REG(PWM_COUNT) = 0;
      PWM1_REG(PWM_CMP0) = 0xffff;
      PWM1_REG(PWM_CMP1) = 0xffff;
      PWM1_REG(PWM_CMP2) = 0xffff;
      PWM1_REG(PWM_CMP3) = 0xffff;
      break;
    case 2:
      PWM2_REG(PWM_CFG) = PWM_CFG_STICKY | PWM_CFG_ZEROCMP;
      PWM2_REG(PWM_COUNT) = 0;
      PWM2_REG(PWM_CMP0) = 0xffff;
      PWM2_REG(PWM_CMP1) = 0xffff;
      PWM2_REG(PWM_CMP2) = 0xffff;
      PWM2_REG(PWM_CMP3) = 0xffff;
      break;
    default:
      break;
  }
}

void pwm_hal_set_prescaler (uint32_t pwm_index, uint32_t prescaler) {
  if (pwm_index >= PWM_INDEX_COUNT) return;
  g_prescalers [pwm_index] = prescaler;
}

void pwm_hal_set_comparator (uint32_t pwm_index, uint32_t comp_value) {
  if (pwm_index >= PWM_INDEX_COUNT) return;
  uint32_t prescaler = 0;
  while (comp_value > 0xffff) {
    comp_value = comp_value / 2;
    prescaler ++;
  }
  switch (g_prescalers [pwm_index]) {
    case PWM_PRESCALER_1:
      break;
    case PWM_PRESCALER_8:
      prescaler += 3;
      break;
    case PWM_PRESCALER_64:
      prescaler += 6;
      break;
    default:
      break;
  }
  if (prescaler >= 16) {
    prescaler = 15; // maximal prescaler value
  }
  switch (pwm_index) {
    case 1:
      PWM1_REG(PWM_CFG) &= ~0xf;
      PWM1_REG(PWM_CFG) |= prescaler;
      PWM1_REG(PWM_CMP0) = comp_value;
      break;
    case 2:
      PWM2_REG(PWM_CFG) &= ~0xf;
      PWM2_REG(PWM_CFG) |= prescaler;
      PWM2_REG(PWM_CMP0) = comp_value;
      break;
    default:
      break;
  }
}

void pwm_hal_start (uint32_t pwm_index) {
  if (pwm_index >= PWM_INDEX_COUNT) return;
  switch (pwm_index) {
    case 1:
      PWM1_REG(PWM_COUNT) = 0;
      PWM1_REG(PWM_CFG) |= PWM_CFG_ENALWAYS;
      break;
    case 2:
      PWM2_REG(PWM_COUNT) = 0;
      PWM2_REG(PWM_CFG) |= PWM_CFG_ENALWAYS;
      break;
    default:
      break;
  }
}

void pwm_hal_disable (uint32_t pwm_index) {
  switch (pwm_index) {
    case 1:
      PWM1_REG(PWM_CFG) &= ~PWM_CFG_ENALWAYS;
      break;
    case 2:
      PWM2_REG(PWM_CFG) &= ~PWM_CFG_ENALWAYS;
      break;
    default:
      break;
  }
}

void pwm_hal_interrupt_register (uint32_t pwm_index, function_ptr_t fn) {
  if (pwm_index >= PWM_INDEX_COUNT) return;
  g_pwm_handlers [pwm_index] = fn;
  switch (pwm_index) {
    case 1:
      g_ext_interrupt_handlers [INT_PWM1_BASE] = pwm1_interrupt_handler;
      PLIC_set_priority(&g_plic, INT_PWM1_BASE, 1);
      PLIC_enable_interrupt (&g_plic, INT_PWM1_BASE);
      break;
    case 2:
      g_ext_interrupt_handlers [INT_PWM2_BASE] = pwm2_interrupt_handler;
      PLIC_set_priority(&g_plic, INT_PWM2_BASE, 1);
      PLIC_enable_interrupt (&g_plic, INT_PWM2_BASE);
      break;
    default:
      break;
  }
}

