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
		winStyle - WS_POPUP;

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
	
}

void GameWindow::initGraphics() {

}

void GameWindow::onSize() {

}

LRESULT GameWindow::onEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

}

void GameWindow::processEvent(UINT message, WPARAM wParam, LPARAM lParam) {

}

void GameWindow::update() {

}

void GameWindow::draw() {

}
