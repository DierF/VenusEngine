#version 330

/*
  Filename: GeneralShader.frag
  Authors: Marshall Feng
  Description: A fragment shader corresponding to the GeneralShader.vert.
*/

precision highp float;

in vec3 vColor;

layout(location = 0) out vec4 fColor;
// buffer to draw id
layout(location = 1) out int IDColor;

// ID of the object provided by c++ code
uniform int objectID;

void
main ()
{
  fColor = vec4(vColor, 1.0);

  IDColor = objectID;
}
