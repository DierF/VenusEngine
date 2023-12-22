/*
  Filename: ShaderProgram.h
  Authors: Gary M. Zoppetti, Ph.D. & Chad Hogg
  Course: CSCI375
  Assignment: A01Primitives
  Description: A class that simplifies loading and running OpenGL shaders.
*/

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

#include <glad/glad.h>

class ShaderProgram
{
public:
  ShaderProgram ();

  ~ShaderProgram ();

  // Disable default copy ctor and copy assignment
  ShaderProgram (const ShaderProgram&) = delete;
  ShaderProgram& operator= (const ShaderProgram&) = delete;

  GLint
  getAttributeLocation (const std::string& attributeName) const;

  void
  createVertexShader (const std::string& vertexShaderFilename);

  void
  createFragmentShader (const std::string& fragmentShaderFilename);

  void
  link () const;

  void
  enable ();

  void
  disable ();

private:
  void
  compileShader (const std::string& shaderFilename, GLuint shaderId);

  std::string
  readShaderSource (const std::string& filename) const;

  void
  writeInfoLog (GLuint shaderId, bool isShader,
		const std::string& logFilename) const;

private:
  GLuint m_programId;
  GLuint m_vertexShaderId;
  GLuint m_fragmentShaderId;
};

#endif
