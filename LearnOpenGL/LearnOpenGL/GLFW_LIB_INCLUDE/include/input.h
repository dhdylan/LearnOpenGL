#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
namespace myAPI
{
	class button_state
	{
	public:
		bool pressed;
		bool down;
		bool up;
		std::string name;

		button_state(std::string _name)
		{
			this->name = _name;
		}
	};

	class Input
	{
	public:
		std::vector<button_state> button_states = {};
		std::map<std::string, int> button_names = {};

		Input(GLFWwindow* _window, std::map<std::string, int> _buttons = {})
		{
			this->window = _window;
			//if we have a buttons map
			if (!_buttons.empty())
			{
				for (auto iter = _buttons.begin(); iter != _buttons.end(); iter++)
				{
					this->button_names.insert({ iter->first, iter->second });
				}
			}
		}

		void add_button(std::string& name, int glfw_key_macro)
		{
			this->button_names.insert({ name, glfw_key_macro });
			this->button_states.push_back(button_state(name));
		}


		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			std::cout << "test" << std::endl;
		}

	private:

		GLFWwindow* window;
	};
}

#endif