#include "pch.h"
#include "Number.h"
#include "Texture.h"
#include "TextureManager.h"

// Constructors / Destructors
Number::Number(const Point2f& drawPos, TextureManager* textureManager, float scale)
	: m_DrawRect{ drawPos, 0.f, 0.f }
	, m_pTextureManager{ textureManager }
{
	// Set Total Width / Height
	m_DrawRect.width = m_pTextureManager->GetTextureVector("numbers")[0]->GetWidth() * scale;
	m_DrawRect.height = m_pTextureManager->GetTextureVector("numbers")[0]->GetHeight() * scale;

	// setting defaults
	m_AddNumberTime = 0.f;
	m_CurrentNumber = 0;
	m_Number = 0;
}

void Number::Update(float elapsedSec)
{
	if (m_Number > m_CurrentNumber)
	{
		if (utils::GetClockTime(m_AddNumberTime, elapsedSec, 0.01f))	// exactly the same as every frame... but ok
		{
			++m_CurrentNumber;
		}
	}
	else if (m_Number < m_CurrentNumber)
	{
		if ((m_CurrentNumber - m_Number) > 2)
		{
			m_CurrentNumber -= rand() % 2 + 1;
		}
		else
		{
			--m_CurrentNumber;
		}
	}
}

void Number::Draw()
{
	// Set number you can change
	int tempNumber{ m_CurrentNumber };

	std::vector< int > digits;

	while (tempNumber > 0) {
		digits.emplace_back(tempNumber % 10);
		tempNumber = tempNumber / 10;
	}
	if (digits.size() == 0)
	{
		digits.emplace_back(0);
	}

	// set draw Pos
	Rectf drawRect{ m_DrawRect };

	// draw number
	for (size_t idx{ 1 }; idx <= digits.size(); ++idx)
	{
		m_pTextureManager->GetTextureVector("numbers")[digits[digits.size() - idx]]->Draw(drawRect);

		drawRect.left += m_DrawRect.width;
	}
}

// Getters / Setters
int Number::GetNumber() const
{
	return m_Number;
}

void Number::SetNumber(int num)
{
	m_Number = num;
}

void Number::AddNumber(int num)
{
	m_Number += num;
}

// Private Functions //
// Getters
int Number::GetMultiply() const
{
	int multiply{};

	if (m_Number > 9999)
	{
		multiply = 5;
	}
	else if (m_Number > 999)
	{
		multiply = 4;
	}
	else if (m_Number > 99)
	{
		multiply = 3;
	}
	else if (m_Number > 9)
	{
		multiply = 2;
	}
	else if (m_Number >= 0)
	{
		multiply = 1;
	}

	return multiply;
}