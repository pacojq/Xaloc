// Blit Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(a_Position, 1.0);
	v_TexCoord = a_TexCoord;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_PxPerUnit;
uniform vec4 u_ScreenSize;
uniform vec3 u_ColorLight;
uniform vec3 u_ColorDark;

in vec2 v_TexCoord;

void main()
{
	int px = int(v_TexCoord.x * u_ScreenSize.x);
	int py = int(v_TexCoord.y * u_ScreenSize.y);

    float t = float( ( int( px / u_PxPerUnit ) ^ int( py / u_PxPerUnit ) ) & 1 );

	vec3 col = mix(u_ColorLight, u_ColorDark, t);

	color = vec4(col, 1.0);
	//color = vec4(v_TexCoord.x, v_TexCoord.y, 0.0, 1.0);
}