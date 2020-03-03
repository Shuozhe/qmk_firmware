#include QMK_KEYBOARD_H

#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#ifdef SSD1306OLED
#include "ssd1306.h"
#endif

// Add headers for raw hid communication
#include <split_scomm.h>
#include "raw_hid.h"

extern keymap_config_t keymap_config;
extern uint8_t is_master;

// Layer defines
#define _QWERTY 0
#define _COLEMAK 1
#define _CODING 2
#define _ARROWS 3
#define _NUMPAD 4
#define _RGB 5
#define _GAMING 6
#define _TEMPLATE 7

// Shorthand Macros
#define KC_TD(a) TD(TD_##a)
#define KC_TO(a) TO(a)
#define KC_LGZ LGUI_T(KC_Z)
#define KC_LAX LALT_T(KC_X)
#define KC_SHL LSFT_T(KC_LEFT)
#define KC_SHR LSFT_T(KC_RGHT)
#define KC_CET LCTL_T(KC_ENT)
#define KC_CTLL LCTL_T(KC_LEFT)
#define KC_CTLR LCTL_T(KC_RGHT)
#define KC_SDEL LSFT_T(KC_DEL)
#define KC_ASPC LT(_ARROWS,KC_SPC)
#define KC_LSB LSFT_T(KC_BSPC)
#define KC_APS SCREENCAP

#define L0(KC) LT(0, KC)
#define L1(KC) LT(1, KC)
#define L2(KC) LT(2, KC)

// Custom keycodes
enum
{
    SHIFT_TAB = SAFE_RANGE,
    REDO_NUM_F11,
    SCREENCAP,
    CCCV,
    RAND,
    PAREN
};

// Tap-dance built-ins
void tapdance_oneach(qk_tap_dance_state_t* state, void* user_data);
void tapdance_onfinished(qk_tap_dance_state_t* state, void* user_data);
void tapdance_onreset(qk_tap_dance_state_t* state, void* user_data);

int cur_dance(qk_tap_dance_state_t* state);
typedef struct
{
    bool is_press_action;
    int state;
} tap;

typedef struct
{
    int single_tap;
    int single_hold;
    int double_tap;
    bool is_double_tap_layer_lock;
    bool use_instant_layer;
} tri_layer_action;

enum
{
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    DOUBLE_HOLD = 4,
    DOUBLE_SINGLE_TAP = 5,
    TRIPLE_TAP = 6,
    TRIPLE_HOLD = 7
};

// Tap-dance defines
enum
{
    TD_ENT = 0,
    TD_NUM,
    TD_SPC,
    TD_SFT,
    TD_DEL
};

static tri_layer_action tri_layer_actions[] = {
  [TD_ENT] = {.single_tap = KC_ENT, .single_hold = _ARROWS, .double_tap = _ARROWS, .is_double_tap_layer_lock = true, .use_instant_layer = false },
  [TD_NUM] = {.single_tap = KC_F11, .single_hold = _NUMPAD, .double_tap = KC_F5, .is_double_tap_layer_lock = false, .use_instant_layer = true  },
  [TD_SPC] = {.single_tap = KC_SPC, .single_hold = _CODING, .double_tap = KC_SPC, .is_double_tap_layer_lock = false, .use_instant_layer = true  },
  [TD_SFT] = {.single_tap = KC_ENT, .single_hold = _CODING, .double_tap = KC_SPC, .is_double_tap_layer_lock = false, .use_instant_layer = true  },
  [TD_DEL] = {.single_tap = KC_DEL, .single_hold = _RGB, .double_tap = _RGB, .is_double_tap_layer_lock = true, .use_instant_layer = false  }
};

static tap tap_states[] = {
  [TD_ENT] = {.is_press_action = true, .state = 0 },
  [TD_NUM] = {.is_press_action = true, .state = 0 },
  [TD_SPC] = {.is_press_action = true, .state = 0 },
  [TD_SFT] = {.is_press_action = true, .state = 0 },
  [TD_DEL] = {.is_press_action = true, .state = 0 }
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_ENT] = {.fn = { tapdance_oneach, tapdance_onfinished, tapdance_onreset }, .user_data = (void*)TD_ENT },
  [TD_NUM] = {.fn = { tapdance_oneach, tapdance_onfinished, tapdance_onreset }, .user_data = (void*)TD_NUM },
  [TD_SPC] = {.fn = { tapdance_oneach, tapdance_onfinished, tapdance_onreset }, .user_data = (void*)TD_SPC },
  [TD_SFT] = {.fn = { tapdance_oneach, tapdance_onfinished, tapdance_onreset }, .user_data = (void*)TD_SFT },
  [TD_DEL] = {.fn = { tapdance_oneach, tapdance_onfinished, tapdance_onreset }, .user_data = (void*)TD_DEL }
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [_QWERTY] = LAYOUT(
          KC_GESC,   KC_1,   KC_2,   KC_3,    KC_4,    KC_5,                   \
          KC_TAB,    KC_Q,   KC_W,   KC_E,    KC_R,    KC_T,                   \
          KC_LSFT,   KC_A,   KC_S,   KC_D,    KC_F,    KC_G,                   \
          KC_LCTL,   KC_Z,   KC_X,   KC_C,    KC_V,    KC_B,    L1(KC_ENT),    \
                                              KC_LALT, KC_LGUI, KC_SPC,        \
    \
                        CCCV,      KC_6,    KC_7,   KC_8,    KC_9,    KC_0,     KC_EQUAL, \
                        KC_PLUS,   KC_Y,    KC_U,   KC_I,    KC_O,    KC_P,     KC_MINS,  \
                        RAND,      KC_H,    KC_J,   KC_K,    KC_L,    KC_LBRC,  KC_RBRC,  \
          L2(KC_SCLN),  KC_QUOT,   KC_N,    KC_M,   KC_COMM, KC_DOT,  KC_UP,    KC_SLSH,  \
          KC_BSPC,      TT(3),     KC_BSPC,                  KC_LEFT, KC_DOWN,  KC_RIGHT),

        [_COLEMAK] = LAYOUT(
          KC_GESC, KC_1, KC_2, KC_3, KC_4,    KC_5, \
          KC_TAB,  KC_Q, KC_W, KC_F, KC_P,    KC_G, \
          KC_LSFT, KC_A, KC_R, KC_S, KC_T,    KC_D, \
          KC_LCTL, KC_Z, KC_X, KC_C, KC_V,    KC_B, LT(1, KC_ENT), \
                                     KC_LALT, KC_LALT, KC_SPC, \
    \
                   PAREN,   KC_6,    KC_7,    KC_8,     KC_9,     KC_0,     KC_EQUAL, \
                   KC_LGUI, KC_J,    KC_L,    KC_U,     KC_Y,     KC_SCLN,  KC_MINS, \
                   RAND,    KC_H,    KC_N,    KC_E,     KC_I,     KC_O,     KC_QUOT, \
          KC_BSPC, CCCV,    KC_K,    KC_M,    KC_COMM,  KC_DOT,   KC_SLSH,  RSFT_T(KC_CAPS), \
          KC_SPC,  TT(3),   KC_BSPC,                    KC_LEFT,  KC_RIGHT, KC_DOWN),

    /*[_CODING] = LAYOUT(\
    // .-----------------------------------------.                    .-----------------------------------------.
        TO(0),   F1,    F2,    F3,    F4,    F5,                         F6,    F7,    F8,    F9,   F10,   F11, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TILD,   DLR,  LCBR,  PPLS,   EQL,                       PIPE,  LCBR,  RCBR,  CIRC,  COLN,  F12, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,   GRV,   DLR,  ASTR,  PMNS,   GT,                        AMPR,  LPRN,  RPRN,  LBRC,  RBRC,  DQUO, \
    // |------+------+------+------+------+------|-------|    |-------|------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  PERC,  UNDS,   LT,   TRNS,        TRNS,   EXLM,   EQL,    LT,    GT,  QUES, TO(0), \
    // `-----------------------------------------/-------/    \-------\-----------------------------------------'
                           TRNS,  TRNS,  TRNS,    TRNS,           TRNS,     TRNS,  TRNS,  TRNS  \
    //                   `------+------+------''------'          '-------''------+------+------'
    ),

    [_ARROWS] = LAYOUT(\
    // .-----------------------------------------.                    .-----------------------------------------.
        TO(0),  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,  CTLL,   UP,   CTLR,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       HOME,  LEFT,  DOWN,  RGHT,   END,  TRNS, \
    // |------+------+------+------+------+------|-------|    |-------|------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,        TRNS,   TRNS,   SHL,  TRNS,   SHR,  TRNS, TO(1), \
    // `-----------------------------------------/-------/    \-------\-----------------------------------------'
                           TRNS,  TRNS,  TRNS,    TRNS,             TRNS,   TRNS,  PSCR,  APS  \
    //                   `------+------+------''------'          '-------''------+------+------'
    ),

    [_NUMPAD] = LAYOUT(\
    // .-----------------------------------------.                    .-----------------------------------------.
        TO(0),  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,  TRNS,  PSLS,  PAST,  PMNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,    P7,    P8,    P9,  PPLS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,    P4,    P5,    P6,  PCMM,  TRNS, \
    // |------+------+------+------+------+------|-------|    |-------|------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,        TRNS,   TRNS,    P1,    P2,    P3,   EQL, TO(2), \
    // `-----------------------------------------/-------/    \-------\-----------------------------------------'
                           TRNS,  TRNS,  TRNS,    TRNS,             TRNS,     P0,  PDOT,  TRNS  \
    //                   `------+------+------''------'          '-------''------+------+------'
    ),

    [_RGB] = LAYOUT(\
    // .-----------------------------------------.                    .-----------------------------------------.
        TO(0),  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                        RTG,   RHI,   RHD,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                        RMI,   RSI,   RSD,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|-------|    |-------|------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,        TRNS,    RMD,   RVI,   RVD,  TRNS,  TRNS, TO(3), \
    // `-----------------------------------------/-------/    \-------\-----------------------------------------'
                           TRNS,  TRNS,  TRNS,    TRNS,             TRNS,   TRNS,  TRNS,  TRNS  \
    //                   `------+------+------''------'          '-------''------+------+------'
    ),

    [_GAMING] = LAYOUT(\
    // .-----------------------------------------.                    .-----------------------------------------.
         ESC,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,    layQ,    W,     E,     R,     T,                        TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,    A,     S,     D,    F,     G,                        TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|-------|    |-------|------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,   V,     B,   TRNS,        TRNS,   TRNS,  TRNS,  TRNS,  TRNS,  TRNS, TO(4), \
    // `-----------------------------------------/-------/    \-------\-----------------------------------------'
                           TRNS,  TRNS,  SPC,    TRNS,             TRNS,   TRNS,  TRNS,  TRNS  \
    //                   `------+------+------''------'          '-------''------+------+------'
    ),

    [_TEMPLATE] = LAYOUT(\
    // .-----------------------------------------.                    .-----------------------------------------.
        TO(0),  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|                    |------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,                       TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // |------+------+------+------+------+------|-------|    |-------|------+------+------+------+------+------|
         TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,        TRNS,   TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS, \
    // `-----------------------------------------/-------/    \-------\-----------------------------------------'
                           TRNS,  TRNS,  TRNS,    TRNS,             TRNS,   TRNS,  TRNS,  TRNS  \
    //                   `------+------+------''------'          '-------''------+------+------'
    ),*/
};

const char* read_logo(void);

void matrix_init_user(void)
{
    iota_gfx_init(true);
}

void matrix_scan_user(void)
{
    iota_gfx_task();
}

// HID input
bool is_hid_connected = false; // Flag indicating if we have a PC connection yet
uint8_t new_connection = 1;
uint8_t screen_max_count = 0;  // Number of info screens we can scroll through (set by connecting node script)
uint8_t screen_show_index = 0; // Current index of the info screen we are displaying
uint8_t screen_data_buffer[SERIAL_SCREEN_BUFFER_LENGTH - 1] = {0}; // Buffer used to store the screen data sent by connected node script
int screen_data_index = 0; // Current index into the screen_data_buffer that we should write to

void raw_hid_send_screen_index(void)
{
    // Send the current info screen index to the connected node script so that it can pass back the new data
    uint8_t send_data[32] = {0};
    send_data[0] = ++screen_show_index; // Add one so that we can distinguish it from a null byte
    raw_hid_send(send_data, sizeof(send_data));
    new_connection = 0;
}

void raw_hid_receive(uint8_t* data, uint8_t length)
{
    // PC connected, so set the flag to show a message on the master display
    is_hid_connected = true;

    // Initial connections use '1' in the first byte to indicate this
    if (length > 1 && data[0] == 1) {
        // New connection so restart screen_data_buffer
        screen_data_index = 0;

        // The second byte is the number of info screens the connected node script allows us to scroll through
        screen_max_count = data[1];
        if (screen_show_index >= screen_max_count) {
            screen_show_index = 0;
            new_connection = 1;
        }

        // Tell the connection which info screen we want to look at initially
        raw_hid_send_screen_index();
        return;
    }

    // Otherwise the data we receive is one line of the screen to show on the display
    if (length >= 21) {
        // Copy the data into our buffer and increment the number of lines we have got so far
        memcpy((char*)&screen_data_buffer[screen_data_index * 21], data, 21);
        screen_data_index++;

        // Once we reach 4 lines, we have a full screen
        if (screen_data_index == 4) {
            // Reset the buffer back to receive the next full screen data
            screen_data_index = 0;

            // Now get ready to transfer the whole 4 lines to the slave side of the keyboard.
            // First clear the transfer buffer with spaces just in case.
            memset((char*)&serial_slave_screen_buffer[0], ' ', sizeof(serial_slave_screen_buffer));

            // Copy in the 4 lines of screen data, but start at index 1, we use index 0 to indicate a connection in the slave code
            memcpy((char*)&serial_slave_screen_buffer[1], screen_data_buffer, sizeof(screen_data_buffer));

            // Set index 0 to indicate a connection has been established
            serial_slave_screen_buffer[0] = 1;

            // Make sure to zero terminate the buffer
            serial_slave_screen_buffer[sizeof(serial_slave_screen_buffer) - 1] = 0;

            // Indicate that the screen data has changed and needs transferring to the slave side
            hid_screen_change = true;
        }
    }
}

// Screen printing
char layer_state_str[20];
const char* write_layer(void)
{
    // Print the layer name for the current layer
    switch (biton32(layer_state)) {
    case _QWERTY:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: QWERTY");
        break;
    case _COLEMAK:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Colemak");
        break;
    case _CODING:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Code Symbols");
        break;
    case _ARROWS:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Navigation");
        break;
    case _NUMPAD:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Numpad");
        break;
    case _RGB:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: RGB Lighting");
        break;
    case _GAMING:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Gaming Mode");
        break;
    default:
        snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Id-%ld", layer_state);
    }

    return layer_state_str;
}

char hid_info_str[20];
const char* write_hid(void)
{
    if (is_hid_connected)
        snprintf(hid_info_str, sizeof(hid_info_str), "Connected(%i/%i) %s", screen_show_index, screen_max_count, new_connection == 1 ? "new" : "existing");
    else
        hid_info_str[0] = '\0';
    return hid_info_str;
}

void write_slave_info_screen(struct CharacterMatrix* matrix)
{
    if (serial_slave_screen_buffer[0] > 0) {
        // If the first byte of the buffer is non-zero we should have a full set of data to show,
        // So we copy it into the display
        matrix_write(matrix, (char*)serial_slave_screen_buffer + 1);
    }
    else {
        // Otherwise we just draw the logo
        matrix_write_ln(matrix, "");
        matrix_write(matrix, read_logo());
    }
}

void matrix_render_user(struct CharacterMatrix* matrix)
{
    if (is_master) {
        // Show layer and rgb values on the master side
        matrix_write_ln(matrix, write_layer());
        matrix_write_ln(matrix, " ");
        matrix_write(matrix, write_hid()); // Add if we have a connection established
    }
    else {
        // Show the logo or screen info on the slave side
        write_slave_info_screen(matrix);
    }
}

void matrix_update(struct CharacterMatrix* dest, const struct CharacterMatrix* source)
{
    if (memcmp(dest->display, source->display, sizeof(dest->display))) {
        memcpy(dest->display, source->display, sizeof(dest->display));
        dest->dirty = true;
    }
}

void iota_gfx_task_user(void)
{
    struct CharacterMatrix matrix;
    matrix_clear(&matrix);
    matrix_render_user(&matrix);
    matrix_update(&display, &matrix);
}

// Custom keys
bool is_ctrl_tabbing = false;
bool is_ctrl_f10 = false;

bool paren_active = false;
uint16_t paren_timer = 0;

uint16_t user_key_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
    switch (keycode) {
    case KC_TAB: {
        const uint8_t is_ctrl = (get_mods() & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_RCTL)));
        const uint8_t is_shift = (get_mods() & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT)));

        if (record->event.pressed) {
            if (is_ctrl) {
                // CTRL + Tab gets converted to SHIFT + Tab
                is_ctrl_tabbing = true;
                unregister_code(KC_LCTRL);
                register_code(KC_LSFT);
                register_code(KC_TAB);
            }
            else {
                register_code(KC_TAB);
            }
        }
        else {
            if (is_ctrl_tabbing) {
                // Turn off the SHIFT + Tab
                is_ctrl_tabbing = false;
                unregister_code(KC_LSFT);
                unregister_code(KC_TAB);
                if (is_shift) {
                    // Re-register CTRL if the mod button was still held down
                    register_code(KC_LCTRL);
                }
            }
            else {
                unregister_code(KC_TAB);
            }
        }

        return false;
    }

    case KC_F10: {
        const uint8_t is_ctrl = (get_mods() & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_RCTL)));
        if (record->event.pressed) {
            if (is_ctrl) {
                // CTRL + F10 is converted into CTRL + Y (Redo)
                is_ctrl_f10 = true;
                register_code(KC_Y);
                return false;
            }
        }
        else {
            if (is_ctrl_f10) {
                // Turn off the CTRL + Y (Redo)
                is_ctrl_f10 = false;
                unregister_code(KC_Y);
                return false;
            }
        }

        return true;
    }

    case SCREENCAP: {
        if (record->event.pressed) {
            // Use ALT + PrintScreen to only capture the focused window
            register_code(KC_LALT);
            tap_code(KC_PSCR);
            unregister_code(KC_LALT);
            return false;
        }

        return true;
    }

    case PAREN:
        if (record->event.pressed) {
            SEND_STRING("(");
            paren_active = true;
            paren_timer = timer_read();
        }
        else {
            if (timer_elapsed(paren_timer) > 100)
                SEND_STRING(")"SS_TAP(X_LEFT));
        }
        break;

    case RAND:
        raw_hid_send_screen_index();
        tap_random_base64();
        break;

    case CCCV:  // One key copy/paste
        if (record->event.pressed) {
            user_key_timer = timer_read32();
        }
        else {
            if (timer_elapsed32(user_key_timer) > TAPPING_TERM) {  // Hold, copy
                register_code(KC_LCTL);
                tap_code(KC_C);
                unregister_code(KC_LCTL);
            }
            else {  // Tap, paste
                register_code(KC_LCTL);
                tap_code(KC_V);
                unregister_code(KC_LCTL);
            }
        }
        break;
    }

    return true;
}


// Tap Dance
int cur_dance(qk_tap_dance_state_t* state)
{
    if (state->count == 1) {
        if (!state->pressed)  return SINGLE_TAP;
        else return SINGLE_HOLD;
    }
    else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    else return 8;
}

void tapdance_oneach(qk_tap_dance_state_t* state, void* user_data)
{
    int id = (int)user_data;
    tri_layer_action action = tri_layer_actions[id];

    if (action.use_instant_layer && state->pressed) {
        layer_on(action.single_hold);
    }
}

void tapdance_onfinished(qk_tap_dance_state_t* state, void* user_data)
{
    int id = (int)user_data;
    tap_states[id].state = cur_dance(state);
    tri_layer_action action = tri_layer_actions[id];

    switch (tap_states[id].state) {
    case SINGLE_TAP: register_code(action.single_tap); break;
    case SINGLE_HOLD: layer_on(action.single_hold); break;
    case DOUBLE_TAP: {
        if (action.is_double_tap_layer_lock) {
            layer_on(action.double_tap);
        }
        else {
            register_code(action.double_tap);
        }
        break;
    }
    }
}

void tapdance_onreset(qk_tap_dance_state_t* state, void* user_data)
{
    int id = (int)user_data;
    tri_layer_action action = tri_layer_actions[id];

    switch (tap_states[id].state) {
    case SINGLE_TAP: unregister_code(action.single_tap); break;
    case SINGLE_HOLD: layer_off(action.single_hold); break;
    case DOUBLE_TAP: {
        if (action.is_double_tap_layer_lock) {
        }
        else {
            unregister_code(action.double_tap);
        }
        break;
    }
    }

    if (action.use_instant_layer) {
        layer_off(action.single_hold);
    }

    tap_states[id].state = 0;
}
