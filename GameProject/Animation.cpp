#include "pch.h"
#include "Animation.h"

// Constructors / Destructors
Animation::Animation(Rectf& srcRect, const Rectf& startSrcRect, int nrOfFrames, float fps, float startDelay, float endDelay)
	: m_ActualRect{ srcRect }
	, m_StartRect{ startSrcRect }
	, m_NrOfFrames{ nrOfFrames }
	, m_FramesPerSeconds{ fps }
	, m_FirstFrameDelay{ startDelay }
	, m_EndFrameDelay{ endDelay }
	, m_FrameWidth{ startSrcRect.width }
	, m_CurrentFrame{ 1 }
	, m_FrameTime{ }
	, m_FirstEndFrameTime{ }
{
}

// Player
bool Animation::Play(float elapsedSec)
{
	bool isDone{ false };
	if (m_CurrentFrame == 1)
	{
		m_ActualRect = m_StartRect;
		m_ActualRect.width = m_StartRect.width;
		m_ActualRect.height = m_StartRect.height;

		if (utils::GetClockTime(m_FirstEndFrameTime, elapsedSec, m_FirstFrameDelay + (1.f / m_FramesPerSeconds)))
		{
			m_ActualRect.left += m_FrameWidth;
			++m_CurrentFrame;
		}
	}
	else if (m_CurrentFrame == m_NrOfFrames)
	{
		if (utils::GetClockTime(m_FirstEndFrameTime, elapsedSec, m_EndFrameDelay + (1.f / m_FramesPerSeconds)))
		{
			// Reset to start
			m_ActualRect = m_StartRect;
			m_CurrentFrame = 1;
			isDone = true;
		}
	}
	else if (utils::GetClockTime(m_FrameTime, elapsedSec, (1.f / m_FramesPerSeconds)))
	{
		m_ActualRect.width = m_StartRect.width;
		m_ActualRect.height = m_StartRect.height;
		m_ActualRect.left += m_FrameWidth;
		++m_CurrentFrame;
	}
	return isDone;
}

// Getters / Setters
void Animation::SetCurrentFrame(const int frameNr)
{
	if (frameNr > 0 && frameNr < m_NrOfFrames)
	{
		m_CurrentFrame = frameNr;
	}
	else
	{
		std::cout << "CurrentFrame not Available!\n";
	}
}

const int Animation::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

const Rectf& Animation::GetStartRect() const
{
	return m_StartRect;
}

const float Animation::GetFPS() const
{
	return m_FramesPerSeconds;
}

const int Animation::GetNrOfFrames() const
{
	return m_NrOfFrames;
}