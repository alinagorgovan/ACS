#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 color;
uniform float dx;
uniform float alpha;

out vec3 frag_color;

void main()
{
	mat4 Translate = transpose(mat4(1,0,0,dx * sin(alpha),
							  0,1,0,0,
							  0,0,1,dx * cos(alpha),
							  0,0,0,1));
	frag_color = color;
	gl_Position = Projection * View * Translate * Model * vec4(v_position, 1.0);
}
