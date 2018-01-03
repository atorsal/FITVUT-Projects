#include <windows.h>
#include <string.h>
#include <stdio.h>


// Global variable
HINSTANCE hInst;

int x = 300;
int y = 200;
int color_switch = 0;

// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

void onPaint(HWND hWnd);


// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bRet;
    WNDCLASS wcx;          // register class
    HWND hWnd;

    hInst = hInstance;     // Save the application-instance handle.
	    // Fill in the window class structure with parameters that describe the main window.

    wcx.style = CS_HREDRAW | CS_VREDRAW;              // redraw if size changes
    wcx.lpfnWndProc = (WNDPROC) MainWndProc;                    // points to window procedure
    wcx.cbClsExtra = 0;                               // no extra class memory
    wcx.cbWndExtra = 0;                               // no extra window memory
    wcx.hInstance = hInstance;                        // handle to instance
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);      // predefined app. icon
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);                                   // predefined arrow
    wcx.hbrBackground = GetStockObject(WHITE_BRUSH);  // white background brush
    wcx.lpszMenuName =  (LPCSTR) "MainMenu";                   // name of menu resource
    wcx.lpszClassName = (LPCSTR) "MainWClass";                 // name of window class

    // Register the window class.

    if (!RegisterClass(&wcx)) return FALSE;

    // create window of registered class

    hWnd = CreateWindow(
        "MainWClass",        // name of window class
        "ITU",       // title-bar string
        WS_OVERLAPPEDWINDOW, // top-level window
        50,                  // default horizontal position
        100,                 // default vertical position
        640,                 // default width
        480,                 // default height
        (HWND) NULL,         // no owner window
        (HMENU) NULL,        // use class menu
        hInstance,           // handle to application instance
        (LPVOID) NULL);      // no window-creation data
    if (!hWnd) return FALSE;

    // Show the window and send a WM_PAINT message to the window procedure.
    // Record the current cursor position.

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // loop of message processing

    while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
    {
        if (bRet == -1)
        {
            // handle the error and possibly exit
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}

// **************************************************************** //
LRESULT CALLBACK MainWndProc(
    HWND hWnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{

    switch (uMsg)
    {
        case WM_CREATE:
            // Initialize the window.
            return 0;

        case WM_SIZE:
            // Set the size and position of the window.
            return 0;

        case WM_DESTROY:
            // Clean up window-specific data objects.
            PostQuitMessage(0);
            return 0;

		case WM_PAINT:
			// Paint the window's client area.
			onPaint(hWnd);
			return 0;

		case WM_MOUSEMOVE:
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;

		case WM_KEYDOWN:
			switch (wParam) {
			case VK_LEFT:
				x--;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			case VK_RIGHT:
				x++;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			case VK_UP:
				y--;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			case VK_DOWN:
				y++;
				InvalidateRect(hWnd, NULL, TRUE);
				return 0;
			default:
				return 0;
			}

		case WM_CHAR:
			color_switch = !color_switch;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;

        //
        // Process other messages.
        //

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void onPaint(HWND hWnd) {
	PAINTSTRUCT ps;                 // information can be used to paint the client area of a window owned by that application
	HDC         hDC;                // device context
	char        text[256];          // buffer to store an output text
	HFONT       font;               // new large font
	HFONT       oldFont;            // saves the previous font

	hDC = BeginPaint(hWnd, &ps);    // prepares the specified window for painting

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	SetTextColor(hDC, RGB(0, color_switch ? 255 : 0, 0));

	font = CreateFont(30, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);

	oldFont = (HFONT)SelectObject(hDC, font);

	sprintf(text, "X=%u, Y=%u", point.x, point.y);  // output text
	TextOut(                        // writes a character string at the specified location, using the currently selected font, background color, and text color
		hDC,                        // handle to device context
		1, 1,
		text,                       // character string
		(int)strlen(text));              // number of characters

	SelectObject(hDC, oldFont);
	DeleteObject(font);

	Rectangle(hDC, x, y, x + 50, y + 50);

	DeleteDC(hDC);                  // deletes the specified device context
	EndPaint(hWnd, &ps);            // marks the end of painting in the specified window
}