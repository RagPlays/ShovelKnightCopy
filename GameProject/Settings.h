#pragma once

class Settings final
{
public:
	// constructors / Destructors
	explicit Settings(const std::string& filePath);
	~Settings() = default;

	// Getters
	int GetSoundEffectVolume();
	int GetSoundEffectVolumePos();
	int GetMusicVolume();
	int GetMusicVolumePos();

	// VolumeFunctions
	void LowerMusicVolume();
	void IncreaseMusicVolume();
	void LowerSoundEffectVolume();
	void IncreaseSoundEffectVolume();
	void SetSoundsToDefault();
	void SaveSettingsToFile();

private:

	const std::string m_FilePath;
	const float m_SoundMuliplier;	// 128 / 20 = 6.4 per step

	int m_MusicVolumePosition;		// range 0-20 for in settings
	int m_SoundEffectVolumePosition;	// range 0-20 for in settings
	int m_MusicVolume;				// actual volume range 0-128
	int m_SoundEffectVolume;			// actual volume range 0-128

private:

	// Init
	void InitSettingsFromFile();
};