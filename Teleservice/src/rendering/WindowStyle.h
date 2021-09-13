#ifndef WINDOW_STYLE_HEADER
#define WINDOW_STYLE_HEADER

enum class WindowStyle
{
	Border             = 0x00800000L, // The window has a thin-line border.
	Caption            = 0x00C00000L, // The window has a title bar (includes the WS_BORDER style).
	Child              = 0x40000000L, // The window is a child window. A window with this style cannot have a menu bar. This style cannot be used with the WS_POPUP style.
	ClipChildren       = 0x02000000L, // Excludes the area occupied by child windows when drawing occurs within the parent window. This style is used when creating the parent window..
	ClipSiblings       = 0x04000000L, // Clips child windows relative to each other; that is, when a particular child window receives a WM_PAINT message.
	Disabled           = 0x08000000L, // The window is initially disabled. A disabled window cannot receive input from the user.
	DialogFrame        = 0x00400000L, // The window has a border of a style typically used with dialog boxes. A window with this style cannot have a title bar.
	Group              = 0x00020000L, // The window is the first control of a group of controls. The group consists of this first control and all controls defined after it, up to the next control with the WS_GROUP style.
	HorizontalScollBar = 0x00100000L, // The window has a horizontal scroll bar.
	VerticalScrollBar  = 0x00200000L, // The window has a vertical scroll bar.
	Minimized          = 0x20000000L, // The window is initially minimized.
	Maximized          = 0x01000000L, // The window is initially maximized.
	MinimizeBox        = 0x00020000L, // The window has a minimize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.
	MaximizeBox        = 0x00010000L, // The window has a maximize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.
	Overlapped         = 0x00000000L, // The window is an overlapped window. An overlapped window has a title bar and a border.
	OverlappedWindow   = (0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L),
	Popup              = 0x80000000L, // Applications usually use pop-up windows for dialog boxes. When a pop-up window does not have a caption, it can be created without a border.
	SizeBox            = 0x00040000L, // The window has a sizing border.
	SystemMenu         = 0x00080000L, // The window has a window menu on its title bar. The WS_CAPTION style must also be specified.
	TabStop            = 0x00010000L, // The window is a control that can receive the keyboard focus when the user presses the TAB key. Pressing the TAB key changes the keyboard focus to the next control with the WS_TABSTOP style.
	InitiallyVisible   = 0x10000000L  // The window is initially visible. This style can be turned on and off by using the ShowWindow or SetWindowPos function.
};
#endif