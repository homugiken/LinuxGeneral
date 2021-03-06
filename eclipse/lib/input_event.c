/*==============================================================================
 * input_event.c
 *----------------------------------------------------------------------------*/

#include "input_event.h"
#include "clock_time.h"

static char * syn_name[SYN_MAX + 1] =
{
	[0 ... SYN_MAX] = NULL,
	NAME_INITIALIZER(SYN_REPORT),
	NAME_INITIALIZER(SYN_CONFIG),
	NAME_INITIALIZER(SYN_MT_REPORT),
	NAME_INITIALIZER(SYN_DROPPED)
};

static char * rel_name[REL_MAX + 1] =
{
	[0 ... REL_MAX] = NULL,
	NAME_INITIALIZER(REL_X),
	NAME_INITIALIZER(REL_Y),
	NAME_INITIALIZER(REL_Z),
	NAME_INITIALIZER(REL_RX),
	NAME_INITIALIZER(REL_RY),
	NAME_INITIALIZER(REL_RZ),
	NAME_INITIALIZER(REL_HWHEEL),
	NAME_INITIALIZER(REL_DIAL),
	NAME_INITIALIZER(REL_WHEEL),
	NAME_INITIALIZER(REL_MISC)
};

static char * abs_name[ABS_MAX + 1] =
{
	[0 ... ABS_MAX] = NULL,
	NAME_INITIALIZER(ABS_X),
	NAME_INITIALIZER(ABS_Y),
	NAME_INITIALIZER(ABS_Z),
	NAME_INITIALIZER(ABS_RX),
	NAME_INITIALIZER(ABS_RY),
	NAME_INITIALIZER(ABS_RZ),
	NAME_INITIALIZER(ABS_THROTTLE),
	NAME_INITIALIZER(ABS_RUDDER),
	NAME_INITIALIZER(ABS_WHEEL),
	NAME_INITIALIZER(ABS_GAS),
	NAME_INITIALIZER(ABS_BRAKE),
	NAME_INITIALIZER(ABS_HAT0X),
	NAME_INITIALIZER(ABS_HAT0Y),
	NAME_INITIALIZER(ABS_HAT1X),
	NAME_INITIALIZER(ABS_HAT1Y),
	NAME_INITIALIZER(ABS_HAT2X),
	NAME_INITIALIZER(ABS_HAT2Y),
	NAME_INITIALIZER(ABS_HAT3X),
	NAME_INITIALIZER(ABS_HAT3Y),
	NAME_INITIALIZER(ABS_PRESSURE),
	NAME_INITIALIZER(ABS_DISTANCE),
	NAME_INITIALIZER(ABS_TILT_X),
	NAME_INITIALIZER(ABS_TILT_Y),
	NAME_INITIALIZER(ABS_TOOL_WIDTH),
	NAME_INITIALIZER(ABS_VOLUME),
	NAME_INITIALIZER(ABS_MISC),
	NAME_INITIALIZER(ABS_MT_SLOT),
	NAME_INITIALIZER(ABS_MT_TOUCH_MAJOR),
	NAME_INITIALIZER(ABS_MT_TOUCH_MINOR),
	NAME_INITIALIZER(ABS_MT_WIDTH_MAJOR),
	NAME_INITIALIZER(ABS_MT_WIDTH_MINOR),
	NAME_INITIALIZER(ABS_MT_ORIENTATION),
	NAME_INITIALIZER(ABS_MT_POSITION_X),
	NAME_INITIALIZER(ABS_MT_POSITION_Y),
	NAME_INITIALIZER(ABS_MT_TOOL_TYPE),
	NAME_INITIALIZER(ABS_MT_BLOB_ID),
	NAME_INITIALIZER(ABS_MT_TRACKING_ID),
	NAME_INITIALIZER(ABS_MT_PRESSURE),
	NAME_INITIALIZER(ABS_MT_DISTANCE),
	NAME_INITIALIZER(ABS_MT_TOOL_X),
	NAME_INITIALIZER(ABS_MT_TOOL_Y)
};

static char * sw_name[SW_MAX + 1] =
{
	[0 ... SW_MAX] = NULL,
	NAME_INITIALIZER(SW_LID),
	NAME_INITIALIZER(SW_TABLET_MODE),
	NAME_INITIALIZER(SW_HEADPHONE_INSERT),
	NAME_INITIALIZER(SW_RFKILL_ALL),
	NAME_INITIALIZER(SW_RADIO),
	NAME_INITIALIZER(SW_MICROPHONE_INSERT),
	NAME_INITIALIZER(SW_DOCK),
	NAME_INITIALIZER(SW_LINEOUT_INSERT),
	NAME_INITIALIZER(SW_JACK_PHYSICAL_INSERT),
	NAME_INITIALIZER(SW_VIDEOOUT_INSERT),
	NAME_INITIALIZER(SW_CAMERA_LENS_COVER),
	NAME_INITIALIZER(SW_KEYPAD_SLIDE),
	NAME_INITIALIZER(SW_FRONT_PROXIMITY),
	NAME_INITIALIZER(SW_ROTATE_LOCK),
	NAME_INITIALIZER(SW_LINEIN_INSERT),
	NAME_INITIALIZER(SW_MUTE_DEVICE)
};

static char * msc_name[MSC_MAX + 1] =
{
	[ 0 ... MSC_MAX] = NULL,
	NAME_INITIALIZER(MSC_SERIAL),
	NAME_INITIALIZER(MSC_PULSELED),
	NAME_INITIALIZER(MSC_GESTURE),
	NAME_INITIALIZER(MSC_RAW),
	NAME_INITIALIZER(MSC_SCAN),
	NAME_INITIALIZER(MSC_TIMESTAMP)
};

static char * led_name[LED_MAX + 1] =
{
	[0 ... LED_MAX] = NULL,
	NAME_INITIALIZER(LED_NUML),
	NAME_INITIALIZER(LED_CAPSL),
	NAME_INITIALIZER(LED_SCROLLL),
	NAME_INITIALIZER(LED_COMPOSE),
	NAME_INITIALIZER(LED_KANA),
	NAME_INITIALIZER(LED_SLEEP),
	NAME_INITIALIZER(LED_SUSPEND),
	NAME_INITIALIZER(LED_MUTE),
	NAME_INITIALIZER(LED_MISC),
	NAME_INITIALIZER(LED_MAIL),
	NAME_INITIALIZER(LED_CHARGING)
};

static char * rep_name[REP_MAX + 1] =
{
	[0 ... REP_MAX] = NULL,
	NAME_INITIALIZER(REP_DELAY),
	NAME_INITIALIZER(REP_PERIOD)
};

static char * snd_name[SND_MAX + 1] =
{
	[0 ... SND_MAX] = NULL,
	NAME_INITIALIZER(SND_CLICK),
	NAME_INITIALIZER(SND_BELL),
	NAME_INITIALIZER(SND_TONE)
};

static char * key_name[KEY_MAX + 1] =
{
	[0 ... KEY_MAX] = NULL,
	NAME_INITIALIZER(KEY_RESERVED),
	NAME_INITIALIZER(KEY_ESC),
	NAME_INITIALIZER(KEY_1),
	NAME_INITIALIZER(KEY_2),
	NAME_INITIALIZER(KEY_3),
	NAME_INITIALIZER(KEY_4),
	NAME_INITIALIZER(KEY_5),
	NAME_INITIALIZER(KEY_6),
	NAME_INITIALIZER(KEY_7),
	NAME_INITIALIZER(KEY_8),
	NAME_INITIALIZER(KEY_9),
	NAME_INITIALIZER(KEY_0),
	NAME_INITIALIZER(KEY_MINUS),
	NAME_INITIALIZER(KEY_EQUAL),
	NAME_INITIALIZER(KEY_BACKSPACE),
	NAME_INITIALIZER(KEY_TAB),
	NAME_INITIALIZER(KEY_Q),
	NAME_INITIALIZER(KEY_W),
	NAME_INITIALIZER(KEY_E),
	NAME_INITIALIZER(KEY_R),
	NAME_INITIALIZER(KEY_T),
	NAME_INITIALIZER(KEY_Y),
	NAME_INITIALIZER(KEY_U),
	NAME_INITIALIZER(KEY_I),
	NAME_INITIALIZER(KEY_O),
	NAME_INITIALIZER(KEY_P),
	NAME_INITIALIZER(KEY_LEFTBRACE),
	NAME_INITIALIZER(KEY_RIGHTBRACE),
	NAME_INITIALIZER(KEY_ENTER),
	NAME_INITIALIZER(KEY_LEFTCTRL),
	NAME_INITIALIZER(KEY_A),
	NAME_INITIALIZER(KEY_S),
	NAME_INITIALIZER(KEY_D),
	NAME_INITIALIZER(KEY_F),
	NAME_INITIALIZER(KEY_G),
	NAME_INITIALIZER(KEY_H),
	NAME_INITIALIZER(KEY_J),
	NAME_INITIALIZER(KEY_K),
	NAME_INITIALIZER(KEY_L),
	NAME_INITIALIZER(KEY_SEMICOLON),
	NAME_INITIALIZER(KEY_APOSTROPHE),
	NAME_INITIALIZER(KEY_GRAVE),
	NAME_INITIALIZER(KEY_LEFTSHIFT),
	NAME_INITIALIZER(KEY_BACKSLASH),
	NAME_INITIALIZER(KEY_Z),
	NAME_INITIALIZER(KEY_X),
	NAME_INITIALIZER(KEY_C),
	NAME_INITIALIZER(KEY_V),
	NAME_INITIALIZER(KEY_B),
	NAME_INITIALIZER(KEY_N),
	NAME_INITIALIZER(KEY_M),
	NAME_INITIALIZER(KEY_COMMA),
	NAME_INITIALIZER(KEY_DOT),
	NAME_INITIALIZER(KEY_SLASH),
	NAME_INITIALIZER(KEY_RIGHTSHIFT),
	NAME_INITIALIZER(KEY_KPASTERISK),
	NAME_INITIALIZER(KEY_LEFTALT),
	NAME_INITIALIZER(KEY_SPACE),
	NAME_INITIALIZER(KEY_CAPSLOCK),
	NAME_INITIALIZER(KEY_F1),
	NAME_INITIALIZER(KEY_F2),
	NAME_INITIALIZER(KEY_F3),
	NAME_INITIALIZER(KEY_F4),
	NAME_INITIALIZER(KEY_F5),
	NAME_INITIALIZER(KEY_F6),
	NAME_INITIALIZER(KEY_F7),
	NAME_INITIALIZER(KEY_F8),
	NAME_INITIALIZER(KEY_F9),
	NAME_INITIALIZER(KEY_F10),
	NAME_INITIALIZER(KEY_NUMLOCK),
	NAME_INITIALIZER(KEY_SCROLLLOCK),
	NAME_INITIALIZER(KEY_KP7),
	NAME_INITIALIZER(KEY_KP8),
	NAME_INITIALIZER(KEY_KP9),
	NAME_INITIALIZER(KEY_KPMINUS),
	NAME_INITIALIZER(KEY_KP4),
	NAME_INITIALIZER(KEY_KP5),
	NAME_INITIALIZER(KEY_KP6),
	NAME_INITIALIZER(KEY_KPPLUS),
	NAME_INITIALIZER(KEY_KP1),
	NAME_INITIALIZER(KEY_KP2),
	NAME_INITIALIZER(KEY_KP3),
	NAME_INITIALIZER(KEY_KP0),
	NAME_INITIALIZER(KEY_KPDOT),
	NAME_INITIALIZER(KEY_ZENKAKUHANKAKU),
	NAME_INITIALIZER(KEY_102ND),
	NAME_INITIALIZER(KEY_F11),
	NAME_INITIALIZER(KEY_F12),
	NAME_INITIALIZER(KEY_RO),
	NAME_INITIALIZER(KEY_KATAKANA),
	NAME_INITIALIZER(KEY_HIRAGANA),
	NAME_INITIALIZER(KEY_HENKAN),
	NAME_INITIALIZER(KEY_KATAKANAHIRAGANA),
	NAME_INITIALIZER(KEY_MUHENKAN),
	NAME_INITIALIZER(KEY_KPJPCOMMA),
	NAME_INITIALIZER(KEY_KPENTER),
	NAME_INITIALIZER(KEY_RIGHTCTRL),
	NAME_INITIALIZER(KEY_KPSLASH),
	NAME_INITIALIZER(KEY_SYSRQ),
	NAME_INITIALIZER(KEY_RIGHTALT),
	NAME_INITIALIZER(KEY_LINEFEED),
	NAME_INITIALIZER(KEY_HOME),
	NAME_INITIALIZER(KEY_UP),
	NAME_INITIALIZER(KEY_PAGEUP),
	NAME_INITIALIZER(KEY_LEFT),
	NAME_INITIALIZER(KEY_RIGHT),
	NAME_INITIALIZER(KEY_END),
	NAME_INITIALIZER(KEY_DOWN),
	NAME_INITIALIZER(KEY_PAGEDOWN),
	NAME_INITIALIZER(KEY_INSERT),
	NAME_INITIALIZER(KEY_DELETE),
	NAME_INITIALIZER(KEY_MACRO),
	NAME_INITIALIZER(KEY_MUTE),
	NAME_INITIALIZER(KEY_VOLUMEDOWN),
	NAME_INITIALIZER(KEY_VOLUMEUP),
	NAME_INITIALIZER(KEY_POWER),
	NAME_INITIALIZER(KEY_KPEQUAL),
	NAME_INITIALIZER(KEY_KPPLUSMINUS),
	NAME_INITIALIZER(KEY_PAUSE),
	NAME_INITIALIZER(KEY_SCALE),
	NAME_INITIALIZER(KEY_KPCOMMA),
	NAME_INITIALIZER(KEY_HANGUEL),
	NAME_INITIALIZER(KEY_HANJA),
	NAME_INITIALIZER(KEY_YEN),
	NAME_INITIALIZER(KEY_LEFTMETA),
	NAME_INITIALIZER(KEY_RIGHTMETA),
	NAME_INITIALIZER(KEY_COMPOSE),
	NAME_INITIALIZER(KEY_STOP),
	NAME_INITIALIZER(KEY_AGAIN),
	NAME_INITIALIZER(KEY_PROPS),
	NAME_INITIALIZER(KEY_UNDO),
	NAME_INITIALIZER(KEY_FRONT),
	NAME_INITIALIZER(KEY_COPY),
	NAME_INITIALIZER(KEY_OPEN),
	NAME_INITIALIZER(KEY_PASTE),
	NAME_INITIALIZER(KEY_FIND),
	NAME_INITIALIZER(KEY_CUT),
	NAME_INITIALIZER(KEY_HELP),
	NAME_INITIALIZER(KEY_MENU),
	NAME_INITIALIZER(KEY_CALC),
	NAME_INITIALIZER(KEY_SETUP),
	NAME_INITIALIZER(KEY_SLEEP),
	NAME_INITIALIZER(KEY_WAKEUP),
	NAME_INITIALIZER(KEY_FILE),
	NAME_INITIALIZER(KEY_SENDFILE),
	NAME_INITIALIZER(KEY_DELETEFILE),
	NAME_INITIALIZER(KEY_XFER),
	NAME_INITIALIZER(KEY_PROG1),
	NAME_INITIALIZER(KEY_PROG2),
	NAME_INITIALIZER(KEY_WWW),
	NAME_INITIALIZER(KEY_MSDOS),
	NAME_INITIALIZER(KEY_COFFEE),
	NAME_INITIALIZER(KEY_SCREENLOCK),
	NAME_INITIALIZER(KEY_ROTATE_DISPLAY),
	NAME_INITIALIZER(KEY_DIRECTION),
	NAME_INITIALIZER(KEY_CYCLEWINDOWS),
	NAME_INITIALIZER(KEY_MAIL),
	NAME_INITIALIZER(KEY_BOOKMARKS),
	NAME_INITIALIZER(KEY_COMPUTER),
	NAME_INITIALIZER(KEY_BACK),
	NAME_INITIALIZER(KEY_FORWARD),
	NAME_INITIALIZER(KEY_CLOSECD),
	NAME_INITIALIZER(KEY_EJECTCD),
	NAME_INITIALIZER(KEY_EJECTCLOSECD),
	NAME_INITIALIZER(KEY_NEXTSONG),
	NAME_INITIALIZER(KEY_PLAYPAUSE),
	NAME_INITIALIZER(KEY_PREVIOUSSONG),
	NAME_INITIALIZER(KEY_STOPCD),
	NAME_INITIALIZER(KEY_RECORD),
	NAME_INITIALIZER(KEY_REWIND),
	NAME_INITIALIZER(KEY_PHONE),
	NAME_INITIALIZER(KEY_ISO),
	NAME_INITIALIZER(KEY_CONFIG),
	NAME_INITIALIZER(KEY_HOMEPAGE),
	NAME_INITIALIZER(KEY_REFRESH),
	NAME_INITIALIZER(KEY_EXIT),
	NAME_INITIALIZER(KEY_MOVE),
	NAME_INITIALIZER(KEY_EDIT),
	NAME_INITIALIZER(KEY_SCROLLUP),
	NAME_INITIALIZER(KEY_SCROLLDOWN),
	NAME_INITIALIZER(KEY_KPLEFTPAREN),
	NAME_INITIALIZER(KEY_KPRIGHTPAREN),
	NAME_INITIALIZER(KEY_NEW),
	NAME_INITIALIZER(KEY_REDO),
	NAME_INITIALIZER(KEY_F13),
	NAME_INITIALIZER(KEY_F14),
	NAME_INITIALIZER(KEY_F15),
	NAME_INITIALIZER(KEY_F16),
	NAME_INITIALIZER(KEY_F17),
	NAME_INITIALIZER(KEY_F18),
	NAME_INITIALIZER(KEY_F19),
	NAME_INITIALIZER(KEY_F20),
	NAME_INITIALIZER(KEY_F21),
	NAME_INITIALIZER(KEY_F22),
	NAME_INITIALIZER(KEY_F23),
	NAME_INITIALIZER(KEY_F24),
	NAME_INITIALIZER(KEY_PLAYCD),
	NAME_INITIALIZER(KEY_PAUSECD),
	NAME_INITIALIZER(KEY_PROG3),
	NAME_INITIALIZER(KEY_PROG4),
	NAME_INITIALIZER(KEY_DASHBOARD),
	NAME_INITIALIZER(KEY_SUSPEND),
	NAME_INITIALIZER(KEY_CLOSE),
	NAME_INITIALIZER(KEY_PLAY),
	NAME_INITIALIZER(KEY_FASTFORWARD),
	NAME_INITIALIZER(KEY_BASSBOOST),
	NAME_INITIALIZER(KEY_PRINT),
	NAME_INITIALIZER(KEY_HP),
	NAME_INITIALIZER(KEY_CAMERA),
	NAME_INITIALIZER(KEY_SOUND),
	NAME_INITIALIZER(KEY_QUESTION),
	NAME_INITIALIZER(KEY_EMAIL),
	NAME_INITIALIZER(KEY_CHAT),
	NAME_INITIALIZER(KEY_SEARCH),
	NAME_INITIALIZER(KEY_CONNECT),
	NAME_INITIALIZER(KEY_FINANCE),
	NAME_INITIALIZER(KEY_SPORT),
	NAME_INITIALIZER(KEY_SHOP),
	NAME_INITIALIZER(KEY_ALTERASE),
	NAME_INITIALIZER(KEY_CANCEL),
	NAME_INITIALIZER(KEY_BRIGHTNESSDOWN),
	NAME_INITIALIZER(KEY_BRIGHTNESSUP),
	NAME_INITIALIZER(KEY_MEDIA),
	NAME_INITIALIZER(KEY_SWITCHVIDEOMODE),
	NAME_INITIALIZER(KEY_KBDILLUMTOGGLE),
	NAME_INITIALIZER(KEY_KBDILLUMDOWN),
	NAME_INITIALIZER(KEY_KBDILLUMUP),
	NAME_INITIALIZER(KEY_SEND),
	NAME_INITIALIZER(KEY_REPLY),
	NAME_INITIALIZER(KEY_FORWARDMAIL),
	NAME_INITIALIZER(KEY_SAVE),
	NAME_INITIALIZER(KEY_DOCUMENTS),
	NAME_INITIALIZER(KEY_BATTERY),
	NAME_INITIALIZER(KEY_BLUETOOTH),
	NAME_INITIALIZER(KEY_WLAN),
	NAME_INITIALIZER(KEY_UWB),
	NAME_INITIALIZER(KEY_UNKNOWN),
	NAME_INITIALIZER(KEY_VIDEO_NEXT),
	NAME_INITIALIZER(KEY_VIDEO_PREV),
	NAME_INITIALIZER(KEY_BRIGHTNESS_CYCLE),
	NAME_INITIALIZER(KEY_BRIGHTNESS_AUTO),
	NAME_INITIALIZER(KEY_BRIGHTNESS_ZERO),
	NAME_INITIALIZER(KEY_DISPLAY_OFF),
	NAME_INITIALIZER(KEY_WWAN),
	NAME_INITIALIZER(KEY_WIMAX),
	NAME_INITIALIZER(KEY_RFKILL),
	NAME_INITIALIZER(KEY_MICMUTE),
	NAME_INITIALIZER(BTN_MISC),
	NAME_INITIALIZER(BTN_0),
	NAME_INITIALIZER(BTN_1),
	NAME_INITIALIZER(BTN_2),
	NAME_INITIALIZER(BTN_3),
	NAME_INITIALIZER(BTN_4),
	NAME_INITIALIZER(BTN_5),
	NAME_INITIALIZER(BTN_6),
	NAME_INITIALIZER(BTN_7),
	NAME_INITIALIZER(BTN_8),
	NAME_INITIALIZER(BTN_9),
	NAME_INITIALIZER(BTN_LEFT),
	NAME_INITIALIZER(BTN_RIGHT),
	NAME_INITIALIZER(BTN_MIDDLE),
	NAME_INITIALIZER(BTN_SIDE),
	NAME_INITIALIZER(BTN_EXTRA),
	NAME_INITIALIZER(BTN_FORWARD),
	NAME_INITIALIZER(BTN_BACK),
	NAME_INITIALIZER(BTN_TASK),
	NAME_INITIALIZER(BTN_JOYSTICK),
	NAME_INITIALIZER(BTN_TRIGGER),
	NAME_INITIALIZER(BTN_THUMB),
	NAME_INITIALIZER(BTN_THUMB2),
	NAME_INITIALIZER(BTN_TOP),
	NAME_INITIALIZER(BTN_TOP2),
	NAME_INITIALIZER(BTN_PINKIE),
	NAME_INITIALIZER(BTN_BASE),
	NAME_INITIALIZER(BTN_BASE2),
	NAME_INITIALIZER(BTN_BASE3),
	NAME_INITIALIZER(BTN_BASE4),
	NAME_INITIALIZER(BTN_BASE5),
	NAME_INITIALIZER(BTN_BASE6),
	NAME_INITIALIZER(BTN_DEAD),
	NAME_INITIALIZER(BTN_GAMEPAD),
	NAME_INITIALIZER(BTN_SOUTH),
	NAME_INITIALIZER(BTN_A),
	NAME_INITIALIZER(BTN_EAST),
	NAME_INITIALIZER(BTN_B),
	NAME_INITIALIZER(BTN_C),
	NAME_INITIALIZER(BTN_NORTH),
	NAME_INITIALIZER(BTN_X),
	NAME_INITIALIZER(BTN_WEST),
	NAME_INITIALIZER(BTN_Y),
	NAME_INITIALIZER(BTN_Z),
	NAME_INITIALIZER(BTN_TL),
	NAME_INITIALIZER(BTN_TR),
	NAME_INITIALIZER(BTN_TL2),
	NAME_INITIALIZER(BTN_TR2),
	NAME_INITIALIZER(BTN_SELECT),
	NAME_INITIALIZER(BTN_START),
	NAME_INITIALIZER(BTN_MODE),
	NAME_INITIALIZER(BTN_THUMBL),
	NAME_INITIALIZER(BTN_THUMBR),
	NAME_INITIALIZER(BTN_DIGI),
	NAME_INITIALIZER(BTN_TOOL_PEN),
	NAME_INITIALIZER(BTN_TOOL_RUBBER),
	NAME_INITIALIZER(BTN_TOOL_BRUSH),
	NAME_INITIALIZER(BTN_TOOL_PENCIL),
	NAME_INITIALIZER(BTN_TOOL_AIRBRUSH),
	NAME_INITIALIZER(BTN_TOOL_FINGER),
	NAME_INITIALIZER(BTN_TOOL_MOUSE),
	NAME_INITIALIZER(BTN_TOOL_LENS),
	NAME_INITIALIZER(BTN_TOOL_QUINTTAP),
	NAME_INITIALIZER(BTN_TOUCH),
	NAME_INITIALIZER(BTN_STYLUS),
	NAME_INITIALIZER(BTN_STYLUS2),
	NAME_INITIALIZER(BTN_TOOL_DOUBLETAP),
	NAME_INITIALIZER(BTN_TOOL_TRIPLETAP),
	NAME_INITIALIZER(BTN_TOOL_QUADTAP),
	NAME_INITIALIZER(BTN_WHEEL),
	NAME_INITIALIZER(BTN_GEAR_DOWN),
	NAME_INITIALIZER(BTN_GEAR_UP),
	NAME_INITIALIZER(KEY_OK),
	NAME_INITIALIZER(KEY_SELECT),
	NAME_INITIALIZER(KEY_GOTO),
	NAME_INITIALIZER(KEY_CLEAR),
	NAME_INITIALIZER(KEY_POWER2),
	NAME_INITIALIZER(KEY_OPTION),
	NAME_INITIALIZER(KEY_INFO),
	NAME_INITIALIZER(KEY_TIME),
	NAME_INITIALIZER(KEY_VENDOR),
	NAME_INITIALIZER(KEY_ARCHIVE),
	NAME_INITIALIZER(KEY_PROGRAM),
	NAME_INITIALIZER(KEY_CHANNEL),
	NAME_INITIALIZER(KEY_FAVORITES),
	NAME_INITIALIZER(KEY_EPG),
	NAME_INITIALIZER(KEY_PVR),
	NAME_INITIALIZER(KEY_MHP),
	NAME_INITIALIZER(KEY_LANGUAGE),
	NAME_INITIALIZER(KEY_TITLE),
	NAME_INITIALIZER(KEY_SUBTITLE),
	NAME_INITIALIZER(KEY_ANGLE),
	NAME_INITIALIZER(KEY_ZOOM),
	NAME_INITIALIZER(KEY_MODE),
	NAME_INITIALIZER(KEY_KEYBOARD),
	NAME_INITIALIZER(KEY_SCREEN),
	NAME_INITIALIZER(KEY_PC),
	NAME_INITIALIZER(KEY_TV),
	NAME_INITIALIZER(KEY_TV2),
	NAME_INITIALIZER(KEY_VCR),
	NAME_INITIALIZER(KEY_VCR2),
	NAME_INITIALIZER(KEY_SAT),
	NAME_INITIALIZER(KEY_SAT2),
	NAME_INITIALIZER(KEY_CD),
	NAME_INITIALIZER(KEY_TAPE),
	NAME_INITIALIZER(KEY_RADIO),
	NAME_INITIALIZER(KEY_TUNER),
	NAME_INITIALIZER(KEY_PLAYER),
	NAME_INITIALIZER(KEY_TEXT),
	NAME_INITIALIZER(KEY_DVD),
	NAME_INITIALIZER(KEY_AUX),
	NAME_INITIALIZER(KEY_MP3),
	NAME_INITIALIZER(KEY_AUDIO),
	NAME_INITIALIZER(KEY_VIDEO),
	NAME_INITIALIZER(KEY_DIRECTORY),
	NAME_INITIALIZER(KEY_LIST),
	NAME_INITIALIZER(KEY_MEMO),
	NAME_INITIALIZER(KEY_CALENDAR),
	NAME_INITIALIZER(KEY_RED),
	NAME_INITIALIZER(KEY_GREEN),
	NAME_INITIALIZER(KEY_YELLOW),
	NAME_INITIALIZER(KEY_BLUE),
	NAME_INITIALIZER(KEY_CHANNELUP),
	NAME_INITIALIZER(KEY_CHANNELDOWN),
	NAME_INITIALIZER(KEY_FIRST),
	NAME_INITIALIZER(KEY_LAST),
	NAME_INITIALIZER(KEY_AB),
	NAME_INITIALIZER(KEY_NEXT),
	NAME_INITIALIZER(KEY_RESTART),
	NAME_INITIALIZER(KEY_SLOW),
	NAME_INITIALIZER(KEY_SHUFFLE),
	NAME_INITIALIZER(KEY_BREAK),
	NAME_INITIALIZER(KEY_PREVIOUS),
	NAME_INITIALIZER(KEY_DIGITS),
	NAME_INITIALIZER(KEY_TEEN),
	NAME_INITIALIZER(KEY_TWEN),
	NAME_INITIALIZER(KEY_VIDEOPHONE),
	NAME_INITIALIZER(KEY_GAMES),
	NAME_INITIALIZER(KEY_ZOOMIN),
	NAME_INITIALIZER(KEY_ZOOMOUT),
	NAME_INITIALIZER(KEY_ZOOMRESET),
	NAME_INITIALIZER(KEY_WORDPROCESSOR),
	NAME_INITIALIZER(KEY_EDITOR),
	NAME_INITIALIZER(KEY_SPREADSHEET),
	NAME_INITIALIZER(KEY_GRAPHICSEDITOR),
	NAME_INITIALIZER(KEY_PRESENTATION),
	NAME_INITIALIZER(KEY_DATABASE),
	NAME_INITIALIZER(KEY_NEWS),
	NAME_INITIALIZER(KEY_VOICEMAIL),
	NAME_INITIALIZER(KEY_ADDRESSBOOK),
	NAME_INITIALIZER(KEY_MESSENGER),
	NAME_INITIALIZER(KEY_DISPLAYTOGGLE),
	NAME_INITIALIZER(KEY_BRIGHTNESS_TOGGLE),
	NAME_INITIALIZER(KEY_SPELLCHECK),
	NAME_INITIALIZER(KEY_LOGOFF),
	NAME_INITIALIZER(KEY_DOLLAR),
	NAME_INITIALIZER(KEY_EURO),
	NAME_INITIALIZER(KEY_FRAMEBACK),
	NAME_INITIALIZER(KEY_FRAMEFORWARD),
	NAME_INITIALIZER(KEY_CONTEXT_MENU),
	NAME_INITIALIZER(KEY_MEDIA_REPEAT),
	NAME_INITIALIZER(KEY_10CHANNELSUP),
	NAME_INITIALIZER(KEY_10CHANNELSDOWN),
	NAME_INITIALIZER(KEY_IMAGES),
	NAME_INITIALIZER(KEY_DEL_EOL),
	NAME_INITIALIZER(KEY_DEL_EOS),
	NAME_INITIALIZER(KEY_INS_LINE),
	NAME_INITIALIZER(KEY_DEL_LINE),
	NAME_INITIALIZER(KEY_FN),
	NAME_INITIALIZER(KEY_FN_ESC),
	NAME_INITIALIZER(KEY_FN_F1),
	NAME_INITIALIZER(KEY_FN_F2),
	NAME_INITIALIZER(KEY_FN_F3),
	NAME_INITIALIZER(KEY_FN_F4),
	NAME_INITIALIZER(KEY_FN_F5),
	NAME_INITIALIZER(KEY_FN_F6),
	NAME_INITIALIZER(KEY_FN_F7),
	NAME_INITIALIZER(KEY_FN_F8),
	NAME_INITIALIZER(KEY_FN_F9),
	NAME_INITIALIZER(KEY_FN_F10),
	NAME_INITIALIZER(KEY_FN_F11),
	NAME_INITIALIZER(KEY_FN_F12),
	NAME_INITIALIZER(KEY_FN_1),
	NAME_INITIALIZER(KEY_FN_2),
	NAME_INITIALIZER(KEY_FN_D),
	NAME_INITIALIZER(KEY_FN_E),
	NAME_INITIALIZER(KEY_FN_F),
	NAME_INITIALIZER(KEY_FN_S),
	NAME_INITIALIZER(KEY_FN_B),
	NAME_INITIALIZER(KEY_BRL_DOT1),
	NAME_INITIALIZER(KEY_BRL_DOT2),
	NAME_INITIALIZER(KEY_BRL_DOT3),
	NAME_INITIALIZER(KEY_BRL_DOT4),
	NAME_INITIALIZER(KEY_BRL_DOT5),
	NAME_INITIALIZER(KEY_BRL_DOT6),
	NAME_INITIALIZER(KEY_BRL_DOT7),
	NAME_INITIALIZER(KEY_BRL_DOT8),
	NAME_INITIALIZER(KEY_BRL_DOT9),
	NAME_INITIALIZER(KEY_BRL_DOT10),
	NAME_INITIALIZER(KEY_NUMERIC_0),
	NAME_INITIALIZER(KEY_NUMERIC_1),
	NAME_INITIALIZER(KEY_NUMERIC_2),
	NAME_INITIALIZER(KEY_NUMERIC_3),
	NAME_INITIALIZER(KEY_NUMERIC_4),
	NAME_INITIALIZER(KEY_NUMERIC_5),
	NAME_INITIALIZER(KEY_NUMERIC_6),
	NAME_INITIALIZER(KEY_NUMERIC_7),
	NAME_INITIALIZER(KEY_NUMERIC_8),
	NAME_INITIALIZER(KEY_NUMERIC_9),
	NAME_INITIALIZER(KEY_NUMERIC_STAR),
	NAME_INITIALIZER(KEY_NUMERIC_POUND),
	NAME_INITIALIZER(KEY_NUMERIC_A),
	NAME_INITIALIZER(KEY_NUMERIC_B),
	NAME_INITIALIZER(KEY_NUMERIC_C),
	NAME_INITIALIZER(KEY_NUMERIC_D),
	NAME_INITIALIZER(KEY_CAMERA_FOCUS),
	NAME_INITIALIZER(KEY_WPS_BUTTON),
	NAME_INITIALIZER(KEY_TOUCHPAD_TOGGLE),
	NAME_INITIALIZER(KEY_TOUCHPAD_ON),
	NAME_INITIALIZER(KEY_TOUCHPAD_OFF),
	NAME_INITIALIZER(KEY_CAMERA_ZOOMIN),
	NAME_INITIALIZER(KEY_CAMERA_ZOOMOUT),
	NAME_INITIALIZER(KEY_CAMERA_UP),
	NAME_INITIALIZER(KEY_CAMERA_DOWN),
	NAME_INITIALIZER(KEY_CAMERA_LEFT),
	NAME_INITIALIZER(KEY_CAMERA_RIGHT),
	NAME_INITIALIZER(KEY_ATTENDANT_ON),
	NAME_INITIALIZER(KEY_ATTENDANT_OFF),
	NAME_INITIALIZER(KEY_ATTENDANT_TOGGLE),
	NAME_INITIALIZER(KEY_LIGHTS_TOGGLE),
	NAME_INITIALIZER(BTN_DPAD_UP),
	NAME_INITIALIZER(BTN_DPAD_DOWN),
	NAME_INITIALIZER(BTN_DPAD_LEFT),
	NAME_INITIALIZER(BTN_DPAD_RIGHT),
	NAME_INITIALIZER(KEY_ALS_TOGGLE),
	NAME_INITIALIZER(KEY_BUTTONCONFIG),
	NAME_INITIALIZER(KEY_TASKMANAGER),
	NAME_INITIALIZER(KEY_JOURNAL),
	NAME_INITIALIZER(KEY_CONTROLPANEL),
	NAME_INITIALIZER(KEY_APPSELECT),
	NAME_INITIALIZER(KEY_SCREENSAVER),
	NAME_INITIALIZER(KEY_VOICECOMMAND),
	// NAME_INITIALIZER(KEY_ASSISTANT),
	NAME_INITIALIZER(KEY_BRIGHTNESS_MIN),
	NAME_INITIALIZER(KEY_BRIGHTNESS_MAX),
	NAME_INITIALIZER(KEY_KBDINPUTASSIST_PREV),
	NAME_INITIALIZER(KEY_KBDINPUTASSIST_NEXT),
	NAME_INITIALIZER(KEY_KBDINPUTASSIST_PREVGROUP),
	NAME_INITIALIZER(KEY_KBDINPUTASSIST_NEXTGROUP),
	NAME_INITIALIZER(KEY_KBDINPUTASSIST_ACCEPT),
	NAME_INITIALIZER(KEY_KBDINPUTASSIST_CANCEL),
	// NAME_INITIALIZER(KEY_RIGHT_UP),
	// NAME_INITIALIZER(KEY_RIGHT_DOWN),
	// NAME_INITIALIZER(KEY_LEFT_UP),
	// NAME_INITIALIZER(KEY_LEFT_DOWN),
	// NAME_INITIALIZER(KEY_ROOT_MENU),
	// NAME_INITIALIZER(KEY_MEDIA_TOP_MENU),
	// NAME_INITIALIZER(KEY_NUMERIC_11),
	// NAME_INITIALIZER(KEY_NUMERIC_12),
	// NAME_INITIALIZER(KEY_AUDIO_DESC),
	// NAME_INITIALIZER(KEY_3D_MODE),
	// NAME_INITIALIZER(KEY_NEXT_FAVORITE),
	// NAME_INITIALIZER(KEY_STOP_RECORD),
	// NAME_INITIALIZER(KEY_PAUSE_RECORD),
	// NAME_INITIALIZER(KEY_VOD),
	// NAME_INITIALIZER(KEY_UNMUTE),
	// NAME_INITIALIZER(KEY_FASTREVERSE),
	// NAME_INITIALIZER(KEY_SLOWREVERSE),
	// NAME_INITIALIZER(KEY_DATA),
	// NAME_INITIALIZER(KEY_ONSCREEN_KEYBOARD),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_1),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_2),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_3),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_4),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_5),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_6),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_7),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_8),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_9),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_10),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_11),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_12),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_13),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_14),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_15),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_16),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_17),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_18),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_19),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_20),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_21),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_22),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_23),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_24),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_25),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_26),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_27),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_28),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_29),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_30),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_31),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_32),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_33),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_34),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_35),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_36),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_37),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_38),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_39),
	// NAME_INITIALIZER(BTN_TRIGGER_HAPPY_40),
	NAME_INITIALIZER(KEY_MIN_INTERESTING)
};

static char ** event_code_name[EV_MAX + 1] =
{
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = syn_name,
	[EV_KEY] = key_name,
	[EV_REL] = rel_name,
	[EV_ABS] = abs_name,
	[EV_MSC] = msc_name,
	[EV_SW] = sw_name,
	[EV_LED] = led_name,
	[EV_SND] = snd_name,
	[EV_REP] = rep_name
};

static char * event_name[EV_MAX + 1] =
{
	[0 ... EV_MAX] = NULL,
	NAME_INITIALIZER(EV_SYN),
	NAME_INITIALIZER(EV_KEY),
	NAME_INITIALIZER(EV_REL),
	NAME_INITIALIZER(EV_ABS),
	NAME_INITIALIZER(EV_MSC),
	NAME_INITIALIZER(EV_SW),
	NAME_INITIALIZER(EV_LED),
	NAME_INITIALIZER(EV_SND),
	NAME_INITIALIZER(EV_REP),
	NAME_INITIALIZER(EV_FF),
	NAME_INITIALIZER(EV_PWR),
	NAME_INITIALIZER(EV_FF_STATUS)
};

#define KEYEV_RELEASED					0x00
#define KEYEV_PRESSED					0x01
#define KEYEV_REPEATED					0x02
#define KEYEV_MAX					0x03
#define KEYEV_CNT					(KEYEV_MAX + 1)

static char * key_status[KEYEV_MAX + 1] =
{
	[0 ... KEYEV_MAX] = NULL,
	NAME_INITIALIZER(KEYEV_RELEASED),
	NAME_INITIALIZER(KEYEV_PRESSED),
	NAME_INITIALIZER(KEYEV_REPEATED)
};

/*==============================================================================
 * input_event_open()
 * Open input event.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int input_event_open (INPUT_EVENT_ENTITY * const input, const char * const name)
{
	int ret = -1;
	char * path;

	IPEVD_NULL_ARGUMENT(input);

	path = input->path;

	if (name == NULL) {
		printf("\nPlease enter input event path: ");
		ret = scanf("%128s", path);
		IPEVD_NEGATIVE_RETURN(ret, scanf);
	}
	else if ((input->fd >= 0) && (strcmp(path, name) == 0)) {
		ret = input->fd;
		IPEVD("%s already opened fd=%d\n", path, input->fd);
		goto exit;
	}
	else {
		IPEVD_NULL_RETURN(strcpy(path, name), strcpy);
	}

	IPEVD("try to open %s\n", path);
	ret = open(path, O_RDONLY);
	IPEVD_NEGATIVE_RETURN(ret, open);

	input->fd = ret;
	IPEVD("open %s fd=%d\n", path, input->fd);

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_close()
 * Close input event.
 * Return 0 when success. Return -1 when error.
 *----------------------------------------------------------------------------*/
int input_event_close (INPUT_EVENT_ENTITY * const input)
{
	int ret = -1;

	IPEVD_NULL_ARGUMENT(input);
	IPEVD_NEGATIVE_ARGUMENT(input->fd);

	ret = close(input->fd);
	IPEVD_NEGATIVE_RETURN(ret, close);

	printf("close %s fd=%d\n", input->path, input->fd);
	memset(input, 0, sizeof (INPUT_EVENT_ENTITY));
	input->fd = -1;

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_read()
 * Read input event data.
 * Return number of bytes read. Return -1 when error.
 *----------------------------------------------------------------------------*/
int input_event_read (INPUT_EVENT_ENTITY * const input)
{
	int ret = -1;

	IPEVD_NULL_ARGUMENT(input);
	IPEVD_NEGATIVE_ARGUMENT(input->fd);

	ret = read(input->fd, &input->event, sizeof (struct input_event));
	IPEVD_NEGATIVE_RETURN(ret, read);

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

#define BITFIELD uint32_t
static __inline__ int test_bit(int nr, BITFIELD * addr)
{
	BITFIELD mask;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *addr) != 0);
}

/*==============================================================================
 * input_event_device_dump()
 * Dump input event device information.
 *----------------------------------------------------------------------------*/
void input_event_device_dump (INPUT_EVENT_ENTITY * const input)
{
	struct input_id id;
	BITFIELD bits[32];
	int bit;
	int ret;
	int version;
	int fd;
	char * info;

	IPEVD_NULL_ARGUMENT(input);
	IPEVD_NEGATIVE_ARGUMENT(input->fd);

	fd = input->fd;
	info = input->info;

	printf("\nInput device information:\n");
	if (ioctl(fd, EVIOCGVERSION, &version) > 0) {
		printf("EVIOCGVERSION:\t0x%04X\n", version);
	}
	if (ioctl(fd, EVIOCGID, &id) > 0) {
		printf("EVIOCGID:\tbustype=0x%04x vendor=0x%04x product=0x%04x version=0x%04x\n",
		       id.bustype, id.vendor, id.product, id.version);
	}
	if (ioctl(fd, EVIOCGNAME(sizeof (info)), info) > 0) {
		printf("EVIOCGNAME:\t%s\n", info);
	}
	if (ioctl(fd, EVIOCGPHYS(sizeof (info)), info) > 0) {
		printf("EVIOCGPHYS:\t%s\n", info);
	}
	if (ioctl(fd, EVIOCGUNIQ(sizeof (info)), info) > 0) {
		printf("EVIOCGUNIQ:\t%s\n", info);
	}
	if (ioctl(fd, EVIOCGMTSLOTS(sizeof (info)), info) > 0) {
		printf("EVIOCGMTSLOTS:\t%s\n", info);
	}
	ret = ioctl(fd, EVIOCGBIT(0, sizeof (bits)), bits);
	if (ret < 0) {
		goto exit;
	}
	printf("EVIOCGBIT:\n");
	for (bit = 0; (bit < (ret * 8)) && (bit < EV_MAX); bit++) {
		if ((test_bit(bit, bits)) && (event_name[bit] != NULL)) {
			printf("\t\t0x%02X %s\n", bit, event_name[bit]);
		}
	}

exit:
	return;
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * general_event_info()
 * Print general event information.
 * Return length of info. Return -1 when error.
 *----------------------------------------------------------------------------*/
static int general_event_info (INPUT_EVENT_ENTITY * const input)
{
	int ret = -1;
	uint16_t type;
	uint16_t code;
	int32_t value;
	char * info;

	IPEVD_NULL_ARGUMENT(input);
	IPEVD_NEGATIVE_ARGUMENT(input->fd);

	type = input->event.type;
	code = input->event.code;
	value = input->event.value;
	info  = input->info;

	if (event_code_name[type][code] != NULL) {
		ret = sprintf(info, "%s value=%u|0x%X",
		              event_code_name[type][code], value, value);
	}
	else if (event_name[type] != NULL) {
		ret = sprintf(info, "%s code=%u|0x%X value=%u|0x%X",
		              event_name[type], code, code, value, value);
	}
	else {
		ret = sprintf(info, "type=%u|0x%X code=%u|0x%X value=%u|0x%X",
		              type, type, code, code, value, value);
	}
	IPEVD_NEGATIVE_RETURN(ret, sprintf);

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * key_event_info()
 * Print key event information.
 * Return length of info. Return -1 when error.
 *----------------------------------------------------------------------------*/
static int key_event_info (INPUT_EVENT_ENTITY * const input)
{
	int ret = -1;
	uint16_t type;
	int32_t value;

	IPEVD_NULL_ARGUMENT(input);
	IPEVD_NEGATIVE_ARGUMENT(input->fd);

	type = input->event.type;
	value = input->event.value;

	if (type != EV_KEY) {
		IPEVD("type=%u|0x%X\n", type, type);
		goto exit;
	}

	ret = general_event_info(input);
	IPEVD_NEGATIVE_RETURN(ret, general_event_info);

	ret = -1;
	if ((value >= 0) && (value < KEYEV_MAX) && (key_status[value] != NULL)) {
		IPEVD_NULL_RETURN(strcat(input->info, " "), strcat);
		IPEVD_NULL_RETURN(strcat(input->info, key_status[value]), strcat);
	}
	else {
		IPEVD_NULL_RETURN(strcat(input->info, " unknown"), strcat);
	}

	ret = strlen(input->info);

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_dump()
 * Dump input event information.
 *----------------------------------------------------------------------------*/
void input_event_dump (INPUT_EVENT_ENTITY * const input)
{
	int ret = -1;
	uint16_t type;
	struct timeval * time;

	IPEVD_NULL_ARGUMENT(input);
	IPEVD_NEGATIVE_ARGUMENT(input->fd);

	type = input->event.type;
	time = &input->event.time;

	switch (type) {
	case (EV_SYN):
		break;
	case (EV_KEY):
		ret = key_event_info(input);
		break;
	case (EV_REL):
		// break;
	case (EV_ABS):
		// break;
	case (EV_MSC):
		// break;
	case (EV_SW):
		// break;
	case (EV_LED):
		// break;
	case (EV_SND):
		// break;
	case (EV_REP):
		// break;
	case (EV_FF):
		// break;
	case (EV_PWR):
		// break;
	case (EV_FF_STATUS):
		// break;
	default:
		ret = general_event_info(input);
		break;
	}
	if (ret < 0) {
		goto exit;
	}

	printf("\t[%lds:%06ldus]\t{%s}\n", time->tv_sec, time->tv_usec, input->info);

exit:
	return;
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_test()
 * Test input event.
 *----------------------------------------------------------------------------*/
int input_event_test (void)
{
	int ret = -1;
	long timeout = INPUT_EVENT_TEST_TIMEOUT;
	clkspec _clkStart;
	clkspec * clkStart = &_clkStart;
	INPUT_EVENT_ENTITY _input;
	INPUT_EVENT_ENTITY * input = &_input;

	printf("\nInput event test:\n");
	ret = system("ls -lR /dev/input/");

	ret = input_event_open(input, NULL);
	IPEVD_NEGATIVE_RETURN(ret, input_event_open);

	input_event_device_dump(input);

	ret = clock_gettime(CLOCK_ID_DEFAULT, clkStart);
	IPEVD_NEGATIVE_RETURN(ret, clock_gettime);
	printf("\nTest ends in %lds...\n", timeout);

	while (1) {
		ret = input_event_read(input);
		if (ret > 0) {
			input_event_dump(input);
		}

		ret = clock_wait_sec (clkStart, timeout);
		if (ret == 0) {
			break;
		}
	}

exit:
	if (input->fd >= 0) {
		ret = input_event_close(input);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

