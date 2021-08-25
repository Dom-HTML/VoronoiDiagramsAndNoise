#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "iostream"
#include <chrono>

struct ColourAlusio
{
	int r;
	int g;
	int b;
};

struct Vector2
{
	int x;
	int y;
};

struct Point
{
	Vector2 position;
	ColourAlusio cellColour;
};

int screenWidth = 256 * 4;
int screenHeight = 240 * 4;

const int numPoints = 30;

float p = 20;

ColourAlusio bgColour{ 0, 0, 0};
ColourAlusio pntColour{ 0, 0, 255 };
//ColourAlusio pntColour = ColourAlusio{ 0, 0, 256 };

Point* generatedPoints;

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	Example()
	{		
		// Name your application
		sAppName = "AGE  |  v 0.1 |  olcPixelGameEngine ";
	}

public:
	bool OnUserCreate() override // Called once at the start, so create things here
	{								
		start = std::chrono::steady_clock::now();
		srand(time(0));
		
		generatedPoints = generatePoints();
		generateColours(generatedPoints);
		
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				//Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
				Draw(x, y, olc::Pixel(bgColour.r, bgColour.g, bgColour.b));

		colourCells(generatedPoints);
		
		for (int i = 0; i < numPoints; i++)
		{
			//Draw(generatedPoints[i].position.x, generatedPoints[i].position.y, olc::Pixel(generatedPoints[i].cellColour.r, generatedPoints[i].cellColour.g, generatedPoints[i].cellColour.b));
			//Draw(generatedPoints[i].position.x, generatedPoints[i].position.y, olc::Pixel(0, 0, 0));
			DrawCircle(generatedPoints[i].position.x, generatedPoints[i].position.y, 3,olc::Pixel(0, 0, 0));
			FillCircle(generatedPoints[i].position.x, generatedPoints[i].position.y, 2, olc::Pixel(255, 255, 255));
		}	
		
		end = std::chrono::steady_clock::now();
		std::cout << "Elapsed time in milliseconds: "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
			<< " ms" << std::endl;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override // Called once per frame
	{								
			
		return true;
	}
	
public:
	//all my custon functions ill put here
	int randColour()
	{
		int newColour = rand() % 256;
		return newColour;
	}

	Point * generatePoints()
	{
		static Point points[numPoints];
		
		for (int i = 0; i < numPoints; i++) 
		{
			points[i].position.x = int(rand() % screenWidth);
			points[i].position.y = int(rand() % screenHeight);
			std::cout << points[i].position.x;
			std::cout << ",";
			std::cout << points[i].position.y;
			std::cout << " ";
		}

		return points;
	}

	//randoly generates the colours the cells will be 
	void generateColours(Point* points)
	{
		for (int i = 0; i < numPoints; i++)
		{
			points[i].cellColour = ColourAlusio{ rand() % 255,rand() % 255 ,rand() % 255 };
		}
	}

	void colourCells(Point* points) {
		for (int x = 0; x < screenWidth; x++)
		{
			for (int y = 0; y < screenHeight; y++)
			{
				bool first = true;
				Point closest;
				float closestDst;
				for (int i = 0; i < numPoints; i++)
				{
					if (first == true) 
					{
						closest = points[0];				
						//closestDst = EuclideanDistance(x , points[0].position.x, y, points[0].position.y);
						//closestDst = ManhattanDistance(x, points[0].position.x, y, points[0].position.y);
						closestDst = MinkowskiDistance(x, points[0].position.x, y, points[0].position.y);
						first = false;
					}

					//float distance = EuclideanDistance(x, points[i].position.x, y, points[i].position.y);
					//float distance = ManhattanDistance(x, points[i].position.x, y, points[i].position.y);
					float distance = MinkowskiDistance(x, points[i].position.x, y, points[i].position.y);

					if (distance <= closestDst) {
						closest = points[i];
						closestDst = distance;
					}
				}
				Draw(x, y, olc::Pixel(closest.cellColour.r, closest.cellColour.g, closest.cellColour.b));
			}
		}
	}

	float EuclideanDistance(int x1, int x2, int y1, int y2)
	{	
		float distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

		return distance;
	}

	float ManhattanDistance(int x1, int x2, int y1, int y2) 
	{
		float distance = abs(x1 - x2) + abs(y1 - y2);

		return distance;
	}

	float MinkowskiDistance(int x1, int x2, int y1, int y2)
	{
		float distance = pow(pow(abs(x1 - x2), p) + pow(abs(y1 - y2), p), 1 / p);

		return distance;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(screenWidth, screenHeight, 1, 1, false, true))
		demo.Start();
	return 0;
}
