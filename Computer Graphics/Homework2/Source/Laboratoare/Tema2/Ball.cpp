#include "Ball.h"

Ball::~Ball()
{
}
void Ball::Update()
{
	this->translate += this->movement;
	if (this->speed > 0) {
		this->isMoving = true;
		this->speed -= this->friction;
	}
	else {
		this->speed = 0;
		this->isMoving = false;
	}
	this->movement.x = -this->directionX * this->speed * sin(this->angle);
	this->movement.z = -this->directionZ * this->speed * cos(this->angle);
}
bool Ball::CheckCollisions(std::vector<Ball*> &balls)
{
	return false;
}