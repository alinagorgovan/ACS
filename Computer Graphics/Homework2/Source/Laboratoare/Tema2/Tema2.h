#pragma once
#include <Component/SimpleScene.h>
#include <vector>
#include "Camera.h"
#include "Transform3DH.h"
#include "RenderableObject.h"
#include "Ball.h"
#include "Cue.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderLines();
	void RenderBall(int i);
	void Render(RenderableObject *obj);
	void CreateBalls();
	void CheckWallsCollision(Ball *b);
	bool DetectBallBallCollision(Ball *b1, Ball *b2);
	void CalculateCollisionSpeed(Ball *b1, Ball *b2);
	void computeAngle(Ball *b);
	void MoveWhiteBall();
	void ShowStatistics();
	void OnKeyPress(int key, int mods);
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
public:
	struct player {
		player() {
			score = 0;
			fault = 0;
			won = false;
			color = glm::vec3(0, 0, 0);
		}
		glm::vec3 color;
		int score;
		int fault;
		bool won;
	};
protected:
	player player1;
	player player2;
	player *currentPlayer;
	float ballSize;
	Tema::Camera *camera;
	glm::mat4 projectionMatrix;
	std::vector<RenderableObject *> objects;
	std::vector<RenderableObject *> lines;
	std::vector<Ball *> balls;
	std::vector<Ball *> pockets;
	Cue *cue;
	Ball* whiteBall;
	bool gameStarted;
	float dist;
	bool fault = true;
	bool gameEnd = false;
	bool firstTime;
	bool moveWhiteBall = true;
	bool renderLines = false;
};
