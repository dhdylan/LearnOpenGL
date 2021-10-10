#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader {
	public:
		//program id
		unsigned int program_id;

		//constructor that reads and builds the shader
		Shader(const std::string& vertex_path, const std::string& frag_path)
		{
			//first, read in source code and store as c_str
			//---------------------------------------------

			std::string vertex_code;
			std::string frag_code;
			std::ifstream vertex_file;
			std::ifstream frag_file;

			// ensure ifstream objects can throw exceptions:
			vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			frag_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				//open files
				vertex_file.open(vertex_path);
				frag_file.open(frag_path);

				std::stringstream vertex_stream, frag_stream;

				//read files' contents into streams
				vertex_stream << vertex_file.rdbuf();
				frag_stream << frag_file.rdbuf();
				//close file objects
				vertex_file.close();
				frag_file.close();
				vertex_code = vertex_stream.str();
				frag_code = frag_stream.str();
			}
			catch(std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
			}

			//now use cstr source code to set up the actual shaders
			const char* vertex_code_cstr = vertex_code.c_str();
			const char* frag_code_cstr = frag_code.c_str();

			//set up shader objects
			//---------------------
			unsigned int vertex_id, frag_id;
			char infolog[512];
			int success;

			//set up vertex shader
			vertex_id = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_id, 1, &vertex_code_cstr, NULL);
			glCompileShader(vertex_id);

			//error check
			glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertex_id, 512, NULL, infolog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
			}

			//set up frag shader
			frag_id = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(frag_id, 1, &frag_code_cstr, NULL);
			glCompileShader(frag_id);

			//error check
			glGetShaderiv(frag_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(frag_id, 512, NULL, infolog);
				std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infolog << std::endl;
			}

			//link shader program
			//-------------------
			program_id = glCreateProgram();
			glAttachShader(program_id, vertex_id);
			glAttachShader(program_id, frag_id);
			glLinkProgram(program_id);
			//check for linking errors
			glGetProgramiv(program_id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(program_id, 512, NULL, infolog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
			}

			//delete shader objects
			glDeleteShader(vertex_id);
			glDeleteShader(frag_id);
		}

		//tell opengl to use this shader
		void use()
		{
			glUseProgram(program_id);
		}

		static void unuse()
		{
			glUseProgram(0);
		}

		//functions to set uniforms
		void setBool(const std::string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
		}
		void setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
		}
		void setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
		}
		void setMat4(const std::string& name, const glm::mat4& value)
		{
			glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
		}
		void setVec3(const std::string& name, const glm::vec3& value) const
		{
			glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
		}
};

#endif
