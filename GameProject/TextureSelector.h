#pragma once

class Texture;

class TextureSelector final
{
public:
	// Constructors / Destructors
	explicit TextureSelector(const Point2f& pos, int gridsize, const Texture* textureSheet);
	~TextureSelector() = default;

	// Update / Draw
	void Update(float elapsedSec, const Point2f& mousePos);
	void Draw() const;

	// Getters / Setters
	void SetIsActive(bool active);
	bool GetIsActive() const;
	void SetIsHidden(bool hidden);
	bool GetIsHidden() const;
	const Rectf& GetSrcRect() const;

private:

	const float m_Scale;

	float m_GridSizeScaled;

	bool m_IsActive;
	bool m_IsHidden;

	Rectf m_SelectorWindow;			// window of the whole selector
	Rectf m_Selector;				// little square that shows what item is selected
	const Texture* m_pTexture;		// the tileSheet
	Point2i m_MousePosGrid;			// the mousePos of the grid
	Rectf m_TextureSrcRect;			// srcRect of the selected tile
};

