#include "pch.h"
#include "Enemy.h"
#include "Animation.h"

Enemy::Enemy(const Point2f& enemyPos, bool enemyDir, int hp, TextureManager* textureManager)
	: m_HitBox{ enemyPos, 0.f, 0.f }
	, m_EnemyDir{ EnemyDir(enemyDir) }
	, m_HP{ hp }
	, m_pTextureManager{ textureManager }
	, m_IsOnScreen{ false }
	, m_Velocity{ }
	, m_DrawRect{ }
	, m_SrcRect{ }	// setting default
	, m_IsHit{ false }
	, m_IsHitTime{ 0.f }
	, m_IsDead{ false }
	, m_CanBeDelete{ false }
	, m_Scale{ 3.5f }
	, m_IsOnGround{ false }
	, m_Gravity{ -1500 }
	, m_GetHitVelocity{ }
	, m_KnockBackVelocity{ }
	, m_PlayerKnockUpVelocity{ }
	, m_PoefAnimationOn{ false }
	, m_KillValue{ }
	, m_ValueIsGiven{ false }
{
	InitRays();
	InitAnimations();
}

Enemy::~Enemy()
{
	// Delete the animation
	delete m_pPoefAnimation;
	m_pPoefAnimation = nullptr;

	DestroyRays();
}

// Update
void Enemy::UpdateRays()
{
	const float offset{ 2.f };
	// vertical
	if (m_pVerticalRays.size() == 2)
	{
		/// LEFT ///
		// left under
		m_pVerticalRays[0]->pointOne = Point2f{ m_HitBox.left + offset * 5.f, m_HitBox.bottom - offset };

		// left upper
		m_pVerticalRays[0]->pointTwo = Point2f{ m_HitBox.left + offset * 5.f, m_HitBox.bottom + m_HitBox.height + offset };

		/// RIGHT ///
		// right under
		m_pVerticalRays[1]->pointOne = Point2f{ m_HitBox.left + m_HitBox.width - offset * 5.f, m_HitBox.bottom - offset };

		// right upper
		m_pVerticalRays[1]->pointTwo = Point2f{ m_HitBox.left + m_HitBox.width - offset * 5.f, m_HitBox.bottom + m_HitBox.height + offset };
	}
	else
	{
		std::cout << "Something went wrong!\n";
	}

	// horizontal
	if (m_pHorizontalRays.size() == 2)
	{
		/// TOP ///
		// top left
		m_pHorizontalRays[0]->pointOne = Point2f{ m_HitBox.left - offset, m_HitBox.bottom + m_HitBox.height - offset * 5.f };

		// top right
		m_pHorizontalRays[0]->pointTwo = Point2f{ m_HitBox.left + m_HitBox.width + offset , m_HitBox.bottom + m_HitBox.height - offset * 5.f };

		/// BOT ///
		// bottom left
		m_pHorizontalRays[1]->pointOne = Point2f{ m_HitBox.left - offset, m_HitBox.bottom + offset * 5.f };

		// bottom right
		m_pHorizontalRays[1]->pointTwo = Point2f{ m_HitBox.left + m_HitBox.width + offset , m_HitBox.bottom + offset * 5.f };
	}
	else
	{
		std::cout << "Something went wrong!\n";
	}
}

// Getters / Setters
const Rectf& Enemy::GetHitBox() const
{
	return m_HitBox;
}

bool Enemy::GetIsOnScreen() const
{
	return m_IsOnScreen;
}

void Enemy::SetIsOnScreen(bool isOnScreen)
{
	m_IsOnScreen = isOnScreen;
}

bool Enemy::GetIsDead() const
{
	return m_IsDead;
}

bool Enemy::GetCanBeDeleted() const
{
	return m_CanBeDelete;
}

int Enemy::GetKillValue() const
{
	return m_KillValue;
}

bool Enemy::GetValueIsGiven() const
{
	return m_ValueIsGiven;
}

void Enemy::SetValueIsGiven(bool valueIsGiven)
{
	m_ValueIsGiven = valueIsGiven;
}

bool Enemy::GetIsPoefAnimationOn() const
{
	return m_PoefAnimationOn;
}

// Handle
void Enemy::HandleIsOnGround(const std::vector< std::vector< Point2f > >& vertices)
{
	for (size_t idx{}; idx < vertices.size(); ++idx)
	{
		for (size_t index{}; index < m_pVerticalRays.size(); ++index)
		{
			// bottom
			Point2f pointOne{ m_pVerticalRays[index]->pointOne.x, m_pVerticalRays[index]->pointOne.y - 1.f };
			// top
			Point2f pointTwo{ m_pVerticalRays[index]->pointTwo.x, m_pVerticalRays[index]->pointTwo.y };

			if (utils::Raycast(vertices[idx], pointOne, pointTwo, m_VerticalHitInfo))
			{
				if (m_VerticalHitInfo.lambda < 0.5f)
				{
					m_IsOnGround = true;
					return;
				}
			}
		}
	}
	m_IsOnGround = false;
}

// Private Functions //
// Init
void Enemy::InitRays()
{
	// setting the right vertical and horizontal rays numberj

	m_pVerticalRays.reserve(2);
	m_pHorizontalRays.reserve(2);

	m_pVerticalRays.emplace_back(new Rayf{});
	m_pVerticalRays.emplace_back(new Rayf{});

	m_pHorizontalRays.emplace_back(new Rayf{});
	m_pHorizontalRays.emplace_back(new Rayf{});
}

void Enemy::InitAnimations()
{
	m_pPoefAnimation = new Animation{ m_SrcRect, Rectf{0.f, 16.f, 24.f, 16.f}, 5, 8.f };
}

// Destroy
void Enemy::DestroyRays()
{
	// vertical rays
	for (size_t idx{}; idx < m_pVerticalRays.size(); ++idx)
	{
		delete m_pVerticalRays[idx];
	}
	m_pVerticalRays.clear();

	// horizontal rays
	for (size_t idx{}; idx < m_pHorizontalRays.size(); ++idx)
	{
		delete m_pHorizontalRays[idx];
	}
	m_pHorizontalRays.clear();
}