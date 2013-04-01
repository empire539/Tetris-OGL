#include "GameWindow.h"

#define WINDOW_NAME "Tetris OGL"

/** Constructor. */
GameWindow::GameWindow(int width, int height, bool fullscreen)
	:m_hWindow(NULL), 
	 m_hDeviceContext(NULL), 
	 m_hGlContext(NULL), 
	 m_bFullscreen(fullscreen) {

	registerWindowClass();


}

/** Deconstructor. */
GameWindow::~GameWindow() {

}

void GameWindow::registerWindowClass() {

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
