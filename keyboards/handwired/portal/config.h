/*
Copyright 2020 net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID 0xFEED
#define PRODUCT_ID 0x0000
#define DEVICE_VER 0x0001
#define MANUFACTURER net
#define PRODUCT portal
#define DESCRIPTION A custom keyboard

/* key matrix size */
// Rows are doubled-up
#define MATRIX_ROWS 10
#define MATRIX_COLS 7

#define TAPPING_TERM 200

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
 */
 // wiring of each half
#define MATRIX_ROW_PINS \
    { D4, C6, D7, E6, B4 }
// B2 not wirde on left (Arke)
#define MATRIX_COL_PINS \
    { B2, B3, B1, F7, F6, F5, F4 }

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

/*
 * Split Keyboard specific options, make sure you have 'SPLIT_KEYBOARD = yes' in your rules.mk, and define SOFT_SERIAL_PIN.
 */
//#define EE_HANDS
#define MASTER_RIGHT
#define SOFT_SERIAL_PIN D3  // TX0 (left top)
#define SERIAL_USE_MULTI_TRANSACTION

 /* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

/* If defined, GRAVE_ESC will always act as ESC when CTRL is held.
 * This is userful for the Windows task manager shortcut (ctrl+shift+esc).
 */
 // #define GRAVE_ESC_CTRL_OVERRIDE

 /*
  * Feature disable options
  *  These options are also useful to firmware size reduction.
  */

  /* disable debug print */
  //#define NO_DEBUG

  /* disable print */
  //#define NO_PRINT

  /* disable action features */
  //#define NO_ACTION_LAYER
  //#define NO_ACTION_TAPPING
  //#define NO_ACTION_ONESHOT

  /* disable these deprecated features by default */
#ifndef LINK_TIME_OPTIMIZATION_ENABLE
#    define NO_ACTION_MACRO
#    define NO_ACTION_FUNCTION
#endif
