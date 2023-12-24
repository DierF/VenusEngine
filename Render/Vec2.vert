#version 330
/*
  Filename: Vec2.vert
  Authors: Gary M. Zoppetti, Ph.D. & Chad Hogg
  Course: CSCI375
  Assignment: A01Primitives
  Description: A vertex shader that simply copies its input to its output.
    It is so simple because 2-D vertices are already expressed in screen
    coordinates and colors are already provided in the VAO.
*/

// First, we specify the attributes that are inputs to the shader program
// These were specified in Main.cpp
// Positions are the first attributes, and they are 2-D vectors (X, Y)
layout (location = 0) in vec2 aPosition;
// Colors are the second attributes, and they are 3-D vectors (R, G, B)
layout (location = 1) in vec3 aColor;

// Second, we specify any additional outputs our shader produces
// We want to output a color, which is a 3-D vector (R, G, B)
out vec3 vColor;

void
main ()
{
  // Every vertex shader must write gl_Position
  // It is a 4-D vector (X, Y, Z, W)
  gl_Position = vec4 (aPosition, 0.0, 1.0);
  // We are choosinng to write a color also (R, G, B)
  vColor = aColor;
}
