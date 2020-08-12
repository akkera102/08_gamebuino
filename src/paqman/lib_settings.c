#ifndef SETTINGS_C
#define	SETTINGS_C

//---------------------------------------------------------------------------
// ADRUBOY MODE
// SETTINGS YOU CAN EDIT
#define NUM_CHANNELS 4		// 0-4
#define EXTENDED_NOTE_RANGE 1


//---------------------------------------------------------------------------
// DEFAULT SET

// DISPLAY_ROT NOROT
// ENABLE_GUI 1
// ENABLE_BITMAPS 1
// ENABLE_GRAYSCALE 1
// ENABLE_BATTERY 0
// ENABLE_BACKLIGHT 0


//---------------------------------------------------------------------------
// 40 = 40 frames (2sec) before start menu is skipped, 0 = no start menu, 255 = start menu until you press A
#define START_MENU_TIMER 255

// GUI
#define KEYBOARD_W 16
#define KEYBOARD_H  8

// screen
#define SCR_CLK 15
#define SCR_DIN 16
#define SCR_DC   4
#define SCR_CS  12
#define SCR_RST  6

// sound
#define VOLUME_GLOBAL_MAX 1
// 7=instrument volume 9=note volume
#define VOLUME_CHANNEL_MAX 255/NUM_CHANNELS/VOLUME_GLOBAL_MAX/7/9

// battery
#define NUM_LVL 4

// button
#define NUM_BTN       7
#define BTN_UP        1
#define BTN_RIGHT     2
#define BTN_DOWN      3
#define BTN_LEFT      0
#define BTN_A         4
#define BTN_B         5
#define BTN_C         6

#define BTN_UP_PIN    A0
#define BTN_RIGHT_PIN A1
#define BTN_DOWN_PIN  A3
#define BTN_LEFT_PIN  A2
#define BTN_A_PIN     7
#define BTN_B_PIN     8
#define BTN_C_PIN     0		// no use

#endif
