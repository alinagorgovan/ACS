#pragma once

#include <math.h>
#include <iostream>
#include "RenderableObject.h"
#include "Camera.h"
#include "Transform3DH.h"


class Cue : public RenderableObject
{
public:
	Cue() { }
	Cue(Mesh *mesh, Shader *shader, glm::vec3 color, glm::vec3 position, glm::vec3 scaleFactor) : RenderableObject(mesh, shader, color, position, scaleFactor) {
		this->aimed = false;
		this->shoot = false;
		this->renderCue = false;
		this->deltaX = 0;
		this->translateCueX = 0;
		this->sign = 1;
	}
	~Cue() { };

	void RenderCue(Tema::Camera* camera, glm::mat4 projectionMatrix, glm::vec3 whiteBallPos);
	void Aim();

public:
	bool aimed;
	bool shoot;
	bool renderCue;
	float deltaX;
	float translateCueX;
	int sign;
	float maxAimDistance = 0.3f;
	float angle;
};
