#include "pch.h"
#include "Settings.h"

// Constructors / Destructors
Settings::Settings(const std::string& filePath)
	: m_SoundMuliplier{ 6.4f }
	, m_SoundEffectVolume{}
	, m_SoundEffectVolumePosition{}
	, m_MusicVolume{}
	, m_MusicVolumePosition{}
	, m_FilePath{ filePath }
{
	InitSettingsFromFile();
}

// Getters
int Settings::GetSoundEffectVolume()
{
	return m_SoundEffectVolume;
}

int Settings::GetSoundEffectVolumePos()
{
	return m_SoundEffectVolumePosition;
}

int Settings::GetMusicVolume()
{
	return m_MusicVolume;
}

int Settings::GetMusicVolumePos()
{
	return m_MusicVolumePosition;
}

// VolumeFunctions
void Settings::LowerMusicVolume()
{
	if (m_MusicVolumePosition > 0)
	{
		--m_MusicVolumePosition;
	}
	m_MusicVolume = int(std::round(m_MusicVolumePosition * m_SoundMuliplier));
}

void Settings::IncreaseMusicVolume()
{
	if (m_MusicVolumePosition < 20)
	{
		++m_MusicVolumePosition;
	}
	m_MusicVolume = int(std::round(m_MusicVolumePosition * m_SoundMuliplier));
}

void Settings::LowerSoundEffectVolume()
{
	if (m_SoundEffectVolumePosition > 0)
	{
		--m_SoundEffectVolumePosition;
	}
	m_SoundEffectVolume = int(std::round(m_SoundEffectVolumePosition * m_SoundMuliplier));
}

void Settings::IncreaseSoundEffectVolume()
{
	if (m_SoundEffectVolumePosition < 20)
	{
		++m_SoundEffectVolumePosition;
	}
	m_SoundEffectVolume = int(std::round(m_SoundEffectVolumePosition * m_SoundMuliplier));
}

void Settings::SetSoundsToDefault()
{
	m_MusicVolumePosition = 10;
	m_SoundEffectVolumePosition = 10;
	m_MusicVolume = int(std::round(m_MusicVolumePosition * m_SoundMuliplier));
	m_SoundEffectVolume = int(std::round(m_SoundEffectVolumePosition * m_SoundMuliplier));
}

void Settings::SaveSettingsToFile()
{
	std::ofstream outFile{};
	outFile.open(m_FilePath);

	if (!outFile.is_open())
	{
		std::cerr << "ERROR::SETTINGS::CPP::COULD_NOT_SAVE_SETTINGS_IN_FILE: " << m_FilePath << "\n";
	}
	else
	{
		outFile << "musicVol: " << m_MusicVolumePosition << "\n";
		outFile << "soundEffectVol: " << m_SoundEffectVolumePosition;
	}
	outFile.close();

	std::cout << "\nSettings Saved!\n";
}

// Private Functions //
// Init
void Settings::InitSettingsFromFile()
{
	std::ifstream inFile{};
	inFile.open(m_FilePath);

	// variables
	int value{};

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::SETTINGS::CPP::COULD_NOT_LOAD_SETTINGS_FROM_FILE: " << m_FilePath << "\n";
	}
	else
	{
		std::string settingName{};

		while (inFile >> settingName)
		{
			if (settingName == "musicVol:")
			{
				inFile >> value;
				m_MusicVolumePosition = value;
				m_MusicVolume = int(std::round(m_MusicVolumePosition * m_SoundMuliplier));
			}
			else if (settingName == "soundEffectVol:")
			{
				inFile >> value;
				m_SoundEffectVolumePosition = value;
				m_SoundEffectVolume = int(std::round(m_SoundEffectVolumePosition * m_SoundMuliplier));
			}
			else
			{
				std::cerr << "ERROR::OBJECTMANAGER::CPP::COULD_NOT_FIND_SPECIFIED_OBJECTNAME\n";
			}
		}
	}
	inFile.close();
}