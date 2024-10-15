#include "TerminalBlockCharacterRenderWindow.h"
#include "../Plato/EventManager.h"
#include "RenderWindow.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <iostream>

TerminalBlockCharacterRenderWindow::TerminalBlockCharacterRenderWindow(const Eule::Vector2i& resolution, const std::string& name, const TorGL::PixelBuffer<3>* renderResultPixelBuffer) :
RenderWindow(resolution, name, renderResultPixelBuffer)
{
    // Disable canonical mode and echo
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void TerminalBlockCharacterRenderWindow::PollEvents()
{
    // Handle key presses
    const char key = GetKeyPressed();
    // Was a key actually pressed?
    if (key != -1) {
        // Yes: is it not the same character as in the last frame?
        if (key != keyLastFrame) {
            // Yes: Tell the event manager that the last frame key was released
            Input::EventManager::RegisterEventKeyUp((Input::KEY_CODE)keyLastFrame);

            // Also tell the event manager about the new keypress
            Input::EventManager::RegisterEventKeyDown((Input::KEY_CODE)key);
        }
        /* else if (key == keyPressed) {
         * // the key is already registered as down, and is still held down. Nothing to do.
         * // this commented cout piece of code is just for clarity that this is in fact NOT missing an else-if-branch!
         * }
        */
    }
    else if (keyLastFrame != -1) {
        // No key is pressed right now, but a key was just pressed in the last frame
        // Tell the event manager that it was released
        Input::EventManager::RegisterEventKeyUp((Input::KEY_CODE)keyLastFrame);
    }

    // Propagate the current pressed key into the last pressed key
    keyLastFrame = key;

    // TODO: Implement multiple simultanious keypresses per frame
    // TODO: Implement multiple keypresses per frame
    // TODO: Implement keypresses coming in immediately
    // (if i hold w, it types a w immediately, then waits, and only then types more w's. instead, it should write more w's immediately.)
    // TODO: Implement mouse movement and mouse delta
    // TODO: Catch and handle ctrl+c and call Close() instead of just quitting immediately
}

void TerminalBlockCharacterRenderWindow::RedrawWindow()
{
    std::cout << "\e[2J\e[H";

    for (std::size_t y = 0; y < resolution.y; y++) {
        for (std::size_t x = 0; x < resolution.x; x++) {
            const uint8_t* px = renderResultPixelBuffer->GetPixel(Vector2i(x, resolution.y - 1 - y));
            const uint8_t& r = *(px+0);
            const uint8_t& g = *(px+1);
            const uint8_t& b = *(px+2);

            std::cout << "\e[38;2;" << (int)r << ";" << (int)g << ";" << (int)b << "m" << "\u2588\u2588";
        }
        std::cout << std::endl;
    }
    std::cout << "\e[0m";
    std::cout.flush();
    usleep(10000);
}

void TerminalBlockCharacterRenderWindow::SetWindowTitle(const std::string& title)
{
    // Not implemented
}

void TerminalBlockCharacterRenderWindow::Close()
{
    RenderWindow::Close();

    // Re-enable canonical mode and echo
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

bool TerminalBlockCharacterRenderWindow::WasKeyPressed()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char TerminalBlockCharacterRenderWindow::GetKeyPressed()
{
    if (WasKeyPressed()) {
        char ch;
        read(STDIN_FILENO, &ch, 1);
        return ch;
    }
    return -1;
}

