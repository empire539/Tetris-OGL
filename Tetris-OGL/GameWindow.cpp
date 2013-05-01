#include "GameException.h"
#include "GameWindow.h"

#define WINDOW_NAME "Tetris OGL"

/** Constructor. */
GameWindow::GameWindow(int width, int height, bool fullscreen)
	:m_hWindow(NULL), 
	 m_hDeviceContext(NULL), 
	 m_hGlContext(NULL), 
	 m_bFullscreen(fullscreen) {

	registerWindowClass();

	// Sets Window (extended) style
	DWORD winExtStyle = 0;
	DWORD winStyle = 0;
	
	RECT winRect;
	winRect.top = 0;
	winRect.bottom = height;
	winRect.left = 0;
	winRect.right = width;

	if (m_bFullscreen) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Changes dipslay settings whne going full screen
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			throw GameException("Cannot go full screen!");
		}

		winExtStyle = WS_EX_APPWINDOW;
		winStyle = WS_POPUP;

		// Hide cursor in full screen mode
		ShowCursor(FALSE);
	}
	else {
		winExtStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		winStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&winRect, winStyle, FALSE, winExtStyle);

	// Create window
	m_hWindow = CreateWindowEx(
		winExtStyle, 
		TEXT(WINDOW_NAME), 
		TEXT("Tetris OGL"), 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | winStyle, 
		0, 0, winRect.right-winRect.left, winRect.bottom-winRect.top, 
		NULL, NULL, 
		GetModuleHandle(NULL), 
		this
	);

	if (m_hWindow == NULL) {
		throw GameException("Window creation failed!");
	}

	createGraphicsContext();
	initGraphics();
	ShowWindow(m_hWindow, SW_SHOW);

	onSize(width, height);
}

/** Deconstructor. */
GameWindow::~GameWindow() {
	// Exit out of full screen mode
	if (m_bFullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	// Destroy the OpenGL rendering context
	if (m_hGlContext) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hGlContext);
		m_hGlContext = NULL;
	}

	// Release the device context
	if (m_hDeviceContext) {
		ReleaseDC(m_hWindow, m_hDeviceContext);
		m_hDeviceContext = NULL;
	}

	// Destroy the game window, unregister window class
	DestroyWindow(m_hWindow);
	UnregisterClass(TEXT(WINDOW_NAME), GetModuleHandle(NULL));
}

void GameWindow::registerWindowClass() {
	WNDCLASS winClass;
	winClass.style			= 0;
	winClass.lpfnWndProc	= &GameWindow::onEvent;
	winClass.cbClsExtra		= 0;
	winClass.cbWndExtra		= 0;
	winClass.hInstance		= GetModuleHandle(NULL);
	winClass.hIcon			= NULL;
	winClass.hCursor		= 0;
	winClass.hbrBackground	= 0;
	winClass.lpszMenuName	= NULL;
	winClass.lpszClassName	= WINDOW_NAME;

	RegisterClass(&winClass);
}

void GameWindow::createGraphicsContext() {
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;					// Version number
	pfd.dwFlags = PFD_DRAW_TO_WINDOW	// Draw to window
				| PFD_SUPPORT_OPENGL	// Must support OpenGL
				| PFD_DOUBLEBUFFER;		// Support double buffering
	pfd.iPixelType = PFD_TYPE_RGBA;		// RGBA pixel format
	pfd.cColorBits = 32;				// 32 bits of color

	if (!(m_hDeviceContext = GetDC(m_hWindow))) {
		throw GameException("RC: Cannot retrieve device context");
	}

	int pxlFormat;

	if (!(pxlFormat = ChoosePixelFormat(m_hDeviceContext, &pfd))) {
		throw GameException("RC: Cannot find matching pixel format");
	}
	if (!SetPixelFormat(m_hDeviceContext, pxlFormat, &pfd)) {
		throw GameException("RC: Pixel format could not be set");
	}
	if (!(m_hGlContext = wglCreateContext(m_hDeviceContext))) {
		throw GameException("RC: Cannot create OpenGL rendering context");
	}
	if (!wglMakeCurrent(m_hDeviceContext, m_hGlContext)) {
		throw GameException("Could not make the rendering context current");
	}
}

void GameWindow::initGraphics() {
	glEnable(GL_TEXTURE_2D);			// 2D texturing
	glShadeModel(GL_SMOOTH);			// Smooth shade model
	glClearColor(0.0, 0.0, 0.0, 0.0);	// Black clear color
	glEnable(GL_ALPHA_TEST);			// Enables transparency 
	glAlphaFunc(GL_GREATER, 0.0f);		// in images
}

void GameWindow::onSize(GLsizei width, GLsizei height) {
	// Sets size of the OpenGL viewport
	glViewport(0, 0, width, height);

	// Select projection stack, apply orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

LRESULT GameWindow::onEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	// Since this method is static, it cannot access nonstatic members.
	// Create a structure CREATESTRUCT that contains window creation information.
	// SetWindowLongPtr saves user data for a specific window to the class instance.
	
	if (message == WM_NCCREATE) {
		CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<long>(pCreateStruct->lpCreateParams));
	}

	// Get the window instance corresponding to the handle
	GameWindow* pWindow = reinterpret_cast<GameWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));

	if (pWindow) {
		pWindow->processEvent(message, wParam, lParam);
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

void GameWindow::processEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			onSize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_KEYDOWN:
			break;
		case WM_KEYUP:
			break;
	}
}

void GameWindow::update(DWORD currentTime) {

}

void GameWindow::draw() {
	// Test drawing - some rectangles and triangles

	// Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Here goes the drawing code
	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);		glVertex3i(50,200,0);
		glColor3f(0.0,1.0,0.0);		glVertex3i(250,200,0);
		glColor3f(0.0,0.0,1.0);		glVertex3i(250,350,0);
		glColor3f(1.0,1.0,1.0);		glVertex3i(50,350,0);
	glEnd();

	glBegin(GL_TRIANGLES);
		glColor3f(1.0,0.0,0.0);  glVertex3i(400,350,0);
		glColor3f(0.0,1.0,0.0);  glVertex3i(500,200,0);
		glColor3f(0.0,0.0,1.0);  glVertex3i(600,350,0);
	glEnd();

	SwapBuffers(m_hDeviceContext);
}
