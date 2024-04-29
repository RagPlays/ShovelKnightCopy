#pragma once

class State
{
public:
	// Constructors / Destructors
	explicit State();
	virtual ~State();

	State(const State& state) = delete;
	State& operator=(const State& state) = delete;
	State(State&& state) noexcept = delete;
	State& operator=(State&& state) noexcept = delete;

	// Updates / Draw
	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;

	// Input
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) = 0;
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) = 0;
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) = 0;
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) = 0;
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) = 0;

	// Getters
	const bool GetQuit() const;
	const bool GetPaused() const;

	// Other
	void EndState();
	void PauseState();
	void UnPauseState();

protected:

	bool m_Quit;
	bool m_Paused;
};