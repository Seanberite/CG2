



#version 150

in  vec3  in_Position;
in  vec2  in_TexCoord;
out vec2 exTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 camSkyMatrix;



void main(void)
{
  exTexCoord = in_TexCoord;
	gl_Position = vec4(projectionMatrix*camSkyMatrix*vec4(in_Position, 1.0));
}
