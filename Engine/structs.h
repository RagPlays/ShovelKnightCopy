#pragma once
#include <string>

struct Window
{
	explicit Window(const std::string& title = "Title", float width = 320.0f, 
		float height = 180.0f, bool isVSyncOn = false);

	std::string title;
	float width;
	float height;
	bool isVSyncOn;
};

struct Point2f
{
	Point2f();
	explicit Point2f(float x, float y);

	float x;
	float y;
};

struct Point2i
{
	Point2i();
	explicit Point2i(int x, int y);

	int x;
	int y;
};


struct Rectf
{
	Rectf();
	explicit Rectf(float left, float bottom, float width, float height);
	explicit Rectf(const Point2f& leftbottom, float width, float height);
	
	float left;
	float bottom;
	float width;
	float height;
};


struct Color4f
{
	Color4f();
	explicit Color4f(float r, float g, float b, float a);
	
	float r;
	float g;
	float b;
	float a;
};

struct Circlef
{
	Circlef();
	explicit Circlef(const Point2f& center, float radius);
	explicit Circlef(float centerX, float centerY, float radius);

	Point2f center;
	float radius;
};


struct Ellipsef
{
	Ellipsef();
	explicit Ellipsef(const Point2f& center, float radiusX, float radiusY);
	explicit Ellipsef(float centerX, float centerY, float radiusX, float radiusY);

	Point2f center;
	float radiusX;
	float radiusY;
};

struct Rayf
{
	Rayf();
	explicit Rayf(const Point2f& pointOne, const Point2f& pointTwo);
	explicit Rayf(float x1, float y1, float x2, float y2);

	Point2f pointOne;
	Point2f pointTwo;
};

struct LadderRectf
{
	LadderRectf();
	explicit LadderRectf(const Rectf& rect, bool endRect);
	explicit LadderRectf(float left, float bottom, float width, float height, bool endRect);
	Rectf rect;
	bool endRect;
	bool isOnScreen;
};