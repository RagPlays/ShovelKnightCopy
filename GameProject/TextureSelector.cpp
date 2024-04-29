#include "pch.h"
#include "TextureSelector.h"

// Constructors / Destructors
TextureSelector::TextureSelector(const Point2f& pos, int gridsize /*16pxls*/, const Texture* textureSheet)
	: m_pTexture{ textureSheet }
	, m_IsActive{ false }
	, m_IsHidden{ true }
	, m_Scale { 2.f }
{
	// Set Defaults
	m_MousePosGrid = Point2i{};
	m_GridSizeScaled = gridsize * m_Scale;

	// Selector Window
	m_SelectorWindow.left = pos.x;
	m_SelectorWindow.bottom = pos.y;
	m_SelectorWindow.width = m_pTexture->GetWidth() * m_Scale;
	m_SelectorWindow.height = m_pTexture->GetHeight() * m_Scale;

	// The selector (red rect)
	m_Selector.left = pos.x;
	m_Selector.bottom = pos.y;
	m_Selector.width = m_GridSizeScaled;
	m_Selector.height = m_GridSizeScaled;

	// srcRect
	m_TextureSrcRect.width = float(gridsize);
	m_TextureSrcRect.height = float(gridsize);
	m_TextureSrcRect.left = 0.f;
	m_TextureSrcRect.bottom = 0.f;
}

// Update / Draw
void TextureSelector::Update(float elapsedSec, const Point2f& mousePos)
{
	if (!m_IsHidden)
	{
		if (utils::IsPointInRect(mousePos, m_SelectorWindow))
		{
			m_IsActive = true;
		}
		else
		{
			m_IsActive = false;
		}

		if (m_IsActive)
		{
			m_MousePosGrid.x = int((mousePos.x - m_SelectorWindow.left) / m_GridSizeScaled );
			m_MousePosGrid.y = int((mousePos.y - m_SelectorWindow.bottom) / m_GridSizeScaled );

			m_Selector.left = m_SelectorWindow.left + m_MousePosGrid.x * m_GridSizeScaled;
			m_Selector.bottom = m_SelectorWindow.bottom + m_MousePosGrid.y * m_GridSizeScaled;
			

			// Update srcRect
			m_TextureSrcRect.left = (m_Selector.left - m_SelectorWindow.left) / m_Scale;
			m_TextureSrcRect.bottom = m_pTexture->GetHeight() - ((m_Selector.bottom - m_SelectorWindow.bottom) / m_Scale);
		}
	}
}

void TextureSelector::Draw() const
{
	if (!m_IsHidden)
	{
		// Draw the bounds
		utils::SetRGBColor(158, 86, 57);
		utils::FillRect(m_SelectorWindow);
		utils::SetRGBColor(255, 255, 255);
		utils::DrawRect(m_SelectorWindow, 3.f);
		
		// Draw the texture
		m_pTexture->Draw(m_SelectorWindow);

		if (m_IsActive)
		{
			utils::SetRGBColor(255, 20, 20);
			utils::DrawRect(m_Selector);
		}
	}
}

// Getters / Setters
void TextureSelector::SetIsActive(bool active)
{
	m_IsActive = active;
}

bool TextureSelector::GetIsActive() const
{
	return m_IsActive;
}

void TextureSelector::SetIsHidden(bool hidden)
{
	if (hidden && !m_IsHidden)
	{
		m_IsHidden = true;
		m_IsActive = false;
	}
	else
	{
		m_IsHidden = false;
	}
}

bool TextureSelector::GetIsHidden() const
{
	return m_IsHidden;
}

const Rectf& TextureSelector::GetSrcRect() const
{
	return m_TextureSrcRect;
}