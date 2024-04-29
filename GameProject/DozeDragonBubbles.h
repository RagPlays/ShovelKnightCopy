#pragma once

class Texture;
class Animation;
class Player;
class DozeDragonBubble;

class DozeDragonBubbles final
{
public:
	// Constructors / Destructors
	explicit DozeDragonBubbles(const Point2f& position);
	~DozeDragonBubbles();

	DozeDragonBubbles(const DozeDragonBubbles& dozeDragonBubbles) = delete;
	DozeDragonBubbles& operator=(const DozeDragonBubbles& dozeDragonBubbles) = delete;
	DozeDragonBubbles(DozeDragonBubbles&& dozeDragonBubbles) noexcept = delete;
	DozeDragonBubbles& operator=(DozeDragonBubbles&& dozeDragonBubbles) noexcept = delete;

	// Update / Draw
	void Update(float elapsedSec, Player* player);
	void Draw(const Texture* texture) const;

	// Getters / Setters
	bool GetIsOnScreen(const Rectf& cameraRect) const;

private:

	Point2f m_StartPos;

	float m_ExistTime;
	bool m_AllSpawned;
	bool m_IsOnScreen;

	std::vector< DozeDragonBubble* > m_pDragonBubbles;

private:

	// Init
	void InitDragonBubbles();

	// Destroy
	void DestroyDragonBubbles();

	// Update
	void UpdateVisibility(float elapsedSec);
	void UpdateBubbles(float elapsedSec, Player* player);
	void UpdateCanBeDeleted();
};