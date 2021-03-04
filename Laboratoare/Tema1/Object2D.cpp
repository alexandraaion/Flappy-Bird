#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateBird(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, 4 * length, 0), color),
		VertexFormat(corner + glm::vec3(4 * length, 4 * length, 0), color),
		VertexFormat(corner + glm::vec3(4 * length, 0, 0), color),
	    VertexFormat(corner + glm::vec3(4 * length, length, 0), color),
		VertexFormat(corner + glm::vec3(4 * length, 3 * length, 0), color),
		VertexFormat(corner + glm::vec3(5 * length, 3 * length, 0), color),
		VertexFormat(corner + glm::vec3(5 * length, length, 0), color),
		VertexFormat(corner + glm::vec3(5 * length, 5 * length / 2, 0), color),
		VertexFormat(corner + glm::vec3(5 * length, 3 * length / 2, 0), color),
		VertexFormat(corner + glm::vec3(11 * length / 2, 2 * length , 0), color)
	};

	Mesh* bird = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3};
	
	if (!fill) {
		bird->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(6);
		indices.push_back(7);
		indices.push_back(4);
		indices.push_back(6);
		indices.push_back(8);
		indices.push_back(9);
		indices.push_back(10);
	}

	bird->InitFromData(vertices, indices);
	return bird;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, 2 * length, 0), color),
		VertexFormat(corner + glm::vec3(length, 2 * length, 0), color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
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

