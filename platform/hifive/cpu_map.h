/*
  cpu_map.h - CPU and pin mapping configuration file
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC
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

/* The cpu_map.h files serve as a central pin mapping selection file for different
   processor types or alternative pin layouts. This version of Grbl officially supports
   only the Arduino Mega328p. */


#ifndef cpu_map_h
#define cpu_map_h


// Define serial port pins and interrupt vectors.
#define SERIAL_RX     USART_RX_vect
#define SERIAL_UDRE   USART_UDRE_vect

#define GPIO_SET_INPUTS(DDR_REG, INPUT_MASK) \
  GPIO_REG(GPIO_OUTPUT_EN) &= (~(INPUT_MASK)); \
  GPIO_REG(GPIO_INPUT_EN) |= (INPUT_MASK)

#define GPIO_SET_OUTPUTS(DDR_REG, OUTPUT_MASK) \
  GPIO_REG(GPIO_OUTPUT_EN) |= (OUTPUT_MASK); \
  GPIO_REG(GPIO_INPUT_EN) &= (~(OUTPUT_MASK))

#define GPIO_SET_PWM_OUTPUTS(DDR_REG, OUTPUT_MASK) \
  GPIO_REG(GPIO_OUTPUT_EN) |= (OUTPUT_MASK); \
  GPIO_REG(GPIO_INPUT_EN) &= (~(OUTPUT_MASK)); \
  GPIO_REG(GPIO_IOF_SEL) |= (OUTPUT_MASK); \
  GPIO_REG(GPIO_IOF_EN) |= (OUTPUT_MASK)

#define GPIO_SET_PULLUPS(PORT_REG, PULLUP_MASK) \
  GPIO_REG(GPIO_PULLUP_EN) |= (PULLUP_MASK)

#define GPIO_DISABLE_PULLUPS(PORT_REG, PULLUP_MASK) \
  GPIO_REG(GPIO_PULLUP_EN) &= (~(PULLUP_MASK))


// Define step pulse output pins. NOTE: All step bit pins must be on the same port.
#define STEP_PORT       GPIO_REG(GPIO_OUTPUT_VAL)
#define X_STEP_BIT      PIN_2_OFFSET // Uno Digital Pin 2
#define Y_STEP_BIT      PIN_3_OFFSET // Uno Digital Pin 3
#define Z_STEP_BIT      PIN_4_OFFSET // Uno Digital Pin 4
#define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

// Define step direction output pins. NOTE: All direction pins must be on the same port.
#define DIRECTION_PORT    GPIO_REG(GPIO_OUTPUT_VAL)
#define X_DIRECTION_BIT   PIN_5_OFFSET // Uno Digital Pin 5
#define Y_DIRECTION_BIT   PIN_6_OFFSET // Uno Digital Pin 6
#define Z_DIRECTION_BIT   PIN_7_OFFSET // Uno Digital Pin 7
#define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

// Define stepper driver enable/disable output pin.
#define STEPPERS_DISABLE_PORT   GPIO_REG(GPIO_OUTPUT_VAL)
#define STEPPERS_DISABLE_BIT    PIN_8_OFFSET // Uno Digital Pin 8
#define STEPPERS_DISABLE_MASK   (1<<STEPPERS_DISABLE_BIT)
#define STEPPERS_PWM_NUMBER     1
#define PULSE_OFF_PWM_NUMBER    2

// Define homing/hard limit switch input pins and limit interrupt vectors.
// NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
#define LIMIT_PIN        GPIO_REG(GPIO_INPUT_VAL)
#define LIMIT_PORT       GPIO_REG(GPIO_OUTPUT_VAL)
#define X_LIMIT_BIT      PIN_9_OFFSET // Uno Digital Pin 9
#define Y_LIMIT_BIT      PIN_10_OFFSET // Uno Digital Pin 10
#ifdef VARIABLE_SPINDLE // Z Limit pin and spindle enabled swapped to access hardware PWM on Pin 11.
  #define Z_LIMIT_BIT	 PIN_12_OFFSET // Uno Digital Pin 12
#else
  #define Z_LIMIT_BIT    PIN_11_OFFSET // Uno Digital Pin 11
#endif
#define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits
#define LIMIT_INT        PCIE0  // Pin change interrupt enable pin
#define LIMIT_INT_vect   PCINT0_vect
#define LIMIT_PCMSK      PCMSK0 // Pin change interrupt register

// Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_PORT   GPIO_REG(GPIO_OUTPUT_VAL)
// Z Limit pin and spindle PWM/enable pin swapped to access hardware PWM on Pin 11.
#ifdef VARIABLE_SPINDLE
  #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
    // If enabled, spindle direction pin now used as spindle enable, while PWM remains on D11.
    #define SPINDLE_ENABLE_BIT PIN_13_OFFSET // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
  #else
    #define SPINDLE_ENABLE_BIT PIN_11_OFFSET // Uno Digital Pin 11
  #endif
#else
  #define SPINDLE_ENABLE_BIT   PIN_12_OFFSET // Uno Digital Pin 12
#endif
#ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
  #define SPINDLE_DIRECTION_PORT  GPIO_REG(GPIO_OUTPUT_VAL)
  #define SPINDLE_DIRECTION_BIT   PIN_13_OFFSET // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
#endif

// Define flood and mist coolant enable output pins.
#define COOLANT_FLOOD_PORT  GPIO_REG(GPIO_OUTPUT_VAL)
#define COOLANT_FLOOD_BIT   PIN_17_OFFSET // Uno Analog Pin 3
#define COOLANT_MIST_PORT  GPIO_REG(GPIO_OUTPUT_VAL)
#define COOLANT_MIST_BIT   PIN_18_OFFSET // Uno Analog Pin 4

// Define user-control controls (cycle start, reset, feed hold) input pins.
// NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
#define CONTROL_PIN       GPIO_REG(GPIO_INPUT_VAL)
#define CONTROL_PORT      GPIO_REG(GPIO_OUTPUT_VAL)
// TODO: Pin 14 is not connected on HiFive1?
#define CONTROL_RESET_BIT       PIN_15_OFFSET // Uno Analog Pin 0
#define CONTROL_FEED_HOLD_BIT   PIN_15_OFFSET // Uno Analog Pin 1
#define CONTROL_CYCLE_START_BIT PIN_16_OFFSET // Uno Analog Pin 2
#define CONTROL_SAFETY_DOOR_BIT PIN_15_OFFSET // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
#define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
#define CONTROL_INT_vect  PCINT1_vect
#define CONTROL_PCMSK     PCMSK1 // Pin change interrupt register
#define CONTROL_MASK      ((1<<CONTROL_RESET_BIT)|(1<<CONTROL_FEED_HOLD_BIT)|(1<<CONTROL_CYCLE_START_BIT)|(1<<CONTROL_SAFETY_DOOR_BIT))
#define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.

// Define probe switch input pin.
#define PROBE_PIN       GPIO_REG(GPIO_INPUT_VAL)
#define PROBE_PORT      GPIO_REG(GPIO_OUTPUT_VAL)
#define PROBE_BIT       PIN_19_OFFSET // Uno Analog Pin 5
#define PROBE_MASK      (1<<PROBE_BIT)

// Variable spindle configuration below. Do not change unless you know what you are doing.
// NOTE: Only used when variable spindle is enabled.
#define SPINDLE_PWM_MAX_VALUE     0 // Don't change. 328p fast PWM mode fixes top value as 255.
#ifndef SPINDLE_PWM_MIN_VALUE
  #define SPINDLE_PWM_MIN_VALUE   255   // Must be greater than zero.
#endif
#define SPINDLE_PWM_OFF_VALUE     255
#define SPINDLE_PWM_RANGE         (SPINDLE_PWM_MAX_VALUE-SPINDLE_PWM_MIN_VALUE)
#define SPINDLE_TCCRA_REGISTER	  TCCR2A
#define SPINDLE_TCCRB_REGISTER	  TCCR2B
#define SPINDLE_OCR_REGISTER      PWM0_REG(PWM_CMP3)
#define SPINDLE_COMB_BIT	        COM2A1
#define SPINDLE_PWM_PRESCALER PWM_PRESCALER_64
#define SPINDLE_PWM_NUMBER 0

// Prescaled, 8-bit Fast PWM mode.
#define SPINDLE_TCCRA_INIT_MASK   ((1<<WGM20) | (1<<WGM21))  // Configures fast PWM mode.
// #define SPINDLE_TCCRB_INIT_MASK   (1<<CS20)               // Disable prescaler -> 62.5kHz
// #define SPINDLE_TCCRB_INIT_MASK   (1<<CS21)               // 1/8 prescaler -> 7.8kHz (Used in v0.9)
// #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
#define SPINDLE_TCCRB_INIT_MASK      (1<<CS22)               // 1/64 prescaler -> 0.98kHz (J-tech laser)

// NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
#define SPINDLE_PWM_PORT  GPIO_REG(GPIO_OUTPUT_VAL)
#define SPINDLE_PWM_BIT	  PIN_11_OFFSET // Arduino-like Digital Pin 11


#endif
