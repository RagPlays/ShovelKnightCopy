#include "pch.h"
#include "Game.h"
#include "State.h"
#include "StartMenuState.h"

Game::Game(const Window& window) 
	: BaseGame{ window }
{
	SDL_ShowCursor(false);
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	m_pStates.reserve(2);
	m_pStates.emplace_back(new StartMenuState{m_pStates, GetViewPort()});
}

void Game::Cleanup()
{
	for (size_t idx{}; idx < m_pStates.size(); ++idx)
	{
		delete m_pStates[idx];
	}
	m_pStates.clear();
}

void Game::Update(float elapsedSec)
{
	UpdateStates(elapsedSec);
	//SDL_SetWindowTitle(SDL_GL_GetCurrentWindow(), std::to_string(int(1.f / elapsedSec)).c_str());
}

void Game::Draw() const
{
	ClearBackground();
	DrawStates();
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent & e)
{
	if (!m_pStates.empty()) // als er een state is
	{
		m_pStates.back()->ProcessKeyDownEvent(e);
	}
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (!m_pStates.empty()) // als er een state is
	{
		m_pStates.back()->ProcessKeyUpEvent(e);
	}

	if (e.keysym.sym == SDLK_i)
	{
		ShowKeyBordInfo();
	}
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	if (!m_pStates.empty())
	{
		m_pStates.back()->ProcessMouseMotionEvent(e);
	}
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (!m_pStates.empty())
	{
		m_pStates.back()->ProcessMouseDownEvent(e);
	}
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (!m_pStates.empty())
	{
		m_pStates.back()->ProcessMouseUpEvent(e);
	}
}

void Game::ClearBackground() const
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

/////////////  OWN_FUNCTIONS  //////////////

// Update
void Game::UpdateStates(float elapsedSec)
{
	if (!m_pStates.empty())
	{
		m_pStates.back()->Update(elapsedSec);

		if (m_pStates.back()->GetQuit())
		{
			delete m_pStates.back();
			m_pStates.pop_back();
		}
		return;
	}
	EndGame();
}

// Draw
void Game::DrawStates() const
{
	if (!m_pStates.empty()) // als er een state is
	{
		m_pStates.back()->Draw();  // Update de bovenste state
	}
}

// Other
void Game::ShowKeyBordInfo()
{
	std::cout << "\n\n";

	std::ifstream inFile{};
	inFile.open("../../Resources/Saves/Other/keyBoardInfo.morrog");

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::GAME::CPP::COULD_NOT_GET_KEYBOARDINFO_FROM_FILE\n";
	}
	else
	{
		std::string line{};

		while (!inFile.fail() && !inFile.eof())
		{
			std::getline(inFile, line);

			std::cout << line << "\n";
		}
	}
	inFile.close();
}