#ifndef WORLD_H
#define WORLD_H

#include <light.h>
#include <camera.h>
#include <shader.h>
#include <glm/glm.hpp>

namespace engine {
	class World
	{
	public:
		#pragma region variables
		std::vector<engine::Point_Light> point_lights;
		std::vector<engine::Spot_Light> spot_lights;
		engine::Dir_Light dir_light;
		std::vector<engine::Cube_Object> cubes;
		engine::Camera* user_camera;
		#pragma endregion

		#pragma region instance methods
		void draw_world()
		{
			//draw cubes
			for (auto cube = cubes.begin(); cube != cubes.end(); cube++)
			{
				cube->draw_object(dir_light, point_lights, spot_lights, *user_camera);
			}

			//draw lights
			for (auto point_light = point_lights.begin(); point_light != point_lights.end(); point_light++)
			{
				point_light->draw(*user_camera);
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