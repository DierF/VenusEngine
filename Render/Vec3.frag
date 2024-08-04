#version 330

in vec3 vColor;

//out vec4 fColor;

layout(location = 0) out vec4 fColor;
layout(location = 1) out int IDColor;

uniform int objectID;

void
main ()
{
  fColor = vec4(vColor, 1.0);

  IDColor = objectID;
}
