#include "Object2DH.h"

#include <Core/Engine.h>
#include <math.h>

Mesh* Object2DH::CreateRectangle(std::string name, glm::vec3 center, float width, float height, glm::vec3 color, bool fill)
{

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center + glm::vec3(-width / 2, -height / 2, 0), color),
		VertexFormat(center + glm::vec3(width / 2, -height / 2, 0), color),
		VertexFormat(center + glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(center + glm::vec3(-width / 2, height / 2, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* Object2DH::CreateCircle(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color)
{
	glm::vec3 center = leftBottomCorner;

	std::vector<VertexFormat> vertices;
	// add center and first point
	vertices.push_back(VertexFormat(center, color));
	vertices.push_back(VertexFormat(center + glm::vec3(radius, 0, 0), color));

	Mesh* circle = new Mesh(name);
	std::vector<unsigned short> indices;

	int j = 1;
	for (float i = 0; i < 2 * M_PI; i += 0.001f) {
		vertices.push_back(VertexFormat(center + glm::vec3(radius * cos(i + 0.001f), radius * sin(i + 0.001f), 0), color));

		indices.push_back(0);
		indices.push_back(j);
		indices.push_back(++j);
	}


	circle->InitFromData(vertices, indices);
	return circle;
}