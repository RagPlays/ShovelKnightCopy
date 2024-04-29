#pragma once

class Texture;

class Animation final
{
public:
	// Constructors / Destructors
	explicit Animation(Rectf& srcRect, const Rectf& startSrcRect, int nrOfFrames, float fps, float firstFrameDelay = 0.f, float endDelay = 0.f);
	~Animation() = default;
	
	// Update / Draw
	bool Play(float elapsedSec);

	// Getters / Setters
	void SetCurrentFrame(const int frameNr);
	const int GetCurrentFrame() const;
	const Rectf& GetStartRect() const;
	const float GetFPS() const;
	const int GetNrOfFrames() const;

private:

	Rectf& m_ActualRect;
	const Rectf m_StartRect;
	float m_FrameWidth;

	int m_CurrentFrame;
	const int m_NrOfFrames;

	float m_FrameTime;
	const float m_FramesPerSeconds;

	float m_FirstEndFrameTime;
	const float m_FirstFrameDelay;
	const float m_EndFrameDelay;
};