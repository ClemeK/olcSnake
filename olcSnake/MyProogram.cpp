#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>>

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "olcSnake";
	}
public:
	// ************************************************
	struct snakePart
	{
		int x;
		int y;
	};

	int charSize = 10;
	int snakeSpeed = 3;
	std::list<snakePart> snake = { };
	int currentDirection = 3; // 0=North, 1=East, 2=South, 4=West
	int previousDirection = 0;
	olc::vi2d food{ rand() % ScreenWidth() , rand() % ScreenHeight() };
	int score = 0;
	bool dead = false;

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		snake.push_front({ ScreenWidth() / 2, ScreenHeight() / 2 });

		return true;
	}
	// ************************************************
	bool Between(int nbr, int lower, int higher)
	{
		if (nbr < lower)
		{
			return false;
		}

		if (nbr > higher)
		{
			return false;
		}

		return true;
	}

	bool InVincinity(snakePart s, olc::vi2d f, int range)
	{
		if (Between(f.x, s.x - range, s.x + range))
		{
			if (Between(f.y, s.y - range, s.y + range))
			{
				// eat food
				return true;
			}
		}

		return false;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear the screen
		Clear(olc::BLACK);

		Sleep(20);

		if (!dead && GetKey(olc::LEFT).bReleased || GetKey(olc::A).bReleased)
		{
			previousDirection = currentDirection;
			currentDirection--;
			if (currentDirection == -1) currentDirection = 3;
		}
		if (!dead && GetKey(olc::RIGHT).bReleased || GetKey(olc::D).bReleased)
		{
			previousDirection = currentDirection;
			currentDirection++;
			if (currentDirection == 4) currentDirection = 0;
		}
		if (GetKey(olc::ESCAPE).bReleased)
		{
			return 0;
		}
		if (dead && GetKey(olc::SPACE).bReleased)
		{
			snake.clear();
			snake.push_front({ ScreenWidth() / 2, ScreenHeight() / 2 });
			currentDirection = 3;
			previousDirection = 0;
			food.x = rand() % ScreenWidth();
			food.y = rand() % ScreenHeight();
			score = 0;
			dead = false;
		}

		// Check snake is on screen
		if (snake.front().y < 0 || snake.front().y > ScreenHeight())
		{
			dead = true;
			if (snake.front().y < 0)
			{
				snake.front().y = 0;
			}
			else if (snake.front().y > ScreenHeight() - charSize)
			{
				snake.front().y = ScreenHeight() - 15;
			}
		}
		if (snake.front().x < 0 || snake.front().x > ScreenWidth() - charSize)
		{
			dead = true;
			if (snake.front().x < 0)
			{
				snake.front().x = 0;
			}
			else if (snake.front().x > ScreenWidth() - 15)
			{
				snake.front().x = ScreenWidth();
			}
		}

		// Check the snake hasn't hit itself
		for (std::list<snakePart>::iterator i = snake.begin(); i != snake.end(); i++)
		{
			if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
			{
				dead = true;
			}
		}

		// If Food is eaten, recreate somewhere else
		if (InVincinity(snake.front(), food, 5))
		{
			score += 10;
			food.x = rand() % ScreenWidth();
			food.y = rand() % ScreenHeight();

			// Add to snake body
			for (size_t i = 0; i < 3; i++)
			{
				switch (previousDirection)
				{
					case 0: // Up - North
						snake.push_back({ snake.back().x, snake.back().y - charSize });
						break;
					case 1: // Right - East
						snake.push_back({ snake.back().x + charSize, snake.back().y });
						break;
					case 2: // Down - South
						snake.push_back({ snake.back().x, snake.back().y + charSize });
						break;
					case 3: // Left - West
						snake.push_back({ snake.back().x - charSize, snake.back().y });
						break;
				}
			}
		}

		// Update snake
		switch (currentDirection)
		{
			case 0: // Up - North
				snake.push_front({ snake.front().x, snake.front().y - snakeSpeed });
				break;
			case 1: // Right - East
				snake.push_front({ snake.front().x + snakeSpeed, snake.front().y });
				break;
			case 2: // Down - South
				snake.push_front({ snake.front().x, snake.front().y + snakeSpeed });
				break;
			case 3: // Left - West
				snake.push_front({ snake.front().x - snakeSpeed, snake.front().y });
				break;
		}
		snake.pop_back();

		// Draw Snake
		// Snake Body
		for (auto seg : snake)
		{
			DrawString(seg.x, seg.y, dead ? "+" : "O", olc::RED);
		}
		//Snake Head
		DrawString(snake.front().x, snake.front().y, dead ? "X" : "@", olc::RED);

		// Draw Food
		DrawString({ food }, "%", olc::GREEN);

		// Draw the Headings
		for (size_t i = 0; i < ScreenWidth(); i++)
		{
		DrawString(i,1,"=", olc::WHITE);

		DrawString(i, 20, "=", olc::WHITE);
		}
		DrawString(5, 10, "Score:", olc::WHITE);
		DrawString(55, 10, std::to_string(score), olc::WHITE);
		DrawString(70, 10, "- S n a k e -",olc::WHITE);
		DrawString(190, 10, "Level:", olc::WHITE);
		DrawString(240, 10, std::to_string(snakeSpeed), olc::WHITE);




		if (dead)
		{
			DrawString(10, 30, "Press Space Bar to play again", olc::YELLOW);
		}

		return true;
	}
};
// ************************************************
int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}