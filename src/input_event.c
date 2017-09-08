/*==============================================================================
 * input_event.c
 *----------------------------------------------------------------------------*/

#include "input_event.h"
#include "clock.h"

static char * event_name[EV_MAX + 1] =
{
	[0 ... EV_MAX]	= NULL,
	[EV_SYN]	= "EV_SYN",
	[EV_KEY]	= "EV_KEY",
	[EV_REL]	= "EV_REL",
	[EV_ABS]	= "EV_ABS",
	[EV_MSC]	= "EV_MSC",
	[EV_SW]		= "EV_SW",
	[EV_LED]	= "EV_LED",
	[EV_SND]	= "EV_SND",
	[EV_REP]	= "EV_REP",
	[EV_FF]		= "EV_FF",
	[EV_PWR]	= "EV_PWR",
	[EV_FF_STATUS]	= "EV_FF_STATUS"
};

static char * syn_name[SYN_MAX + 1] =
{
	[0 ... SYN_MAX]	= NULL,
	[SYN_REPORT]	= "SYN_REPORT",
	[SYN_CONFIG]	= "SYN_CONFIG",
	[SYN_MT_REPORT]	= "SYN_MT_REPORT",
	[SYN_DROPPED]	= "SYN_DROPPED"
};

static char * rel_name[REL_MAX + 1] =
{
	[0 ... REL_MAX]	= NULL,
	[REL_X]		= "REL_X",
	[REL_Y]		= "REL_Y",
	[REL_Z]		= "REL_Z",
	[REL_RX]	= "REL_RX",
	[REL_RY]	= "REL_RY",
	[REL_RZ]	= "REL_RZ",
	[REL_HWHEEL]	= "REL_HWHEEL",
	[REL_DIAL]	= "REL_DIAL",
	[REL_WHEEL]	= "REL_WHEEL",
	[REL_MISC]	= "REL_MISC"
};

static char * abs_name[ABS_MAX + 1] =
{
	[0 ... ABS_MAX]		= NULL,
	[ABS_X]			= "ABS_X",
	[ABS_Y]			= "ABS_Y",
	[ABS_Z]			= "ABS_Z",
	[ABS_RX]		= "ABS_RX",
	[ABS_RY]		= "ABS_RY",
	[ABS_RZ]		= "ABS_RZ",
	[ABS_THROTTLE]		= "ABS_THROTTLE",
	[ABS_RUDDER]		= "ABS_RUDDER",
	[ABS_WHEEL]		= "ABS_WHEEL",
	[ABS_GAS]		= "ABS_GAS",
	[ABS_BRAKE]		= "ABS_BRAKE",
	[ABS_HAT0X]		= "ABS_HAT0X",
	[ABS_HAT0Y]		= "ABS_HAT0Y",
	[ABS_HAT1X]		= "ABS_HAT1X",
	[ABS_HAT1Y]		= "ABS_HAT1Y",
	[ABS_HAT2X]		= "ABS_HAT2X",
	[ABS_HAT2Y]		= "ABS_HAT2Y",
	[ABS_HAT3X]		= "ABS_HAT3X",
	[ABS_HAT3Y]		= "ABS_HAT3Y",
	[ABS_PRESSURE]		= "ABS_PRESSURE",
	[ABS_DISTANCE]		= "ABS_DISTANCE",
	[ABS_TILT_X]		= "ABS_TILT_X",
	[ABS_TILT_Y]		= "ABS_TILT_Y",
	[ABS_TOOL_WIDTH]	= "ABS_TOOL_WIDTH",
	[ABS_VOLUME]		= "ABS_VOLUME",
	[ABS_MISC]		= "ABS_MISC",
	[ABS_MT_SLOT]		= "ABS_MT_SLOT",
	[ABS_MT_TOUCH_MAJOR]	= "ABS_MT_TOUCH_MAJOR",
	[ABS_MT_TOUCH_MINOR]	= "ABS_MT_TOUCH_MINOR",
	[ABS_MT_WIDTH_MAJOR]	= "ABS_MT_WIDTH_MAJOR",
	[ABS_MT_WIDTH_MINOR]	= "ABS_MT_WIDTH_MINOR",
	[ABS_MT_ORIENTATION]	= "ABS_MT_ORIENTATION",
	[ABS_MT_POSITION_X]	= "ABS_MT_POSITION_X",
	[ABS_MT_POSITION_Y]	= "ABS_MT_POSITION_Y",
	[ABS_MT_TOOL_TYPE]	= "ABS_MT_TOOL_TYPE",
	[ABS_MT_BLOB_ID]	= "ABS_MT_BLOB_ID",
	[ABS_MT_TRACKING_ID]	= "ABS_MT_TRACKING_ID",
	[ABS_MT_PRESSURE]	= "ABS_MT_PRESSURE",
	[ABS_MT_DISTANCE]	= "ABS_MT_DISTANCE",
	[ABS_MT_TOOL_X]		= "ABS_MT_TOOL_X",
	[ABS_MT_TOOL_Y]		= "ABS_MT_TOOL_Y"
};

static char * sw_name[SW_MAX + 1] =
{
	[0 ... SW_MAX]			= NULL,
	[SW_LID]			= "SW_LID",
	[SW_TABLET_MODE]		= "SW_TABLET_MODE",
	[SW_HEADPHONE_INSERT]		= "SW_HEADPHONE_INSERT",
	[SW_RFKILL_ALL]			= "SW_RFKILL_ALL",
	[SW_RADIO]			= "SW_RADIO",
	[SW_MICROPHONE_INSERT]		= "SW_MICROPHONE_INSERT",
	[SW_DOCK]			= "SW_DOCK",
	[SW_LINEOUT_INSERT]		= "SW_LINEOUT_INSERT",
	[SW_JACK_PHYSICAL_INSERT]	= "SW_JACK_PHYSICAL_INSERT",
	[SW_VIDEOOUT_INSERT]		= "SW_VIDEOOUT_INSERT",
	[SW_CAMERA_LENS_COVER]		= "SW_CAMERA_LENS_COVER",
	[SW_KEYPAD_SLIDE]		= "SW_KEYPAD_SLIDE",
	[SW_FRONT_PROXIMITY]		= "SW_FRONT_PROXIMITY",
	[SW_ROTATE_LOCK]		= "SW_ROTATE_LOCK",
	[SW_LINEIN_INSERT]		= "SW_LINEIN_INSERT",
	[SW_MUTE_DEVICE]		= "SW_MUTE_DEVICE"
};

static char * msc_name[MSC_MAX + 1] =
{
	[ 0 ... MSC_MAX]	= NULL,
	[MSC_SERIAL]		= "MSC_SERIAL",
	[MSC_PULSELED]		= "MSC_PULSELED",
	[MSC_GESTURE]		= "MSC_GESTURE",
	[MSC_RAW]		= "MSC_RAW",
	[MSC_SCAN]		= "MSC_SCAN",
	[MSC_TIMESTAMP]		= "MSC_TIMESTAMP"
};

static char * led_name[LED_MAX + 1] =
{
	[0 ... LED_MAX]	= NULL,
	[LED_NUML]	= "LED_NUML",
	[LED_CAPSL]	= "LED_CAPSL",
	[LED_SCROLLL]	= "LED_SCROLLL",
	[LED_COMPOSE]	= "LED_COMPOSE",
	[LED_KANA]	= "LED_KANA",
	[LED_SLEEP]	= "LED_SLEEP",
	[LED_SUSPEND]	= "LED_SUSPEND",
	[LED_MUTE]	= "LED_MUTE",
	[LED_MISC]	= "LED_MISC",
	[LED_MAIL]	= "LED_MAIL",
	[LED_CHARGING]	= "LED_CHARGING"
};


static char * rep_name[REP_MAX + 1] =
{
	[0 ... REP_MAX]	= NULL,
	[REP_DELAY]	= "REP_DELAY",
	[REP_PERIOD]	= "REP_PERIOD"
};

static char * snd_name[SND_MAX + 1] =
{
	[0 ... SND_MAX]	= NULL,
	[SND_CLICK]	= "SND_CLICK",
	[SND_BELL]	= "SND_BELL",
	[SND_TONE]	= "SND_TONE"
};

static char * key_name[KEY_MAX + 1] =
{
	[0 ... KEY_MAX]			= NULL,
	[KEY_RESERVED]			= "KEY_RESERVED",
	[KEY_ESC]			= "KEY_ESC",
	[KEY_1]				= "KEY_1",
	[KEY_2]				= "KEY_2",
	[KEY_3]				= "KEY_3",
	[KEY_4]				= "KEY_4",
	[KEY_5]				= "KEY_5",
	[KEY_6]				= "KEY_6",
	[KEY_7]				= "KEY_7",
	[KEY_8]				= "KEY_8",
	[KEY_9]				= "KEY_9",
	[KEY_0]				= "KEY_0",
	[KEY_MINUS]			= "KEY_MINUS",
	[KEY_EQUAL]			= "KEY_EQUAL",
	[KEY_BACKSPACE]			= "KEY_BACKSPACE",
	[KEY_TAB]			= "KEY_TAB",
	[KEY_Q]				= "KEY_Q",
	[KEY_W]				= "KEY_W",
	[KEY_E]				= "KEY_E",
	[KEY_R]				= "KEY_R",
	[KEY_T]				= "KEY_T",
	[KEY_Y]				= "KEY_Y",
	[KEY_U]				= "KEY_U",
	[KEY_I]				= "KEY_I",
	[KEY_O]				= "KEY_O",
	[KEY_P]				= "KEY_P",
	[KEY_LEFTBRACE]			= "KEY_LEFTBRACE",
	[KEY_RIGHTBRACE]		= "KEY_RIGHTBRACE",
	[KEY_ENTER]			= "KEY_ENTER",
	[KEY_LEFTCTRL]			= "KEY_LEFTCTRL",
	[KEY_A]				= "KEY_A",
	[KEY_S]				= "KEY_S",
	[KEY_D]				= "KEY_D",
	[KEY_F]				= "KEY_F",
	[KEY_G]				= "KEY_G",
	[KEY_H]				= "KEY_H",
	[KEY_J]				= "KEY_J",
	[KEY_K]				= "KEY_K",
	[KEY_L]				= "KEY_L",
	[KEY_SEMICOLON]			= "KEY_SEMICOLON",
	[KEY_APOSTROPHE]		= "KEY_APOSTROPHE",
	[KEY_GRAVE]			= "KEY_GRAVE",
	[KEY_LEFTSHIFT]			= "KEY_LEFTSHIFT",
	[KEY_BACKSLASH]			= "KEY_BACKSLASH",
	[KEY_Z]				= "KEY_Z",
	[KEY_X]				= "KEY_XX",
	[KEY_C]				= "KEY_C",
	[KEY_V]				= "KEY_V",
	[KEY_B]				= "KEY_B",
	[KEY_N]				= "KEY_N",
	[KEY_M]				= "KEY_M",
	[KEY_COMMA]			= "KEY_COMMA",
	[KEY_DOT]			= "KEY_DOT",
	[KEY_SLASH]			= "KEY_SLASH",
	[KEY_RIGHTSHIFT]		= "KEY_RIGHTSHIFT",
	[KEY_KPASTERISK]		= "KEY_KPASTERISK",
	[KEY_LEFTALT]			= "KEY_LEFTALT",
	[KEY_SPACE]			= "KEY_SPACE",
	[KEY_CAPSLOCK]			= "KEY_CAPSLOCK",
	[KEY_F1]			= "KEY_F1",
	[KEY_F2]			= "KEY_F2",
	[KEY_F3]			= "KEY_F3",
	[KEY_F4]			= "KEY_F4",
	[KEY_F5]			= "KEY_F5",
	[KEY_F6]			= "KEY_F6",
	[KEY_F7]			= "KEY_F7",
	[KEY_F8]			= "KEY_F8",
	[KEY_F9]			= "KEY_F9",
	[KEY_F10]			= "KEY_F10",
	[KEY_NUMLOCK]			= "KEY_NUMLOCK",
	[KEY_SCROLLLOCK]		= "KEY_SCROLLLOCK",
	[KEY_KP7]			= "KEY_KP7",
	[KEY_KP8]			= "KEY_KP8",
	[KEY_KP9]			= "KEY_KP9",
	[KEY_KPMINUS]			= "KEY_KPMINUS",
	[KEY_KP4]			= "KEY_KP4",
	[KEY_KP5]			= "KEY_KP5",
	[KEY_KP6]			= "KEY_KP6",
	[KEY_KPPLUS]			= "KEY_KPPLUS",
	[KEY_KP1]			= "KEY_KP1",
	[KEY_KP2]			= "KEY_KP2",
	[KEY_KP3]			= "KEY_KP3",
	[KEY_KP0]			= "KEY_KP0",
	[KEY_KPDOT]			= "KEY_KPDOT",
	[KEY_ZENKAKUHANKAKU]		= "KEY_ZENKAKUHANKAKU",
	[KEY_102ND]			= "KEY_102ND",
	[KEY_F11]			= "KEY_F11",
	[KEY_F12]			= "KEY_F12",
	[KEY_RO]			= "KEY_RO",
	[KEY_KATAKANA]			= "KEY_KATAKANA",
	[KEY_HIRAGANA]			= "KEY_HIRAGANA",
	[KEY_HENKAN]			= "KEY_HENKAN",
	[KEY_KATAKANAHIRAGANA]		= "KEY_KATAKANAHIRAGANA",
	[KEY_MUHENKAN]			= "KEY_MUHENKAN",
	[KEY_KPJPCOMMA]			= "KEY_KPJPCOMMA",
	[KEY_KPENTER]			= "KEY_KPENTER",
	[KEY_RIGHTCTRL]			= "KEY_RIGHTCTRL",
	[KEY_KPSLASH]			= "KEY_KPSLASH",
	[KEY_SYSRQ]			= "KEY_SYSRQ",
	[KEY_RIGHTALT]			= "KEY_RIGHTALT",
	[KEY_LINEFEED]			= "KEY_LINEFEED",
	[KEY_HOME]			= "KEY_HOME",
	[KEY_UP]			= "KEY_UP",
	[KEY_PAGEUP]			= "KEY_PAGEUP",
	[KEY_LEFT]			= "KEY_LEFT",
	[KEY_RIGHT]			= "KEY_RIGHT",
	[KEY_END]			= "KEY_END",
	[KEY_DOWN]			= "KEY_DOWN",
	[KEY_PAGEDOWN]			= "KEY_PAGEDOWN",
	[KEY_INSERT]			= "KEY_INSERT",
	[KEY_DELETE]			= "KEY_DELETE",
	[KEY_MACRO]			= "KEY_MACRO",
	[KEY_MUTE]			= "KEY_MUTE",
	[KEY_VOLUMEDOWN]		= "KEY_VOLUMEDOWN",
	[KEY_VOLUMEUP]			= "KEY_VOLUMEUP",
	[KEY_POWER]			= "KEY_POWER",
	[KEY_KPEQUAL]			= "KEY_KPEQUAL",
	[KEY_KPPLUSMINUS]		= "KEY_KPPLUSMINUS",
	[KEY_PAUSE]			= "KEY_PAUSE",
	[KEY_SCALE]			= "KEY_SCALE",
	[KEY_KPCOMMA]			= "KEY_KPCOMMA",
	[KEY_HANGUEL]			= "KEY_HANGUEL",
	[KEY_HANJA]			= "KEY_HANJA",
	[KEY_YEN]			= "KEY_YEN",
	[KEY_LEFTMETA]			= "KEY_LEFTMETA",
	[KEY_RIGHTMETA]			= "KEY_RIGHTMETA",
	[KEY_COMPOSE]			= "KEY_COMPOSE",
	[KEY_STOP]			= "KEY_STOP",
	[KEY_AGAIN]			= "KEY_AGAIN",
	[KEY_PROPS]			= "KEY_PROPS",
	[KEY_UNDO]			= "KEY_UNDO",
	[KEY_FRONT]			= "KEY_FRONT",
	[KEY_COPY]			= "KEY_COPY",
	[KEY_OPEN]			= "KEY_OPEN",
	[KEY_PASTE]			= "KEY_PASTE",
	[KEY_FIND]			= "KEY_FIND",
	[KEY_CUT]			= "KEY_CUT",
	[KEY_HELP]			= "KEY_HELP",
	[KEY_MENU]			= "KEY_MENU",
	[KEY_CALC]			= "KEY_CALC",
	[KEY_SETUP]			= "KEY_SETUP",
	[KEY_SLEEP]			= "KEY_SLEEP",
	[KEY_WAKEUP]			= "KEY_WAKEUP",
	[KEY_FILE]			= "KEY_FILE",
	[KEY_SENDFILE]			= "KEY_SENDFILE",
	[KEY_DELETEFILE]		= "KEY_DELETEFILE",
	[KEY_XFER]			= "KEY_XFER",
	[KEY_PROG1]			= "KEY_PROG1",
	[KEY_PROG2]			= "KEY_PROG2",
	[KEY_WWW]			= "KEY_WWW",
	[KEY_MSDOS]			= "KEY_MSDOS",
	[KEY_COFFEE]			= "KEY_COFFEE",
	[KEY_SCREENLOCK]		= "KEY_SCREENLOCK",
	[KEY_ROTATE_DISPLAY]		= "KEY_ROTATE_DISPLAY",
	[KEY_DIRECTION]			= "KEY_DIRECTION",
	[KEY_CYCLEWINDOWS]		= "KEY_CYCLEWINDOWS",
	[KEY_MAIL]			= "KEY_MAIL",
	[KEY_BOOKMARKS]			= "KEY_BOOKMARKS",
	[KEY_COMPUTER]			= "KEY_COMPUTER",
	[KEY_BACK]			= "KEY_BACK",
	[KEY_FORWARD]			= "KEY_FORWARD",
	[KEY_CLOSECD]			= "KEY_CLOSECD",
	[KEY_EJECTCD]			= "KEY_EJECTCD",
	[KEY_EJECTCLOSECD]		= "KEY_EJECTCLOSECD",
	[KEY_NEXTSONG]			= "KEY_NEXTSONG",
	[KEY_PLAYPAUSE]			= "KEY_PLAYPAUSE",
	[KEY_PREVIOUSSONG]		= "KEY_PREVIOUSSONG",
	[KEY_STOPCD]			= "KEY_STOPCD",
	[KEY_RECORD]			= "KEY_RECORD",
	[KEY_REWIND]			= "KEY_REWIND",
	[KEY_PHONE]			= "KEY_PHONE",
	[KEY_ISO]			= "KEY_ISO",
	[KEY_CONFIG]			= "KEY_CONFIG",
	[KEY_HOMEPAGE]			= "KEY_HOMEPAGE",
	[KEY_REFRESH]			= "KEY_REFRESH",
	[KEY_EXIT]			= "KEY_EXIT",
	[KEY_MOVE]			= "KEY_MOVE",
	[KEY_EDIT]			= "KEY_EDIT",
	[KEY_SCROLLUP]			= "KEY_SCROLLUP",
	[KEY_SCROLLDOWN]		= "KEY_SCROLLDOWN",
	[KEY_KPLEFTPAREN]		= "KEY_KPLEFTPAREN",
	[KEY_KPRIGHTPAREN]		= "KEY_KPRIGHTPAREN",
	[KEY_NEW]			= "KEY_NEW",
	[KEY_REDO]			= "KEY_REDO",
	[KEY_F13]			= "KEY_F13",
	[KEY_F14]			= "KEY_F14",
	[KEY_F15]			= "KEY_F15",
	[KEY_F16]			= "KEY_F16",
	[KEY_F17]			= "KEY_F17",
	[KEY_F18]			= "KEY_F18",
	[KEY_F19]			= "KEY_F19",
	[KEY_F20]			= "KEY_F20",
	[KEY_F21]			= "KEY_F21",
	[KEY_F22]			= "KEY_F22",
	[KEY_F23]			= "KEY_F23",
	[KEY_F24]			= "KEY_F24",
	[KEY_PLAYCD]			= "KEY_PLAYCD",
	[KEY_PAUSECD]			= "KEY_PAUSECD",
	[KEY_PROG3]			= "KEY_PROG3",
	[KEY_PROG4]			= "KEY_PROG4",
	[KEY_DASHBOARD]			= "KEY_DASHBOARD",
	[KEY_SUSPEND]			= "KEY_SUSPEND",
	[KEY_CLOSE]			= "KEY_CLOSE",
	[KEY_PLAY]			= "KEY_PLAY",
	[KEY_FASTFORWARD]		= "KEY_FASTFORWARD",
	[KEY_BASSBOOST]			= "KEY_BASSBOOST",
	[KEY_PRINT]			= "KEY_PRINT",
	[KEY_HP]			= "KEY_HP",
	[KEY_CAMERA]			= "KEY_CAMERA",
	[KEY_SOUND]			= "KEY_SOUND",
	[KEY_QUESTION]			= "KEY_QUESTION",
	[KEY_EMAIL]			= "KEY_EMAIL",
	[KEY_CHAT]			= "KEY_CHAT",
	[KEY_SEARCH]			= "KEY_SEARCH",
	[KEY_CONNECT]			= "KEY_CONNECT",
	[KEY_FINANCE]			= "KEY_FINANCE",
	[KEY_SPORT]			= "KEY_SPORT",
	[KEY_SHOP]			= "KEY_SHOP",
	[KEY_ALTERASE]			= "KEY_ALTERASE",
	[KEY_CANCEL]			= "KEY_CANCEL",
	[KEY_BRIGHTNESSDOWN]		= "KEY_BRIGHTNESSDOWN",
	[KEY_BRIGHTNESSUP]		= "KEY_BRIGHTNESSUP",
	[KEY_MEDIA]			= "KEY_MEDIA",
	[KEY_SWITCHVIDEOMODE]		= "KEY_SWITCHVIDEOMODE",
	[KEY_KBDILLUMTOGGLE]		= "KEY_KBDILLUMTOGGLE",
	[KEY_KBDILLUMDOWN]		= "KEY_KBDILLUMDOWN",
	[KEY_KBDILLUMUP]		= "KEY_KBDILLUMUP",
	[KEY_SEND]			= "KEY_SEND",
	[KEY_REPLY]			= "KEY_REPLY",
	[KEY_FORWARDMAIL]		= "KEY_FORWARDMAIL",
	[KEY_SAVE]			= "KEY_SAVE",
	[KEY_DOCUMENTS]			= "KEY_DOCUMENTS",
	[KEY_BATTERY]			= "KEY_BATTERY",
	[KEY_BLUETOOTH]			= "KEY_BLUETOOTH",
	[KEY_WLAN]			= "KEY_WLAN",
	[KEY_UWB]			= "KEY_UWB",
	[KEY_UNKNOWN]			= "KEY_UNKNOWN",
	[KEY_VIDEO_NEXT]		= "KEY_VIDEO_NEXT",
	[KEY_VIDEO_PREV]		= "KEY_VIDEO_PREV",
	[KEY_BRIGHTNESS_CYCLE]		= "KEY_BRIGHTNESS_CYCLE",
	[KEY_BRIGHTNESS_AUTO]		= "KEY_BRIGHTNESS_AUTO",
	[KEY_BRIGHTNESS_ZERO]		= "KEY_BRIGHTNESS_ZERO",
	[KEY_DISPLAY_OFF]		= "KEY_DISPLAY_OFF",
	[KEY_WWAN]			= "KEY_WWAN",
	[KEY_WIMAX]			= "KEY_WIMAX",
	[KEY_RFKILL]			= "KEY_RFKILL",
	[KEY_MICMUTE]			= "KEY_MICMUTE",
	[BTN_MISC]			= "BTN_MISC",
	[BTN_0]				= "BTN_0",
	[BTN_1]				= "BTN_1",
	[BTN_2]				= "BTN_2",
	[BTN_3]				= "BTN_3",
	[BTN_4]				= "BTN_4",
	[BTN_5]				= "BTN_5",
	[BTN_6]				= "BTN_6",
	[BTN_7]				= "BTN_7",
	[BTN_8]				= "BTN_8",
	[BTN_9]				= "BTN_9",
	[BTN_LEFT]			= "BTN_LEFT",
	[BTN_RIGHT]			= "BTN_RIGHT",
	[BTN_MIDDLE]			= "BTN_MIDDLE",
	[BTN_SIDE]			= "BTN_SIDE",
	[BTN_EXTRA]			= "BTN_EXTRA",
	[BTN_FORWARD]			= "BTN_FORWARD",
	[BTN_BACK]			= "BTN_BACK",
	[BTN_TASK]			= "BTN_TASK",
	[BTN_JOYSTICK]			= "BTN_JOYSTICK",
	[BTN_TRIGGER]			= "BTN_TRIGGER",
	[BTN_THUMB]			= "BTN_THUMB",
	[BTN_THUMB2]			= "BTN_THUMB2",
	[BTN_TOP]			= "BTN_TOP",
	[BTN_TOP2]			= "BTN_TOP2",
	[BTN_PINKIE]			= "BTN_PINKIE",
	[BTN_BASE]			= "BTN_BASE",
	[BTN_BASE2]			= "BTN_BASE2",
	[BTN_BASE3]			= "BTN_BASE3",
	[BTN_BASE4]			= "BTN_BASE4",
	[BTN_BASE5]			= "BTN_BASE5",
	[BTN_BASE6]			= "BTN_BASE6",
	[BTN_DEAD]			= "BTN_DEAD",
	[BTN_GAMEPAD]			= "BTN_GAMEPAD",
	[BTN_SOUTH]			= "BTN_SOUTH",
	[BTN_A]				= "BTN_A",
	[BTN_EAST]			= "BTN_EAST",
	[BTN_B]				= "BTN_B",
	[BTN_C]				= "BTN_C",
	[BTN_NORTH]			= "BTN_NORTH",
	[BTN_X]				= "BTN_X",
	[BTN_WEST]			= "BTN_WEST",
	[BTN_Y]				= "BTN_Y",
	[BTN_Z]				= "BTN_Z",
	[BTN_TL]			= "BTN_TL",
	[BTN_TR]			= "BTN_TR",
	[BTN_TL2]			= "BTN_TL2",
	[BTN_TR2]			= "BTN_TR2",
	[BTN_SELECT]			= "BTN_SELECT",
	[BTN_START]			= "BTN_START",
	[BTN_MODE]			= "BTN_MODE",
	[BTN_THUMBL]			= "BTN_THUMBL",
	[BTN_THUMBR]			= "BTN_THUMBR",
	[BTN_DIGI]			= "BTN_DIGI",
	[BTN_TOOL_PEN]			= "BTN_TOOL_PEN",
	[BTN_TOOL_RUBBER]		= "BTN_TOOL_RUBBER",
	[BTN_TOOL_BRUSH]		= "BTN_TOOL_BRUSH",
	[BTN_TOOL_PENCIL]		= "BTN_TOOL_PENCIL",
	[BTN_TOOL_AIRBRUSH]		= "BTN_TOOL_AIRBRUSH",
	[BTN_TOOL_FINGER]		= "BTN_TOOL_FINGER",
	[BTN_TOOL_MOUSE]		= "BTN_TOOL_MOUSE",
	[BTN_TOOL_LENS]			= "BTN_TOOL_LENS",
	[BTN_TOOL_QUINTTAP]		= "BTN_TOOL_QUINTTAP",
	[BTN_TOUCH]			= "BTN_TOUCH",
	[BTN_STYLUS]			= "BTN_STYLUS",
	[BTN_STYLUS2]			= "BTN_STYLUS2",
	[BTN_TOOL_DOUBLETAP]		= "BTN_TOOL_DOUBLETAP",
	[BTN_TOOL_TRIPLETAP]		= "BTN_TOOL_TRIPLETAP",
	[BTN_TOOL_QUADTAP]		= "BTN_TOOL_QUADTAP",
	[BTN_WHEEL]			= "BTN_WHEEL",
	[BTN_GEAR_DOWN]			= "BTN_GEAR_DOWN",
	[BTN_GEAR_UP]			= "BTN_GEAR_UP",
	[KEY_OK]			= "KEY_OK",
	[KEY_SELECT]			= "KEY_SELECT",
	[KEY_GOTO]			= "KEY_GOTO",
	[KEY_CLEAR]			= "KEY_CLEAR",
	[KEY_POWER2]			= "KEY_POWER2",
	[KEY_OPTION]			= "KEY_OPTION",
	[KEY_INFO]			= "KEY_INFO",
	[KEY_TIME]			= "KEY_TIME",
	[KEY_VENDOR]			= "KEY_VENDOR",
	[KEY_ARCHIVE]			= "KEY_ARCHIVE",
	[KEY_PROGRAM]			= "KEY_PROGRAM",
	[KEY_CHANNEL]			= "KEY_CHANNEL",
	[KEY_FAVORITES]			= "KEY_FAVORITES",
	[KEY_EPG]			= "KEY_EPG",
	[KEY_PVR]			= "KEY_PVR",
	[KEY_MHP]			= "KEY_MHP",
	[KEY_LANGUAGE]			= "KEY_LANGUAGE",
	[KEY_TITLE]			= "KEY_TITLE",
	[KEY_SUBTITLE]			= "KEY_SUBTITLE",
	[KEY_ANGLE]			= "KEY_ANGLE",
	[KEY_ZOOM]			= "KEY_ZOOM",
	[KEY_MODE]			= "KEY_MODE",
	[KEY_KEYBOARD]			= "KEY_KEYBOARD",
	[KEY_SCREEN]			= "KEY_SCREEN",
	[KEY_PC]			= "KEY_PC",
	[KEY_TV]			= "KEY_TV",
	[KEY_TV2]			= "KEY_TV2",
	[KEY_VCR]			= "KEY_VCR",
	[KEY_VCR2]			= "KEY_VCR2",
	[KEY_SAT]			= "KEY_SAT",
	[KEY_SAT2]			= "KEY_SAT2",
	[KEY_CD]			= "KEY_CD",
	[KEY_TAPE]			= "KEY_TAPE",
	[KEY_RADIO]			= "KEY_RADIO",
	[KEY_TUNER]			= "KEY_TUNER",
	[KEY_PLAYER]			= "KEY_PLAYER",
	[KEY_TEXT]			= "KEY_TEXT",
	[KEY_DVD]			= "KEY_DVD",
	[KEY_AUX]			= "KEY_AUX",
	[KEY_MP3]			= "KEY_MP3",
	[KEY_AUDIO]			= "KEY_AUDIO",
	[KEY_VIDEO]			= "KEY_VIDEO",
	[KEY_DIRECTORY]			= "KEY_DIRECTORY",
	[KEY_LIST]			= "KEY_LIST",
	[KEY_MEMO]			= "KEY_MEMO",
	[KEY_CALENDAR]			= "KEY_CALENDAR",
	[KEY_RED]			= "KEY_RED",
	[KEY_GREEN]			= "KEY_GREEN",
	[KEY_YELLOW]			= "KEY_YELLOW",
	[KEY_BLUE]			= "KEY_BLUE",
	[KEY_CHANNELUP]			= "KEY_CHANNELUP",
	[KEY_CHANNELDOWN]		= "KEY_CHANNELDOWN",
	[KEY_FIRST]			= "KEY_FIRST",
	[KEY_LAST]			= "KEY_LAST",
	[KEY_AB]			= "KEY_AB",
	[KEY_NEXT]			= "KEY_NEXT",
	[KEY_RESTART]			= "KEY_RESTART",
	[KEY_SLOW]			= "KEY_SLOW",
	[KEY_SHUFFLE]			= "KEY_SHUFFLE",
	[KEY_BREAK]			= "KEY_BREAK",
	[KEY_PREVIOUS]			= "KEY_PREVIOUS",
	[KEY_DIGITS]			= "KEY_DIGITS",
	[KEY_TEEN]			= "KEY_TEEN",
	[KEY_TWEN]			= "KEY_TWEN",
	[KEY_VIDEOPHONE]		= "KEY_VIDEOPHONE",
	[KEY_GAMES]			= "KEY_GAMES",
	[KEY_ZOOMIN]			= "KEY_ZOOMIN",
	[KEY_ZOOMOUT]			= "KEY_ZOOMOUT",
	[KEY_ZOOMRESET]			= "KEY_ZOOMRESET",
	[KEY_WORDPROCESSOR]		= "KEY_WORDPROCESSOR",
	[KEY_EDITOR]			= "KEY_EDITOR",
	[KEY_SPREADSHEET]		= "KEY_SPREADSHEET",
	[KEY_GRAPHICSEDITOR]		= "KEY_GRAPHICSEDITOR",
	[KEY_PRESENTATION]		= "KEY_PRESENTATION",
	[KEY_DATABASE]			= "KEY_DATABASE",
	[KEY_NEWS]			= "KEY_NEWS",
	[KEY_VOICEMAIL]			= "KEY_VOICEMAIL",
	[KEY_ADDRESSBOOK]		= "KEY_ADDRESSBOOK",
	[KEY_MESSENGER]			= "KEY_MESSENGER",
	[KEY_DISPLAYTOGGLE]		= "KEY_DISPLAYTOGGLE",
	[KEY_BRIGHTNESS_TOGGLE]		= "KEY_BRIGHTNESS_TOGGLE",
	[KEY_SPELLCHECK]		= "KEY_SPELLCHECK",
	[KEY_LOGOFF]			= "KEY_LOGOFF",
	[KEY_DOLLAR]			= "KEY_DOLLAR",
	[KEY_EURO]			= "KEY_EURO",
	[KEY_FRAMEBACK]			= "KEY_FRAMEBACK",
	[KEY_FRAMEFORWARD]		= "KEY_FRAMEFORWARD",
	[KEY_CONTEXT_MENU]		= "KEY_CONTEXT_MENU",
	[KEY_MEDIA_REPEAT]		= "KEY_MEDIA_REPEAT",
	[KEY_10CHANNELSUP]		= "KEY_10CHANNELSUP",
	[KEY_10CHANNELSDOWN]		= "KEY_10CHANNELSDOWN",
	[KEY_IMAGES]			= "KEY_IMAGES",
	[KEY_DEL_EOL]			= "KEY_DEL_EOL",
	[KEY_DEL_EOS]			= "KEY_DEL_EOS",
	[KEY_INS_LINE]			= "KEY_INS_LINE",
	[KEY_DEL_LINE]			= "KEY_DEL_LINE",
	[KEY_FN]			= "KEY_FN",
	[KEY_FN_ESC]			= "KEY_FN_ESC",
	[KEY_FN_F1]			= "KEY_FN_F1",
	[KEY_FN_F2]			= "KEY_FN_F2",
	[KEY_FN_F3]			= "KEY_FN_F3",
	[KEY_FN_F4]			= "KEY_FN_F4",
	[KEY_FN_F5]			= "KEY_FN_F5",
	[KEY_FN_F6]			= "KEY_FN_F6",
	[KEY_FN_F7]			= "KEY_FN_F7",
	[KEY_FN_F8]			= "KEY_FN_F8",
	[KEY_FN_F9]			= "KEY_FN_F9",
	[KEY_FN_F10]			= "KEY_FN_F10",
	[KEY_FN_F11]			= "KEY_FN_F11",
	[KEY_FN_F12]			= "KEY_FN_F12",
	[KEY_FN_1]			= "KEY_FN_1",
	[KEY_FN_2]			= "KEY_FN_2",
	[KEY_FN_D]			= "KEY_FN_D",
	[KEY_FN_E]			= "KEY_FN_E",
	[KEY_FN_F]			= "KEY_FN_F",
	[KEY_FN_S]			= "KEY_FN_S",
	[KEY_FN_B]			= "KEY_FN_B",
	[KEY_BRL_DOT1]			= "KEY_BRL_DOT1",
	[KEY_BRL_DOT2]			= "KEY_BRL_DOT2",
	[KEY_BRL_DOT3]			= "KEY_BRL_DOT3",
	[KEY_BRL_DOT4]			= "KEY_BRL_DOT4",
	[KEY_BRL_DOT5]			= "KEY_BRL_DOT5",
	[KEY_BRL_DOT6]			= "KEY_BRL_DOT6",
	[KEY_BRL_DOT7]			= "KEY_BRL_DOT7",
	[KEY_BRL_DOT8]			= "KEY_BRL_DOT8",
	[KEY_BRL_DOT9]			= "KEY_BRL_DOT9",
	[KEY_BRL_DOT10]			= "KEY_BRL_DOT10",
	[KEY_NUMERIC_0]			= "KEY_NUMERIC_0",
	[KEY_NUMERIC_1]			= "KEY_NUMERIC_1",
	[KEY_NUMERIC_2]			= "KEY_NUMERIC_2",
	[KEY_NUMERIC_3]			= "KEY_NUMERIC_3",
	[KEY_NUMERIC_4]			= "KEY_NUMERIC_4",
	[KEY_NUMERIC_5]			= "KEY_NUMERIC_5",
	[KEY_NUMERIC_6]			= "KEY_NUMERIC_6",
	[KEY_NUMERIC_7]			= "KEY_NUMERIC_7",
	[KEY_NUMERIC_8]			= "KEY_NUMERIC_8",
	[KEY_NUMERIC_9]			= "KEY_NUMERIC_9",
	[KEY_NUMERIC_STAR]		= "KEY_NUMERIC_STAR",
	[KEY_NUMERIC_POUND]		= "KEY_NUMERIC_POUND",
	[KEY_NUMERIC_A]			= "KEY_NUMERIC_A",
	[KEY_NUMERIC_B]			= "KEY_NUMERIC_B",
	[KEY_NUMERIC_C]			= "KEY_NUMERIC_C",
	[KEY_NUMERIC_D]			= "KEY_NUMERIC_D",
	[KEY_CAMERA_FOCUS]		= "KEY_CAMERA_FOCUS",
	[KEY_WPS_BUTTON]		= "KEY_WPS_BUTTON",
	[KEY_TOUCHPAD_TOGGLE]		= "KEY_TOUCHPAD_TOGGLE",
	[KEY_TOUCHPAD_ON]		= "KEY_TOUCHPAD_ON",
	[KEY_TOUCHPAD_OFF]		= "KEY_TOUCHPAD_OFF",
	[KEY_CAMERA_ZOOMIN]		= "KEY_CAMERA_ZOOMIN",
	[KEY_CAMERA_ZOOMOUT]		= "KEY_CAMERA_ZOOMOUT",
	[KEY_CAMERA_UP]			= "KEY_CAMERA_UP",
	[KEY_CAMERA_DOWN]		= "KEY_CAMERA_DOWN",
	[KEY_CAMERA_LEFT]		= "KEY_CAMERA_LEFT",
	[KEY_CAMERA_RIGHT]		= "KEY_CAMERA_RIGHT",
	[KEY_ATTENDANT_ON]		= "KEY_ATTENDANT_ON",
	[KEY_ATTENDANT_OFF]		= "KEY_ATTENDANT_OFF",
	[KEY_ATTENDANT_TOGGLE]		= "KEY_ATTENDANT_TOGGLE",
	[KEY_LIGHTS_TOGGLE]		= "KEY_LIGHTS_TOGGLE",
	[BTN_DPAD_UP]			= "BTN_DPAD_UP",
	[BTN_DPAD_DOWN]			= "BTN_DPAD_DOWN",
	[BTN_DPAD_LEFT]			= "BTN_DPAD_LEFT",
	[BTN_DPAD_RIGHT]		= "BTN_DPAD_RIGHT",
	[KEY_ALS_TOGGLE]		= "KEY_ALS_TOGGLE",
	[KEY_BUTTONCONFIG]		= "KEY_BUTTONCONFIG",
	[KEY_TASKMANAGER]		= "KEY_TASKMANAGER",
	[KEY_JOURNAL]			= "KEY_JOURNAL",
	[KEY_CONTROLPANEL]		= "KEY_CONTROLPANEL",
	[KEY_APPSELECT]			= "KEY_APPSELECT",
	[KEY_SCREENSAVER]		= "KEY_SCREENSAVER",
	[KEY_VOICECOMMAND]		= "KEY_VOICECOMMAND",
	// [KEY_ASSISTANT]		= "KEY_ASSISTANT",
	[KEY_BRIGHTNESS_MIN]		= "KEY_BRIGHTNESS_MIN",
	[KEY_BRIGHTNESS_MAX]		= "KEY_BRIGHTNESS_MAX",
	[KEY_KBDINPUTASSIST_PREV]	= "KEY_KBDINPUTASSIST_PREV",
	[KEY_KBDINPUTASSIST_NEXT]	= "KEY_KBDINPUTASSIST_NEXT",
	[KEY_KBDINPUTASSIST_PREVGROUP]	= "KEY_KBDINPUTASSIST_PREVGROUP",
	[KEY_KBDINPUTASSIST_NEXTGROUP]	= "KEY_KBDINPUTASSIST_NEXTGROUP",
	[KEY_KBDINPUTASSIST_ACCEPT]	= "KEY_KBDINPUTASSIST_ACCEPT",
	[KEY_KBDINPUTASSIST_CANCEL]	= "KEY_KBDINPUTASSIST_CANCEL",
	// [KEY_RIGHT_UP]		= "KEY_RIGHT_UP",
	// [KEY_RIGHT_DOWN]		= "KEY_RIGHT_DOWN",
	// [KEY_LEFT_UP]		= "KEY_LEFT_UP",
	// [KEY_LEFT_DOWN]		= "KEY_LEFT_DOWN",
	// [KEY_ROOT_MENU]		= "KEY_ROOT_MENU",
	// [KEY_MEDIA_TOP_MENU]		= "KEY_MEDIA_TOP_MENU",
	// [KEY_NUMERIC_11]		= "KEY_NUMERIC_11",
	// [KEY_NUMERIC_12]		= "KEY_NUMERIC_12",
	// [KEY_AUDIO_DESC]		= "KEY_AUDIO_DESC",
	// [KEY_3D_MODE]		= "KEY_3D_MODE",
	// [KEY_NEXT_FAVORITE]		= "KEY_NEXT_FAVORITE",
	// [KEY_STOP_RECORD]		= "KEY_STOP_RECORD",
	// [KEY_PAUSE_RECORD]		= "KEY_PAUSE_RECORD",
	// [KEY_VOD]			= "KEY_VOD",
	// [KEY_UNMUTE]			= "KEY_UNMUTE",
	// [KEY_FASTREVERSE]		= "KEY_FASTREVERSE",
	// [KEY_SLOWREVERSE]		= "KEY_SLOWREVERSE",
	// [KEY_DATA]			= "KEY_DATA",
	// [KEY_ONSCREEN_KEYBOARD]	= "KEY_ONSCREEN_KEYBOARD",
	// [BTN_TRIGGER_HAPPY]		= "BTN_TRIGGER_HAPPY",
	// [BTN_TRIGGER_HAPPY_1]	= "BTN_TRIGGER_HAPPY_1",
	// [BTN_TRIGGER_HAPPY_2]	= "BTN_TRIGGER_HAPPY_2",
	// [BTN_TRIGGER_HAPPY_3]	= "BTN_TRIGGER_HAPPY_3",
	// [BTN_TRIGGER_HAPPY_4]	= "BTN_TRIGGER_HAPPY_4",
	// [BTN_TRIGGER_HAPPY_5]	= "BTN_TRIGGER_HAPPY_5",
	// [BTN_TRIGGER_HAPPY_6]	= "BTN_TRIGGER_HAPPY_6",
	// [BTN_TRIGGER_HAPPY_7]	= "BTN_TRIGGER_HAPPY_7",
	// [BTN_TRIGGER_HAPPY_8]	= "BTN_TRIGGER_HAPPY_8",
	// [BTN_TRIGGER_HAPPY_9]	= "BTN_TRIGGER_HAPPY_9",
	// [BTN_TRIGGER_HAPPY_10]	= "BTN_TRIGGER_HAPPY_10",
	// [BTN_TRIGGER_HAPPY_11]	= "BTN_TRIGGER_HAPPY_11",
	// [BTN_TRIGGER_HAPPY_12]	= "BTN_TRIGGER_HAPPY_12",
	// [BTN_TRIGGER_HAPPY_13]	= "BTN_TRIGGER_HAPPY_13",
	// [BTN_TRIGGER_HAPPY_14]	= "BTN_TRIGGER_HAPPY_14",
	// [BTN_TRIGGER_HAPPY_15]	= "BTN_TRIGGER_HAPPY_15",
	// [BTN_TRIGGER_HAPPY_16]	= "BTN_TRIGGER_HAPPY_16",
	// [BTN_TRIGGER_HAPPY_17]	= "BTN_TRIGGER_HAPPY_17",
	// [BTN_TRIGGER_HAPPY_18]	= "BTN_TRIGGER_HAPPY_18",
	// [BTN_TRIGGER_HAPPY_19]	= "BTN_TRIGGER_HAPPY_19",
	// [BTN_TRIGGER_HAPPY_20]	= "BTN_TRIGGER_HAPPY_20",
	// [BTN_TRIGGER_HAPPY_21]	= "BTN_TRIGGER_HAPPY_21",
	// [BTN_TRIGGER_HAPPY_22]	= "BTN_TRIGGER_HAPPY_22",
	// [BTN_TRIGGER_HAPPY_23]	= "BTN_TRIGGER_HAPPY_23",
	// [BTN_TRIGGER_HAPPY_24]	= "BTN_TRIGGER_HAPPY_24",
	// [BTN_TRIGGER_HAPPY_25]	= "BTN_TRIGGER_HAPPY_25",
	// [BTN_TRIGGER_HAPPY_26]	= "BTN_TRIGGER_HAPPY_26",
	// [BTN_TRIGGER_HAPPY_27]	= "BTN_TRIGGER_HAPPY_27",
	// [BTN_TRIGGER_HAPPY_28]	= "BTN_TRIGGER_HAPPY_28",
	// [BTN_TRIGGER_HAPPY_29]	= "BTN_TRIGGER_HAPPY_29",
	// [BTN_TRIGGER_HAPPY_30]	= "BTN_TRIGGER_HAPPY_30",
	// [BTN_TRIGGER_HAPPY_31]	= "BTN_TRIGGER_HAPPY_31",
	// [BTN_TRIGGER_HAPPY_32]	= "BTN_TRIGGER_HAPPY_32",
	// [BTN_TRIGGER_HAPPY_33]	= "BTN_TRIGGER_HAPPY_33",
	// [BTN_TRIGGER_HAPPY_34]	= "BTN_TRIGGER_HAPPY_34",
	// [BTN_TRIGGER_HAPPY_35]	= "BTN_TRIGGER_HAPPY_35",
	// [BTN_TRIGGER_HAPPY_36]	= "BTN_TRIGGER_HAPPY_36",
	// [BTN_TRIGGER_HAPPY_37]	= "BTN_TRIGGER_HAPPY_37",
	// [BTN_TRIGGER_HAPPY_38]	= "BTN_TRIGGER_HAPPY_38",
	// [BTN_TRIGGER_HAPPY_39]	= "BTN_TRIGGER_HAPPY_39",
	// [BTN_TRIGGER_HAPPY_40]	= "BTN_TRIGGER_HAPPY_40",
	[KEY_MIN_INTERESTING]		= "KEY_MIN_INTERESTING"
};

char ** event_code_name[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = syn_name,
	[EV_KEY] = key_name,
	[EV_REL] = rel_name,
	[EV_ABS] = abs_name,
	[EV_MSC] = msc_name,
	[EV_LED] = led_name,
	[EV_SND] = snd_name,
	[EV_REP] = rep_name
};

/*==============================================================================
 * input_event_open()
 * Open input event.
 *----------------------------------------------------------------------------*/
int input_event_open (const char * const name, int * const fd)
{
	int ret = -1;
	char path[INPUT_EVENT_PATH_LENGTH];

	if (name != NULL) {
		if (strcpy(path, name) != path) {
			INPTD("strcpy error\n");
			goto exit;
		}
	}
	else {
		printf("\nPlease enter input event path: ");
		ret = scanf("%128s", path);
		if (ret <= 0) {
			INPTD("scanf error ret=%d\n", ret);
			goto exit;
		}
	}

	INPTD("try to open: %s\n", path);
	ret = open(path, O_RDONLY);
	if (ret < 0) {
		INPTD("open error ret=%d\n", ret);
	}
	else {
		*fd = ret;
		printf("open %s fd=%d\n", path, *fd);
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_close()
 * Close input event.
 *----------------------------------------------------------------------------*/
int input_event_close (const int fd)
{
	int ret;

	ret = close(fd);
	if (ret < 0) {
		INPTD("close error ret=%d\n", ret);
	}
	else {
		INPTD("close success fd=%d\n", fd);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_read()
 * Return number of bytes read. Return -1 when error.
 *----------------------------------------------------------------------------*/
int input_event_read (const int fd, inputevent * const input)
{
	int ret;

	ret = read(fd, input, sizeof (inputevent));
	if (ret < 0) {
		INPTD("read error ret=%d\n", ret);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_device_dump()
 * Dump input event device information.
 *----------------------------------------------------------------------------*/
void input_event_device_dump (const int fd)
{
	struct input_id id;
	BITFIELD bits[32];
	char buf[INPUT_EVENT_DUMP_LENGTH];
	int bit;
	int ret;
	int version;

	printf("\ndevice information:\n");

	if (ioctl(fd, EVIOCGVERSION, &version) >= 0) {
		printf("EVIOCGVERSION:\t0x%04X\n", version);
	}

	if (ioctl(fd, EVIOCGID, &id) >= 0) {
		printf("EVIOCGID:\tbustype=0x%04x vendor=0x%04x product=0x%04x version=0x%04x\n",
		       id.bustype, id.vendor, id.product, id.version);
	}

	if (ioctl(fd, EVIOCGNAME(sizeof (buf)), buf) >= 0) {
		printf("EVIOCGNAME:\t%s\n", buf);
	}

	if (ioctl(fd, EVIOCGPHYS(sizeof (buf)), buf) >= 0) {
		printf("EVIOCGPHYS:\t%s\n", buf);
	}

	ret = ioctl(fd, EVIOCGBIT(0, sizeof (bits)), bits);
	if (ret >= 0) {
		printf("EVIOCGBIT:\n");
		for (bit = 0; (bit < (ret * 8)) && (bit < EV_MAX); bit++) {
			if (test_bit(bit, bits)) {
				printf("\t\t0x%02X %s\n", bit, event_name[bit]);
			}
		}
	}
	printf("\n");
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * general_event_info()
 * Dump general event information.
 *----------------------------------------------------------------------------*/
static int general_event_info (const inputevent * const input, char * const info)
{
	int ret = -1;

	if ((input == NULL) || (info == NULL)) {
		INPTD("parameter error\n");
		goto exit;
	}

	if (event_code_name[input->type][input->code] != NULL) {
		sprintf(info, "%s value=%u|0x%X",
		        event_code_name[input->type][input->code],
		        input->value, input->value
		        );
	}
	else if (event_name[input->type] != NULL) {
		sprintf(info, "%s code=%u|0x%X value=%u|0x%X",
		        event_name[input->type],
		        input->code, input->code,
		        input->value, input->value
		        );
	}
	else {
		sprintf(info, "type=%u|0x%X code=%u|0x%X value=%u|0x%X",
		        input->type, input->type,
		        input->code, input->code,
		        input->value, input->value
		        );
	}

	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * key_event_info()
 * Dump key event information.
 *----------------------------------------------------------------------------*/
static int key_event_info (const inputevent * const input, char * const info)
{
	int ret = -1;
	char * buf;

	if ((input == NULL) || (info == NULL) || (input->type != EV_KEY)) {
		INPTD("parameter error\n");
		goto exit;
	}

	ret = general_event_info(input, info);
	if (ret < 0) {
		goto exit;
	}

	switch (input->value) {
	case (INPUT_KEY_RELEASED):
		buf = " released";
		break;
	case (INPUT_KEY_PRESSED):
		buf = " pressed";
		break;
	case (INPUT_KEY_REPEATED):
		buf = " repeated";
		break;
	default:
		break;
	}
	strcat(info, buf);

	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_dump()
 * Dump input event information.
 *----------------------------------------------------------------------------*/
void input_event_dump (const inputevent * const input)
{
	char info[128];
	int ret = -1;

	if (input == NULL) {
		INPTD("parameter error\n");
		return;
	}
	memset(info, 0, sizeof (info));

	switch (input->type) {
	case (EV_SYN):
		break;
	case (EV_KEY):
		ret = key_event_info(input, info);
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
		ret = general_event_info(input, info);
		break;
	}
	if (ret == 0) {
		printf("\t[%lds:%06ldus]\t{%s}\n",
		       input->time.tv_sec, input->time.tv_usec, info);
	}
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_event_test()
 * Dump input event information.
 *----------------------------------------------------------------------------*/
int input_event_test (void)
{
	int ret = -1;
	int fd = -1;
	int i;
	inputevent input;
	clockspec clkStart;
	clockspec clkNow;
	clockspec clkWait;

	ret = system("ls -lR /dev/input/");

	ret = input_event_open(NULL, &fd);
	if (ret < 0) {
		goto exit;
	}
	input_event_device_dump(fd);

	clkWait.tv_sec	= INPUT_EVENT_TEST_TIMEOUT;
	clkWait.tv_nsec	= 0;
	printf("input event test start, time limit %lds\n", clkWait.tv_sec);

	ret = clock_gettime(CLOCK_MONOTONIC, &clkStart);
	if (ret != 0) {
		INPTD("clock_gettime error ret=%d\n", ret);
	}

	while (1) {
		ret = input_event_read(fd, &input);
		if (ret > 0) {
			input_event_dump(&input);
		}

		ret = clock_gettime(CLOCK_MONOTONIC, &clkNow);
		if (ret != 0) {
			INPTD("clock_gettime error ret=%d\n", ret);
			continue;
		}

		if (clock_wait(&clkStart, &clkNow, &clkWait) == 0) {
			printf("input event test timeout\n");
			break;
		}
	}

exit:
	if (fd >= 0) {
		ret = input_event_close(fd);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/
