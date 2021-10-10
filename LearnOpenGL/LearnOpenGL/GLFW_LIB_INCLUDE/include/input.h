#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

namespace engine
{
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
#pragma region static methods
		//static methods
		// -------------
		//this is to get a reference to the singleton instance. note that the button_states and window members must be set in order to use this object.
		static InputManager* getptr();
		//static method that calls the key_callback implementation on the singleton instance
		static void static_key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods);
#pragma endregion

#pragma region instance methods
		//instance methods
		//----------------
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
						if (!iter->second.down)
						{
							iter->second.down = true;
						}
						//it was pressed down last frame, so set down back to false
						else
						{
							iter->second.down = false;
						}
						iter->second.held = true;
					}
					//if it was released this frame
					else if (action == GLFW_RELEASE)
					{
						iter->second.up = true;
						iter->second.held = false;
						//pretty sure this is impossible but fuck it
						iter->second.down = false;
					}
				}
			}

			for (auto& button : buttons)
			{
				std::cout << button.first << " :: " << button.second.down << std::endl;
			}
			std::cout << std::endl;

		}
		//set the window pointer so you can actually use this object
		void setWindow(GLFWwindow* _window)
		{
			window = _window;
		}
#pragma endregion

	private:
#pragma region variables
		//variables
		//---------
		GLFWwindow* window;
		inline static InputManager* instance;
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
		}
		return instance;
	}

	void InputManager::static_key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods)
	{
		getptr()->instance_key_callback(window, key_code, scancode, action, mods);
	}
#pragma endregion
}

#endif