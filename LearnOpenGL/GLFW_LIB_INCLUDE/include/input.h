#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

namespace engine
{
	enum ButtonState {
		held,
		down,
		up
	};
	class Button
	{
	public:
		//button is down at all
		bool held = false;
		//button was pressed this frame
		bool down = false;
		//button was released this frame
		bool up = false;
		//glfw key code
		int key_code;
		ButtonState button_state = ButtonState::up;

		Button(int _key_code)
		{
			key_code = _key_code;
		}
	};

	//singleton class
	class InputManager
	{
	public:
		//variables
		//---------
		//a list of all the buttons we care about and their current state
		std::map<std::string, Button> buttons = {};
		glm::vec2 mouse_pos;
		glm::vec2 mouse_offset;
		glm::vec2 scroll_offset;
		bool first_frame;

#pragma region static methods
		//static methods
		// -------------
		//this is to get a reference to the singleton instance. note that the button_states and window members must be set in order to use this object.
		static InputManager* getptr();
		//static method that calls the key_callback implementation on the singleton instance
		static void static_key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods);
		//static method that calls the instance_mouse_callback
		static void static_mouse_callback(GLFWwindow*, double x_pos, double y_pos);
		//callback for the scroll wheel
		static void static_scroll_callback(GLFWwindow* window, double x_scroll, double y_scroll);
		//set whether or not the mouse should move around (exist) or not
		static void set_cursor_mode(int glfw_cursor_state);
#pragma endregion

#pragma region instance methods
		//instance methods

		//adds a button to our buttons vector
		void add_button(std::string name, int glfw_key_macro)
		{
			buttons.insert({ name, Button(glfw_key_macro) });
		}

		//every time a key is pressed and GLFW raises the key pressed event, this function gets called
		void instance_key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods)
		{
			for (auto iter = buttons.begin(); iter != buttons.end(); iter++)
			{
				if (iter->second.key_code == key_code)
				{
					//then set it's up/down/held states accordingly

					//if the button was held down at all this frame
					if (action == GLFW_PRESS || action == GLFW_REPEAT)
					{
						//if it wasnt pressed down last frame
						if (!iter->second.down && !iter->second.held)
						{
							iter->second.down = true;
							iter->second.button_state = ButtonState::down;
						}
						//it was pressed down last frame, so set down back to false
						else
						{
							iter->second.down = false;
							iter->second.button_state = ButtonState::held;
						}
						iter->second.up = false;
						iter->second.held = true;
					}
					//if it was released this frame
					else if (action == GLFW_RELEASE)
					{
						iter->second.button_state = ButtonState::up;
						iter->second.up = true;
						iter->second.held = false;
						//pretty sure this is impossible but fuck it
						iter->second.down = false;
					}
				}
			}

			for (auto& button : buttons)
			{
				std::cout << "Button: " << button.first << "\t::   Down: " << button.second.down << "\t::   Held: " << button.second.held << "\t::   Released: " << button.second.up << std::endl;
			}
			std::cout << std::endl;

		}
		//every time the mouse moves this function is called
		void instance_mouse_callback(GLFWwindow*, double x_pos, double y_pos)
		{
			if (first_frame)
			{
				mouse_pos.x = (float)x_pos;
				mouse_pos.y = (float)y_pos;
				first_frame = false;
			}
			mouse_offset = glm::vec2(x_pos - mouse_pos.x, mouse_pos.y - y_pos);
			mouse_pos = glm::vec2(x_pos, y_pos);
		}
		//every time the scroll wheel is "rolled" or whatever you want to call it, this is called. keep in mind that this is not called every frame, as are the rest of the functions. it is only called when the event is raised.
		void instance_scroll_callback(GLFWwindow* window, double x_scroll, double y_scroll)
		{
			scroll_offset.x = x_scroll;
			scroll_offset.y = y_scroll;
		}
		//set the window pointer so you can actually use this object
		void setWindow(GLFWwindow* _window)
		{
			window = _window;
		}

		bool get_mouse_enabled_status()
		{
			return mouse_enabled;
		};
#pragma endregion

	private:
#pragma region variables
		//variables
		GLFWwindow* window;
		inline static InputManager* instance;
		bool mouse_enabled;
#pragma endregion

#pragma region constructors
		//constructors
		//------------
		//i guess i dont need these anymore since this class is now a singleton........
		InputManager() {}
		InputManager(InputManager const& other) = delete;
		InputManager(InputManager const&& other) = delete;
		void operator=(InputManager const&) = delete;
#pragma endregion
	};
#pragma region static definitions
	InputManager* InputManager::getptr()
	{
		if (instance == nullptr)
		{
			instance = new InputManager();
			instance->first_frame = true;
		}
		return instance;
	}

	void InputManager::static_key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods)
	{
		getptr()->instance_key_callback(window, key_code, scancode, action, mods);
	}

	void InputManager::static_mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
	{
		getptr()->instance_mouse_callback(window, x_pos, y_pos);
	}

	void InputManager::static_scroll_callback(GLFWwindow* window, double x_scroll, double y_scroll)
	{
		getptr()->instance_scroll_callback(window, x_scroll, y_scroll);
	}

	void InputManager::set_cursor_mode(int glfw_cursor_state)
	{
		if (glfw_cursor_state == GLFW_CURSOR_NORMAL)
		{
			glfwSetInputMode(getptr()->window, GLFW_CURSOR, glfw_cursor_state);
			getptr()->mouse_enabled = true;
		}
		else if(glfw_cursor_state == GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(getptr()->window, GLFW_CURSOR, glfw_cursor_state);
			getptr()->mouse_enabled = false;
		}
		else
		{
			std::cerr << "\"InputManager::set_cursor_mode\" || Invalid GLFW int" << std::endl;
		}
	}
#pragma endregion
}

#endif