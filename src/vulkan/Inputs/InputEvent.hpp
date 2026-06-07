# pragma once

#include "lib.hpp"


class InputEvent {
    public:
        static int   keystates[GLFW_KEY_LAST + 1];
        enum EventType {
            Key,
            MouseButton,
            MouseMove,
            Scroll
        };
        enum ActionType {
            NONE,
            PRESSED,
            HOLD,
            RELEASED,
        };

        EventType type;
        ActionType action;

        int key;

        double x;
        double y;
        InputEvent();
        InputEvent(EventType type, int key, int action);

        InputEvent(EventType type, double x, double y);

        ~InputEvent();

        bool is_action_just_pressed(int key);

        bool is_action_pressed(int key);

        bool is_action_released(int key);





};
