#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <Windows.h>
#include "GL/gl.h"

/**
 * The GameWindow class wraps the window handle and deals with graphics initialization.
 * This class also processes event messages.
 */
class GameWindow {
public:
	GameWindow(int width, int height, bool fullscreen);
	~GameWindow();

	void update(DWORD currentTime);
	void draw();

private:
	bool m_bFullscreen;

	HWND	m_hWindow;				// Window handle
	HDC		m_hDeviceContext;		// Window device context
	HGLRC	m_hGlContext			// OpenGL Context
	
	void registerWindowClass();
	void createGraphicsContext();
	void initGraphics();

	void onSize(GLsizei width, GLsizei height);
	static LRESULT CALLBACK onEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void processEvent(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif
