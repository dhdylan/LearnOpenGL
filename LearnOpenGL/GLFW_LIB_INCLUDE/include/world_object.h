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
		#pragma region variables
		const int vao;
		engine::Lit_Textured_Material* material;
		glm::vec3 position;
		glm::vec3 rotation;
		#pragma endregion

		#pragma region instance methods
		void draw_object(
			engine::Dir_Light& dir_light,
			std::vector<engine::Point_Light>& point_lights,
			std::vector<engine::Spot_Light>& spot_lights,
			engine::Camera& user_camera
		)
		{
			engine::Shader& shader = material->shader;
			shader.use();

			material->send_material_to_shader();
			shader.setMat4("u_projection", user_camera.get_projection_matrix());
			shader.setMat4("u_view", user_camera.get_view_matrix());
			shader.setMat4("u_model", glm::translate(glm::mat4(1.0f), position));
			shader.setVec3("u_viewPos", user_camera.position);

			#pragma region lights
			shader.setVec3("u_dirLight.direction", dir_light.direction);
			shader.setVec3("u_dirLight.ambient", dir_light.ambient * dir_light.color);
			shader.setVec3("u_dirLight.diffuse", dir_light.color);
			shader.setVec3("u_dirLight.specular", glm::normalize(glm::vec3(1.5f) * glm::vec3(dir_light.color)));

			//point lights
			shader.setInt("u_pointLightCount", static_cast<int>(point_lights.size()));
			for (int i = 0; i < static_cast<int>(point_lights.size()); i++)
			{
				shader.setVec3("u_pointLight[" + std::to_string(i) + "].position", point_lights[i].position);
				shader.setVec3("u_pointLight[" + std::to_string(i) + "].diffuse", point_lights[i].color);
				shader.setVec3("u_pointLight[" + std::to_string(i) + "].specular", glm::normalize(glm::vec3(1.5f) * glm::vec3(point_lights[i].color)));
				shader.setFloat("u_pointLight[" + std::to_string(i) + "].constant", point_lights[i].constant);
				shader.setFloat("u_pointLight[" + std::to_string(i) + "].linear", point_lights[i].linear);
				shader.setFloat("u_pointLight[" + std::to_string(i) + "].quadratic", point_lights[i].quadratic);
			}

			//spot lights
			shader.setInt("u_spotLightCount", static_cast<int>(spot_lights.size()));
			for (int i = 0; i < static_cast<int>(spot_lights.size()); i++)
			{
				shader.setVec3("u_spotLight[i].direction", spot_lights[i].direction);
				shader.setVec3("u_spotLight[i].position", spot_lights[i].position);
				shader.setVec3("u_spotLight[i].diffuse", spot_lights[i].color);
				shader.setVec3("u_spotLight[i].specular", glm::normalize(glm::vec3(1.5f) * glm::vec3(spot_lights[i].color)));
				shader.setFloat("u_spotLight[i].innerCutoff", glm::cos(glm::radians(spot_lights[i].inner_cutoff)));
				shader.setFloat("u_spotLight[i].outerCutoff", glm::cos(glm::radians(spot_lights[i].outer_cutoff)));
				shader.setFloat("u_spotLight[i].constant", spot_lights[i].constant);
				shader.setFloat("u_spotLight[i].linear", spot_lights[i].linear);
				shader.setFloat("u_spotLight[i].quadratic", spot_lights[i].quadratic);
			}
			#pragma endregion

			/*    not gonna do rotation for now. I need to understand eulers and quaternions better first
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			*/

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->diffuse_map);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material->specular_map);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		#pragma endregion

		#pragma region constructor
		Cube_Object(engine::Shader& shader, const int _vao, unsigned int diffuse_map, unsigned int specular_map) : vao(_vao)
		{
			material = new engine::Lit_Textured_Material(shader, diffuse_map, specular_map);
			position = glm::vec3(0.0f);
			rotation = glm::vec3(0.0f);
		}
		#pragma endregion


	private:

	};
}
#endif