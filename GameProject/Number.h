#pragma once

class Texture;
class TextureManager;

class Number final
{
public:
	// Constructors / Destructor
	explicit Number(const Point2f& drawPos, TextureManager* textureManager, float scale = 1.f);
	~Number() = default;
	
	// Update / Draw
	void Update(float elapsedSec);
	void Draw();

	// Getters / Setters
	int GetNumber() const;
	void SetNumber(int num);
	void AddNumber(int num);

private:
	
	TextureManager* m_pTextureManager;

	Rectf m_DrawRect;	// where the number needs to start drawing

	int m_Number;			// the actual number
	int m_CurrentNumber;	// the current number that is drawn

	float m_AddNumberTime;	// tracks the time that is past to add value to number

private:

	// Getters
	int GetMultiply() const;
};

