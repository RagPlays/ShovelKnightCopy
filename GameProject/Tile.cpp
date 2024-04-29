#include "pch.h"
#include "Tile.h"

// Constructors / Destructors
Tile::Tile(const Point2i gridPos, float gridSize, const Rectf& srcRect)
	: m_TileDest{ Point2f{gridPos.x * gridSize, gridPos.y * gridSize }, gridSize, gridSize }
	, m_SrcRect{ srcRect }
{
}

// Getters / Setters
const Rectf& Tile::GetPosition() const
{
	return m_TileDest;
}

const std::string Tile::GetTileInfo() const
{
	return std::to_string(int(m_SrcRect.left)) + " " + std::to_string(int(m_SrcRect.bottom));
}

// Draw
void Tile::Draw(const Texture* texture) const
{
	texture->Draw(m_TileDest, m_SrcRect);
}