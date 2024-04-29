#pragma once

class Texture;

class TextureManager final
{
public:
	// Constructors / Destructors
	explicit TextureManager(const std::string& pathName);
	~TextureManager();

	TextureManager(const TextureManager& textureManager) = delete;
	TextureManager& operator=(const TextureManager& textureManger) = delete;
	TextureManager(TextureManager&& textureManager) noexcept = delete;
	TextureManager& operator=(TextureManager&& textureManager) noexcept = delete;

	// Getters
	Texture* GetTexture(const std::string& key);
	std::vector< Texture* >& GetTextureVector(const std::string& key);

private:

	const std::string m_PathName;

	std::unordered_map< std::string, Texture* > m_pTextures;
	std::unordered_map< std::string, std::vector< Texture* > > m_pVectorTexures;

private:

	// Init
	void InitTexturesFromFile();

	// Destroy
	void DestroyTextures();
};