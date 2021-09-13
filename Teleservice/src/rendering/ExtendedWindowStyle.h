#ifndef EXTENDED_WINDOW_STYLE_HEADER
#define EXTENDED_WINDOW_STYLE_HEADER

enum class ExtendedWindowStyle
{
	AcceptFiles      = 0x00000010L, // The window accepts drag-drop files.
	AppWindow        = 0x00040000L, // Forces a top-level window onto the taskbar when the window is visible.
	ClientEdge       = 0x00000200L, // The window has a border with a sunken edge.
	Composited       = 0x02000000L, // Paints all descendants of a window in bottom-to-top painting order using double-buffering which allows a descendant window to have alpha and color-key effects if the descendant window also has the Transparent-style set.
	ContextHelp      = 0x00000400L, // The title bar of the window includes a question mark. Upon clicking the question mark, the cursor changes to a question mark with a pointer. If the user then clicks a child window, the child receives a WM_HELP message.
	ControlParent    = 0x00010000L, // The window itself contains child windows that should take part in dialog box navigation. The dialog manager recurses into children of this window when performing navigation operations.
	DialogModalFrame = 0x00000001L, // The window has a double border; the window can, optionally, be created with a title bar by specifying the WS_CAPTION style in the dwStyle parameter.
	Layered          = 0x00080000,  // The window is a layered window. This style cannot be used if the window has a class style of either CS_OWNDC or CS_CLASSDC.
	LayoutRTL        = 0x00400000L, // If the shell language is Hebrew, Arabic, or another language that supports reading order alignment, the horizontal origin of the window is on the right edge. Increasing horizontal values advance to the left.
	LeftAligned      = 0x00000000L, // The window has generic left-aligned properties. This is the default.
	RightAligned     = 0x00001000L, // The window has generic "right-aligned" properties. This style has an effect only if the shell language is Hebrew, Arabic, or another language that supports reading-order alignment; otherwise, the style is ignored.
	LeftScrollBar    = 0x00004000L, // If the shell language is Hebrew, Arabic, or another language that supports reading order alignment, the vertical scroll bar (if present) is to the left of the client area. For other languages, the style is ignored.
	RightScrollBar   = 0x00000000L, // The vertical scroll bar (if present) is to the right of the client area. This is the default.
	LTRReading       = 0x00000000L, // The window text is displayed using left - to - right reading - order properties. This is the default.
	RTLReading       = 0x00002000L, // If the shell language is Hebrew, Arabic, or another language that supports reading-order alignment, the window text is displayed using right-to-left reading-order properties. For other languages, the style is ignored.
	MDIChild         = 0x00000040L, // The window is a MDI child window.
	NoActivate       = 0x08000000L, // A top-level window created with this style does not become the foreground window when the user clicks it. The system does not bring this window to the foreground when the user minimizes or closes the foreground window.
	NoInheritLayout  = 0x00100000L, // The window does not pass its window layout to its child windows.
	NoParentNotify   = 0x00000004L, // The child window created with this style does not send the WM_PARENTNOTIFY message to its parent window when it is created or destroyed.
	NoRedirectionBMP = 0x00200000L, // The window does not render to a redirection surface. This is for windows that do not have visible content or that use mechanisms other than surfaces to provide their visual.
	StaticEdge       = 0x00020000L, // The window has a three-dimensional border style intended to be used for items that do not accept user input.
	ToolWindow       = 0x00000080L, // The window is intended to be used as a floating toolbar. A tool window has a title bar that is shorter than a normal title bar, and the window title is drawn using a smaller font.
	TopMost          = 0x00000008L, // The window should be placed above all non-topmost windows and should stay above them, even when the window is deactivated. To add or remove this style, use the SetWindowPos function.
	Transparent      = 0x00000020L, // The window should not be painted until siblings beneath the window have been painted. To achieve transparency without these restrictions, use the SetWindowRgn function.
	WindowEdge       = 0x00000100L, // The window has a border with a raised edge.
	PaletteWindow    = (0x00000100L | 0x00000080L | 0x00000008L) // The window is a palette window, which is a modeless dialog box that presents an array of commands.
};
#endif