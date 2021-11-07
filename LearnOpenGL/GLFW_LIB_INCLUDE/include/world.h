#ifndef WORLD_H
#define WORLD_H

#include <light.h>
#include <world_object.h>
#include <camera.h>
#include <shader.h>
#include <glm.hpp>

namespace engine {
	class World
	{
	public:
		#pragma region variables
		std::vector<engine::Point_Light> point_lights;
		std::vector<engine::Spot_Light> spot_lights;
		engine::Dir_Light dir_light;
		std::vector<engine::Cube_Object> world_objects;
		engine::Camera* user_camera;
		#pragma endregion

		#pragma region instance methods
		void draw_world()
		{
			for (auto world_obj = world_objects.begin(); world_obj != world_objects.end(); world_obj++)
			{
				engine::Shader& shader = world_obj->material->shader;
				shader.setMat4("u_projection", user_camera->get_projection_matrix());
				shader.setMat4("u_view", user_camera->get_view_matrix());
				shader.setVec3("u_viewPos", user_camera->position);
				world_obj->draw(dir_light, point_lights, spot_lights, *user_camera);
			}
		}
		#pragma endregion

		#pragma region constructors
		World()
		{
			user_camera = new engine::Camera();
		}
		#pragma endregion
	};
}

#endif