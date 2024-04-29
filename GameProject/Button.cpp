#include "pch.h"
#include "Button.h"
#include "TextureManager.h"

// Constructors / Destructors
Button::Button(const int buttonNumber, const Rectf& destRect, int id, TextureManager* textureManager)
	: m_pTextureManager{ textureManager }
	, m_ButtonNumber{ buttonNumber }
	, m_DestRect{ destRect }
	, m_ID{ id }
	, m_SelectTimer{ 0.f }
	, m_SelectIdx { false } // 0
	, m_Scale { 3.5f }
{
	InitSelectorRects();
}

// Update / Draw
void Button::Update(float elapsedSec, int activeID)
{
	if (m_ID == activeID)
	{
		m_IsSelected = true;
		if (utils::GetClockTime(m_SelectTimer, elapsedSec, 0.5f))
		{
			m_SelectIdx = m_SelectIdx ? false : true;	// switching the selectidx (for select textures)
		}
	}
	else m_IsSelected = false;
}

void Button::Draw() const
{
	// Draw the button texture
	m_pTextureManager->GetTextureVector("buttons")[m_ButtonNumber]->Draw(m_DestRect);

	if (m_IsSelected)
	{
		// Draw the selector textures
		m_pTextureManager->GetTextureVector("selectTextures")[m_SelectIdx]->Draw(m_LeftSelectDestRect, m_LeftSrcRect);
		m_pTextureManager->GetTextureVector("selectTextures")[m_SelectIdx]->Draw(m_RightSelectDestRect, m_RightSrcRect);
	}
}

// Getters / Setters
int Button::GetID() const
{
	return m_ID;
}

bool Button::GetIsSelected() const
{
	return m_IsSelected;
}

const Rectf& Button::GetDrawRect() const
{
	return m_DestRect;
}

// Init
void Button::InitSelectorRects()
{
	// SrcRects
	m_LeftSrcRect =
		Rectf{
			0.f,
			m_pTextureManager->GetTextureVector("selectTextures")[0]->GetHeight(),			// using the 0 idx but it doesnt matter because png of both have equal dimentions
			m_pTextureManager->GetTextureVector("selectTextures")[0]->GetWidth() / 2.f,
			m_pTextureManager->GetTextureVector("selectTextures")[0]->GetHeight() };

	m_RightSrcRect =
		Rectf{ 
			m_pTextureManager->GetTextureVector("selectTextures")[0]->GetWidth() / 2.f,
			m_pTextureManager->GetTextureVector("selectTextures")[0]->GetHeight(),
			m_pTextureManager->GetTextureVector("selectTextures")[0]->GetWidth() / 2.f,
			m_pTextureManager->GetTextureVector("selectTextures")[0]->GetHeight() };

	// DestRects

	// Left
	m_LeftSelectDestRect.width = m_LeftSrcRect.width * m_Scale;
	m_LeftSelectDestRect.height = m_LeftSrcRect.height * m_Scale;

	m_LeftSelectDestRect.left = m_DestRect.left - (m_LeftSelectDestRect.width / 2.f);
	m_LeftSelectDestRect.bottom = m_DestRect.bottom - ((m_LeftSelectDestRect.height - m_DestRect.height) / 2.f);

	// Right
	m_RightSelectDestRect.width = m_LeftSelectDestRect.width;
	m_RightSelectDestRect.height = m_LeftSelectDestRect.height;

	m_RightSelectDestRect.left = (m_DestRect.left + m_DestRect.width) - (m_RightSelectDestRect.width / 2.f);
	m_RightSelectDestRect.bottom = m_LeftSelectDestRect.bottom;
}