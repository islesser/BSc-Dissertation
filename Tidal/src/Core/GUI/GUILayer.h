#ifndef GUILAYER_H
#define GUILAYER_H

// Include: ImGUI
#include "Headers/ImGUI.hpp"

#include "../Window/Window.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// Encapsulate the debug GUI interface, based on ImGUI.
// Allows display and adjustment of set variables.
class GUILayer
{
private:
    std::vector<std::function<void()>> windows; // Display elements
    ImGuiIO *io; // ImGUI context
    GLFWwindow *window; // Rendering context

public:
    GUILayer() {};
    virtual ~GUILayer();

    void constantElements();
    void addElement(std::function<void()> window);
    bool init(GLFWwindow *_window);
    void render();

    void begin();
    void end();
};

#endif /* GUILAYER_H */
