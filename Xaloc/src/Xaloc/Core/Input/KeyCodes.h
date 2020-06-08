#pragma once

/*
	Key codes for Xaloc.
	Taken from glfw3.h key codes.
*/

namespace Xaloc
{
	typedef enum class KeyCode : uint16_t
	{
		/* Printable keys */
		Space      = 32,
		Apostrophe = 39, /* ' */
		Comma      = 44, /* , */
		Minus      = 45, /* - */
		Period     = 46, /* . */
		Slash      = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal     = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket  = 91,  /* [ */
		Backslash    = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent  = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape      = 256,
		Enter       = 257,
		Tab         = 258,
		Backspace   = 259,
		Insert      = 260,
		Delete      = 261,
		Right       = 262,
		Left        = 263,
		Down        = 264,
		Up          = 265,
		PageUp      = 266,
		PageDown    = 267,
		Home        = 268,
		End         = 269,
		CapsLock    = 280,
		ScrollLock  = 281,
		NumLock     = 282,
		PrintScreen = 283,
		Pause       = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal  = 330,
		KPDivide   = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd      = 334,
		KPEnter    = 335,
		KPEqual    = 336,

		LeftShift    = 340,
		LeftControl  = 341,
		LeftAlt      = 342,
		LeftSuper    = 343,
		RightShift   = 344,
		RightControl = 345,
		RightAlt     = 346,
		RightSuper   = 347,
		Menu         = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

/* Printable keys */
#define XA_KEY_SPACE           ::Xaloc::Key::Space
#define XA_KEY_APOSTROPHE      ::Xaloc::Key::Apostrophe    /* ' */
#define XA_KEY_COMMA           ::Xaloc::Key::Comma         /* , */
#define XA_KEY_MINUS           ::Xaloc::Key::Minus         /* - */
#define XA_KEY_PERIOD          ::Xaloc::Key::Period        /* . */
#define XA_KEY_SLASH           ::Xaloc::Key::Slash         /* / */
#define XA_KEY_0               ::Xaloc::Key::D0
#define XA_KEY_1               ::Xaloc::Key::D1
#define XA_KEY_2               ::Xaloc::Key::D2
#define XA_KEY_3               ::Xaloc::Key::D3
#define XA_KEY_4               ::Xaloc::Key::D4
#define XA_KEY_5               ::Xaloc::Key::D5
#define XA_KEY_6               ::Xaloc::Key::D6
#define XA_KEY_7               ::Xaloc::Key::D7
#define XA_KEY_8               ::Xaloc::Key::D8
#define XA_KEY_9               ::Xaloc::Key::D9
#define XA_KEY_SEMICOLON       ::Xaloc::Key::Semicolon     /* ; */
#define XA_KEY_EQUAL           ::Xaloc::Key::Equal         /* = */
#define XA_KEY_A               ::Xaloc::Key::A
#define XA_KEY_B               ::Xaloc::Key::B
#define XA_KEY_C               ::Xaloc::Key::C
#define XA_KEY_D               ::Xaloc::Key::D
#define XA_KEY_E               ::Xaloc::Key::E
#define XA_KEY_F               ::Xaloc::Key::F
#define XA_KEY_G               ::Xaloc::Key::G
#define XA_KEY_H               ::Xaloc::Key::H
#define XA_KEY_I               ::Xaloc::Key::I
#define XA_KEY_J               ::Xaloc::Key::J
#define XA_KEY_K               ::Xaloc::Key::K
#define XA_KEY_L               ::Xaloc::Key::L
#define XA_KEY_M               ::Xaloc::Key::M
#define XA_KEY_N               ::Xaloc::Key::N
#define XA_KEY_O               ::Xaloc::Key::O
#define XA_KEY_P               ::Xaloc::Key::P
#define XA_KEY_Q               ::Xaloc::Key::Q
#define XA_KEY_R               ::Xaloc::Key::R
#define XA_KEY_S               ::Xaloc::Key::S
#define XA_KEY_T               ::Xaloc::Key::T
#define XA_KEY_U               ::Xaloc::Key::U
#define XA_KEY_V               ::Xaloc::Key::V
#define XA_KEY_W               ::Xaloc::Key::W
#define XA_KEY_X               ::Xaloc::Key::X
#define XA_KEY_Y               ::Xaloc::Key::Y
#define XA_KEY_Z               ::Xaloc::Key::Z
#define XA_KEY_LEFT_BRACKET    ::Xaloc::Key::LeftBracket   /* [ */
#define XA_KEY_BACKSLASH       ::Xaloc::Key::Backslash     /* \ */
#define XA_KEY_RIGHT_BRACKET   ::Xaloc::Key::RightBracket  /* ] */
#define XA_KEY_GRAVE_ACCENT    ::Xaloc::Key::GraveAccent   /* ` */
#define XA_KEY_WORLD_1         ::Xaloc::Key::World1        /* non-US #1 */
#define XA_KEY_WORLD_2         ::Xaloc::Key::World2        /* non-US #2 */

/* Function keys */
#define XA_KEY_ESCAPE          ::Xaloc::Key::Escape
#define XA_KEY_ENTER           ::Xaloc::Key::Enter
#define XA_KEY_TAB             ::Xaloc::Key::Tab
#define XA_KEY_BACKSPACE       ::Xaloc::Key::Backspace
#define XA_KEY_INSERT          ::Xaloc::Key::Insert
#define XA_KEY_DELETE          ::Xaloc::Key::Delete
#define XA_KEY_RIGHT           ::Xaloc::Key::Right
#define XA_KEY_LEFT            ::Xaloc::Key::Left
#define XA_KEY_DOWN            ::Xaloc::Key::Down
#define XA_KEY_UP              ::Xaloc::Key::Up
#define XA_KEY_PAGE_UP         ::Xaloc::Key::PageUp
#define XA_KEY_PAGE_DOWN       ::Xaloc::Key::PageDown
#define XA_KEY_HOME            ::Xaloc::Key::Home
#define XA_KEY_END             ::Xaloc::Key::End
#define XA_KEY_CAPS_LOCK       ::Xaloc::Key::CapsLock
#define XA_KEY_SCROLL_LOCK     ::Xaloc::Key::ScrollLock
#define XA_KEY_NUM_LOCK        ::Xaloc::Key::NumLock
#define XA_KEY_PRINT_SCREEN    ::Xaloc::Key::PrintScreen
#define XA_KEY_PAUSE           ::Xaloc::Key::Pause
#define XA_KEY_F1              ::Xaloc::Key::F1
#define XA_KEY_F2              ::Xaloc::Key::F2
#define XA_KEY_F3              ::Xaloc::Key::F3
#define XA_KEY_F4              ::Xaloc::Key::F4
#define XA_KEY_F5              ::Xaloc::Key::F5
#define XA_KEY_F6              ::Xaloc::Key::F6
#define XA_KEY_F7              ::Xaloc::Key::F7
#define XA_KEY_F8              ::Xaloc::Key::F8
#define XA_KEY_F9              ::Xaloc::Key::F9
#define XA_KEY_F10             ::Xaloc::Key::F10
#define XA_KEY_F11             ::Xaloc::Key::F11
#define XA_KEY_F12             ::Xaloc::Key::F12
#define XA_KEY_F13             ::Xaloc::Key::F13
#define XA_KEY_F14             ::Xaloc::Key::F14
#define XA_KEY_F15             ::Xaloc::Key::F15
#define XA_KEY_F16             ::Xaloc::Key::F16
#define XA_KEY_F17             ::Xaloc::Key::F17
#define XA_KEY_F18             ::Xaloc::Key::F18
#define XA_KEY_F19             ::Xaloc::Key::F19
#define XA_KEY_F20             ::Xaloc::Key::F20
#define XA_KEY_F21             ::Xaloc::Key::F21
#define XA_KEY_F22             ::Xaloc::Key::F22
#define XA_KEY_F23             ::Xaloc::Key::F23
#define XA_KEY_F24             ::Xaloc::Key::F24
#define XA_KEY_F25             ::Xaloc::Key::F25

/* Keypad */
#define XA_KEY_KP_0            ::Xaloc::Key::KP0
#define XA_KEY_KP_1            ::Xaloc::Key::KP1
#define XA_KEY_KP_2            ::Xaloc::Key::KP2
#define XA_KEY_KP_3            ::Xaloc::Key::KP3
#define XA_KEY_KP_4            ::Xaloc::Key::KP4
#define XA_KEY_KP_5            ::Xaloc::Key::KP5
#define XA_KEY_KP_6            ::Xaloc::Key::KP6
#define XA_KEY_KP_7            ::Xaloc::Key::KP7
#define XA_KEY_KP_8            ::Xaloc::Key::KP8
#define XA_KEY_KP_9            ::Xaloc::Key::KP9
#define XA_KEY_KP_DECIMAL      ::Xaloc::Key::KPDecimal
#define XA_KEY_KP_DIVIDE       ::Xaloc::Key::KPDivide
#define XA_KEY_KP_MULTIPLY     ::Xaloc::Key::KPMultiply
#define XA_KEY_KP_SUBTRACT     ::Xaloc::Key::KPSubtract
#define XA_KEY_KP_ADD          ::Xaloc::Key::KPAdd
#define XA_KEY_KP_ENTER        ::Xaloc::Key::KPEnter
#define XA_KEY_KP_EQUAL        ::Xaloc::Key::KPEqual

#define XA_KEY_LEFT_SHIFT      ::Xaloc::Key::LeftShift
#define XA_KEY_LEFT_CONTROL    ::Xaloc::Key::LeftControl
#define XA_KEY_LEFT_ALT        ::Xaloc::Key::LeftAlt
#define XA_KEY_LEFT_SUPER      ::Xaloc::Key::LeftSuper
#define XA_KEY_RIGHT_SHIFT     ::Xaloc::Key::RightShift
#define XA_KEY_RIGHT_CONTROL   ::Xaloc::Key::RightControl
#define XA_KEY_RIGHT_ALT       ::Xaloc::Key::RightAlt
#define XA_KEY_RIGHT_SUPER     ::Xaloc::Key::RightSuper
#define XA_KEY_MENU            ::Xaloc::Key::Menu
