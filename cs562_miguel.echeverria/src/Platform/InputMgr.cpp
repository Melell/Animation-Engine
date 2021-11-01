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
            m_prevKeyIsPressed{false}
    {
    }

    InputMgr& InputMgr::get_instance()
    {
        static InputMgr instance;
        return instance;
    }



    // ------------------------ CALLBACK FUNCTIONS ----------------------------
    
    // Set the callbacks for receiving key pressed events mouse events etc.
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        unsigned ourIndex;
        InputMgr& inputMgr = InputMgr::get_instance();
        if (inputMgr.get_glfw_key_index(key, &ourIndex))
            inputMgr.set_key_status(ourIndex, action);      // action can only be released (0), pressed(1), or repeat(2)
    }

    // ------------------------ CALLBACK FUNCTIONS ----------------------------



    // Set the callbacks for receiving key pressed events mouse events etc.
    bool InputMgr::initialize()
    {
        Renderer& renderer = Renderer::get_instance();

        glfwSetKeyCallback(renderer.get_window().get_handle(), key_callback);

        return true;
    }

    // Update previous and current arrays
    void InputMgr::update()
    {
        // Update the previous states
        for (unsigned i = 0; i < NUMBER_OF_KEYS; ++i)
            m_prevKeyIsPressed[i] = m_currKeyIsPressed[i];
    }

    // Does nothing for now
    void InputMgr::close()
    {

    }


    // True the first time the given key is pressed
    bool InputMgr::is_key_pressed(KeyId key)
    {
        return !m_prevKeyIsPressed[key] && m_currKeyIsPressed[key];
    }

    // True the first time the given key is released
    bool InputMgr::is_key_released(KeyId key)
    {
        return m_prevKeyIsPressed[key] && !m_currKeyIsPressed[key];
    }

    // True while the given key is pressed continuously (down)
    bool InputMgr::is_key_down(KeyId key)
    {
        return m_prevKeyIsPressed[key] && m_currKeyIsPressed[key];
    }

    // True while the given key is not being touched (up)
    bool InputMgr::is_key_up(KeyId key)
    {
        return !m_prevKeyIsPressed[key] && !m_currKeyIsPressed[key];
    }


    // Sets the value for key of the current isPressed vector
    void InputMgr::set_key_status(KeyId key, bool isPressed)
    {
        m_currKeyIsPressed[key] = isPressed;
    }


    // Converts the glfw key index to its corresponding index in the range of indices in KEYS namespace
    bool InputMgr::get_glfw_key_index(unsigned int glfwKey, unsigned int* idx)
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
}