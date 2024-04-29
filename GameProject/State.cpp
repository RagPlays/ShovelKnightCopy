#include "pch.h"
#include "State.h"

// Constructors / Destructors
State::State()
	: m_Paused{false}
	, m_Quit{false}
{
	std::cout << "Creating State!\n";
}

State::~State()
{
	std::cout << "Clearing State!\n";
}

// Getters
const bool State::GetQuit() const
{
	return m_Quit;
}

const bool State::GetPaused() const
{
	return m_Paused;
}

// Other
void State::EndState()
{
	m_Quit = true;
}

void State::PauseState()
{
	m_Paused = true;
}

void State::UnPauseState()
{
	m_Paused = false;
}