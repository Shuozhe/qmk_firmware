/*
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert
Copyright 2017 F_YUUCHI

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

/* USB Device descriptor parameter */
#define VENDOR_ID       0xDEAD
#define PRODUCT_ID      0x0001
#define DEVICE_VER      0x0001
#define MANUFACTURER    SHUOZHE
#define PRODUCT         SPLIT_PERSONALITY
#define DESCRIPTION     First attempt at keebs

/* key matrix size */
// Rows are doubled-up
#define MATRIX_ROWS 10
#define MATRIX_COLS 7

// wiring of each half
#define MATRIX_ROW_PINS { D4, C6, D7, E6, B4 }
// B2 not wired on left (Arke)
#define MATRIX_COL_PINS { B2, B3, B1, F7, F6, F5, F4 }

/* define tapping term */
#define TAPPING_TERM 100

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

/* ws2812 RGB LED */
#define RGB_DI_PIN D3
#define RGBLED_NUM 12    // Number of LEDs

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

 /* disable debug print */
 // #define NO_DEBUG

 /* disable print */
 // #define NO_PRINT

 /* disable action features */
 //#define NO_ACTION_LAYER
 //#define NO_ACTION_TAPPING
 //#define NO_ACTION_ONESHOT
 //#define NO_ACTION_MACRO
 //#define NO_ACTION_FUNCTION
