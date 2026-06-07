
#include "InputEvent.hpp"



int InputEvent::keystates[GLFW_KEY_LAST + 1];

InputEvent::InputEvent(){

}

InputEvent::InputEvent(EventType type, int key, int action) : type(type), key(key) {

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (keystates[key] == PRESSED || keystates[key] == HOLD)
            this->action = HOLD;
        else
            this->action = PRESSED;
    }
    if (action == GLFW_RELEASE)
    {
        this->action = RELEASED;
    }
    keystates[key] = this->action;

    if (type == Key)
    {
        std::cout <<"[KEY] create" << std::endl;

    }
}

InputEvent::InputEvent(EventType type, double x, double y) : type(type), x(x), y(y) {

}

bool InputEvent::is_action_just_pressed(int key)
{
    return key == this->key && action == PRESSED;
}

bool InputEvent::is_action_pressed(int key)
{
    return key == this->key && (action == PRESSED || action == HOLD);
}

bool InputEvent::is_action_released(int key)
{
    return key == this->key && (action == RELEASED);
}

InputEvent::~InputEvent()
{

}