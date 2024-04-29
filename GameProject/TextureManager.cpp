#include "pch.h"
#include "TextureManager.h"

// Constructors / Destructors
TextureManager::TextureManager(const std::string& pathName)
	: m_PathName{ pathName }
{
	InitTexturesFromFile();
}

TextureManager::~TextureManager()
{
	DestroyTextures();
}

// Getters
Texture* TextureManager::GetTexture(const std::string& key)
{
	return m_pTextures.at(key);
}

std::vector<Texture*>& TextureManager::GetTextureVector(const std::string& key)
{
	return m_pVectorTexures.at(key);
}

// Private Functions
// Init
void TextureManager::InitTexturesFromFile()
{
	// load from file
	std::ifstream inFile{};
	inFile.open(m_PathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::TEXTUREMANAGER::CPP::COULD_NOT_LOAD_TEXTURES_FROM_FILE: " << m_PathName << "\n";
	}
	else
	{
		std::string textureType{};
		std::string key{};
		std::string loadPath{};
		size_t textureAmount{};

		while (inFile >> textureType)
		{
			if (textureType == "T")	// texture
			{
				inFile >> key >> loadPath;

				m_pTextures.insert(std::pair< std::string, Texture* >{ key, new Texture{ loadPath } } );
			}
			else if (textureType == "VT")	// vector texture
			{
				inFile >> key >> loadPath >> textureAmount;

				m_pVectorTexures.insert(std::pair< std::string, std::vector< Texture* > >{ key, std::vector< Texture* >{} });
				m_pVectorTexures.at(key).reserve(textureAmount);
				
				for (size_t idx{}; idx < textureAmount; ++idx)
				{
					std::string path{ loadPath + std::to_string(idx) + ".png" };
					m_pVectorTexures.at(key).emplace_back(new Texture{ path });
				}
			}
		}
	}
	inFile.close();
}

// Destroy
void TextureManager::DestroyTextures()
{
	// texture map
	for (std::pair< std::string, Texture* > texture : m_pTextures)
	{
		delete texture.second;
	}
	m_pTextures.clear();

	// vectorTexture map
	for (std::pair< std::string, std::vector< Texture* > > vecTexture : m_pVectorTexures)
	{
		for (size_t idx{}; idx < vecTexture.second.size(); ++idx)
		{
			delete vecTexture.second[idx];

			if (idx == (vecTexture.second.size() - 1))	// if last idx then clear whole vector
			{
				vecTexture.second.clear();
			}
		}
	}
	m_pVectorTexures.clear();
}