/**
* @file InputMgr.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Input manager system. Partly taken from Diego Sanz's GAM300 input system.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "InputMgr.h"
#include "Graphics/Systems/Renderer.h"
#include <GLFW/glfw3.h>


namespace cs460
{
    InputMgr::InputMgr()
        :   m_currKeyIsPressed{false},
            m_prevKeyIsPressed{false},
            m_currMouseIsPressed{false},
            m_prevMouseIsPressed{false}
    {
    }

    InputMgr& InputMgr::get_instance()
    {
        static InputMgr instance;
        return instance;
    }



    // ------------------------ CALLBACK FUNCTIONS ----------------------------
    
    // Key callback function
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        unsigned ourIndex;
        InputMgr& inputMgr = InputMgr::get_instance();
        if (inputMgr.get_glfw_key_index(key, &ourIndex))
            inputMgr.set_key_status(ourIndex, action);      // action can only be released (0), pressed(1), or repeat(2)
    }


    // Mouse button callback function
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        InputMgr& inputMgr = InputMgr::get_instance();
        if (button >= 0 && button < NUMBER_OF_MOUSE_BUTTONS)
            inputMgr.set_mouse_button_status(button, action);
    }


    // Mouse position change callback
    void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
    {
        InputMgr& inputMgr = InputMgr::get_instance();
        inputMgr.set_mouse_position(glm::vec2((float)xPos, (float)yPos));
    }


    // Scrolling callback
    void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
    {
        InputMgr& inputMgr = InputMgr::get_instance();
        inputMgr.set_vertical_scroll(yOffset);
    }

    // ------------------------ CALLBACK FUNCTIONS ----------------------------



    // Set the callbacks for receiving key pressed events mouse events etc.
    bool InputMgr::initialize()
    {
        Renderer& renderer = Renderer::get_instance();
        GLFWwindow* window = renderer.get_window().get_handle();

        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetScrollCallback(window, scroll_callback);

        return true;
    }

    // Update previous and current arrays
    void InputMgr::update()
    {
        // Update the previous states of the keys
        for (unsigned i = 0; i < NUMBER_OF_KEYS; ++i)
            m_prevKeyIsPressed[i] = m_currKeyIsPressed[i];


        // Update the previous states of the mouse buttons
        for (unsigned i = 0; i < NUMBER_OF_MOUSE_BUTTONS; ++i)
            m_prevMouseIsPressed[i] = m_currMouseIsPressed[i];


        // Update the previous cursor position and the scroll
        m_prevMousePos = m_currMousePos;
        m_verticalScroll = 0.0f;


        // Update the any key/mouse button pressed flags
        m_anyKeyPressed = false;
        m_anyMouseButtonPressed = false;
    }

    // Does nothing for now
    void InputMgr::close()
    {

    }


    // True the first time the given key is pressed
    bool InputMgr::is_key_pressed(KeyId key) const
    {
        return !m_prevKeyIsPressed[key] && m_currKeyIsPressed[key];
    }

    // True the first time the given key is released
    bool InputMgr::is_key_released(KeyId key) const
    {
        return m_prevKeyIsPressed[key] && !m_currKeyIsPressed[key];
    }

    // True while the given key is pressed continuously (down)
    bool InputMgr::is_key_down(KeyId key) const
    {
        return m_prevKeyIsPressed[key] && m_currKeyIsPressed[key];
    }

    // True while the given key is not being touched (up)
    bool InputMgr::is_key_up(KeyId key) const
    {
        return !m_prevKeyIsPressed[key] && !m_currKeyIsPressed[key];
    }

    // True if any key has been pressed this frame
    bool InputMgr::is_any_key_pressed() const
    {
        return m_anyKeyPressed;
    }


    // Sets the value for key of the current isPressed vector
    void InputMgr::set_key_status(KeyId key, bool isPressed)
    {
        m_currKeyIsPressed[key] = isPressed;

        if (isPressed)
            m_anyKeyPressed = true;
    }


    // Converts the glfw key index to its corresponding index in the range of indices in KEYS namespace
    bool InputMgr::get_glfw_key_index(unsigned int glfwKey, unsigned int* idx) const
    {
        // Numbers
        if (glfwKey >= 48 && glfwKey <= 57)
            *idx = glfwKey - 48;

        // Letters
        else if (glfwKey >= 65 && glfwKey <= 90)
            *idx = glfwKey - 55;

        // Space bar
        else if (glfwKey == 32)
            *idx = KEYS::key_space;

        // Enter, arrow keys, escape
        else if (glfwKey >= 256 && glfwKey <= 265)
            *idx = glfwKey - 219;

        // Left shift, control, alt
        else if (glfwKey >= 340 && glfwKey <= 342)
            *idx = glfwKey - 293;

        // Right shift, control, alt
        else if (glfwKey >= 344 && glfwKey <= 346)
            *idx = glfwKey - 294;

        // Function keys
        else if (glfwKey >= 290 && glfwKey <= 301)
            *idx = glfwKey - 237;

        // The key is not in the container
        else
            return false;

        // The key is in the container
        return true;
    }



    // True the first time the given mouse button is pressed
    bool InputMgr::is_mouse_button_pressed(MouseButtonId button) const
    {
        return !m_prevMouseIsPressed[button] && m_currMouseIsPressed[button];
    }

    // True the first time the given mouse button is released
    bool InputMgr::is_mouse_button_released(MouseButtonId button) const
    {
        return m_prevMouseIsPressed[button] && !m_currMouseIsPressed[button];
    }

    // True while the given mouse button is pressed continuously (down)
    bool InputMgr::is_mouse_button_down(MouseButtonId button) const
    {
        return m_prevMouseIsPressed[button] && m_currMouseIsPressed[button];
    }

    // True while the given mouse button is not being touched (up)
    bool InputMgr::is_mouse_button_up(MouseButtonId button) const
    {
        return !m_prevMouseIsPressed[button] && !m_currMouseIsPressed[button];
    }

    // True if any mouse button has been pressed this frame
    bool InputMgr::is_any_mouse_button_pressed() const
    {
        return m_anyMouseButtonPressed;
    }


    // Sets the value for button of the current mouse button isPressed vector
    void InputMgr::set_mouse_button_status(MouseButtonId button, bool isPressed)
    {
        m_currMouseIsPressed[button] = isPressed;

        if (isPressed)
            m_anyMouseButtonPressed = true;
    }


    // Returns the current mouse position in window coordinates
    glm::vec2 InputMgr::get_mouse_position() const
    {
        return m_currMousePos;
    }

    // Returns the not normalized direction of movement of the cursor this frame
    glm::vec2 InputMgr::get_mouse_direction() const
    {
        return m_currMousePos - m_prevMousePos;
    }

    // Returns the wheel scroll factor
    float InputMgr::get_vertical_scroll() const
    {
        return m_verticalScroll;
    }

    // Returns true if the mouse scroll wheel is being used
    bool InputMgr::is_scrolling() const
    {
        return m_verticalScroll != 0;
    }


    void InputMgr::set_mouse_position(const glm::vec2& newPos)
    {
        m_currMousePos = newPos;
    }

    void InputMgr::set_vertical_scroll(float newScroll)
    {
        m_verticalScroll = newScroll;
    }
}