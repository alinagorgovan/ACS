#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec3 v_coordinate;
layout(location = 1) in vec3 v_color;
// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// TODO: output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_coordinate;
out vec3 frag_color;
out float frag_time;

void main()
{
	// TODO: send output to fragment shader
	frag_coordinate = v_coordinate;
	frag_position = v_position;
	frag_normal = v_normal;
	frag_color = v_color;
	frag_time = time;
	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position * sin(time), 1.0);
}
