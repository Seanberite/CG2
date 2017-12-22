/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in float shade;
in vec2 exTexCoord;

uniform sampler2D objectTexture;

out vec4 out_Color;

void main(void)
{
	out_Color=vec4(shade,shade,shade,1.0) * texture(objectTexture, exTexCoord);
}
