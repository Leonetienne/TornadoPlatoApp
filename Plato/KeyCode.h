#pragma once

namespace Plato
{
	namespace Input
	{
		/** Key representations.
		* Yes, these are mirrors of microsoft's virtual key codes
		*/
		enum class KEY_CODE
		{
			NONE,

			// Mouse buttons
			MOUSE_L = 0x01,
			MOUSE_R = 0x02,
			MOUSE_M = 0x04,
			MOUSE_X1 = 0x05,
			MOUSE_X2 = 0x06,

			// Letters
			A = 'a',
			B = 'b',
			C = 'c',
			D = 'd',
			E = 'e',
			F = 'f',
			G = 'g',
			H = 'h',
			I = 'i',
			J = 'j',
			K = 'k',
			L = 'l',
			M = 'm',
			N = 'n',
			O = 'o',
			P = 'p',
			Q = 'q',
			R = 'r',
			S = 's',
			T = 't',
			U = 'u',
			V = 'v',
			W = 'w',
			X = 'x',
			Y = 'y',
			Z = 'z',

			// Numbers
			NUM_0 = '0',
			NUM_1 = '1',
			NUM_2 = '2',
			NUM_3 = '3',
			NUM_4 = '4',
			NUM_5 = '5',
			NUM_6 = '6',
			NUM_7 = '7',
			NUM_8 = '8',
			NUM_9 = '9',

			// Numpad numbers
			NUMPAD_0 = 0x60,
			NUMPAD_1 = 0x61,
			NUMPAD_2 = 0x62,
			NUMPAD_3 = 0x63,
			NUMPAD_4 = 0x64,
			NUMPAD_5 = 0x65,
			NUMPAD_6 = 0x66,
			NUMPAD_7 = 0x67,
			NUMPAD_8 = 0x68,
			NUMPAD_9 = 0x69,

			NUMPAD_NLOCK = 0x90,
			DIVIDE = 0x6f,
			MULTIPLY = 0x6a,
			SUBTRACT = 0x6d,
			ADD = 0x6b,

			// Symbols
			PERIOD = 0xbe,
			MINUS = 0xbd,
			COMMA = 0xbc,
			PLUS = 0xbb,
			SPACE = ' ',
			CIRCUMFLEX = 0x1b,

			// Arrows
			ARROW_UP = 0x26,
			ARROW_DOWN = 0x28,
			ARROW_LEFT = 0x25,
			ARROW_RIGHT = 0x27,

			// Function keys
			F1 = 0x70,
			F2 = 0x71,
			F3 = 0x72,
			F4 = 0x73,
			F5 = 0x74,
			F6 = 0x75,
			F7 = 0x76,
			F8 = 0x77,
			F9 = 0x78,
			F10 = 0x79,
			F11 = 0x7a,
			F12 = 0x7b,
			F13 = 0x7c,
			F14 = 0x7d,
			F15 = 0x7e,
			F16 = 0x7f,
			F17 = 0x80,
			F18 = 0x81,
			F19 = 0x82,
			F20 = 0x83,
			F21 = 0x84,
			F22 = 0x85,
			F23 = 0x86,
			F24 = 0x87,

			// Other method keys
			ESCAPE = 0x1b,
			BACK = 0x08,
			TAB = 0x09,
			LSHIFT = 0xa0,
			RSHIFT = 0xa1,
			LCONTROL = 0xa2,
			RCONTROL = 0xa3,
			ALT = 0x12,
			CAPSLOCK = 0x14,
			INSERT = 0x2d,
			DEL = 0x2e,
			PRINT = 0x2c,
			PAUSE = 0x13,
			SCROLL_LOCK = 0x91,
			PAGE_UP = 0x21,
			PAGE_DOWN = 0x22,
			END = 0x23
		};
	}
}
