#pragma once

#include "Tile.h"

class Camera;
class Texture;
class Tile;
class TextureManager;

class TileMap final
{
public:
	// Constructor / Destructors
	explicit TileMap(TextureManager* textureManager);
	~TileMap();

	TileMap(const TileMap& tileMap) = delete;
	TileMap& operator=(const TileMap& tileMap) = delete;
	TileMap(TileMap&& tileMap) noexcept = delete;
	TileMap& operator=(TileMap&& tileMap) noexcept = delete;

	// Update / Draw
	void Draw() const;

	// Getters / Setters
	float GetParallaxScale() const;
	const Point2i& GetMapDimentions() const;
	void SetDrawArea(const Point2f& cameraPos, const Rectf& windowBorder, bool testing = false);
	void SetDefaultDrawArea();

	// Others
	void AddTile(const Point2i& gridPos, const Rectf& srcRect);
	void RemoveTile(const Point2i& gridPos);

	// Loading / Saving
	void SaveToFile(const std::string& pathName);
	bool LoadFromFile(const std::string& pathName);

private:

	const float m_Scale;

	TextureManager* m_pTextureManager;

	std::vector< std::vector< std::vector< Tile* > > > m_pTileMap;
		
	float m_GridSizeWorld;
	Point2i m_MaxWorldSize;
	float m_ParallaxScale;
	std::string m_TileMapTextureKey;

	int m_StartX{};
	int m_EndX{};
	int m_StartY{};
	int m_EndY{};

private:

	// Destroy
	void DestroyTileMap();
};