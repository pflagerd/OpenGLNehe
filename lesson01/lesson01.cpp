//
// This code was created by Jeff Molofee '99
// and updated to windows 2010 by Daniel P Pflager.
//

#include <windows.h>	// Header File For Windows
#include <gl\gl.h>		// Header File For The OpenGL32 Library
#include <gl\glu.h>		// Header File For The GLu32 Library

static	HGLRC hRC;		// Permanent Rendering Context
static	HDC hDC;		// Private GDI Device Context

BOOL	keys[256];		// Array Used For The Keyboard Routine

GLvoid InitGL(GLsizei Width, GLsizei Height)	// This Will Be Called Right After The GL Window Is Created
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
	glClearDepth(1.0);							// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);						// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);					// Enables Depth Testing
	glShadeModel(GL_SMOOTH);					// Enables Smooth Color Shading

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();							// Reset The Projection Matrix

	gluPerspective(45.0f, (GLfloat)Width/(GLfloat)Height, 0.1f, 100.0f);	// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height==0)								// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;

	glViewport(0, 0, Width, Height);			// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)Width/(GLfloat)Height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

GLvoid DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();										// Reset The View
}

LRESULT CALLBACK WndProc(	HWND	hWnd,
				UINT	message,
				WPARAM	wParam,
				LPARAM	lParam)
{
	RECT	Screen;							// Used Later On To Get The Size Of The Window
	GLuint	PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),		// Size Of This Pixel Format Descriptor
		1,									// Version Number (?)
		PFD_DRAW_TO_WINDOW |				// Format Must Support Window
		PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,					// Must Support Double Buffering
		PFD_TYPE_RGBA,						// Request An RGBA Format
		16,									// Select A 16Bit Color Depth
		0, 0, 0, 0, 0, 0,					// Color Bits Ignored (?)
		0,									// No Alpha Buffer
		0,									// Shift Bit Ignored (?)
		0,									// No Accumulation Buffer
		0, 0, 0, 0,							// Accumulation Bits Ignored (?)
		16,									// 16Bit Z-Buffer (Depth Buffer)  
		0,									// No Stencil Buffer
		0,									// No Auxiliary Buffer (?)
		PFD_MAIN_PLANE,						// Main Drawing Layer
		0,									// Reserved (?)
		0, 0, 0								// Layer Masks Ignored (?)
	};

	switch (message)						// Tells Windows We Want To Check The Message
	{
		case WM_CREATE:
			hDC = GetDC(hWnd);				// Gets A Device Context For The Window
			PixelFormat = ChoosePixelFormat(hDC, &pfd);		// Finds The Closest Match To The Pixel Format We Set Above

			if (!PixelFormat)
			{
				MessageBox(0, L"Can't Find A Suitable PixelFormat.", L"Error", MB_OK | MB_ICONERROR);
				PostQuitMessage(0);			// This Sends A 'Message' Telling The Program To Quit
				break;						// Prevents The Rest Of The Code From Running
			}

			if (!SetPixelFormat(hDC, PixelFormat, &pfd))
			{
				MessageBox(0, L"Can't Set The PixelFormat.", L"Error", MB_OK | MB_ICONERROR);
				PostQuitMessage(0);
				break;
			}

			hRC = wglCreateContext(hDC);
			if (!hRC)
			{
				MessageBox(0, L"Can't Create A GL Rendering Context.", L"Error", MB_OK | MB_ICONERROR);
				PostQuitMessage(0);
				break;
			}

			if (!wglMakeCurrent(hDC, hRC))
			{
				MessageBox(0, L"Can't activate GLRC.", L"Error", MB_OK | MB_ICONERROR);
				PostQuitMessage(0);
				break;
			}

			GetClientRect(hWnd, &Screen);
			InitGL(Screen.right, Screen.bottom);
			break;

		case WM_DESTROY:
		case WM_CLOSE:
			ChangeDisplaySettings(NULL, 0);

			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);
			ReleaseDC(hWnd,hDC);

			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			keys[wParam] = TRUE;
			break;

		case WM_KEYUP:
			keys[wParam] = FALSE;
			break;

		case WM_SIZE:
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(	HINSTANCE	hInstance, 
			HINSTANCE	hPrevInstance, 
			LPSTR		lpCmdLine, 
			int			nCmdShow)
{
	MSG			msg;		// Windows Message Structure
	WNDCLASS	wc;			// Windows Class Structure Used To Set Up The Type Of Window
	HWND		hWnd;		// Storage For Window Handle

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= L"OpenGL WinClass";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"Failed To Register The Window Class.", L"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hWnd = CreateWindow(
	L"OpenGL WinClass",
	L"Jeff Molofee's GL Code Tutorial ... NeHe '99",		// Title Appearing At The Top Of The Window

	WS_POPUP |
	WS_CLIPCHILDREN |
	WS_CLIPSIBLINGS,

	0, 0,												// The Position Of The Window On The Screen
	640, 480,											// The Width And Height Of The WIndow

	NULL,
	NULL,
	hInstance,
	NULL);

	if (!hWnd)
	{
		MessageBox(0, L"Window Creation Error.", L"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	DEVMODE dmScreenSettings;											// Developer Mode

	memset(&dmScreenSettings, 0, sizeof(DEVMODE));						// Clear Room To Store Settings
	dmScreenSettings.dmSize			= sizeof(DEVMODE);					// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth	= 640;								// Screen Width
	dmScreenSettings.dmPelsHeight	= 480;								// Screen Height
	dmScreenSettings.dmFields		= DM_PELSWIDTH | DM_PELSHEIGHT;		// Pixel Mode
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);			// Switch To Full Screen

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);
	wglMakeCurrent(hDC,hRC);

	while (TRUE)
	{
		// Process All Messages
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return TRUE;
			}
		}

		DrawGLScene();
		SwapBuffers(hDC);
		if (keys[VK_ESCAPE])
		{
			SendMessage(hWnd,WM_CLOSE,0,0);
		}
	}
}
