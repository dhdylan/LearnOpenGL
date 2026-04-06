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
		unsigned int vao;
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
	};

	class Point_Light : public virtual Light
	{
	public:
		#pragma region variables
		glm::vec3 position;
		engine::Shader& light_shader;
		//attenuation values
		float constant;
		float linear;
		float quadratic;
		#pragma endregion

		#pragma region constructor
		Point_Light(engine::Shader& _light_shader) : Light(), light_shader(_light_shader)
		{
			color = glm::vec4(1.0);
			position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			constant = 1.0f;
			linear = .07f;
			quadratic = .032f;
		}

		Point_Light() = delete;
		#pragma endregion

		#pragma region instance methods
		void draw(engine::Camera& camera)
		{
			light_shader.use();
			light_shader.setMat4("u_projection", camera.get_projection_matrix());
			light_shader.setMat4("u_view", camera.get_view_matrix());
			light_shader.setMat4("u_model", glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(0.3f)));
			light_shader.setVec3("u_lightColor", color);

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
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
		Spot_Light(engine::Shader& _light_shader) : Point_Light(_light_shader)
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

		Spot_Light() = delete;
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