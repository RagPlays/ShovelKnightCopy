#include "pch.h"
#include "SoundManager.h"
#include "Settings.h"

// Constructors / Destructors
SoundManager::SoundManager(Settings* settings, const std::string& soundconfigPathName)
	: m_pSettings{ settings }
{
	InitAllSounds(soundconfigPathName);

	std::cout << "\nAll Sounds Created...\n";
}

SoundManager::~SoundManager()
{
	DestroyAllSounds();

	std::cout << "\nDestroyed All Sounds\n";
}

// Update
void SoundManager::Update()
{
	/*std::cout << "SoundVol: " << m_CurrentSoundEffectVolume << "\n";
	std::cout << "MusicVol: " << m_CurrentMusicVolume << "\n\n";*/

	// Update All Volumes
	UpdateAllMusicVolumes();
	UpdateAllSoundEffectVolumes();
}

// Getters
const SoundEffect* SoundManager::GetSoundEffect(const std::string& key)
{
	return m_pSoundEffects.at(key);
}

const SoundStream* SoundManager::GetMusic(const std::string& key)
{
	return m_pMusics.at(key);
}

// Other
void SoundManager::StopAllMusic()
{
	for (std::pair< std::string, SoundStream* > pmusic : m_pMusics)
	{
		if (pmusic.second->IsPlaying())
		{
			pmusic.second->Stop();
		}
	}
}

// Private Functions //
// Init
void SoundManager::InitAllSounds(const std::string& path)
{
	std::ifstream inFile{};
	inFile.open(path);

	// variables
	std::string key{};
	std::string loadpath{};

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::SOUNDMANAGER::CPP::COULD_NOT_LOAD_SOUNDS_FROM_FILE: " << path << "\n";
	}
	else
	{
		std::string soundType{};

		while (inFile >> soundType)
		{
			if (soundType == "music")
			{
				inFile >> key >> loadpath;
				m_pMusics.insert(std::pair < std::string, SoundStream*>{key, new SoundStream{ loadpath }});

				if (!m_pMusics[key]->IsLoaded())
				{
					std::cerr << "ERROR::SOUNDMANAGER::CPP::MUSIC_WAS_NOT_LOADED_CORRECTLY\n";
				}
			}
			else if (soundType == "soundEff")
			{
				inFile >> key >> loadpath;
				m_pSoundEffects.insert(std::pair < std::string, SoundEffect*>{key, new SoundEffect{ loadpath }});

				if (!m_pSoundEffects[key]->IsLoaded())
				{
					std::cerr << "ERROR::SOUNDMANAGER::CPP::SOUNDEFFECT_WAS_NOT_LOADED_CORRECTLY\n";
				}
			}
			else
			{
				std::cerr << "ERROR::SOUNDMANAGER::CPP::COULD_NOT_FIND_SOUNDTYPE\n";
			}
		}
	}
	inFile.close();

}

// Destroy
void SoundManager::DestroyAllSounds()
{
	// Music
	for (const std::pair< std::string, SoundStream* >& pmusic : m_pMusics)
	{
		delete pmusic.second;
	}
	m_pMusics.clear();

	// SoundsEffects
	for (const std::pair< std::string, SoundEffect* >& psoundEffect : m_pSoundEffects)
	{
		delete psoundEffect.second;
	}
	m_pSoundEffects.clear();
}

// Update
void SoundManager::UpdateAllMusicVolumes()
{
	if (m_pSettings->GetMusicVolume() != m_CurrentMusicVolume)
	{
		m_CurrentMusicVolume = m_pSettings->GetMusicVolume();

		for (std::pair< std::string, SoundStream* > music : m_pMusics)
		{
			music.second->SetVolume(m_CurrentMusicVolume);
		}
	}
}

void SoundManager::UpdateAllSoundEffectVolumes()
{
	if (m_pSettings->GetSoundEffectVolume() != m_CurrentSoundEffectVolume)
	{
		m_CurrentSoundEffectVolume = m_pSettings->GetSoundEffectVolume();

		for (std::pair< std::string, SoundEffect* > soundEffect : m_pSoundEffects)
		{
			soundEffect.second->SetVolume(m_CurrentSoundEffectVolume);
		}
	}
}