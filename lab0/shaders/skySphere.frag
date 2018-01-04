/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in vec2 exTexCoord;

uniform sampler2D skyBoxTexture;

out vec4 out_Color;

void main(void)
{
	out_Color= texture(skyBoxTexture, exTexCoord);
}
