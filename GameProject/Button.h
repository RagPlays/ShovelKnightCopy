#pragma once

class Texture;
class TextureManager;

class Button final
{
public:
	// Constructors / Destructors
	explicit Button(const int buttonNumber, const Rectf& destRect, int id, TextureManager* textureManager);
	~Button() = default;

	// Update / Draw
	void Update(float elapsedSec, int activeID);
	void Draw() const;

	// Getters / Setters
	int GetID() const;
	bool GetIsSelected() const;
	const Rectf& GetDrawRect() const;

private:

	TextureManager* m_pTextureManager;

	const int m_ButtonNumber;

	const float m_Scale;

	const int m_ID;
	const Rectf m_DestRect;

	bool m_SelectIdx;
	Rectf m_LeftSelectDestRect;			// right positions for select textures (blue outline selected button)
	Rectf m_RightSelectDestRect;
	Rectf m_LeftSrcRect;		// the src rect for the right texture (blue outline selected button)
	Rectf m_RightSrcRect;

	float m_SelectTimer;
	bool m_IsSelected;

private:

	// Init
	void InitSelectorRects();
};