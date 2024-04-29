#pragma once
#include "structs.h"
#include "SDL.h"
// https://BaseGameprogrammingpatterns.com/subclass-sandbox.html


class BaseGame
{
public:
	explicit BaseGame( const Window& window );
	virtual ~BaseGame();
	BaseGame( const BaseGame& other ) = delete;
	BaseGame& operator=( const BaseGame& other ) = delete;
	BaseGame(BaseGame&& other) = delete;
	BaseGame& operator=(BaseGame&& other) = delete;

	void Run( );

	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;

	// Event handling
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) = 0;
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) = 0;
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) = 0;
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) = 0;
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) = 0;

	const Rectf& GetViewPort() const
	{
		return m_Viewport;
	}

	void EndGame()
	{
		m_Quit = true;
	}

private:
	// DATA MEMBERS
	// The window properties
	const Window m_Window;
	const Rectf m_Viewport;
	// The window we render to
	SDL_Window* m_pWindow;
	// OpenGL context
	SDL_GLContext m_pContext;
	// Init info
	bool m_Initialized;
	// Prevent timing jumps when debugging
	const float m_MaxElapsedSeconds;

	// Quit
	bool m_Quit;
	
	// FUNCTIONS
	void InitializeGameEngine( );
	void CleanupGameEngine( );
};
