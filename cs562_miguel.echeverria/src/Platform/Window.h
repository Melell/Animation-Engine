/**
* @file Window.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Has window related functionality (window creation, destruction etc).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

struct GLFWwindow;


namespace cs460
{
	class Window
	{
	public:

		friend class Renderer;	// Only Renderer can create instances of Window

		~Window();

		bool create();		// Creates a window and its associated opengl context
		void update();		// Swaps the buffers, clears them, and polls for events
		void close();		// Terminates glfw, thus destroying the window
		
		// 1/0 -> Use/not use v-sync
		void set_swap_interval(int interval);

		bool get_window_should_close() const;		// Returns true if the window has been requested to be closed (for example the red x has been pressed)
		void set_window_should_close(int value);	// Set the glfw flag that indicates that the window is requested to be closed

		GLFWwindow* get_handle() const;				// Get the glfw handle of the window

		int get_window_width() const;
		int get_window_height() const;
		void get_window_dimensions(int* outWidth, int* outHeight) const;

		int get_framebuffer_width() const;
		int get_framebuffer_height() const;
		void get_framebuffer_dimensions(int* outWidth, int* outHeight) const;

	private:

		GLFWwindow* m_handle;

		void set_window_hints();	// Set window options before it is created

		Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	};


	void on_framebuffer_resize(GLFWwindow* window, int newWidth, int newHeight);
}