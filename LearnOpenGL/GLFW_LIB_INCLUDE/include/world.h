#ifndef WORLD_H
#define WORLD_H

namespace engine {
	class World
	{
	public:
		#pragma region setters/getters
		auto get_dir_light()
		{
			return dir_light;
		}
		void set_dir_light(engine::Dir_Light _dir_light)
		{
			dir_light = _dir_light;
		}
		auto get_point_lights()
		{
			return point_lights;
		}
		void set_point_lights(std::vector<engine::Point_Light> _point_lights)
		{
			point_lights = _point_lights;
		}
		auto get_spot_lights()
		{
			return spot_lights;
		}
		void set_spot_lights(std::vector<engine::Spot_Light> _spot_lights)
		{
			spot_lights = _spot_lights;
		}
		auto get_world_objects()
		{
			return world_objects;
		}
		void set_world_objects(std::vector<engine::World_Object> _world_objects)
		{
			world_objects = _world_objects;
		}
		auto get_camera()
		{
			return user_camera;
		}

		#pragma endregion

		#pragma region instance methods
		void draw_world()
		{
			for (auto world_obj = world_objects.begin(); world_obj != world_objects.end(); world_obj++)
			{
				engine::Shader& shader = *world_obj->get_material().get_shader_ptr();
				shader.setMat4("u_projection", user_camera.get_projection_matrix());
				shader.setMat4("u_view", user_camera.get_view_matrix());
				shader.setVec3("u_viewPos", user_camera.get_position());
				world_obj->draw(dir_light, point_lights, spot_lights, user_camera);
			}
		}
		#pragma endregion

		#pragma region constructors
		World()
		{
			
		}
		#pragma endregion



	private:
		std::vector<engine::Point_Light> point_lights;
		std::vector<engine::Spot_Light> spot_lights;
		engine::Dir_Light dir_light;
		std::vector<engine::World_Object> world_objects;
		engine::Camera user_camera;
	};
}

#endif