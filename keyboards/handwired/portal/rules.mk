# MCU name
MCU = atmega32u4
BOOTLOADER = caterina
F_CPU = 16000000
ARCH = AVR8

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       # Virtual DIP switch configuration
MOUSEKEY_ENABLE = no        # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = no            # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
MIDI_ENABLE = no            # MIDI support
BLUETOOTH_ENABLE = no       # Enable Bluetooth with the Adafruit EZ-Key HID
AUDIO_ENABLE = no           # Audio output on port C6
FAUXCLICKY_ENABLE = no      # Use buzzer to emulate clicky switches
HD44780_ENABLE = no         # Enable support for HD44780 based LCDs

SRC += i2c.c
SRC += serial.c
SRC += ssd1306.c
SRC += split_util.c
SRC += split_scomm.c
# If you want to change the display of OLED, you need to change here
#SRC +=  ./glcdfont.c \
#        ./lib/logo_reader.c \

#OLED_DRIVER_ENABLE = yes

#TAP_DANCE_ENABLE = yes
#SPLIT_KEYBOARD = yes
RAW_ENABLE = yes

#EXTRAFLAGS += -flto # Reduce size of firmware by optimizing at link time
