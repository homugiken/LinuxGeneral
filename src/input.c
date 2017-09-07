/*==============================================================================
 * input.c
 *----------------------------------------------------------------------------*/

#include "input.h"

static char * events[EV_MAX + 1] =
{
	[0 ... EV_MAX]	= NULL,
	[EV_SYN]	= "EV_SYN Synchronization",
	[EV_KEY]	= "EV_KEY Keys and buttons",
	[EV_REL]	= "EV_REL Relative axes",
	[EV_ABS]	= "EV_ABS Absolute axes",
	[EV_MSC]	= "EV_MSC Misc",
	[EV_SW]		= "EV_SW Switch",
	[EV_LED]	= "EV_LED LEDs",
	[EV_SND]	= "EV_SND Sounds",
	[EV_REP]	= "EV_REP Autorepeat",
	[EV_FF]		= "EV_FF Force feedback",
	[EV_PWR]	= "EV_PWR Power button",
	[EV_FF_STATUS]	= "EV_FF_STATUS Force feedback status"
};

static char * syncs[SYN_MAX + 1] =
{
	[0 ... SYN_MAX]	= NULL,
	[SYN_REPORT]	= "SYN_REPORT",
	[SYN_CONFIG]	= "SYN_CONFIG",
	[SYN_MT_REPORT]	= "SYN_MT_REPORT",
	[SYN_DROPPED]	= "SYN_DROPPED"
};

static char * rel_axes[REL_MAX + 1] =
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

static char * abs_axes[ABS_MAX + 1] =
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

static char * switches[SW_MAX + 1] =
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

static char * misc[MSC_MAX + 1] =
{
	[ 0 ... MSC_MAX]	= NULL,
	[MSC_SERIAL]		= "MSC_SERIAL",
	[MSC_PULSELED]		= "MSC_PULSELED",
	[MSC_GESTURE]		= "MSC_GESTURE",
	[MSC_RAW]		= "MSC_RAW",
	[MSC_SCAN]		= "MSC_SCAN",
	[MSC_TIMESTAMP]		= "MSC_TIMESTAMP"
};

static char * leds[LED_MAX + 1] =
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


static char * repeats[REP_MAX + 1] =
{
	[0 ... REP_MAX]	= NULL,
	[REP_DELAY]	= "REP_DELAY",
	[REP_PERIOD]	= "REP_PERIOD"
};

static char * sounds[SND_MAX + 1] =
{
	[0 ... SND_MAX]	= NULL,
	[SND_CLICK]	= "SND_CLICK",
	[SND_BELL]	= "SND_BELL",
	[SND_TONE]	= "SND_TONE"
};

static char * keys[KEY_MAX + 1] =
{
	[0 ... KEY_MAX]		= NULL,
	[KEY_RESERVED]		= "KEY_RESERVED",
	[KEY_ESC]		= "KEY_ESC",
	[KEY_1]			= "KEY_1",
	[KEY_2]			= "KEY_2",
	[KEY_3]			= "KEY_3",
	[KEY_4]			= "KEY_4",
	[KEY_5]			= "KEY_5",
	[KEY_6]			= "KEY_6",
	[KEY_7]			= "KEY_7",
	[KEY_8]			= "KEY_8",
	[KEY_9]			= "KEY_9",
	[KEY_0]			= "KEY_0",
	[KEY_MINUS]		= "KEY_MINUS",
	[KEY_EQUAL]		= "KEY_EQUAL",
	[KEY_BACKSPACE]		= "KEY_BACKSPACE",
	[KEY_TAB]		= "KEY_TAB",
	[KEY_Q]			= "KEY_Q",
	[KEY_W]			= "KEY_W",
	[KEY_E]			= "KEY_E",
	[KEY_R]			= "KEY_R",
	[KEY_T]			= "KEY_T",
	[KEY_Y]			= "KEY_Y",
	[KEY_U]			= "KEY_U",
	[KEY_I]			= "KEY_I",
	[KEY_O]			= "KEY_O",
	[KEY_P]			= "KEY_P",
	[KEY_LEFTBRACE]		= "KEY_LEFTBRACE",
	[KEY_RIGHTBRACE]	= "KEY_RIGHTBRACE",
	[KEY_ENTER]		= "KEY_ENTER",
	[KEY_LEFTCTRL]		= "KEY_LEFTCTRL",
	[KEY_A]			= "KEY_A",
	[KEY_S]			= "KEY_S",
	[KEY_D]			= "KEY_D",
	[KEY_F]			= "KEY_F",
	[KEY_G]			= "KEY_G",
	[KEY_H]			= "KEY_H",
	[KEY_J]			= "KEY_J",
	[KEY_K]			= "KEY_K",
	[KEY_L]			= "KEY_L",
	[KEY_SEMICOLON]		= "KEY_SEMICOLON",
	[KEY_APOSTROPHE]	= "KEY_APOSTROPHE",
	[KEY_GRAVE]		= "KEY_GRAVE",
	[KEY_LEFTSHIFT]		= "KEY_LEFTSHIFT",
	[KEY_BACKSLASH]		= "KEY_BACKSLASH",
	[KEY_Z]			= "KEY_Z",
	[KEY_X]			= "KEY_XX",
	[KEY_C]			= "KEY_C",
	[KEY_V]			= "KEY_V",
	[KEY_B]			= "KEY_B",
	[KEY_N]			= "KEY_N",
	[KEY_M]			= "KEY_M",
	[KEY_COMMA]		= "KEY_COMMA",
	[KEY_DOT]		= "KEY_DOT",
	[KEY_SLASH]		= "KEY_SLASH",
	[KEY_RIGHTSHIFT]	= "KEY_RIGHTSHIFT",
	[KEY_KPASTERISK]	= "KEY_KPASTERISK",
	[KEY_LEFTALT]		= "KEY_LEFTALT",
	[KEY_SPACE]		= "KEY_SPACE",
	[KEY_CAPSLOCK]		= "KEY_CAPSLOCK",
	[KEY_F1]		= "KEY_F1",
	[KEY_F2]		= "KEY_F2",
	[KEY_F3]		= "KEY_F3",
	[KEY_F4]		= "KEY_F4",
	[KEY_F5]		= "KEY_F5",
	[KEY_F6]		= "KEY_F6",
	[KEY_F7]		= "KEY_F7",
	[KEY_F8]		= "KEY_F8",
	[KEY_F9]		= "KEY_F9",
	[KEY_F10]		= "KEY_F10",
	[KEY_NUMLOCK]		= "KEY_NUMLOCK",
	[KEY_SCROLLLOCK]	= "KEY_SCROLLLOCK",
	[KEY_KP7]		= "KEY_KP7",
	[KEY_KP8]		= "KEY_KP8",
	[KEY_KP9]		= "KEY_KP9",
	[KEY_KPMINUS]		= "KEY_KPMINUS",
	[KEY_KP4]		= "KEY_KP4",
	[KEY_KP5]		= "KEY_KP5",
	[KEY_KP6]		= "KEY_KP6",
	[KEY_KPPLUS]		= "KEY_KPPLUS",
	[KEY_KP1]		= "KEY_KP1",
	[KEY_KP2]		= "KEY_KP2",
	[KEY_KP3]		= "KEY_KP3",
	[KEY_KP0]		= "KEY_KP0",
	[KEY_KPDOT]		= "KEY_KPDOT",
	[KEY_ZENKAKUHANKAKU]	= "KEY_ZENKAKUHANKAKU",
	[KEY_102ND]		= "KEY_102ND",
	[KEY_F11]		= "KEY_F11",
	[KEY_F12]		= "KEY_F12",
	[KEY_RO]		= "KEY_RO",
	[KEY_KATAKANA]		= "KEY_KATAKANA",
	[KEY_HIRAGANA]		= "KEY_HIRAGANA",
	[KEY_HENKAN]		= "KEY_HENKAN",
	[KEY_KATAKANAHIRAGANA]	= "KEY_KATAKANAHIRAGANA",
	[KEY_MUHENKAN]		= "KEY_MUHENKAN",
	[KEY_KPJPCOMMA]		= "KEY_KPJPCOMMA",
	[KEY_KPENTER]		= "KEY_KPENTER",
	[KEY_RIGHTCTRL]		= "KEY_RIGHTCTRL",
	[KEY_KPSLASH]		= "KEY_KPSLASH",
	[KEY_SYSRQ]		= "KEY_SYSRQ",
	[KEY_RIGHTALT]		= "KEY_RIGHTALT",
	[KEY_LINEFEED]		= "KEY_LINEFEED",
	[KEY_HOME]		= "KEY_HOME",
	[KEY_UP]		= "KEY_UP",
	[KEY_PAGEUP]		= "KEY_PAGEUP",
	[KEY_LEFT]		= "KEY_LEFT",
	[KEY_RIGHT]		= "KEY_RIGHT",
	[KEY_END]		= "KEY_END",
	[KEY_DOWN]		= "KEY_DOWN",
	[KEY_PAGEDOWN]		= "KEY_PAGEDOWN",
	[KEY_INSERT]		= "KEY_INSERT",
	[KEY_DELETE]		= "KEY_DELETE",
	[KEY_MACRO]		= "KEY_MACRO",
	[KEY_MUTE]		= "KEY_MUTE",
	[KEY_VOLUMEDOWN]	= "KEY_VOLUMEDOWN",
	[KEY_VOLUMEUP]		= "KEY_VOLUMEUP",
	[KEY_POWER]		= "KEY_POWER",
	[KEY_KPEQUAL]		= "KEY_KPEQUAL",
	[KEY_KPPLUSMINUS]	= "KEY_KPPLUSMINUS",
	[KEY_PAUSE]		= "KEY_PAUSE",
	[KEY_SCALE]		= "KEY_SCALE",



	[KEY_KPCOMMA] = "KPComma",
	[KEY_HANGUEL] = "Hanguel",		[KEY_HANJA] = "Hanja",
	[KEY_YEN] = "Yen",			[KEY_LEFTMETA] = "LeftMeta",
	[KEY_RIGHTMETA] = "RightMeta",		[KEY_COMPOSE] = "Compose",
	[KEY_STOP] = "Stop",			[KEY_AGAIN] = "Again",
	[KEY_PROPS] = "Props",			[KEY_UNDO] = "Undo",
	[KEY_FRONT] = "Front",			[KEY_COPY] = "Copy",
	[KEY_OPEN] = "Open",			[KEY_PASTE] = "Paste",
	[KEY_FIND] = "Find",			[KEY_CUT] = "Cut",
	[KEY_HELP] = "Help",			[KEY_MENU] = "Menu",
	[KEY_CALC] = "Calc",			[KEY_SETUP] = "Setup",
	[KEY_SLEEP] = "Sleep",			[KEY_WAKEUP] = "WakeUp",
	[KEY_FILE] = "File",			[KEY_SENDFILE] = "SendFile",
	[KEY_DELETEFILE] = "DeleteFile",	[KEY_XFER] = "X-fer",
	[KEY_PROG1] = "Prog1",			[KEY_PROG2] = "Prog2",
	[KEY_WWW] = "WWW",			[KEY_MSDOS] = "MSDOS",
	[KEY_COFFEE] = "Coffee",		[KEY_DIRECTION] = "Direction",
	[KEY_CYCLEWINDOWS] = "CycleWindows",	[KEY_MAIL] = "Mail",
	[KEY_BOOKMARKS] = "Bookmarks",		[KEY_COMPUTER] = "Computer",
	[KEY_BACK] = "Back",			[KEY_FORWARD] = "Forward",
	[KEY_CLOSECD] = "CloseCD",		[KEY_EJECTCD] = "EjectCD",
	[KEY_EJECTCLOSECD] = "EjectCloseCD",	[KEY_NEXTSONG] = "NextSong",
	[KEY_PLAYPAUSE] = "PlayPause",		[KEY_PREVIOUSSONG] = "PreviousSong",
	[KEY_STOPCD] = "StopCD",		[KEY_RECORD] = "Record",
	[KEY_REWIND] = "Rewind",		[KEY_PHONE] = "Phone",
	[KEY_ISO] = "ISOKey",			[KEY_CONFIG] = "Config",
	[KEY_HOMEPAGE] = "HomePage",		[KEY_REFRESH] = "Refresh",
	[KEY_EXIT] = "Exit",			[KEY_MOVE] = "Move",
	[KEY_EDIT] = "Edit",			[KEY_SCROLLUP] = "ScrollUp",
	[KEY_SCROLLDOWN] = "ScrollDown",	[KEY_KPLEFTPAREN] = "KPLeftParenthesis",
	[KEY_KPRIGHTPAREN] = "KPRightParenthesis", [KEY_F13] = "F13",
	[KEY_F14] = "F14",			[KEY_F15] = "F15",
	[KEY_F16] = "F16",			[KEY_F17] = "F17",
	[KEY_F18] = "F18",			[KEY_F19] = "F19",
	[KEY_F20] = "F20",			[KEY_F21] = "F21",
	[KEY_F22] = "F22",			[KEY_F23] = "F23",
	[KEY_F24] = "F24",			[KEY_PLAYCD] = "PlayCD",
	[KEY_PAUSECD] = "PauseCD",		[KEY_PROG3] = "Prog3",
	[KEY_PROG4] = "Prog4",			[KEY_SUSPEND] = "Suspend",
	[KEY_CLOSE] = "Close",			[KEY_PLAY] = "Play",
	[KEY_FASTFORWARD] = "Fast Forward",	[KEY_BASSBOOST] = "Bass Boost",
	[KEY_PRINT] = "Print",			[KEY_HP] = "HP",
	[KEY_CAMERA] = "Camera",		[KEY_SOUND] = "Sound",
	[KEY_QUESTION] = "Question",		[KEY_EMAIL] = "Email",
	[KEY_CHAT] = "Chat",			[KEY_SEARCH] = "Search",
	[KEY_CONNECT] = "Connect",		[KEY_FINANCE] = "Finance",
	[KEY_SPORT] = "Sport",			[KEY_SHOP] = "Shop",
	[KEY_ALTERASE] = "Alternate Erase",	[KEY_CANCEL] = "Cancel",
	[KEY_BRIGHTNESSDOWN] = "Brightness down", [KEY_BRIGHTNESSUP] = "Brightness up",
	[KEY_MEDIA] = "Media",			[KEY_UNKNOWN] = "Unknown",
	[BTN_0] = "Btn0",			[BTN_1] = "Btn1",
	[BTN_2] = "Btn2",			[BTN_3] = "Btn3",
	[BTN_4] = "Btn4",			[BTN_5] = "Btn5",
	[BTN_6] = "Btn6",			[BTN_7] = "Btn7",
	[BTN_8] = "Btn8",			[BTN_9] = "Btn9",
	[BTN_LEFT] = "LeftBtn",			[BTN_RIGHT] = "RightBtn",
	[BTN_MIDDLE] = "MiddleBtn",		[BTN_SIDE] = "SideBtn",
	[BTN_EXTRA] = "ExtraBtn",		[BTN_FORWARD] = "ForwardBtn",
	[BTN_BACK] = "BackBtn",			[BTN_TASK] = "TaskBtn",
	[BTN_TRIGGER] = "Trigger",		[BTN_THUMB] = "ThumbBtn",
	[BTN_THUMB2] = "ThumbBtn2",		[BTN_TOP] = "TopBtn",
	[BTN_TOP2] = "TopBtn2",			[BTN_PINKIE] = "PinkieBtn",
	[BTN_BASE] = "BaseBtn",			[BTN_BASE2] = "BaseBtn2",
	[BTN_BASE3] = "BaseBtn3",		[BTN_BASE4] = "BaseBtn4",
	[BTN_BASE5] = "BaseBtn5",		[BTN_BASE6] = "BaseBtn6",
	[BTN_DEAD] = "BtnDead",			[BTN_A] = "BtnA",
	[BTN_B] = "BtnB",			[BTN_C] = "BtnC",
	[BTN_X] = "BtnX",			[BTN_Y] = "BtnY",
	[BTN_Z] = "BtnZ",			[BTN_TL] = "BtnTL",
	[BTN_TR] = "BtnTR",			[BTN_TL2] = "BtnTL2",
	[BTN_TR2] = "BtnTR2",			[BTN_SELECT] = "BtnSelect",
	[BTN_START] = "BtnStart",		[BTN_MODE] = "BtnMode",
	[BTN_THUMBL] = "BtnThumbL",		[BTN_THUMBR] = "BtnThumbR",
	[BTN_TOOL_PEN] = "ToolPen",		[BTN_TOOL_RUBBER] = "ToolRubber",
	[BTN_TOOL_BRUSH] = "ToolBrush",		[BTN_TOOL_PENCIL] = "ToolPencil",
	[BTN_TOOL_AIRBRUSH] = "ToolAirbrush",	[BTN_TOOL_FINGER] = "ToolFinger",
	[BTN_TOOL_MOUSE] = "ToolMouse",		[BTN_TOOL_LENS] = "ToolLens",
	[BTN_TOUCH] = "Touch",			[BTN_STYLUS] = "Stylus",
	[BTN_STYLUS2] = "Stylus2",		[BTN_TOOL_DOUBLETAP] = "Tool Doubletap",
	[BTN_TOOL_TRIPLETAP] = "Tool Tripletap", [BTN_GEAR_DOWN] = "WheelBtn",
	[BTN_GEAR_UP] = "Gear up",		[KEY_OK] = "Ok",
	[KEY_SELECT] = "Select",		[KEY_GOTO] = "Goto",
	[KEY_CLEAR] = "Clear",			[KEY_POWER2] = "Power2",
	[KEY_OPTION] = "Option",		[KEY_INFO] = "Info",
	[KEY_TIME] = "Time",			[KEY_VENDOR] = "Vendor",
	[KEY_ARCHIVE] = "Archive",		[KEY_PROGRAM] = "Program",
	[KEY_CHANNEL] = "Channel",		[KEY_FAVORITES] = "Favorites",
	[KEY_EPG] = "EPG",			[KEY_PVR] = "PVR",
	[KEY_MHP] = "MHP",			[KEY_LANGUAGE] = "Language",
	[KEY_TITLE] = "Title",			[KEY_SUBTITLE] = "Subtitle",
	[KEY_ANGLE] = "Angle",			[KEY_ZOOM] = "Zoom",
	[KEY_MODE] = "Mode",			[KEY_KEYBOARD] = "Keyboard",
	[KEY_SCREEN] = "Screen",		[KEY_PC] = "PC",
	[KEY_TV] = "TV",			[KEY_TV2] = "TV2",
	[KEY_VCR] = "VCR",			[KEY_VCR2] = "VCR2",
	[KEY_SAT] = "Sat",			[KEY_SAT2] = "Sat2",
	[KEY_CD] = "CD",			[KEY_TAPE] = "Tape",
	[KEY_RADIO] = "Radio",			[KEY_TUNER] = "Tuner",
	[KEY_PLAYER] = "Player",		[KEY_TEXT] = "Text",
	[KEY_DVD] = "DVD",			[KEY_AUX] = "Aux",
	[KEY_MP3] = "MP3",			[KEY_AUDIO] = "Audio",
	[KEY_VIDEO] = "Video",			[KEY_DIRECTORY] = "Directory",
	[KEY_LIST] = "List",			[KEY_MEMO] = "Memo",
	[KEY_CALENDAR] = "Calendar",		[KEY_RED] = "Red",
	[KEY_GREEN] = "Green",			[KEY_YELLOW] = "Yellow",
	[KEY_BLUE] = "Blue",			[KEY_CHANNELUP] = "ChannelUp",
	[KEY_CHANNELDOWN] = "ChannelDown",	[KEY_FIRST] = "First",
	[KEY_LAST] = "Last",			[KEY_AB] = "AB",
	[KEY_NEXT] = "Next",			[KEY_RESTART] = "Restart",
	[KEY_SLOW] = "Slow",			[KEY_SHUFFLE] = "Shuffle",
	[KEY_BREAK] = "Break",			[KEY_PREVIOUS] = "Previous",
	[KEY_DIGITS] = "Digits",		[KEY_TEEN] = "TEEN",
	[KEY_TWEN] = "TWEN",			[KEY_DEL_EOL] = "Delete EOL",
	[KEY_DEL_EOS] = "Delete EOS",		[KEY_INS_LINE] = "Insert line",
	[KEY_DEL_LINE] = "Delete line",

};


char ** names[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = syncs,
	// [EV_KEY] = keys,
	[EV_REL] = rel_axes,
	[EV_ABS] = abs_axes,
	[EV_MSC] = misc,
	[EV_LED] = leds,
	[EV_SND] = sounds,
	[EV_REP] = repeats
};

/*==============================================================================
 * input_open()
 * Open input event.
 *----------------------------------------------------------------------------*/
int input_open (const char * const name)
{
	int ret = -1;
	char path[INPUT_EVENT_PATH_LENGTH];

	if (name == NULL) {
		memset(path, 0, sizeof (path));
		printf("\nPlease enter input event path: ");
		ret = scanf("%1024s", path);
		if (ret <= 0) {
			goto exit;
		}

		printf("Entered input event path: %s\n", path);
		ret = open(path, O_RDONLY);
	} else {
		ret = open(name, O_RDONLY);
	}

	if (ret < 0) {
		INPTD("open failed ret=%d\n", ret);
	}
	else {
		INPTD("open fd=%d\n", ret);
	}

exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_close()
 * Close input event.
 *----------------------------------------------------------------------------*/
int input_close (const int fd)
{
	int ret;

	ret = close(fd);
	if (ret < 0) {
		INPTD("close failed ret=%d\n", ret);
	}
	else {
		INPTD("close fd=%d\n", fd);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_read()
 * Return number of bytes read. Return -1 when error.
 *----------------------------------------------------------------------------*/
int input_read (const int fd, inputevent * const input)
{
	int ret;

	ret = read(fd, input, sizeof (inputevent));
	if (ret < 0) {
		INPTD("read failed ret=%d\n", ret);
	}

	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_key_dump()
 * Dump input key.
 *----------------------------------------------------------------------------*/
int input_get_key_info (const inputevent * const input, char * const info)
{
	int ret = -1;

	if ((input == NULL) || (input->type != EV_KEY)) {
		INPTD("parameter error\n");
		goto exit;
	}

	sprintf(info, "KEY %u|0x%X ", input->code, input->code);
	switch (input->value) {
	case (INPUT_KEY_RELEASED):
		strcat(info, "released");
		break;
	case (INPUT_KEY_PRESSED):
		strcat(info, "pressed");
		break;
	case (INPUT_KEY_REPEATED):
		strcat(info, "repeated");
		break;
	default:
		strcat(info, "unknown operation");
		break;
	}

	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_abs_dump()
 * Dump input absolute axes.
 *----------------------------------------------------------------------------*/
int input_get_abs_info (const inputevent * const input, char * const info)
{
	int ret = -1;

	if ((input == NULL) || (input->type != EV_ABS)) {
		INPTD("parameter error\n");
		goto exit;
	}

	sprintf(info, "ABS value=%u|0x%X ", input->value, input->value);
	switch (input->code) {
	case (ABS_X):
		strcat(info, "X");
		break;
	case (ABS_Y):
		strcat(info, "Y");
		break;
	default:
		strcat(info, "unknown axis");
		break;
	}

	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_rel_dump()
 * Dump input relative axes.
 *----------------------------------------------------------------------------*/
int input_get_rel_info (const inputevent * const input, char * const info)
{
	int ret = -1;

	if ((input == NULL) || (input->type != EV_REL)) {
		INPTD("parameter error\n");
		goto exit;
	}

	sprintf(info, "REL value=%u|0x%X ", input->value, input->value);
	switch (input->code) {
	case (ABS_X):
		strcat(info, "X");
		break;
	case (ABS_Y):
		strcat(info, "Y");
		break;
	default:
		strcat(info, "unknown axis");
		break;
	}

	ret = 0;
exit:
	return (ret);
}
/*----------------------------------------------------------------------------*/

/*==============================================================================
 * input_dump()
 * Dump input event.
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
		ret = input_get_key_info(input, info);
		break;
	case (EV_REL):
		ret = input_get_rel_info(input, info);
		break;
	case (EV_ABS):
		ret = input_get_abs_info(input, info);
		break;
	case (EV_MSC):
		break;
	// case (EV_SW):
	// 	break;
	// case (EV_LED):
	// 	break;
	// case (EV_SND):
	// 	break;
	// case (EV_REP):
	// 	break;
	// case (EV_FF):
	// 	break;
	// case (EV_PWR):
	// 	break;
	// case (EV_FF_STATUS):
	// 	break;
	default:
		sprintf(info, "unknown type=%u|0x%X code=%u|0x%X value=%u|0x%X",
		        input->type, input->type,
		        input->code, input->code,
		        input->value, input->value
		        );
		ret = 0;
		break;
	}
	if (ret == 0) {
		printf("\t[%lds:%06ldus]\t{%s}\n",
		       input->time.tv_sec, input->time.tv_usec, info);
	}
}
/*----------------------------------------------------------------------------*/

void input_device_dump (const int fd)
{
	struct input_id id;
	BITFIELD bits[32];
	char buf[128];
	int bit;
	int ret;

	printf("\nInput device information:\n");

	if (ioctl(fd, EVIOCGID, &id) < 0) {
		INPTD("EVIOCGID error\n");
	}
	else {
		printf("EVIOCGID:\tbustype=0x%04x vendor=0x%04x product=0x%04x version=0x%04x\n",
		       id.bustype, id.vendor, id.product, id.version);
	}

	if (ioctl(fd, EVIOCGNAME(sizeof (buf)), buf) < 0) {
		INPTD("EVIOCGNAME error\n");
	}
	else {
		printf("EVIOCGNAME:\t%s\n", buf);
	}

	if (ioctl(fd, EVIOCGPHYS(sizeof (buf)), buf) < 0) {
		INPTD("EVIOCGPHYS error\n");
	}
	else {
		printf("EVIOCGPHYS:\t%s\n", buf);
	}

	ret = ioctl(fd, EVIOCGBIT(0, sizeof (bits)), bits);
	if (ret < 0) {
		INPTD("EVIOCGBIT error\n");
	}
	else {
		printf("EVIOCGBIT:\n");
		for (bit = 0; (bit < (ret * 8)) && (bit < EV_MAX); bit++) {
			if (test_bit(bit, bits)) {
				printf("\t\t0x%02X %s\n", bit, events[bit]);
			}
		}
	}
	printf("\n");
}

int input_test (void)
{
	int ret = -1;
	int fd = -1;
	int i;
	inputevent input;

	printf("input test start\n");

	ret = system("ls -lR /dev/input/");

	ret = input_open(NULL);
	if (ret < 0) {
		goto exit;
	}
	fd = ret;

	input_device_dump(fd);

	while (1) {
		ret = input_read(fd, &input);
		if (ret > 0) {
			input_event_dump(&input);
		}
	}

exit:
	if (fd >= 0) {
		ret = input_close(fd);
	}

	return (ret);
}
