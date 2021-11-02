/**
* @file InputMgr.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Input manager system. Partly taken from Diego Sanz's GAM300 input system.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	using KeyId = const unsigned;
	namespace KEYS
	{
        // GLFW : 48 - 57
        KeyId key_0 = 0;
        KeyId key_1 = 1;
        KeyId key_2 = 2;
        KeyId key_3 = 3;
        KeyId key_4 = 4;
        KeyId key_5 = 5;
        KeyId key_6 = 6;
        KeyId key_7 = 7;
        KeyId key_8 = 8;
        KeyId key_9 = 9;


        // GLFW : 65 - 90
        KeyId key_a = 10;
        KeyId key_b = 11;
        KeyId key_c = 12;
        KeyId key_d = 13;
        KeyId key_e = 14;
        KeyId key_f = 15;
        KeyId key_g = 16;
        KeyId key_h = 17;
        KeyId key_i = 18;
        KeyId key_j = 19;
        KeyId key_k = 20;
        KeyId key_l = 21;
        KeyId key_m = 22;
        KeyId key_n = 23;
        KeyId key_o = 24;
        KeyId key_p = 25;
        KeyId key_q = 26;
        KeyId key_r = 27;
        KeyId key_s = 28;
        KeyId key_t = 29;
        KeyId key_u = 30;
        KeyId key_v = 31;
        KeyId key_w = 32;
        KeyId key_x = 33;
        KeyId key_y = 34;
        KeyId key_z = 35;

        // GLFW : 32
        KeyId key_space = 36;

        // GLFW : 256 - 265
        KeyId key_escape = 37;
        KeyId key_enter = 38;
        KeyId key_tab = 39;
        KeyId key_backspace = 40;
        KeyId key_insert = 41;
        KeyId key_delete = 42;
        KeyId key_right = 43;
        KeyId key_left = 44;
        KeyId key_down = 45;
        KeyId key_up = 46;

        // GLFW : 340 - 342
        KeyId key_leftShift = 47;
        KeyId key_leftControl = 48;
        KeyId key_leftAlt = 49;

        // GLFW : 344 - 346
        KeyId key_rightShift = 50;
        KeyId key_rightControl = 51;
        KeyId key_rightAlt = 52;

        // GLFW : 290 - 301
        KeyId key_f1 = 53;
        KeyId key_f2 = 54;
        KeyId key_f3 = 55;
        KeyId key_f4 = 56;
        KeyId key_f5 = 57;
        KeyId key_f6 = 58;
        KeyId key_f7 = 59;
        KeyId key_f8 = 60;
        KeyId key_f9 = 61;
        KeyId key_f10 = 62;
        KeyId key_f11 = 63;
        KeyId key_f12 = 64;
	}

	const unsigned NUMBER_OF_KEYS = 65;



    using MouseButtonId = const unsigned;
    namespace MOUSE
    {
        // GLFW: : 0 - 2
        MouseButtonId button_left = 0;
        MouseButtonId button_right = 1;
        MouseButtonId button_middle = 2;
    }

    const unsigned NUMBER_OF_MOUSE_BUTTONS = 3;



	class InputMgr
	{
	public:

        static InputMgr& get_instance();

        bool initialize();                    // Set the callbacks for receiving key pressed events mouse events etc.
        void update();                        // Update previous and current arrays
        void close();                         // Does nothing for now


        // ------------------------------------------------------ KEYBOARD ------------------------------------------------------
        bool is_key_pressed(KeyId key) const;                               // True the first time the given key is pressed
        bool is_key_released(KeyId key) const;                              // True the first time the given key is released
        bool is_key_down(KeyId key) const;                                  // True while the given key is pressed continuously (down)
        bool is_key_up(KeyId key) const;                                    // True while the given key is not being touched (up)
        bool is_any_key_pressed() const;                                    // True if any key has been pressed or is down this frame

        void set_key_status(KeyId key, bool isPressed);                     // Sets the value for key of the current key isPressed vector

        // Converts the glfw key index to its corresponding index in the range of indices in KEYS namespace
        bool get_glfw_key_index(unsigned int glfwKey, unsigned int* idx) const;
        // ------------------------------------------------------ KEYBOARD ------------------------------------------------------


        // ------------------------------------------------------- MOUSE --------------------------------------------------------
        bool is_mouse_button_pressed(MouseButtonId button) const;           // True the first time the given mouse button is pressed
        bool is_mouse_button_released(MouseButtonId button) const;          // True the first time the given mouse button is released
        bool is_mouse_button_down(MouseButtonId button) const;              // True while the given mouse button is pressed continuously (down)
        bool is_mouse_button_up(MouseButtonId button) const;                // True while the given mouse button is not being touched (up)
        bool is_any_mouse_button_pressed() const;                           // True if any mouse button has been pressed (not down) this frame

        void set_mouse_button_status(MouseButtonId button, bool isPressed); // Sets the value for button of the current mouse button isPressed vector

        glm::vec2 get_mouse_position() const;                               // Returns the current mouse position in window coordinates
        glm::vec2 get_mouse_direction() const;                              // Returns the not normalized direction of movement of the cursor this frame
        float get_vertical_scroll() const;                                  // Returns the wheel scroll factor
        bool is_scrolling() const;                                          // Returns true if the mouse scroll wheel is being used
        // ------------------------------------------------------- MOUSE --------------------------------------------------------


        void set_mouse_position(const glm::vec2& newPos);
        void set_vertical_scroll(float newScroll);

	private:
		bool m_currKeyIsPressed[NUMBER_OF_KEYS];
		bool m_prevKeyIsPressed[NUMBER_OF_KEYS];
        bool m_anyKeyPressed = false;

        bool m_currMouseIsPressed[NUMBER_OF_MOUSE_BUTTONS];
        bool m_prevMouseIsPressed[NUMBER_OF_MOUSE_BUTTONS];
        bool m_anyMouseButtonPressed = false;

        glm::vec2 m_currMousePos;
        glm::vec2 m_prevMousePos;
        float m_verticalScroll;


        InputMgr();
        InputMgr(const InputMgr&) = delete;
        InputMgr& operator=(const InputMgr&) = delete;
	};
}