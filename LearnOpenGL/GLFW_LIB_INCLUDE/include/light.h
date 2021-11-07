#ifndef LIGHT_H
#define LIGHT_H
#include <glm.hpp>
#include <dearimgui/imgui.h>

namespace engine
{
	class Light
	{
	public:
		#pragma region variables
		glm::vec4 color;
		#pragma endregion

		#pragma region getters/setters
		void set_color_ImGui(ImVec4 _color)
		{
			color = glm::vec4(_color.x, _color.y, _color.z, _color.w);
		}
		ImVec4 get_color_ImGui()
		{
			return ImVec4(color.x, color.y, color.z, color.w);
		}
		#pragma endregion

	protected:
	};

	class Point_Light : public virtual Light
	{
	public:
		#pragma region variables
		glm::vec4 position;

		//attenuation values
		float constant;
		float linear;
		float quadratic;
		#pragma endregion

		#pragma region constructor
		Point_Light()
		{
			color = glm::vec4(1.0);
			position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			constant = 1.0f;
			linear = .07f;
			quadratic = .032f;
		}
		#pragma endregion
	};

	class Spot_Light : public Point_Light
	{
	public:
		#pragma region variables
		glm::vec3 direction;
		float inner_cutoff;
		float outer_cutoff;
		#pragma endregion

		#pragma region constructor
		Spot_Light()
		{
			color = glm::vec4(1.0f);
			position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			direction = glm::vec3(0.0f);
			inner_cutoff = 6.5f;
			outer_cutoff = 12.0f;
			constant = 1.0f;
			linear = .07f;
			quadratic = .032f;
		}
		#pragma endregion
	};

	class Dir_Light : public virtual Light
	{
	public:
		#pragma region variables
		glm::vec3 direction;
		float ambient;
		#pragma endregion

		#pragma region constructors
		Dir_Light()
		{
			color = glm::vec4(1.0f);
			direction = glm::vec3(0.2f, -0.8f, 0.1f);
			ambient = 0.1f;
		}
		#pragma endregion
	};
}

#endif