#include "pch.h"
#include "DozeDragonBubbles.h"
#include "Animation.h"
#include "DozeDragonBubble.h"

// Constructors / Destructors
DozeDragonBubbles::DozeDragonBubbles(const Point2f& position)
	: m_StartPos{ position }
	, m_ExistTime{ 0.f }
	, m_AllSpawned{ false }
{
	InitDragonBubbles();
}

DozeDragonBubbles::~DozeDragonBubbles()
{
	DestroyDragonBubbles();
}

// Update / Draw
void DozeDragonBubbles::Update(float elapsedSec, Player* player)
{
	UpdateVisibility(elapsedSec);
	UpdateBubbles(elapsedSec, player);
	UpdateCanBeDeleted();
}

void DozeDragonBubbles::Draw(const Texture* texture) const
{
	for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
	{
		m_pDragonBubbles[idx]->Draw(texture);
	}
}

bool DozeDragonBubbles::GetIsOnScreen(const Rectf& cameraRect) const
{
	bool isonScreen{ false };

	for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
	{
		if (m_pDragonBubbles[idx]->GetIsOnScreen(cameraRect))
		{
			isonScreen = true;
		}
	}

	return isonScreen;
}

// Private Functions //
// Init
void DozeDragonBubbles::InitDragonBubbles()
{
	Point2f point1{ m_StartPos.x, m_StartPos.y + 5.f };
	Point2f point2{ m_StartPos.x + 35.f, m_StartPos.y + 80.f };
	Point2f point3{ m_StartPos.x + 80.f, m_StartPos.y + 60.f };

	m_pDragonBubbles.reserve(3);

	m_pDragonBubbles.emplace_back(new DozeDragonBubble{ point1, Point2f{point1.x - 400.f, point1.y} });
	m_pDragonBubbles.emplace_back(new DozeDragonBubble{ point2, Point2f{point2.x - 400.f, point2.y} });
	m_pDragonBubbles.emplace_back(new DozeDragonBubble{ point3, Point2f{point3.x - 400.f, point3.y} });
}

// Destroy
void DozeDragonBubbles::DestroyDragonBubbles()
{
	for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
	{
		delete m_pDragonBubbles[idx];
	}
	m_pDragonBubbles.clear();
}

// Update
void DozeDragonBubbles::UpdateVisibility(float elapsedSec)
{
	if (m_AllSpawned)
	{
		return;
	}

	m_ExistTime += elapsedSec;

	if (m_ExistTime > 0.75f)
	{
		if (m_pDragonBubbles.size() > 2)
		{
			m_pDragonBubbles[2]->SetVisible();
		}
	}
	else if (m_ExistTime > 0.5f)
	{
		if (m_pDragonBubbles.size() > 1)
		{
			m_pDragonBubbles[1]->SetVisible();
		}
	}
	else if (m_ExistTime > 0.25f)
	{
		if (m_pDragonBubbles.size() > 0)
		{
			m_pDragonBubbles[0]->SetVisible();
		}
	}
}

void DozeDragonBubbles::UpdateBubbles(float elapsedSec, Player* player)
{
	for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
	{
		m_pDragonBubbles[idx]->Update(elapsedSec, player);
	}
}

void DozeDragonBubbles::UpdateCanBeDeleted()
{
	for (size_t idx{}; idx < m_pDragonBubbles.size(); ++idx)
	{
		if (m_pDragonBubbles[idx]->GetCanBeDeleted())
		{
			delete m_pDragonBubbles[idx];
			if (idx < m_pDragonBubbles.size())
			{
				m_pDragonBubbles[idx] = m_pDragonBubbles.back();
			}
			m_pDragonBubbles.pop_back();
			return;
		}
	}
}