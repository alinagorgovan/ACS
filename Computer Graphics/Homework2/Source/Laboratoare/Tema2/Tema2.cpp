#include "Tema2.h"

#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}
float tableWidth = 4.0;
float tableLength = 6.0;
float wallLength = 6.5;
float wallWidth = 4.5;
float wallSize = 0.25;
void Tema2::Init()
{

	camera = new Tema::Camera();
	
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Shader *shader = new Shader("ColorShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader1.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader1.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader *shader = new Shader("CueShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader2.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader1.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		RenderableObject* table = new RenderableObject(meshes["box"], shaders["ColorShader"],
			glm::vec3(0/255, 100.0/255, 66.0/255), glm::vec3(0, 0.5, 0), glm::vec3(tableWidth, 1, tableLength));
		objects.push_back(table);
	}
	glm::vec3 wallColor = glm::vec3(100.0 / 255, 0, 0);
	
	{
		RenderableObject* wall;
		wall = new RenderableObject(meshes["box"], shaders["ColorShader"],
			wallColor, glm::vec3((-wallWidth + wallSize) / 2, 0.625, 0),
			glm::vec3(wallSize, 1.25, wallLength));
		objects.push_back(wall);
		wall = new RenderableObject(meshes["box"], shaders["ColorShader"],
			wallColor, glm::vec3((wallWidth - wallSize) / 2, 0.625, 0),
			glm::vec3(wallSize, 1.25, wallLength));
		objects.push_back(wall);
		wall = new RenderableObject(meshes["box"], shaders["ColorShader"],
			wallColor, glm::vec3(0, 0.625, (-wallLength + wallSize) / 2),
			glm::vec3(wallWidth, 1.25, wallSize));
		objects.push_back(wall);
		wall = new RenderableObject(meshes["box"], shaders["ColorShader"],
			wallColor, glm::vec3(0, 0.625, (wallLength - wallSize) / 2),
			glm::vec3(wallWidth, 1.25, wallSize));
		objects.push_back(wall);
	}

	glm::vec3 pocketSize = glm::vec3(0.4, 0.4, 0.4);
	glm::vec3 pocketColor = glm::vec3(0, 0, 0);
	{
		Ball* pocket;
		pocket = new Ball(meshes["sphere"], shaders["ColorShader"],
			pocketColor, glm::vec3(tableWidth / 2, 0.95, tableLength / 2), pocketSize);
		pockets.push_back(pocket);
		pocket = new Ball (meshes["sphere"], shaders["ColorShader"],
			pocketColor, glm::vec3(-tableWidth / 2, 0.95, tableLength / 2), pocketSize);
		pockets.push_back(pocket);
		pocket = new Ball(meshes["sphere"], shaders["ColorShader"],
			pocketColor, glm::vec3(tableWidth / 2, 0.95, -tableLength / 2), pocketSize);
		pockets.push_back(pocket);
		pocket = new Ball(meshes["sphere"], shaders["ColorShader"],
			pocketColor, glm::vec3(-tableWidth / 2, 0.95, -tableLength / 2), pocketSize);
		pockets.push_back(pocket);
		pocket = new Ball(meshes["sphere"], shaders["ColorShader"],
			pocketColor, glm::vec3(-tableWidth / 2, 0.95, 0), pocketSize);
		pockets.push_back(pocket);
		pocket = new Ball(meshes["sphere"], shaders["ColorShader"],
			pocketColor, glm::vec3(tableWidth / 2, 0.95, 0), pocketSize);
		pockets.push_back(pocket);
	}
	ballSize = 0.2f;
	{
		CreateBalls();
		cue = new Cue(meshes["box"], shaders["CueShader"],
			glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0.03, 0.03, 2));
	}

	{
		RenderableObject* helpLine;
		helpLine = new RenderableObject(meshes["box"], shaders["ColorShader"],
			glm::vec3(1, 102.0/255, 1), glm::vec3(0, 1.2, 0), glm::vec3(0.007, 0.007, 1));
		lines.push_back(helpLine);
		helpLine = new RenderableObject(meshes["box"], shaders["ColorShader"],
			glm::vec3(0, 1, 1), glm::vec3(0, 1.2, 0), glm::vec3(0.007, 0.007, 1));
		lines.push_back(helpLine);
	}
	
	camera->Set(glm::vec3(-2, 3, 4), whiteBall->translate, glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	gameStarted = false;
	currentPlayer = NULL;
	firstTime = true;
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	if (gameEnd) {
		if (player1.won) {
			std::cout << "Player 1 won!" << std::endl;
		}
		else if (player2.won) {
			std::cout << "Player 2 won!" << std::endl;
		}
	}
	MoveWhiteBall();
	for (unsigned int i = 0; i < objects.size(); i++) {
		Render(objects[i]);
	}
	for (unsigned int j = 0; j < pockets.size(); j++) {
		Render(pockets[j]);
	}
	for (unsigned int i = 0; i < balls.size(); i++) {
		if (balls[i]->renderBall) {
			RenderBall(i);
		}
	}
	if (cue->renderCue) {
		cue->RenderCue(camera, projectionMatrix, whiteBall->GetTranslate());
	}
	if (renderLines) {
		for (unsigned int i = 0; i < lines.size(); i++) {
			Render(lines[i]);
		}
			
	}
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::Render(RenderableObject *obj)
{
	Mesh *mesh = obj->GetMesh();
	Shader *shader = obj->GetShader();

	if (!mesh || !shader || !shader->program)
		return;

	glUseProgram(shader->program);
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(obj->GetModel()));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	//glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glUniform3fv(glGetUniformLocation(shader->program, "color"), 1, glm::value_ptr(obj->GetBaseColor()));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderBall(int i)
{
	Ball *b = balls[i];
	Render(b);
	b->Update();
	CheckWallsCollision(b);
	if (b->isMoving) {
		for (unsigned int j = 0; j < pockets.size(); j++) {
			if (DetectBallBallCollision(b, pockets[j])) {
				if (firstTime) {
					firstTime = false;
					if (b->GetBaseColor() == glm::vec3(1, 0, 0)) {
						currentPlayer->color = glm::vec3(1, 0, 0);
						if (currentPlayer == &player1) {
							player2.color = glm::vec3(1, 1, 0);
						}
						else {
							player1.color = glm::vec3(1, 1, 0);
						}
						currentPlayer->score++;
					}
					else if (b->GetBaseColor() == glm::vec3(1, 1, 0)) {
						currentPlayer->color = glm::vec3(1, 1, 0);
						if (currentPlayer == &player1) {
							player2.color = glm::vec3(1, 0, 0);
						}
						else {
							player1.color = glm::vec3(1, 0, 0);
						}
						currentPlayer->score++;
						
					}
					else {
						currentPlayer->fault++;
						fault = true;
					}
				}
				else {
					if (b == whiteBall) {
						currentPlayer->fault++;
						fault = true;
					}
					else if (b->GetBaseColor() == player1.color) {
						player1.score++;
						break;
					}
					else if (b->GetBaseColor() == player2.color) {
						player2.score++;
						break;
					}
					else if (b->GetBaseColor() == glm::vec3(0, 0, 0)) {
						gameEnd = true;
						if (currentPlayer->score == 7) {
							currentPlayer->won = true;
						}
						else {
							if (currentPlayer == &player1) {
								player2.won = true;
							}
							else {
								player1.won = true;
							}
						}
						break;
					}
				}
				b->renderBall = false;
				b->isMoving = false;
				ShowStatistics();
			}
		}
		for (unsigned int j = 0; j < balls.size(); j++) {
			if (b == balls[j]) {
				continue;
			}
			else if (balls[j]->renderBall) {
				if (DetectBallBallCollision(b, balls[j])) {
					CalculateCollisionSpeed(b, balls[j]);
				}
			}
		}
	}
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && cue->renderCue)
	{
		cue->Aim();
	}
	if (moveWhiteBall) {
		if (window->KeyHold(GLFW_KEY_W)) {
			if (!gameStarted && whiteBall->translate.z > 1) {
				whiteBall->translate.z -= deltaTime;
			}
			else if (gameStarted) {
				if (whiteBall->translate.z > -2.9) {
					whiteBall->translate.z -= deltaTime;
				}
			}
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			
			if (whiteBall->translate.x > -1.9) {
				whiteBall->translate.x -= deltaTime;
			}
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			if (whiteBall->translate.z < 2.9) {
				whiteBall->translate.z += deltaTime;
			}
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			if (whiteBall->translate.x < 1.9) {
				whiteBall->translate.x += deltaTime;
			}
		}
	}
	if (window->KeyHold(GLFW_KEY_O)) {
		camera->Set(glm::vec3(0, 7, 0.1f), glm::vec3(0, -1, 0.05), glm::vec3(0, 1, 0));
	}
	if (window->KeyHold(GLFW_KEY_P)) {
		glm::vec3 cue_position = cue->GetTranslate();
		camera->Set(glm::vec3(cue_position.x, cue_position.y + 0.5, cue_position.z), whiteBall->GetTranslate(), glm::vec3(0, 1, 0));
	}
		
}
void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE) {
		if (currentPlayer == &player1) {
			currentPlayer = &player2;
		}
		else {
			currentPlayer = &player1;
		}
		cue->renderCue = true;
		cue->deltaX = 0;
		glm::vec3 cue_position = whiteBall->GetTranslate();
		camera->Set(glm::vec3(cue_position.x, cue_position.y + 0.5, cue_position.z + 1), whiteBall->GetTranslate(), glm::vec3(0, 1, 0));
	}
}
void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		cue->deltaX += deltaX;
		camera->RotateThirdPerson_OY(deltaX * 0.001);
	}
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (button == 1)
	{
		cue->aimed = true;
		dist = cue->translateCueX;
	}
}
void Tema2::MoveWhiteBall()
{
	if (fault) {
		fault = false;
		moveWhiteBall = true;
		whiteBall->renderBall = true;
		whiteBall->translate = glm::vec3(0, 1.1, 1);
		whiteBall->speed = 0;
		whiteBall->angle = 0;
		whiteBall->movement = glm::vec3(0, 0, 0);
	}
	if (cue->shoot) {
		renderLines = false;
		moveWhiteBall = false;
		fault = false;
		gameStarted = true;
		cue->shoot = false;
		float speed = whiteBall->maxBallSpeed * dist / cue->maxAimDistance;
		whiteBall->angle = cue->angle;
		whiteBall->directionX = 1;
		whiteBall->directionZ = 1;
		whiteBall->SetSpeed(speed);
		whiteBall->isMoving = true;
		camera->Set(glm::vec3(0, 7, 0.1f), glm::vec3(0, -1, 0.05), glm::vec3(0, 1, 0));
	}
	if (cue->renderCue) {
		RenderLines();
	}
}

void Tema2::RenderLines()
{
	renderLines = false;
	Ball *testBall = new Ball(meshes["sphere"], shaders["ColorShader"],
		glm::vec3(1, 1, 1), whiteBall->translate, glm::vec3(0.2, 0.2, 0.2));
	Ball b;
	testBall->speed = 8;
	testBall->angle = cue->angle;
	testBall->Update();
	for (unsigned int j = 1; j < balls.size(); j++) {
		if (balls[j]->renderBall) {
			b.radius = 0.1f;
			b.translate = balls[j]->translate;
			if (DetectBallBallCollision(testBall, &b)) {
				renderLines = true;
				CalculateCollisionSpeed(testBall, &b);
				lines[1]->scale.z = 0.2f;
				lines[1]->rotation.y = b.angle;
				b.translate = balls[j]->translate;
				lines[1]->SetTranslate(glm::vec3(b.translate.x - 0.2 * sin(b.angle), b.translate.y, b.translate.z - 0.2 * cos(b.angle)));
				
				lines[0]->scale.z = glm::distance(testBall->translate, b.translate);
				float mid = lines[0]->scale.z / 2;
				lines[0]->rotation.y = cue->angle;
				lines[0]->SetTranslate(glm::vec3(testBall->translate.x - mid * sin(cue->angle), testBall->translate.y, testBall->translate.z - mid * cos(cue->angle)));
				break;
			}
		}
	}
}
void Tema2::ShowStatistics() {
	std::cout << "**********************************************" << std::endl;
	std::cout << "Player 1 score: " << player1.score << std::endl;
	std::cout << "Player 1 fault: " << player1.fault << std::endl;

	std::cout << "Player 2 score: " << player2.score << std::endl;
	std::cout << "Player 2 fault: " << player2.fault << std::endl;

}
void Tema2::CalculateCollisionSpeed(Ball *b1, Ball *b2) {
	glm::vec3 n = b1->translate - b2->translate;
	n = glm::normalize(n);

	float a1 = glm::dot(b1->movement, n);
	float a2 = glm::dot(b2->movement, n);

	float optimizedP = a1 - a2;

	b1->movement = b1->movement - optimizedP * n;
	b2->movement = b2->movement + optimizedP * n;

	b1->speed = sqrt(b1->movement.x * b1->movement.x + b1->movement.z * b1->movement.z);
	b2->speed = sqrt(b2->movement.x * b2->movement.x + b2->movement.z * b2->movement.z);
	computeAngle(b1);
	computeAngle(b2);
}
void Tema2::computeAngle(Ball *b) {
	
	float angle = M_PI / 2;
	if (b->movement.z != 0) {
		angle = atan(b->movement.x / b->movement.z);
		if (b->movement.z > 0) {
			angle += M_PI;
		}
	}
	b->angle = angle;
}
bool Tema2::DetectBallBallCollision(Ball *b1, Ball *b2) {

	float dist = glm::distance(b1->translate, b2->translate);
	double len = glm::length(b1->movement);
	double sumRadius = b1->radius + b2->radius;
	dist -= sumRadius;

	if (len < dist) {
		return false;
	}
	glm::vec3 normal = glm::normalize(b1->movement);
	glm::vec3 C = b2->translate - b1->translate;
	float D = glm::dot(normal, C);

	if (D <= 0) {
		return false;
	}
	
	double lengthC = glm::length(C);
	double F = (lengthC * lengthC) - D * D;

	double T = sumRadius * sumRadius - F;
	if (T < 0)
		return false;
	double distance = D - sqrt(T);
	
	if (len < distance) {
		return false;
	}
	return true;
}
void Tema2::CheckWallsCollision(Ball *b) {
	if (b->translate.x + ballSize / 2 >= tableWidth / 2 && b->movement.x > 0) {
		b->directionX *= (-1);
	}
	if (b->translate.x - ballSize / 2 <= -tableWidth / 2 && b->movement.x < 0) {
		b->directionX *= (-1);
	}
	if (b->translate.z + ballSize / 2 >= tableLength / 2 && b->movement.z > 0) {
		b->directionZ *= (-1);
	}
	if (b->translate.z - ballSize / 2 <= -tableLength / 2 && b->movement.z < 0) {
		b->directionZ *= (-1);
	}
}
void Tema2::CreateBalls() {
	float startX = 0;
	float startZ = -1.0;
	glm::vec3 red = glm::vec3(1, 0, 0);
	glm::vec3 scaleFactor = glm::vec3(0.2, 0.2, 0.2);
	glm::vec3 yellow = glm::vec3(1, 1, 0);
	whiteBall = new Ball(meshes["sphere"], shaders["ColorShader"],
		glm::vec3(1, 1, 1), glm::vec3(0, 1.1, 1), scaleFactor);
	balls.push_back(whiteBall);
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		red, glm::vec3(startX, 1.1, startZ), scaleFactor));
	startZ -= ballSize;
	startX += ballSize / 2;
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		red, glm::vec3(startX, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		yellow, glm::vec3(-startX, 1.1, startZ), scaleFactor));
	startZ -= ballSize;
	startX += ballSize / 2;
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		yellow, glm::vec3(startX, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		glm::vec3(0, 0, 0), glm::vec3(startX - ballSize, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		red, glm::vec3(startX - 2 * ballSize, 1.1, startZ), scaleFactor));
	startZ -= ballSize;
	startX += ballSize / 2;
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		yellow, glm::vec3(startX, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		red, glm::vec3(startX - ballSize, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		red, glm::vec3(startX - 2 * ballSize, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		yellow, glm::vec3(startX - 3 * ballSize, 1.1, startZ), scaleFactor));
	startZ -= ballSize;
	startX += ballSize / 2;
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		yellow, glm::vec3(startX, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		red, glm::vec3(startX - ballSize, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		yellow, glm::vec3(startX - 2 * ballSize, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		yellow, glm::vec3(startX - 3 * ballSize, 1.1, startZ), scaleFactor));
	balls.push_back(new Ball(meshes["sphere"], shaders["ColorShader"],
		red, glm::vec3(startX - 4 * ballSize, 1.1, startZ), scaleFactor));
}
