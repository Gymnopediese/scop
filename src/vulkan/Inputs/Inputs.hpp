
#pragma once

#include "lib.hpp"
#include "InputEvent.hpp"



class Inputs
{
    public:

        static float mouse_x;
        static float mouse_y;

        static std::vector<InputEvent>  events;
        static void clear();
        static void keyCallback(GLFWwindow*, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow*, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow*, double x, double y);
        static void scrollCallback(GLFWwindow*, double dx, double dy);

        static bool is_action_just_pressed(int key)
        {
            // std::cout << (InputEvent::keystates[key] == InputEvent::PRESSED) << std::endl;
            return InputEvent::keystates[key] == InputEvent::PRESSED;
        };

        static bool is_action_pressed(int key)
        {
            return InputEvent::keystates[key] == InputEvent::PRESSED || InputEvent::keystates[key] == InputEvent::HOLD;
        };

        static bool is_action_released(int key)
        {
            return InputEvent::keystates[key] == InputEvent::RELEASED;
        };
};