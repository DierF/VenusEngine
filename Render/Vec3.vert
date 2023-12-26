#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

uniform mat4 uModelView;
uniform mat4 uProjection;

out vec3 vColor;

/*********************************************************/

void
main ()
{
  gl_Position = uProjection * uModelView * vec4 (aPosition, 1.0);
  vColor = aColor;
}
