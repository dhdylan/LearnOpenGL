#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <dearimgui/imgui.h>
#include <shader.h>

namespace engine
{
	class Lit_Textured_Material
	{
	public:
		#pragma region setters/getters
		void set_diffuse_map(int texture_unit)
		{
			diffuse_map = texture_unit;
			shader->setInt("u_material.diffuse_map", diffuse_map);
		}
		void set_specular_map(int texture_unit)
		{
			specular_map = texture_unit;
			shader->setInt("u_material.specualr_map", specular_map);
		}
		void set_emission_map(int texture_unit)
		{
			emission_map = texture_unit;
			shader->setInt("u_material.emission_map", emission_map);
		}
		void set_shininess(float _shininess)
		{
			shininess = _shininess;
			shader->setFloat("u_material.shininess", shininess);
		}
		void set_shader_ptr(engine::Shader* _shader)
		{
			shader = _shader;
		}

		int get_diffuse_map()
		{
			return diffuse_map;
		}
		int get_specular_map()
		{
			return specular_map;
		}
		int get_emission_map()
		{
			return emission_map;
		}
		float get_shiniess()
		{
			return shininess;
		}
		engine::Shader* get_shader_ptr()
		{
			return shader;
		}
		#pragma endregion

		#pragma region instance methods
		void send_material_to_shader()
		{
			shader->setInt("u_material.diffuse", diffuse_map);
			shader->setInt("u_material.specular", specular_map);
			shader->setInt("u_material.emission", emission_map);
			shader->setFloat("u_material.shininess", shininess);
		}
		#pragma endregion

		#pragma region constructors
		Lit_Textured_Material()
		{
			diffuse_map, specular_map, emission_map = 0;
			shininess = 32.0f;
			shader = nullptr;
		}
		#pragma endregion


	private:
		#pragma region variables
		int diffuse_map = 0;
		int specular_map = 0;
		int emission_map = 0;
		float shininess = 32.0f;

		engine::Shader* shader;
		#pragma endregion
	};
}
#endif
