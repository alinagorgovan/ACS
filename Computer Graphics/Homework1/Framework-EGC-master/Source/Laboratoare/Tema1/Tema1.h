#pragma once

#include <Component/SimpleScene.h>
#include <map>
#include <vector>

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	public:
		struct brick {
			brick() {}
			brick(float x, float y, int powerup)
				:  x(x), y(y), visible(true), scale_x(1), scale_y(1), powerup(powerup), level(1) {}
			float x;
			float y;
			float scale_x;
			float scale_y;
			int powerup;
			int level;
			bool visible;
		};
		struct ball {
			ball() {}
			ball(float x, float y, float radius, float speed_x, float speed_y, float angle)
				: x(x), y(y), radius(radius), speed_x(speed_x), speed_y(speed_y), angle(angle) {}
			float speed_x;
			float speed_y;
			float x;
			float y;
			float radius;
			float angle;
		};
		struct platform {
			platform() {}
			platform(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};
		struct wall {
			wall() {}
			wall(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};
	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void RenderBall();
		void RenderPlatform();
		void RenderBricks(float deltaTimeSeconds);
		void RenderWalls();
		void RenderLives();
		void SetLevel2();
		void SetLevel1();
		void CheckPlatformCollision();
		void CheckWallsCollision();
		void CheckBricksCollision();
		void CheckPowerUpsCollision();
		void Reset();
	public:
		ball game_ball;
		platform game_platform;
		platform initial_platform;
		std::vector<int> powerups;
		std::vector<brick> bricks;
		std::map<std::string, wall> walls;
		glm::vec2 brick_size;
		glm::mat3 modelMatrix;
		int lives;
		bool game_started;
		glm::vec2 mouse;
		float angularSpeed;
		int brick_columns;
		int brick_lines;
		bool level1_done = false;
		bool game_end;
};

