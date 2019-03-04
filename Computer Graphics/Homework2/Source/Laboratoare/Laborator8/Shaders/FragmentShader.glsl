#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

uniform bool spot;
uniform float angle;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	float ambient_light = 0.25;
	float culoare_ambientala = material_kd * ambient_light;
	float light;

	if (!spot) {
		

		// TODO: compute diffuse light component
		float diffuse_light = material_kd * max(dot(world_normal,L), 0);

		// TODO: compute specular light component
		float specular_light = 0;

		if (diffuse_light > 0)
		{
			specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
		}
		float d = distance(light_position, world_position);
		float atenuare = 1 / (d * d); 
		// TODO: compute light
		 light = culoare_ambientala + atenuare * (diffuse_light + specular_light);

		// TODO: send color light output to fragment shader
		out_color = vec4(object_color * light, 1.0);
	} else {
		float cut_off = angle;
		float spot_light = dot(-L, light_direction);
		
		if(spot_light > cos(cut_off)) {
			float spot_light_limit = cos(cut_off);
 
			// Quadratic attenuation
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);

			// TODO: compute diffuse light component
			float diffuse_light = 0;
			diffuse_light = material_kd * max(dot(world_normal,L),0);

			// TODO: compute specular light component
			float specular_light = 0;

			if (diffuse_light > 0)
			{
				specular_light = material_ks * pow( max(dot(world_normal,H),0), material_shininess);
			}


			float atenuare = 0;
			float distance = distance(light_position, world_position);
			atenuare = 1 / (distance/4);

			// TODO: compute light
			light = ambient_light + atenuare* light_att_factor*(diffuse_light + specular_light);

		

			// TODO: write pixel out color
			out_color = vec4(object_color,1.0) * light;

		} else {
			float ambient_light = 0.25;
			out_color = vec4(object_color,1.0) * ambient_light;
		}
	}
	

}