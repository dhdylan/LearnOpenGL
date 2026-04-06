#define DIFFUSE_UNIT 0
#define SPECULAR_UNIT 1
#define EMISSION_UNIT 2

#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <dearimgui/imgui.h>
#include <shader.h>
#include <texture.h>

namespace engine
{
	class Lit_Textured_Material
	{
	public:

		#pragma region variables
		//------- read only -----------
		const int& diffuse_map = _diffuse_map;
		const int& specular_map = _specular_map;
		const int& emission_map = _emission_map;
		const float& shininess = _shininess;
		//-----------------------------

		engine::Shader& shader;
		#pragma endregion

		#pragma region setters
		void set_diffuse_map(int texture_unit)
		{
			_diffuse_map = texture_unit;
			shader.setInt("u_material.diffuse_map", _diffuse_map);
		}
		void set_specular_map(int texture_unit)
		{
			_specular_map = texture_unit;
			shader.setInt("u_material.specualr_map", _specular_map);
		}
		void set_emission_map(int texture_unit)
		{
			_emission_map = texture_unit;
			shader.setInt("u_material.emission_map", _emission_map);
		}
		void set_shininess(float __shininess)
		{
			_shininess = __shininess;
			shader.setFloat("u_material.shininess", _shininess);
		}
		#pragma endregion

		#pragma region instance methods
		void send_material_to_shader() //remember that when you set the smapler2d uniform on the shader with an int here, youre telling it the TEXTURE UNIT **not** the texture id
		{
			shader.setInt("u_material.diffuse_map", DIFFUSE_UNIT);
			shader.setInt("u_material.specular_map", SPECULAR_UNIT);
			shader.setInt("u_material.emission_map", EMISSION_UNIT);
			shader.setFloat("u_material.shininess", shininess);
		}
		#pragma endregion

		#pragma region constructors
		Lit_Textured_Material(Shader& _shader, unsigned int diffuse, unsigned int specular) : shader(_shader)
		{
			_diffuse_map = diffuse;
			_specular_map = specular;
			_emission_map = 0;
			_shininess = 32.0f;
		}
		#pragma endregion


	private:
		#pragma region private variables
		int _diffuse_map = 0;
		int _specular_map = 0;
		int _emission_map = 0;
		float _shininess = 32.0f;
		#pragma endregion
	};
}
#endif
