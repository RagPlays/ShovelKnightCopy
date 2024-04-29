#pragma once

class Texture;

class Tile final
{
public:
	// Constructors / Destructors
	explicit Tile(const Point2i gridPos, float gridSize, const Rectf& srcRect);
	~Tile() = default;

	// Getters / Setters
	const Rectf& GetPosition() const;
	const std::string GetTileInfo() const;

	// Draw
	void Draw(const Texture* texture) const;
	
private:
	const Rectf m_TileDest;
	const Rectf m_SrcRect;
};