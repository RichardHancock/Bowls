#version 330

uniform mat4 vs_mvp;

layout(location = 0) in vec4 vs_position;

void main()
{
  gl_Position = vs_mvp * vs_position;
}
