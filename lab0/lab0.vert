/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;


uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;
uniform float time;

out float shade;
out vec2 exTexCoord;

// Each row is a column
mat4 shear = mat4(1.0, 0.0, 0.0, 0.0,
                  cos(time), 1.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  0.0, 0.0, 0.0, 1.0);


void main(void)
{
  exTexCoord = in_TexCoord;

	shade = (mat3(camMatrix)*in_Normal).z; // Fake shading
	gl_Position = projectionMatrix*camMatrix*mdlMatrix*shear*vec4(in_Position, 1.0);
}
