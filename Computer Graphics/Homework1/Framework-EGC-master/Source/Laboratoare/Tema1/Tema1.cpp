#include "Tema1.h"

#include <vector>
#include <iostream>
#include <cmath>
//#include <stdlib>

#include <Core/Engine.h>
#include "Transform2DH.h"
#include "Object2DH.h"

int brick_count = 84;
float drop_speed = 6;
int show_down_wall = 0;
int big_ball = 0;
glm::vec3 starting_point;

Tema1::Tema1()
{
}
Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::vec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	
	// Initialize variables 
	glm::vec3 origin = glm::vec3(0, 0, 0);
	brick_size = glm::vec2(60, 40);
	game_started = false;
	angularSpeed = 0;
	brick_columns = 12;
	brick_lines = 8;
	brick_count = brick_columns * brick_lines;
	game_end = false;

	// Game struct variables for ball and platform
	initial_platform = platform(resolution.x / 2, 20, 300, 20);
	game_ball = ball((float)resolution.x / 2, 45, 10.0f, 0.0f, 0.0f, M_PI/2);
	game_platform = platform(resolution.x / 2, 20, 300, 20);

	// Initialize walls with sizes and coordinates
	walls["leftWall"] = wall(15, resolution.y / 2, 30, (float)resolution.y);
	walls["rightWall"] = wall(resolution.x - 15, resolution.y / 2, 30, (float)resolution.y);
	walls["upWall"] = wall(resolution.x / 2, resolution.y - 15, (float)resolution.x, 30);
	walls["downWall"] = wall(resolution.x / 2, 5, (float)resolution.x, 10);

	// Build Level1 bricks
	SetLevel1();

	// Create meshes
	Mesh* leftWall = Object2DH::CreateRectangle("leftWall", origin, walls["leftWall"].width, walls["leftWall"].height, glm::vec3(153.0 / 255, 0, 0), true);
	AddMeshToList(leftWall);
	Mesh* rightWall = Object2DH::CreateRectangle("rightWall", origin, walls["rightWall"].width, walls["rightWall"].height, glm::vec3(153.0 / 255, 0, 0), true);
	AddMeshToList(rightWall);
	Mesh* upWall = Object2DH::CreateRectangle("upWall", origin, walls["upWall"].width, walls["upWall"].height, glm::vec3(153.0 / 255, 0, 0), true);
	AddMeshToList(upWall);
	Mesh* downWall = Object2DH::CreateRectangle("downWall", origin, walls["downWall"].width, walls["downWall"].height, glm::vec3(153.0 / 255, 0, 0), true);
	AddMeshToList(downWall);

	Mesh* platform = Object2DH::CreateRectangle("platform", origin, game_platform.width, game_platform.height, glm::vec3(102.0 / 255, 0, 102.0 / 255), true);
	AddMeshToList(platform);

	Mesh *ball = Object2DH::CreateCircle("ball", origin, game_ball.radius, glm::vec3(1, 1, 1));
	AddMeshToList(ball);

	Mesh *big_ball = Object2DH::CreateCircle("big_ball", origin, game_ball.radius * 2, glm::vec3(1, 1, 1));
	AddMeshToList(big_ball);

	Mesh* brick = Object2DH::CreateRectangle("brick", origin, brick_size.x, brick_size.y, glm::vec3(51.0 / 255, 51.0 / 255, 255), true);
	AddMeshToList(brick);

	Mesh* brick2 = Object2DH::CreateRectangle("brick2", origin, brick_size.x, brick_size.y, glm::vec3(128.0 / 255, 0, 64.0 / 255), true);
	AddMeshToList(brick2);

	Mesh* life = Object2DH::CreateCircle("life", origin, 10, glm::vec3(1, 1, 1));
	AddMeshToList(life);

	Mesh* pwrup0 = Object2DH::CreateRectangle("pwrup0", origin, brick_size.x, brick_size.y, glm::vec3(1, 1, 0), true);
	AddMeshToList(pwrup0);
	Mesh* pwrup1 = Object2DH::CreateRectangle("pwrup1", origin, brick_size.x, brick_size.y, glm::vec3(1, 0, 1), true);
	AddMeshToList(pwrup1);
	Mesh* pwrup2 = Object2DH::CreateRectangle("pwrup2", origin, brick_size.x, brick_size.y, glm::vec3(0, 1, 1), true);
	AddMeshToList(pwrup2);
}
void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	// If the game was completed or all the lives were lost, reset to level1
	if (game_end || lives == 0) {
		Reset();
		SetLevel1();
	}
	RenderLives();
	RenderWalls();
	RenderBricks(deltaTimeSeconds);
	RenderPlatform();
	RenderBall();

}

void Tema1::FrameEnd()
{
	DrawCoordinatSystem();
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// If A key is pressed level2 will be set
	if (window->KeyHold(GLFW_KEY_A)) {
		SetLevel2();
	}
};

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// update platform coordinate
	game_platform.x = (float)mouseX;
};

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// if mouse button is pressed the ball starts moving
	if (game_started == false) {
		game_ball.speed_x = 6;
		game_ball.speed_y = 6;
		game_started = true;
	}
};

void Tema1::SetLevel1() {
	lives = 3;
	// Set bricks positions for the first level
	starting_point = glm::vec3(225, 250, 0);
	bricks.clear();
	for (int i = 0; i < brick_columns; i++) {
		for (int j = 0; j < brick_lines; j++) {
			int random = rand() % 10;
			bricks.push_back(brick(starting_point.x + i * (brick_size.x + 15), starting_point.y + j * (brick_size.y + 15), random));
		}
	}
};

void Tema1::SetLevel2() {
	// Reset ball and platform and mark level1 done
	Reset();
	level1_done = true;
	lives = 3;
	// Set bricks positions for the first level
	brick_count = brick_columns * brick_lines;
	bricks.clear();
	for (int i = 0; i < brick_columns; i++) {
		for (int j = 0; j < brick_lines; j++) {
			int random = rand() % 10;
			brick new_brick = brick(starting_point.x + i * (brick_size.x + 15), starting_point.y + j * (brick_size.y + 15), random);
			// Bricks from even lines are 2x harder to break
			if (j % 2) {
				new_brick.level = 2;
			}
			bricks.push_back(new_brick);
		}
	}
}
void Tema1::RenderBall() {
	if (!game_started) {
		// if the game did not started the ball follows the platform movement
		game_ball.x = game_platform.x;
	} else {
		// Check if any collision was made
		CheckPlatformCollision();
		CheckBricksCollision();
		CheckWallsCollision();
		// Modify x and y with the speed and angle set
		game_ball.x += cos(game_ball.angle) * game_ball.speed_x;
		game_ball.y += sin(game_ball.angle) * game_ball.speed_y;
	}
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2DH::Translate(game_ball.x, game_ball.y);
	// Check if the big ball bonus is activated and decrement the count
	if (big_ball > 0) {
		big_ball--;
		game_ball.radius = 20;
		RenderMesh2D(meshes["big_ball"], shaders["VertexColor"], modelMatrix);
	}
	else {
		game_ball.radius = 10;
		RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
	}
}
void Tema1::RenderPlatform() {
	// Check for any collision with the falling powerups
	CheckPowerUpsCollision();
	// Position the platform accordingly with the modified coordinates
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2DH::Translate(game_platform.x, game_platform.y);
	RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);

}
void Tema1::RenderBricks(float deltaTimeSeconds) {
	for (unsigned int i = 0; i < bricks.size(); i++) {
		// initialize model matrix for every brick
		modelMatrix = glm::mat3(1);
		if (!bricks[i].visible) {
			// if the brick is not visible it means it was hit and it must disappear from the game
			// Powerup field => 3 => brick has no powerup so it must fade
			if (bricks[i].powerup >= 3) {
				// modify scale parameters
				bricks[i].scale_x /= 1.5f;
				bricks[i].scale_y /= 1.5f;

				// Translate and scale the brick
				modelMatrix *= Transform2DH::Translate(bricks[i].x, bricks[i].y);
				modelMatrix *= Transform2DH::Scale(bricks[i].scale_x, bricks[i].scale_y);
				RenderMesh2D(meshes["brick"], shaders["VertexColor"], modelMatrix);
			}
			// Brick has a valid powerup
			else if (bricks[i].powerup < 3) {
				// Scale parameters set so the brick becomes a square
				bricks[i].scale_x = (float)1 / 3;
				bricks[i].scale_y = (float)1 / 2;
				// y coordinate starts decreasing so that the square starts falling
				bricks[i].y -= drop_speed;
				angularSpeed += deltaTimeSeconds;

				// Scale and rotate in origin and then translate the brick
				modelMatrix *= Transform2DH::Translate(bricks[i].x, bricks[i].y);
				modelMatrix *= Transform2DH::Rotate(angularSpeed);
				modelMatrix *= Transform2DH::Scale(bricks[i].scale_x, bricks[i].scale_y);

				// Depending on the type of the powerup, the brick is rendered with a differnt color
				switch (bricks[i].powerup) {
				case 0:
					// 0 type: ball doubles dimension
					RenderMesh2D(meshes["pwrup0"], shaders["VertexColor"], modelMatrix);
					break;
				case 1:
					// 1 type: a wall appears on the floor sa the ball cannot be lost
					RenderMesh2D(meshes["pwrup1"], shaders["VertexColor"], modelMatrix);
					break;
				case 2:
					// 2 type: a life is added, maximum 5 lives allowed
					RenderMesh2D(meshes["pwrup2"], shaders["VertexColor"], modelMatrix);
					break;
				}
			}
		}
		// the brick has not been hit
		else {
			modelMatrix *= Transform2DH::Translate(bricks[i].x, bricks[i].y);
			// depending on the brick type render the right mesh (Color differ)
			if (bricks[i].level == 1) {
				RenderMesh2D(meshes["brick"], shaders["VertexColor"], modelMatrix);
			}
			else {
				RenderMesh2D(meshes["brick2"], shaders["VertexColor"], modelMatrix);
			}
		}
	}
}
void Tema1::RenderWalls() {
	// Render walls
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2DH::Translate(walls["leftWall"].x, walls["leftWall"].y);
	RenderMesh2D(meshes["leftWall"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2DH::Translate(walls["rightWall"].x, walls["rightWall"].y);
	RenderMesh2D(meshes["rightWall"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2DH::Translate(walls["upWall"].x, walls["upWall"].y);
	RenderMesh2D(meshes["upWall"], shaders["VertexColor"], modelMatrix);

	if (show_down_wall) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2DH::Translate(walls["downWall"].x, walls["downWall"].y);
		RenderMesh2D(meshes["downWall"], shaders["VertexColor"], modelMatrix);
	}
}
void Tema1::RenderLives() {
	// Render lives on the bottom of the left wall
	for (int i = 0; i < lives; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2DH::Translate(15, 15 + i * 35.0f);
		RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
	}
}
void Tema1::CheckPlatformCollision() {
	// If the ball hit the floor then a life is lost and its position is set on the platform
	if (game_ball.y < 0) {
		Reset();
		lives--;

	}
	// If the ball y coordinated is aligned with the platform y coordinate
	else if (game_ball.y - game_ball.radius <= game_platform.y + game_platform.height / 2) {
		// Check if the ball fell on the platform by checking if the x coordinate is on the platform width and the ball is falling
		if (abs(game_ball.x - game_platform.x) <= game_ball.radius + game_platform.width / 2 && game_ball.speed_y < 0) {
			// compute the place where the ball hit the platform
			float diff = (game_ball.x - game_platform.x) / (game_platform.width / 2 + game_ball.radius);
			// Modify the x and y coordinates accordingly
			game_ball.angle = acos(diff);
			game_ball.speed_y *= (-1);
			if (game_ball.speed_x < 0) {
				game_ball.speed_x *= (-1);
			}
		}
	}	
}
void Tema1::CheckBricksCollision() {
	for (unsigned int i = 0; i < bricks.size(); i++) {
		// Check collision only if the brick has not been hit before
		if (bricks[i].visible) {
			// check collision between ball and brick by comparing the distances between center of the ball
			// and the center of the brick
			if (abs(game_ball.x - bricks[i].x) <= game_ball.radius + brick_size.x / 2 
				&& abs(game_ball.y - bricks[i].y) <= game_ball.radius + brick_size.y / 2) {
				// check if the ball hit the brick from down/above
				if (abs(game_ball.x - bricks[i].x) - (brick_size.x - brick_size.y) <= abs(game_ball.y - bricks[i].y)) {
					game_ball.speed_y *= (-1);
				}
				// else the ball was hit laterally
				else {
					game_ball.speed_x *= (-1);
				}
				// decrease brick strength
				bricks[i].level--;
				// if level is 0 then the brick should disappear
				if (bricks[i].level == 0) {
					bricks[i].visible = false;
					brick_count--;
					// check for powerups
					if (bricks[i].powerup < 3)
						powerups.push_back(i);
				}
				// if all the bricks were removed and the level1_done hasn't been set true so far
				// then the next level is loaded
				if (brick_count == 0 && !level1_done) {
					SetLevel2();
				}
				// else the both level1 and level 2 were completed
				else if (brick_count == 0) {
					game_end = true;
				}
			}
		}
	}
}
void Tema1::CheckPowerUpsCollision() {
	// powerups vector contains the indices of the current falling powerups 
	for (unsigned int i = 0; i < powerups.size(); i++) {
		brick current_powerup = bricks[powerups[i]];
		// if the square is aligned with the platform on the y axis
		if (abs(current_powerup.y - game_platform.y) <= game_platform.height / 2 + 10 ) {
			// check if the x coordinate of the square is on the platform width
			if (abs(current_powerup.x - game_platform.x) <= 10 + game_platform.width / 2) {
				// collision
				// check type of powerup
				switch (current_powerup.powerup) {
				case 0:
					// type 0 (color yellow): double ball size for 10 seconds
					big_ball = 600;
					break;
				case 1:
					// type 1 (color magenta): make floor wall appear for 10 seconds
					show_down_wall = 600;
					break;
				case 2:
					// type 2 (color cyan): add a life (maximum is reached at 5)
					if (lives < 5) {
						lives++;
					}
					break;
				}
				// remove powerup from vector
				powerups.erase(powerups.begin() + i);
			}
		}
		else if (current_powerup.y < 0) {
			// the powerup was lost, remove from vector
			powerups.erase(powerups.begin() + i);
		}

	}
}
void Tema1::CheckWallsCollision() {
	// leftWall collision checking
	if (game_ball.x - game_ball.radius <= walls["leftWall"].x + walls["leftWall"].width / 2) {
		game_ball.speed_x *= (-1);
	}
	// rightWall collision checking
	if (game_ball.x + game_ball.radius >= walls["rightWall"].x - walls["rightWall"].width / 2) {
		game_ball.speed_x *= (-1);
	}
	// upWall collision checking
	if (game_ball.y + game_ball.radius >= walls["upWall"].y - walls["upWall"].height / 2) {
		game_ball.speed_y *= (-1);
	}
	// downWall collision checking
	if (show_down_wall > 0) {
		show_down_wall--;
		if (game_ball.y - game_ball.radius <= walls["downWall"].y + walls["downWall"].height / 2) {
			game_ball.speed_y *= (-1);
		}
	}
}

void Tema1::Reset() {
	// Reset platform to initial coordinates
	game_platform.x = initial_platform.x;
	game_platform.y = initial_platform.y;

	// Reset ball size, direction and its position on the platform
	game_ball.radius = 10;
	game_ball.x = initial_platform.x;
	game_ball.y = initial_platform.y + game_ball.radius + initial_platform.height;
	game_ball.angle = M_PI / 2;
	
	// Ball is not moving
	game_started = false;

	// If any bonus was active, it is deactivated
	show_down_wall = 0;
	big_ball = 0;
}
