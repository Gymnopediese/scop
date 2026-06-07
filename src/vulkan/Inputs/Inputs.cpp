#include "Inputs.hpp"


std::vector<InputEvent>  Inputs::events;
float Inputs::mouse_x;
float Inputs::mouse_y;

void Inputs::clear() {
    events.clear();

    for (int i = 0; i < GLFW_KEY_LAST; i++)
    {
        if (InputEvent::keystates[i] == InputEvent::PRESSED)
        {
            InputEvent::keystates[i] = InputEvent::HOLD;
        }
        if (InputEvent::keystates[i] == InputEvent::RELEASED)
        {
            InputEvent::keystates[i] = InputEvent::NONE;
        }
    }
}

void Inputs::keyCallback(GLFWwindow*, int key, int scancode, int action, int mods) {
    // std::cout << "[INPUT] new key" << std::endl;
    events.push_back(InputEvent(InputEvent::Key, key, action));
}

void Inputs::mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
    events.push_back({
        InputEvent::MouseButton,
        button,
        action
    });
}

void Inputs::cursorPosCallback(GLFWwindow*, double x, double y) {
    InputEvent e{};
    e.type = InputEvent::MouseMove;
    e.x = x;
    e.y = y;
    mouse_x = x;
    mouse_y = y;
    events.push_back(e);
}

void Inputs::scrollCallback(GLFWwindow*, double dx, double dy)
{
    events.push_back(InputEvent(InputEvent::Scroll, dx,dy));
};