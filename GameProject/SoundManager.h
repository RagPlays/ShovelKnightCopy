#pragma once

class Settings;
class SoundEffect;
class SoundStream;

class SoundManager final
{
public:
	// Construtors / Destructors
	explicit SoundManager(Settings* settings, const std::string& soundconfigPathName);
	~SoundManager();

	SoundManager(const SoundManager& soundManager) = delete;
	SoundManager& operator=(const SoundManager& soundManager) = delete;
	SoundManager(SoundManager&& soundManager) noexcept = delete;
	SoundManager& operator=(SoundManager&& soundManager) noexcept = delete;

	// Update
	void Update();

	// Getters
	const SoundEffect* GetSoundEffect(const std::string& key);
	const SoundStream* GetMusic(const std::string& key);

	// Other
	void StopAllMusic();

private:

	std::unordered_map< std::string, SoundStream* > m_pMusics;
	std::unordered_map< std::string, SoundEffect* > m_pSoundEffects;

	Settings* m_pSettings;

	int m_CurrentMusicVolume;
	int m_CurrentSoundEffectVolume;

private:

	// Init
	void InitAllSounds(const std::string& path);

	// Destroy
	void DestroyAllSounds();

	// Update
	void UpdateAllMusicVolumes();
	void UpdateAllSoundEffectVolumes();
};