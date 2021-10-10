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
		std::string name;
		//glfw key code
		int key_code;

		Button(std::string _name, int _key_code)
		{
			name = _name;
			key_code = _key_code;
		}
	};

	//singleton class
	class InputManager
	{
	public:
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
		void add_button(std::string& name, int glfw_key_macro)
		{
			button_states.push_back(Button(name, glfw_key_macro));
		}
		void set_buttons(std::vector<Button> _buttons)
		{
			button_states = _buttons;
		}
		//every time a key is pressed and GLFW raises the key pressed event, this function gets called
		void instance_key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods)
		{
			//check each of the buttons that are in our button_names map and if the code of
			//the key that raised the event matches one of our buttons...
			for (auto iter = button_states.begin(); iter != button_states.end(); iter++)
			{
				if (iter->key_code == key_code)
				{
					//then set it's up/down/held states accordingly
					//if the button was held down at all this frame
					if (action == GLFW_PRESS || action == GLFW_REPEAT)
					{
						//if it wasnt pressed down last frame
						if (!iter->down)
						{
							iter->down = true;
						}
						//it was pressed down last frame, so set down back to false
						else
						{
							iter->down = false;
						}
						iter->held = true;
					}
					//if it was released this frame
					else if (action == GLFW_RELEASE)
					{
						iter->up = true;
						iter->held = false;
						//pretty sure this is impossible but fuck it
						iter->down = false;
					}
				}
			}

			for (auto& button : button_states)
			{
				std::cout << button.name << " :: " << button.down << std::endl;
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
		//a list of all the buttons we care about and their current state
		std::vector<Button> button_states = {};
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