#ifndef LIGHT_H
#define LIGHT_H
#include <glm.hpp>
#include <dearimgui/imgui.h>

namespace engine
{
	class Light
	{
	public:
		#pragma region getters/setters
		void set_color(glm::vec3 _color)
		{
			color = glm::vec4(_color, 1.0f);
		}
		void set_color(ImVec4 _color)
		{
			color = glm::vec4(_color.x, _color.y, _color.z, _color.w);
		}
		glm::vec4 get_color()
		{
			return color;
		}
		#pragma endregion

	protected:
		#pragma region variables
		glm::vec4 color;
		#pragma endregion
	};

	class Point_Light : public virtual Light
	{
	public:
		#pragma region getters/setters
		void set_position(glm::vec4 _position)
		{
			position = _position;
		}
		void set_attenuation(float _constant, float _linear, float _quadratic)
		{
			constant = _constant;
			linear = _linear;
			quadratic = _quadratic;
		}

		glm::vec3 get_position()
		{
			return position;
		}
		float get_attenuation_constant()
		{
			return constant;
		}
		float get_attenuation_linear()
		{
			return linear;
		}
		float get_attenuation_quadratic()
		{
			return quadratic;
		}
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

	protected:
		#pragma region variables
		glm::vec4 position;

		//attenuation values
		float constant;
		float linear;
		float quadratic;
#pragma endregion
	};

	class Spot_Light : public Point_Light
	{
	public:
		#pragma region getters/setters
		void set_direction(glm::vec3 _direction)
		{
			direction = _direction;
		}
		glm::vec3 get_direction()
		{
			return direction;
		}
		void set_inner_cutoff(float _inner_cutoff)
		{
			inner_cutoff = _inner_cutoff;
		}
		float get_inner_cutoff()
		{
			return inner_cutoff;
		}
		void set_outer_cutoff(float _outer_cutoff)
		{
			outer_cutoff = _outer_cutoff;
		}
		float get_outer_cutoff()
		{
			return outer_cutoff;
		}
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

	private:
		#pragma region variables
		glm::vec3 direction;
		float inner_cutoff;
		float outer_cutoff;
		#pragma endregion
	};

	class Dir_Light : public virtual Light
	{
	public:
		#pragma region getters/setters
		void set_direction(glm::vec3 _direction)
		{
			direction = _direction;
		}
		glm::vec3 get_direction()
		{
			return direction;
		}
		void set_ambient(float _ambient)
		{
			ambient = _ambient;
		}
		float get_ambient()
		{
			return ambient;
		}
		#pragma endregion

		#pragma region constructors
		Dir_Light()
		{
			color = glm::vec4(1.0f);
			direction = glm::vec3(0.0f);
			ambient = 0.1f;
		}
		#pragma endregion

	private:
		#pragma region variables
		glm::vec3 direction;
		float ambient;
		#pragma endregion

	};
}

#endif