#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include <light.h>
#include <material.h>
#include <geometry.h>
#include <glm.hpp>

namespace engine
{
	class Cube_Object
	{
	public:
		#pragma region getters/setters
		void set_geometry(engine::Geometry _geometry)
		{
			geometry = _geometry;
		}
		engine::Geometry& get_geometry()
		{
			return geometry;
		}
		void set_material(engine::Lit_Textured_Material _material)
		{
			material = _material;
		}
		engine::Lit_Textured_Material& get_material()
		{
			return material;
		}
		void set_position(glm::vec3 _position)
		{
			position = _position;
		}
		glm::vec3 get_position()
		{
			return position;
		}
		void set_rotation(glm::vec3 _rotation)
		{
			rotation = _rotation;
		}
		glm::vec3 get_rotation()
		{
			return rotation;
		}
		#pragma endregion

		#pragma region instance methods
		void draw(engine::Dir_Light& dir_light, std::vector<engine::Point_Light>& point_lights, std::vector<engine::Spot_Light>& spot_lights, engine::Camera& user_camera)
		{
			engine::Shader& shader = *material.get_shader_ptr();
			shader.use();

			material.send_material_to_shader();
			shader.setMat4("u_projection", user_camera.get_projection_matrix());
			shader.setMat4("u_view", user_camera.get_view_matrix());
			shader.setVec3("u_viewPos", user_camera.get_position());

			#pragma region lights
			shader.setVec3("u_dirLight.direction", dir_light.get_direction());
			shader.setVec3("u_dirLight.ambient", dir_light.get_ambient() * dir_light.get_color());
			shader.setVec3("u_dirLight.diffuse", dir_light.get_color());
			shader.setVec3("u_dirLight.specular", glm::normalize(glm::vec3(1.5f) * glm::vec3(dir_light.get_color())));

			//point lights
			for (int i = 0; i < point_lights.size(); i++)
			{
				shader.setVec3("u_pointLight[" + std::to_string(i) + "].position", point_lights[i].get_position());
				shader.setVec3("u_pointLight[" + std::to_string(i) + "].diffuse", point_lights[i].get_color());
				shader.setVec3("u_pointLight[" + std::to_string(i) + "].specular", glm::normalize(glm::vec3(1.5f) * glm::vec3(point_lights[i].get_color())));
				shader.setFloat("u_pointLight[" + std::to_string(i) + "].constant", point_lights[i].get_attenuation_constant());
				shader.setFloat("u_pointLight[" + std::to_string(i) + "].linear", point_lights[i].get_attenuation_linear());
				shader.setFloat("u_pointLight[" + std::to_string(i) + "].quadratic", point_lights[i].get_attenuation_quadratic());
			}

			//spot lights
			for (int i = 0; i < spot_lights.size(); i++)
			{
				shader.setVec3("u_spotLight[i].direction", spot_lights[i].get_direction());
				shader.setVec3("u_spotLight[i].position", spot_lights[i].get_position());
				shader.setVec3("u_spotLight[i].diffuse", spot_lights[i].get_color());
				shader.setVec3("u_spotLight[i].specular", glm::normalize(glm::vec3(1.5f) * glm::vec3(spot_lights[i].get_color())));
				shader.setFloat("u_spotLight[i].innerCutoff", glm::cos(glm::radians(spot_lights[i].get_inner_cutoff())));
				shader.setFloat("u_spotLight[i].outerCutoff", glm::cos(glm::radians(spot_lights[i].get_outer_cutoff())));
				shader.setFloat("u_spotLight[i].constant", spot_lights[i].get_attenuation_constant());
				shader.setFloat("u_spotLight[i].linear", spot_lights[i].get_attenuation_linear());
				shader.setFloat("u_spotLight[i].quadratic", spot_lights[i].get_attenuation_quadratic());
			}
			#pragma endregion

			glm::mat4 model(1.0f);
			model = glm::translate(model, position);
			/*    not gonna do rotation for now. I need to understand eulers and quaternions better first
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			*/
			shader.setMat4("u_model", model);

			glBindVertexArray(geometry.get_vao());
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		#pragma endregion

		#pragma region constructor
		Cube_Object()
		{
			position = glm::vec3(0.0f);
			rotation = glm::vec3(0.0f);
		}
		#pragma endregion


	private:
		#pragma region variables
		engine::Geometry geometry;
		engine::Lit_Textured_Material material;
		glm::vec3 position;
		glm::vec3 rotation;
		#pragma endregion

	};
}
#endif