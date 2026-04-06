#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace engine
{
	class Camera
	{
	public:
		#pragma region variables
		//fov and zooming variables
		float fov = 85.0f, min_fov = 5.0f, max_fov = 130.0f;
		//near and far clipping planes
		float near_plane = 0.1f, far_plane = 100.0f;
		//aspect ratio of camera
		glm::vec2 aspect_ratio = glm::vec2(800.0f, 600.0f);

		//world-space position for camera
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
		//
		const glm::vec2& rotation = _rotation;
		//unit vector that represents the forward direction of the camera.
		glm::vec3 local_forward = glm::vec3(0.0f, 0.0f, -1.0f);
		//unit vector that represents the up direction to the camera. NOT world up.
		glm::vec3 local_up = glm::vec3(0.0f, 1.0f, 0.0f);
		//unit vector that represents the local right direction of the camera.
		glm::vec3 local_right = glm::vec3(1.0f, 0.0f, 0.0f);
		#pragma endregion

		#pragma region setters/getters
		//set the rotation vec2 of the camera object, and update it's unit vectors.
		void set_rotation(glm::vec2 new_rotation)
		{
			_rotation = new_rotation;
			update_vectors();
		}
		//get a view matrix based on the current position and direction of the camera in "world space"
		glm::mat4 get_view_matrix()
		{
			return glm::lookAt(position, position + local_forward, local_up);
		}
		glm::mat4 get_projection_matrix()
		{
			return glm::perspective(
				glm::radians(fov),
				aspect_ratio.y / aspect_ratio.x,
				near_plane,
				far_plane
			);

		}
		#pragma endregion

		#pragma region constructors
		Camera()
		{
			set_rotation(glm::vec2(0.0f, -90.0f));
		}
		#pragma endregion

	private:
		#pragma region private variables
		//euler angle rotation for camera. only supports X and Y rotation, we dont want any of that nasty roll business.
		glm::vec2 _rotation;
		#pragma endregion

		#pragma region instance methods
		void update_vectors()
		{
			local_forward = glm::normalize(glm::vec3(
				cos(glm::radians(_rotation.y)) * cos(glm::radians(_rotation.x)),
				sin(glm::radians(_rotation.x)),
				sin(glm::radians(_rotation.y)) * cos(glm::radians(_rotation.x))
			));
			local_right = glm::normalize(glm::cross(local_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
			local_up = glm::normalize(glm::cross(local_forward, local_right));
		}
		#pragma endregion

	};
}
#endif