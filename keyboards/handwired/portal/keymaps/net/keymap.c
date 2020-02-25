/* Copyright 2020 net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

#define LA(KC) LT(0, KC)
#define _QWERTY 0
#define _RAISE 1
#define _CALCULATOR 1

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    PAREN, // {...}
    SQUARE_BRACKET, // [...]
    ANGLE_BRACKET, // <...>
    SQUIGGLY_BRACKET, // {...}
    FOR_MACRO,
    RANDOM,
    CCCV
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT(
    LA(KC_GRV),  KC_1, KC_2, KC_3, KC_4,    KC_5,                         PAREN,   KC_6,    KC_7,    KC_8,     KC_9,     KC_0,     KC_GRV, \
        KC_TAB,  KC_Q, KC_W, KC_E, KC_R,    KC_T,                         MO(2),   KC_Y,    KC_U,    KC_I,     KC_O,     KC_P,     KC_MINS, \
        KC_LSFT, KC_A, KC_S, KC_D, KC_F,    KC_G,                         RANDOM,  KC_H,    KC_J,    KC_K,     KC_L,     KC_SCLN,  KC_QUOT, \
        KC_ESC,  KC_Z, KC_X, KC_C, KC_V,    KC_B,    KC_ENT,     KC_BSPC, CCCV,    KC_N,    KC_M,    KC_COMM,  KC_DOT,   KC_SLSH,  RSFT_T(KC_CAPS), \
                                   KC_LCTL, KC_LALT, KC_SPC,     KC_SPC,  TT(3),   KC_BSPC,                    KC_LEFT,  KC_RIGHT, KC_DOWN),

    [_RAISE] = LAYOUT(
    LA(KC_F12), KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,                               KC_F6, KC_F7, KC_F8,   KC_F9,   KC_F10,  KC_F11, KC_F12, \
        KC_INS, KC_HOME, KC_UP, KC_END, _______, KC_PGUP,                        KC_2,  KC_Y,  KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_PGDN,                    KC_3,  KC_H,  KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
        _______, KC_PSCR, KC_SLCK, KC_PAUS, _______, _______, KC_TRNS,  KC_BSPC, KC_N,  KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT, \
                                            KC_LGUI, _______, KC_TRNS,   KC_SPC, KC_TRNS, KC_BSPC, KC_DOWN, KC_LEFT, KC_RIGHT, KC_UP),

    /* [_CALCULATOR] = LAYOUT(
         LA(KC_KP_SLASH),  KC_KP_7, KC_KP_8,     KC_KP_9,   KC_KP_0, KC_F5,                      KC_1, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV, \
            KC_KP_ASTERIX, KC_KP_4, KC_KP_5,     KC_KP_6,   LSFT(KC_5), KC_PGUP,               KC_2, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
            KC_KP_MINUS,   KC_KP_1, KC_KP_2,     KC_KP_3,   KC_KP_EQUAL, KC_PGDN,           KC_3, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
            KC_KP_PLUS,    KC_KP_0, KC_KP_COMMA, KC_KP_DOT, KC_KP_ENTER, _______, KC_TRNS,  KC_4, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT, \
                                                            KC_LGUI, _______, KC_TRNS,                                      KC_5, RAISE,   KC_BSPC, KC_DOWN, KC_LEFT, KC_RIGHT, KC_UP \
            )*/
};

bool paren_active = false;
uint16_t paren_timer = 0;

uint16_t user_key_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
    switch (keycode)
    {
    case PAREN:
        if (record->event.pressed)
        {
            SEND_STRING("(");
            paren_active = true;
            paren_timer = timer_read();
        }
        else
        {
            if (timer_elapsed(paren_timer) > 100)
                SEND_STRING(")"SS_TAP(X_LEFT));
        }
        break;

    case RANDOM:

        tap_random_base64();
        break;

    case CCCV:  // One key copy/paste
        if (record->event.pressed)
        {
            user_key_timer = timer_read32();
        }
        else
        {
            if (timer_elapsed32(user_key_timer) > TAPPING_TERM)
            {  // Hold, copy
                register_code(KC_LCTL);
                tap_code(KC_C);
                unregister_code(KC_LCTL);
            }
            else
            {  // Tap, paste
                register_code(KC_LCTL);
                tap_code(KC_V);
                unregister_code(KC_LCTL);
            }
        }
        break;
    }

    return true;
};


#ifdef OLED_DRIVER_ENABLE
static void render_logo(void)
{
    static const char PROGMEM qmk_logo[] = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00 };

    oled_write_P(qmk_logo, false);
}

void oled_task_user(void)
{
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state))
    {
    case _QWERTY:
        //oled_write_P(PSTR("Default\n"), false);
        render_logo();
        break;
    case _RAISE:
        oled_write_P(PSTR("RAISE\n"), false);
        break;
    default:
        // Or use the write_ln shortcut over adding '\n' to the end of your string
        oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}
#endif
