#pragma once
#include <vector>
#include "Vector2f.h"

class Texture;
class vector;

namespace utils
{
#pragma region OpenGLDrawFunctionality

	void SetColor(const float r, const float g, const float b, const float a = 1.f);
	void SetColor(const Color4f& color);
	void SetRGBColor(int r, int g, int b);
	
	void DrawPoint(float x, float y, float pointSize = 1.0f);
	void DrawPoint(const Point2f& p, float pointSize = 1.0f);
	void DrawPoints(Point2f *pVertices, int nrVertices, float pointSize = 1.0f);

	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth = 1.0f);
	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth = 1.0f);

	void DrawTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3, float lineWidth = 1);
	void FillTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3);

	void DrawRect(float left, float bottom, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Point2f& bottomLeft, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Rectf& rect, float lineWidth = 1.0f);
	void FillRect(float left, float bottom, float width, float height);
	void FillRect(const Point2f& bottomLeft, float width, float height);
	void FillRect(const Rectf& rect);

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Point2f& center, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Ellipsef& ellipse , float lineWidth = 1.0f );
	void FillEllipse( float centerX, float centerY, float radX, float radY );
	void FillEllipse(const Ellipsef& ellipse );
	void FillEllipse(const Point2f& center, float radX, float radY);

	// Draws an arc. The angle parameters are in radians, not in degrees.
	void DrawArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f);
	// Draws an arc. The angle parameters are in radians, not in degrees.
	void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f);
	// Fills an arc. The angle parameters are in radians, not in degrees.
	void FillArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle);
	// Fills an arc. The angle parameters are in radians, not in degrees.
	void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle);

	void DrawPolygon( const std::vector<Point2f>& vertices, bool closed = true, float lineWidth = 1.0f );
	void DrawPolygon( const Point2f* pVertices, size_t nrVertices, bool closed = true, float lineWidth = 1.0f );
	void FillPolygon( const std::vector<Point2f>& vertices);
	void FillPolygon( const Point2f* pVertices, size_t nrVertices);
#pragma endregion OpenGLDrawFunctionality

#pragma region CollisionFunctionality
	struct HitInfo
	{
		HitInfo()
			: lambda{ 0 }
			, intersectPoint{ Point2f{} }
			, normal{ Vector2f{} }
		{
		}
		float lambda;
		Point2f intersectPoint;
		Vector2f normal;
	};

	float GetDistance(float x1, float y1, float x2, float y2);
	float GetDistance(const Point2f& p1, const Point2f& p2);

	bool IsPointInRect(const Point2f& p, const Rectf& r);
	bool IsPointInCircle(const Point2f& p, const Circlef& c);
	bool IsPointInPolygon( const Point2f& p, const std::vector<Point2f>& vertices );
	bool IsPointInPolygon( const Point2f& p, const Point2f* vertices, size_t nrVertices );

	bool IsOverlapping( const Point2f& a, const Point2f& b, const Circlef& c );
	bool IsOverlapping( const Point2f& a, const Point2f& b, const Rectf& r );

	bool IsOverlapping(const Rectf & r1, const Rectf & r2);
	bool IsOverlapping( const Rectf& r, const Circlef& c );
	bool IsOverlapping( const Circlef& c1, const Circlef& c2 );
	bool IsOverlapping( const std::vector<Point2f>& vertices, const Circlef& c );
	bool IsOverlapping( const Point2f* vertices, size_t nrVertices, const Circlef& c );
	bool Raycast( const Point2f* vertices, const size_t nrVertices, const Point2f& rayP1, const Point2f& rayP2, HitInfo& hitInfo );
	bool Raycast( const std::vector<Point2f>& vertices, const Point2f& rayP1, const Point2f& rayP2, HitInfo& hitInfo );

	bool IntersectLineSegments(const Point2f& p1, const Point2f& p2, const Point2f& q1, const Point2f& q2, float& outLambda1, float& outLambda2, float epsilon = 1e-6);
	float DistPointLineSegment(const Point2f& p, const Point2f& a, const Point2f& b);
	bool IsPointOnLineSegment(const Point2f& p, const Point2f& a, const Point2f& b);
	bool IntersectRectLine(const Rectf& r, const Point2f& p1, const Point2f& p2, float& intersectMin, float& intersectMax);

#pragma endregion CollisionFunctionality


#pragma region VectorFuntionality

	Point2f GetRandomPos(const Rectf& r, float border = 0.f);
	Vector2f GetRandomVector(const int range, bool negative = false);
	bool GetVecEqual(const Vector2f& vec1, const Vector2f& vec2);
	float GetRadians(const float degree);
	float GetDegrees(const float randian);
	float GetAngle(const Vector2f& vec1, const Vector2f& vec2);
	float GetAngle(const Vector2f& vec);
	float GetDotProduct(const Vector2f& vec1, const Vector2f& vec2);
	float GetCrossProduct(const Vector2f& vec1, const Vector2f& vec2);
	float GetVectorLenght(const Vector2f& vec);
	std::string	VecToString(const Vector2f& vec);
	Vector2f GetScaledVector(const Vector2f& vec, const float scalar);
	Vector2f GetNormalizedVector(const Vector2f& vec);
	Vector2f GetAddVectors(const Vector2f& vec1, const Vector2f& vec2);
	Point2f GetAddVectors(const Point2f& point, const Vector2f& vec);
	Vector2f GetSubtractVectors(const Vector2f& vec1, const Vector2f& vec2);
	Point2f GetSubtractVectors(const Point2f& point, const Vector2f& vec);
	void DrawVector(const Vector2f& vec, const Point2f& startPos = Point2f{});

#pragma endregion VectorFuntionality


#pragma region ArrayFuntionality

	void PrintArray(const int* pNumbers, const size_t size);
	void PrintArray(const int* pNumbers, int startIdx, const int endIdx);
	int GetArrayIndex(int rowIdx, int colIdx, int nrCols);
	int CountArrValue(const int* arr, const int arrSize, const int searchValue);
	int MinArrElement(const int* arr, const int arrSize);
	int MaxArrElement(const int* arr, const int arrSize);
	void SwapInArr(int* arr, const int idx1, const int idx2);
	void SwapInArr(float* arr, const int idx1, const int idx2);
	void SwapInArr(Point2f* arr, const int idx1, const int idx2);
	void SwapInArr(Texture** arr, const int idx1, const int idx2);
	void ShuffleArr(int* arr, const int arrSize, const int shuffleTimes);
	void ShuffleArr(float* arr, const int arrSize, const int shuffleTimes);
	void ShuffleArr(Point2f* arr, const int arrSize, const int shuffleTimes);
	void ShuffleArr(Texture* arr, const int arrSize, const int shuffleTimes);
	void BubbleSort(int* arr, const int arrSize);

#pragma endregion ArrayFuntionality

#pragma region VectorContainer

	void PrintVectorContainer(std::vector<int>& vec);

	void SwapInVector(std::vector <int>& vec, const size_t idx1, const size_t idx2);
	void SwapInVector(std::vector <float>& vec, const size_t idx1, const size_t idx2);
	void SwapInVector(std::vector <Texture*>& vec, const size_t idx1, const size_t idx2);

	void ShuffleVector(std::vector<int>& vec, const int shuffleTimes);
	void ShuffleVector(std::vector<float>& vec, const int shuffleTimes);
	void ShuffleVector(std::vector<Texture*>& vec, const int shuffleTimes);

#pragma endregion VectorContainer


#pragma region Color

	void PrintColor(const Color4f& color);
	Color4f GetRandomColor(const float alpha = 1.f);
	Color4f GetRGBColor(int r, int g, int b);

#pragma endregion Color

#pragma region Other

	// ClockTime
	bool GetClockTime(float& runtime, const float elapsedSec, const float maxTime);

	bool AreFloatsEqual(float one, float two, float epsilon = 0.01f);
#pragma endregion Other
}