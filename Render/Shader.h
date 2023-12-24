#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Editor/FileReader.h"

namespace VenusEngine
{
	class Shader
	{
	public:
		Shader(GLenum shaderType)
			: m_shaderId(glCreateShader(shaderType))
		{
		};

		~Shader()
		{
			glDeleteProgram(m_shaderId);
		}

		void compile(std::string const& path)
		{
			std::string sourceCode = FileReader::readFile(path);
			GLchar const* sourceCodePtr = sourceCode.c_str();
			// One array of char*. Do not need to specify length if null-terminated.
			glShaderSource(m_shaderId, 1, &sourceCodePtr, nullptr);
			glCompileShader(m_shaderId);

			GLint isCompiled;
			glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				std::cout << "Compilation error; exiting\n" << path << std::endl;
				exit(-1);
			}
		}

		void attach(GLuint programId)
		{
			glAttachShader(programId, m_shaderId);
		}

		void detach(GLuint programId)
		{
			glDetachShader(programId, m_shaderId);
		}

		void use()
		{
			glUseProgram(m_shaderId);
		}

		GLint getUniformLocation(std::string const& name)
		{
			return glGetUniformLocation(m_shaderId, name.c_str());
		}

		GLuint id()
		{
			return m_shaderId;
		}

		GLuint id() const
		{
			return m_shaderId;
		}

	private:
		GLuint m_shaderId;
	};
}