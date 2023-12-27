#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Render/Shader.h"
#include "Math/MathHeaders.h"

namespace VenusEngine
{
	class ShaderProgram
	{
	public:
		ShaderProgram()
			: m_programId(glCreateProgram()),
			m_vertexShader(GL_VERTEX_SHADER),
			m_fragmentShader(GL_FRAGMENT_SHADER)
		{
		}

		~ShaderProgram()
		{
			// Delete shaders and delete the program object.
			// We assume shaders are not shared among multiple programs.
			glDeleteProgram(m_programId);
		}

		// Disable default copy ctor and copy assignment
		ShaderProgram(ShaderProgram const&) = delete;
		ShaderProgram& operator= (ShaderProgram const&) = delete;

		GLint getAttributeLocation(std::string const& attributeName) const
		{
			return glGetAttribLocation(m_programId, attributeName.c_str());
		}

		GLint getUniformLocation(std::string const& uniformName) const
		{
			return glGetUniformLocation(m_programId, uniformName.c_str());
		}

		void setUniformMatrix(std::string const& uniform, Mat4 const& value)
		{
			GLint location = getUniformLocation(uniform);
			float data[16];
			value.toData(data);
			glUniformMatrix4fv(location, 1, GL_TRUE, data);
		}

		void createVertexShader(std::string const& vertexShaderFilename)
		{
			m_vertexShader.compile(vertexShaderFilename);
			m_vertexShader.attach(m_programId);
		}

		void createFragmentShader(std::string const& fragmentShaderFilename)
		{
			m_fragmentShader.compile(fragmentShaderFilename);
			m_fragmentShader.attach(m_programId);
		}

		void link()
		{
			glLinkProgram(m_programId);
			GLint isLinked;
			glGetProgramiv(m_programId, GL_LINK_STATUS, &isLinked);
			if (isLinked == GL_FALSE)
			{
				fprintf(stderr, "Link error -- see log\n");
				exit(-1);
			}
			// After linking, the shader objects no longer need to be attached.
			// A shader won't be deleted until it is detached.
			m_vertexShader.detach(m_programId);
			m_fragmentShader.detach(m_programId);
		}

		void enable()
		{
			glUseProgram(m_programId);
		}

		void disable()
		{
			glUseProgram(0);
		}

	private:
		void writeInfoLog(GLuint shaderId, bool isShader, std::string const& logFilename) const
		{
			GLint infoLogLength = 0;
			if (isShader)
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
			else
				glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0)
			{
				// Avoid the overhead of vector<char>, which has to initialize
				//   each char.
				std::unique_ptr<char[]> infoLog(new char[infoLogLength]);
				if (isShader)
					glGetShaderInfoLog(shaderId, infoLogLength, nullptr, infoLog.get());
				else
					glGetProgramInfoLog(m_programId, infoLogLength, nullptr, infoLog.get());
				std::ofstream logStream(logFilename);
				logStream << infoLog.get() << std::endl;
			}
		}

	private:
		GLuint m_programId;
		Shader m_vertexShader;
		Shader m_fragmentShader;
	};
}