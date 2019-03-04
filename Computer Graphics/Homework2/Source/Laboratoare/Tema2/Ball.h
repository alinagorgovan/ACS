#pragma once

#include "RenderableObject.h"

class Ball : public RenderableObject
{
public:
	Ball() { }
	Ball(Mesh *mesh, Shader *shader, glm::vec3 color, glm::vec3 position, glm::vec3 scaleFactor) : RenderableObject(mesh, shader, color, position, scaleFactor) {
		this->speed = 0;
		this->renderBall = true;
		this->direction = glm::vec3(1, 0, 0);
		this->radius = scaleFactor.x / 2;
		this->movement = glm::vec3(0, 0, 0);
	}
	~Ball();
	bool CheckCollisions(std::vector<Ball*> &balls);

	float GetSpeed() { return this->speed; }
	void SetSpeed(float speed) { this->speed = speed; }

	glm::vec3 GetDirection() { return this->direction; }
	void SetDirection(const glm::vec3 &val) { this->direction = direction; }
	void Update();
	bool GetRenderBall() { return this->renderBall; }
	void SetRenderBall(bool &val) { this->renderBall = val; }
public:
	float maxBallSpeed = 0.1f;
	float speed;
	float angle;
	float directionX = 1;
	float directionZ = 1;
	glm::vec3 movement;
	bool renderBall;
	glm::vec3 direction;
	float radius;
	bool isMoving = false;
	float friction = 0.0005;
};
