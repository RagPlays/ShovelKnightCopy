#include "pch.h"
#include "SVGParser.h"
#include "EditorState.h"
#include "TileMap.h"
#include "PauseMenu.h"
#include "TextureSelector.h"
#include "Camera.h"
#include "TextureManager.h"

// Constructors / Destructors
EditorState::EditorState(const Rectf& windowBorder, const std::string& levelKey, Settings* settings, TextureManager* textureManager)
	: State{}
	, m_WindowBorder{ windowBorder }
	, m_LevelKey{ levelKey }
	, m_pSettings{ settings }
	, m_pTextureManager{ textureManager }
	, m_MapDimentions{ }
	, m_TileGridSize { 56 }
	, m_Scale{ 3.5f }
	, m_TextureSrcRect{ }
	, m_CameraSpeed{ 700 }
{
	std::cout << "Creating EditorState...\n";

	InitVariables();
	InitPauseMenu();
	InitTileMaps();
	InitGUI();
	InitCamera();
	InitVertices();
	InitLadderRects();
}

EditorState::~EditorState()
{
	std::cout << "Clearing EditorState...\n";

	ClearVariables();
	ClearTileMaps();
	ClearGUI();
	ClearCamera();
}

// Update / Draw
void EditorState::Update(float elapsedSec)
{
	if (m_Paused)
	{
		m_pPauseMenu->Update(elapsedSec);
	}
	else
	{
		if (m_pTextureSelector->GetIsHidden())
		{
			UpdateCamera(elapsedSec);
		}
		UpdateEditor(elapsedSec);
	}
}

void EditorState::Draw() const
{
	// Draw contrasting background to make it easier to edite tilemap
	utils::SetRGBColor(50, 50, 50);
	utils::FillRect(m_WindowBorder);

	// Draw the vertices
	DrawVertices();

	// Draw ladder Rects
	DrawLadderRects();

	// draw the tilemap(s)
	DrawTileMaps();

	// Hover rect
	DrawHoverRect();

	// DrawMovingRects
	DrawCameraMoveRects();

	// Draw the textureSelector
	DrawGUI();

	// Draw PauseMenu if needed
	if (m_Paused)
	{
		m_pPauseMenu->Draw();
	}

	// Draw the cursor
	DrawCursor();
}

// Inputs
void EditorState::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{

}

void EditorState::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (!m_Paused)
	{
		switch (e.keysym.sym)
		{
		case SDLK_SPACE:
			if (m_pTextureSelector->GetIsHidden())
			{
				m_pTextureSelector->SetIsHidden(false);
			}
			else
			{
				m_pTextureSelector->SetIsHidden(true);
			}
			break;
		case SDLK_m:
			m_GetMousePos = m_GetMousePos ? false : true;
			break;
		case SDLK_UP:
			ChangeSelectMode("Up");
			break;
		case SDLK_DOWN:
			ChangeSelectMode("Down");
			break;
		case SDLK_LEFT:
			ChangeSelectMode("Left");
			break;
		case SDLK_RIGHT:
			ChangeSelectMode("Right");
			break;
		case SDLK_LSHIFT:
			if (m_CameraSpeed == 700)	// its a integer so its fine..
			{
				m_CameraSpeed = 1200;
			}
			else if(m_CameraSpeed == 1200)
			{
				m_CameraSpeed = 2500;
			}
			else
			{
				m_CameraSpeed = 700;
			}
			break;
		case SDLK_b:
			m_ShowTileOptimization = m_ShowTileOptimization ? false : true;
			break;
		default:
			break;
		}
	}
	
	if (m_Paused)
	{
		m_pPauseMenu->ProcessKeyUpEvent(e);
		
		if (m_pPauseMenu->GetIsPressed())
		{
			switch (m_pPauseMenu->GetActiveButtonID())
			{
			case 0: // resume
				m_Paused = false;
				m_pPauseMenu->SetActiveButtonID(0);
				break;
			case 1:	// save
				SaveTileMaps();
				break;
			case 2: // load
				LoadTileMaps();
				break;
			case 3:	// exit
				m_Quit = true;
				break;
			default:
				break;
			}
			m_pPauseMenu->SetIsPressed(false);
		}
	}

	if (e.keysym.sym == SDLK_ESCAPE)
	{
		if (m_Paused)
		{
			m_pPauseMenu->SetActiveButtonID(0);
			m_Paused = false;
		}
		else
		{
			m_Paused = true;
		}
	}
}

void EditorState::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
	m_MousePos = Point2f{ float(e.x), float(e.y) };
	m_MousePosGrid.x = int(m_MousePos.x + m_pCamera->GetPosition().x) / m_TileGridSize;
	m_MousePosGrid.y = int(m_MousePos.y + m_pCamera->GetPosition().y) / m_TileGridSize;
}

void EditorState::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void EditorState::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (!m_Paused)
	{
		if (e.button == SDL_BUTTON_LEFT)
		{
			AddTile();
		}
		else if (e.button == SDL_BUTTON_RIGHT)
		{
			RemoveTile();
		}
	}
}

// Private Functions //
// Init
void EditorState::InitVariables()
{
	m_MousePos = Point2f{};
	m_MousePosGrid = Point2i{};
	m_HoverRect.width = float(m_TileGridSize);
	m_HoverRect.height = float(m_TileGridSize);
	m_SelectedViewMode = ViewMode::all;
	m_WaterViewMode = WaterViewMode::one;
	m_WaterViewTime = 0.f;
	m_GetMousePos = false;
	m_ShowTileOptimization = true;
}

void EditorState::InitPauseMenu()
{
	m_pPauseMenu = new PauseMenu{ m_WindowBorder, m_pSettings, m_pTextureManager };
	
	m_pPauseMenu->AddButton(0);
	m_pPauseMenu->AddButton(7);
	m_pPauseMenu->AddButton(8);
	m_pPauseMenu->AddButton(9);
}

void EditorState::InitTileMaps()
{
	std::cout << "\n";
	// Init all tileMaps //

	// background
	m_CloudDestRect.width = m_WindowBorder.width;
	m_CloudDestRect.height = m_pTextureManager->GetTexture("back" + m_LevelKey)->GetHeight() * (m_CloudDestRect.width / m_pTextureManager->GetTexture("back" + m_LevelKey)->GetWidth());
	m_CloudDestRect.left = 0.f;
	m_CloudDestRect.bottom = 0.f;
	std::cout << "initialized background texture\n";

	// castle
	m_pCastleMap = new TileMap{ m_pTextureManager };
	// light forest
	m_pLightForestMap = new TileMap{ m_pTextureManager };
	// dark forest one
	m_pDarkForestOneMap = new TileMap{ m_pTextureManager };
	// dark forest two
	m_pDarkForestTwoMap = new TileMap{ m_pTextureManager };
	// water
	m_pWaterTileMaps.emplace_back(new TileMap{ m_pTextureManager });
	m_pWaterTileMaps.emplace_back(new TileMap{ m_pTextureManager });
	m_pWaterTileMaps.emplace_back(new TileMap{ m_pTextureManager });
	
	// cave 
	m_pCaveMap = new TileMap{ m_pTextureManager };
	// baseMap
	m_pBaseMap = new TileMap{ m_pTextureManager };

	// ladders
	m_pLadderMap = new TileMap{ m_pTextureManager };

	// foreground
	m_pForeGroundMap = new TileMap{ m_pTextureManager };

	std::cout << "All maps initialized!\n";

	LoadTileMaps(); // can uncomment this

	m_MapDimentions = m_pBaseMap->GetMapDimentions();
}

void EditorState::InitGUI()
{
	// TextureSelector
	const Point2f drawPoint{ m_WindowBorder.width / 30.f, (m_WindowBorder.width / 60.f) };
	m_pTextureSelector = new TextureSelector{ drawPoint, int(m_TileGridSize / m_Scale), m_pTextureManager->GetTexture("tilemap" + m_LevelKey) };
}

void EditorState::InitCamera()
{
	// Camera
	m_pCamera = new Camera{ Point2f{m_WindowBorder.width, m_WindowBorder.height} };
	m_pCamera->SetTileMapBounds(Rectf{ 0.f, 0.f, float(m_MapDimentions.x * float(m_TileGridSize)), float(m_MapDimentions.y * float(m_TileGridSize)) });

	// Rects
	const float rectWidth{ m_WindowBorder.width / 20.f };

	m_CameraMoveRects.emplace_back(Rectf{ 0.f, m_WindowBorder.height - rectWidth, m_WindowBorder.width, rectWidth });
	m_CameraMoveRects.emplace_back(Rectf{ 0.f, 0.f, m_WindowBorder.width, rectWidth });
	m_CameraMoveRects.emplace_back(Rectf{ 0.f, 0.f, rectWidth, m_WindowBorder.height });
	m_CameraMoveRects.emplace_back(Rectf{ m_WindowBorder.width - rectWidth, 0.f, rectWidth, m_WindowBorder.height });

	m_pCamera->SetPosition(Point2f{ 0.f, 2128.f });
}

void EditorState::InitVertices()
{	
	std::cout << "\n";

	const std::string pathName{ "Saves/" + m_LevelKey + "/Other/collision.svg" };
	if (SVGParser::GetVerticesFromSvgFile(pathName, m_Vertices))
	{
		std::cout << "Vertices Loaded...\n";
	}
	else
	{
		std::cerr << "ERROR::EDITORSTATE::CPP::FAILED_LOADING_VERTICES\n";
	}
}

void EditorState::InitLadderRects()
{
	std::cout << "Loading LadderRects...\n";

	const std::string loadPathName{ "../../Resources/Saves/" + m_LevelKey + "/Other/ladderRects.morrog" };

	std::ifstream inFile{};
	inFile.open(loadPathName);

	if (!inFile.is_open())
	{
		std::cerr << "ERROR::EDITORSTATE::CPP::COULD_NOT_LOAD_LADDERRECTS\n";
	}
	else
	{
		float left{};
		float bottom{};
		float width{};
		float height{};
		bool endRect{};

		while (inFile >> left >> bottom >> width >> height >> endRect)
		{
			m_LadderRects.emplace_back(LadderRectf{left, bottom, width, height, endRect});
		}
	}
	inFile.close();

	std::cout << "All ladderRects loaded!\n";
}

// Destroy
void EditorState::ClearVariables()
{
	delete m_pPauseMenu;
	m_pPauseMenu = nullptr;
}

void EditorState::ClearTileMaps()
{
	std::cout << "\n";

	// background

	// castle
	if (m_pCastleMap != nullptr)
	{
		delete m_pCastleMap;
		m_pCastleMap = nullptr;
	}

	// light forest
	if (m_pLightForestMap != nullptr)
	{
		delete m_pLightForestMap;
		m_pLightForestMap = nullptr;
	}

	// dark forests
	if (m_pDarkForestOneMap != nullptr)
	{
		delete m_pDarkForestOneMap;
		m_pDarkForestOneMap = nullptr;
	}
	if (m_pDarkForestTwoMap != nullptr)
	{
		delete m_pDarkForestTwoMap;
		m_pDarkForestTwoMap = nullptr;
	}

	// waterTileMaps
	for (size_t idx{}; idx < m_pWaterTileMaps.size(); ++idx)
	{
		if (m_pWaterTileMaps[idx] != nullptr)
		{
			delete m_pWaterTileMaps[idx];
		}
	}
	m_pWaterTileMaps.clear();

	// Cave
	if (m_pCaveMap != nullptr)
	{
		delete m_pCaveMap;
		m_pCaveMap = nullptr;
	}

	// baseMap
	if (m_pBaseMap != nullptr)
	{
		delete m_pBaseMap;
		m_pBaseMap = nullptr;
	}

	// ladders
	if (m_pLadderMap != nullptr)
	{
		delete m_pLadderMap;
		m_pLadderMap = nullptr;
	}

	// gametilemap
	if (m_pForeGroundMap != nullptr)
	{
		delete m_pForeGroundMap;
		m_pForeGroundMap = nullptr;
	}

	std::cout << "All maps cleared!\n";
}

void EditorState::ClearGUI()
{
	// TextureSelector
	delete m_pTextureSelector;
	m_pTextureSelector = nullptr;
}

void EditorState::ClearCamera()
{
	delete m_pCamera;
	m_pCamera = nullptr;
}

// Update
void EditorState::UpdateEditor(float elapsedSec)
{
	UpdateGUI(elapsedSec);

	if (m_SelectedViewMode == ViewMode::all)
	{
		UpdateWaterView(elapsedSec);
		UpdateMapRenderAreas();	// for preformance gain (hopefully)
		
	}

	// to see position
	if (m_GetMousePos)
	{
		std::cout << "GridPosition: X= " << m_MousePosGrid.x << " , Y= " << m_MousePosGrid.y << "\n";
		std::cout << "MousePos: ";
		std::cout << " X= " << std::round(m_MousePos.x + m_pCamera->GetPosition().x);
		std::cout << " , Y= " << std::round(m_MousePos.y + m_pCamera->GetPosition().y) << "\n";
		std::cout << "\n";
	}
}

void EditorState::UpdateGUI(float elapsedSec)
{
	// Update the texture selector
	m_pTextureSelector->Update(elapsedSec, m_MousePos);

	if (!m_pTextureSelector->GetIsActive())
	{
		m_HoverRect.left = float(m_MousePosGrid.x * m_TileGridSize);
		m_HoverRect.bottom = float(m_MousePosGrid.y * m_TileGridSize);
	}
}

void EditorState::UpdateCamera(float elapsedSec)
{
	if (m_CameraMoveRects.size() != size_t(4))
	{
		std::cout << "CameraMoveRects Not Initialized Correctly!!\n";
		return;
	}

	// check
	if (utils::IsPointInRect(m_MousePos, m_CameraMoveRects[0]))
	{
		m_pCamera->ChangePosition(0.f, m_CameraSpeed * elapsedSec);
	}
	if (utils::IsPointInRect(m_MousePos, m_CameraMoveRects[1]))
	{
		m_pCamera->ChangePosition(0.f, -(m_CameraSpeed * elapsedSec));
	}
	if (utils::IsPointInRect(m_MousePos, m_CameraMoveRects[2]))
	{
		m_pCamera->ChangePosition(-(m_CameraSpeed * elapsedSec), 0.f);
	}
	if (utils::IsPointInRect(m_MousePos, m_CameraMoveRects[3]))
	{
		m_pCamera->ChangePosition(m_CameraSpeed * elapsedSec, 0.f);
	}
}

void EditorState::UpdateWaterView(float elapsedSec)
{
	if (utils::GetClockTime(m_WaterViewTime, elapsedSec, 0.3f))
	{
		if (m_WaterViewMode == WaterViewMode::three)
		{
			m_WaterViewMode = WaterViewMode::one;
		}
		else
		{
			m_WaterViewMode = WaterViewMode(int(m_WaterViewMode) + 1);
		}
	}
	// using int(m_WaterViewMode) for the index of the to draw tilemap;
}

void EditorState::UpdateMapRenderAreas()
{
	// background -> doesnt move -> just png

	Point2f cameraPos{ m_pCamera->GetPosition() };
	
	// castle
	m_pCastleMap->SetDrawArea(Point2f{ cameraPos.x * 0.083f, cameraPos.y }, m_WindowBorder, m_ShowTileOptimization);

	// lightForest
	m_pLightForestMap->SetDrawArea(Point2f{ cameraPos.x * 0.740f, cameraPos.y }, m_WindowBorder, m_ShowTileOptimization);

	// DarkForestOne
	m_pDarkForestOneMap->SetDrawArea(Point2f{ cameraPos.x * 0.833f, cameraPos.y }, m_WindowBorder, m_ShowTileOptimization);

	// DrakForestTwo
	m_pDarkForestTwoMap->SetDrawArea(Point2f{ cameraPos.x * 0.917f, cameraPos.y }, m_WindowBorder, m_ShowTileOptimization);

	// Water
	m_pWaterTileMaps[int(m_WaterViewMode)]->SetDrawArea(cameraPos, m_WindowBorder, m_ShowTileOptimization);

	// Cave
	m_pCaveMap->SetDrawArea(cameraPos, m_WindowBorder, m_ShowTileOptimization);

	// baseMap
	m_pBaseMap->SetDrawArea(cameraPos, m_WindowBorder, m_ShowTileOptimization);

	// ladders
	m_pLadderMap->SetDrawArea(cameraPos, m_WindowBorder, m_ShowTileOptimization);

	// foreGround
	m_pForeGroundMap->SetDrawArea(cameraPos, m_WindowBorder, m_ShowTileOptimization);
}

void EditorState::ResetDrawAreas()
{
	// background -> doesnt move -> just png

	// castle
	m_pCastleMap->SetDefaultDrawArea();

	// lightForest
	m_pLightForestMap->SetDefaultDrawArea();

	// DarkForestOne
	m_pDarkForestOneMap->SetDefaultDrawArea();

	// DrakForestTwo
	m_pDarkForestTwoMap->SetDefaultDrawArea();

	// Water
	for (size_t idx{}; idx < m_pWaterTileMaps.size(); ++idx)
	{
		m_pWaterTileMaps[idx]->SetDefaultDrawArea();
	}

	// Cave
	m_pCaveMap->SetDefaultDrawArea();

	// baseMap
	m_pBaseMap->SetDefaultDrawArea();

	// ladders
	m_pLadderMap->SetDefaultDrawArea();

	// foreGround
	m_pForeGroundMap->SetDefaultDrawArea();
}

// Draw
void EditorState::DrawVertices() const
{
	// vertices
	glPushMatrix();
	{
		// Translate for camera
		glTranslatef(-m_pCamera->GetPosition().x, -m_pCamera->GetPosition().y, 0.f);

		// draw vertices
		utils::SetRGBColor(255, 25, 200);
		for (size_t idx{}; idx < m_Vertices.size(); ++idx)
		{
			utils::DrawPolygon(m_Vertices[idx], true, 2.f);
		}
	}
	glPopMatrix();
}

void EditorState::DrawLadderRects() const
{
	glPushMatrix();
	{
		// Translate for camera
		glTranslatef(-m_pCamera->GetPosition().x, -m_pCamera->GetPosition().y, 0.f);

		// Draw LadderRects
		for (size_t idx{}; idx < m_LadderRects.size(); ++idx)
		{
			// draw the ladderrect
			utils::SetRGBColor(230, 20, 100);
			utils::DrawRect(m_LadderRects[idx].rect, 2.f);

			// draw the endrect
			if (m_LadderRects[idx].endRect)
			{
				Rectf endRect{ m_LadderRects[idx].rect };
				endRect.bottom += endRect.height;
				endRect.height = endRect.width / 8.f;
				endRect.bottom -= endRect.height;

				utils::SetRGBColor(180, 200, 150);
				utils::DrawRect(endRect, 2.f);
			}
		}
	}
	glPopMatrix();
}

void EditorState::DrawGUI() const
{
	// Draw Texture Selector
	m_pTextureSelector->Draw();
}

void EditorState::DrawCameraMoveRects() const
{
	if (m_CameraMoveRects.size() != size_t(4))
	{
		std::cout << "CameraMoveRects Not Initialized Correctly!!\n";
		return;
	}

	utils::SetColor(Color4f{ 50.f / 255.f,50.f / 255.f, 50.f / 255.f, 0.4f });
	for (size_t idx{}; idx < m_CameraMoveRects.size(); ++idx)
	{
		utils::FillRect(m_CameraMoveRects[idx]);
	}
}

void EditorState::DrawTileMaps() const
{
	Point2f cameraPos{ m_pCamera->GetPosition() };

	switch (m_SelectedViewMode)
	{
	case EditorState::ViewMode::background:
		m_pTextureManager->GetTexture("back" + m_LevelKey)->Draw(m_CloudDestRect);
		break;
	case EditorState::ViewMode::caste:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pCastleMap->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::lightForest:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pLightForestMap->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::darkForestOne:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pDarkForestOneMap->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::darkForestTwo:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pDarkForestTwoMap->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::water:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pWaterTileMaps[int(m_WaterViewMode)]->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::cave:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pCaveMap->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::base:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pBaseMap->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::ladders:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pLadderMap->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::foreGround:
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
			m_pForeGroundMap->Draw();
		}
		glPopMatrix();
		break;
	case EditorState::ViewMode::all:		
		// in all you can view things with right parallex scrolling effect //
		// no camera tilemaps / textures
		m_pTextureManager->GetTexture("back" + m_LevelKey)->Draw(m_CloudDestRect);

		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pCastleMap->GetParallaxScale(), -cameraPos.y, 0.f);
			m_pCastleMap->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pLightForestMap->GetParallaxScale(), -cameraPos.y, 0.f);
			m_pLightForestMap->Draw();
		}
		glPopMatrix();
		
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pDarkForestOneMap->GetParallaxScale(), -cameraPos.y, 0.f);
			m_pDarkForestOneMap->Draw();
		}
		glPopMatrix();
		
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pDarkForestTwoMap->GetParallaxScale(), -cameraPos.y / 1.f, 0.f);
			m_pDarkForestTwoMap->Draw();
		}
		glPopMatrix();
		
		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pWaterTileMaps[int(m_WaterViewMode)]->GetParallaxScale(), -cameraPos.y, 0.f);
			m_pWaterTileMaps[int(m_WaterViewMode)]->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pCaveMap->GetParallaxScale(), -cameraPos.y / 1.f, 0.f);
			m_pCaveMap->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pLadderMap->GetParallaxScale(), -cameraPos.y / 1.f, 0.f);
			m_pLadderMap->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pBaseMap->GetParallaxScale(), -cameraPos.y / 1.f, 0.f);
			m_pBaseMap->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-cameraPos.x * m_pForeGroundMap->GetParallaxScale(), -cameraPos.y / 1.f, 0.f);
			m_pForeGroundMap->Draw();
		}
		glPopMatrix();
		
		break;
	default:
		break;
	}

	// Draw tileMap border
	glPushMatrix();
	{
		// Translate for camera
		glTranslatef(-cameraPos.x, -cameraPos.y, 0.f);
		// Draw outline of the tileMap
		utils::SetRGBColor(240, 30, 30);
		utils::DrawRect(Rectf{ 0.f, 0.f, float(m_MapDimentions.x) * 56, float(m_MapDimentions.y) * 56 });
	}
	glPopMatrix();
}

void EditorState::DrawCursor() const
{
	utils::SetRGBColor(255, 20, 20);
	utils::DrawEllipse(m_MousePos, 4.f, 4.f, 3.f);
}

void EditorState::DrawHoverRect() const
{
	if (!m_pTextureSelector->GetIsActive())
	{
		glPushMatrix();
		{
			// Translate for camera
			glTranslatef(-m_pCamera->GetPosition().x, -m_pCamera->GetPosition().y, 0.f);

			// Draw Hover rect
			utils::SetRGBColor(255, 255, 255);
			utils::DrawRect(m_HoverRect, 3.f);
		}
		glPopMatrix();
	}
}

void EditorState::PrintCurrentViewMode() const
{
	switch (m_SelectedViewMode)
	{
	case EditorState::ViewMode::background:
		std::cout << "VIEWMODE: " << "background.\n";
		break;
	case EditorState::ViewMode::caste:
		std::cout << "VIEWMODE: " << "castle.\n";
		break;
	case EditorState::ViewMode::lightForest:
		std::cout << "VIEWMODE: " << "lightForest.\n";
		break;
	case EditorState::ViewMode::darkForestOne:
		std::cout << "VIEWMODE: " << "darkForestOne.\n";
		break;
	case EditorState::ViewMode::darkForestTwo:
		std::cout << "VIEWMODE: " << "darkForestTwo.\n";
		break;
	case EditorState::ViewMode::water:
		std::cout << "VIEWMODE: " << "water: ";
		std::cout << "level: " << std::to_string(int(m_WaterViewMode)) << ".\n";
		break;
	case EditorState::ViewMode::cave:
		std::cout << "VIEWMODE: " << "cave.\n";
		break;
	case EditorState::ViewMode::base:
		std::cout << "VIEWMODE: " << "base.\n";
		break;
	case EditorState::ViewMode::ladders:
		std::cout << "VIEWMODE: " << "ladders.\n";
		break;
	case EditorState::ViewMode::foreGround:
		std::cout << "VIEWMODE: " << "foreGround.\n";
		break;
	case EditorState::ViewMode::all:
		std::cout << "VIEWMODE: " << "all.\n";
		break;
	default:
		std::cout << "What the hell are you doing?\n";
		break;
	}
}

// Other
void EditorState::SaveTileMaps()
{
	std::cout << "\n";
	// background
	// doesnt need any saving
	
	// castle
	m_pCastleMap->SaveToFile("Saves/" + m_LevelKey + "/Map/castleMap.morrog");

	// light forest
	m_pLightForestMap->SaveToFile("Saves/" + m_LevelKey + "/Map/lightForest.morrog");

	// dark forest one
	m_pDarkForestOneMap->SaveToFile("Saves/" + m_LevelKey + "/Map/darkForestOne.morrog");

	// dark forest two
	m_pDarkForestTwoMap->SaveToFile("Saves/" + m_LevelKey + "/Map/darkForestTwo.morrog");

	// water
	m_pWaterTileMaps[0]->SaveToFile("Saves/" + m_LevelKey + "/Map/waterOne.morrog");
	m_pWaterTileMaps[1]->SaveToFile("Saves/" + m_LevelKey + "/Map/waterTwo.morrog");
	m_pWaterTileMaps[2]->SaveToFile("Saves/" + m_LevelKey + "/Map/waterThree.morrog");

	// cave
	m_pCaveMap->SaveToFile("Saves/" + m_LevelKey + "/Map/caveMap.morrog");

	// baseMap
	m_pBaseMap->SaveToFile("Saves/" + m_LevelKey + "/Map/baseMap.morrog");

	// ladders
	m_pLadderMap->SaveToFile("Saves/" + m_LevelKey + "/Map/ladderMap.morrog");

	// foreground
	m_pForeGroundMap->SaveToFile("Saves/" + m_LevelKey + "/Map/foreGround.morrog");

	std::cout << "All maps saved!\n";
}

void EditorState::LoadTileMaps()
{
	std::cout << "\n";
	bool succes{ true };

	// background
	// doesnt need any loading

	// castle
	if (!m_pCastleMap->LoadFromFile(m_LevelKey + "/Map/castleMap.morrog"))
	{
		succes = false;
	}

	// light forest
	if (!m_pLightForestMap->LoadFromFile(m_LevelKey + "/Map/lightForest.morrog"))
	{
		succes = false;
	}

	// dark forest one
	if (!m_pDarkForestOneMap->LoadFromFile(m_LevelKey + "/Map/darkForestOne.morrog"))
	{
		succes = false;
	}

	// dark forest two
	if (!m_pDarkForestTwoMap->LoadFromFile(m_LevelKey + "/Map/darkForestTwo.morrog"))
	{
		succes = false;
	}

	// water
	if (!m_pWaterTileMaps[0]->LoadFromFile(m_LevelKey + "/Map/waterOne.morrog"))
	{
		succes = false;
	}
	if (!m_pWaterTileMaps[1]->LoadFromFile(m_LevelKey + "/Map/waterTwo.morrog"))
	{
		succes = false;
	}
	if (!m_pWaterTileMaps[2]->LoadFromFile(m_LevelKey + "/Map/waterThree.morrog"))
	{
		succes = false;
	}

	// cave
	if (!m_pCaveMap->LoadFromFile(m_LevelKey + "/Map/caveMap.morrog"))
	{
		succes = false;
	}

	// baseGame
	if (!m_pBaseMap->LoadFromFile(m_LevelKey + "/Map/baseMap.morrog"))
	{
		succes = false;
	}

	// ladders
	if (!m_pLadderMap->LoadFromFile(m_LevelKey + "/Map/ladderMap.morrog"))
	{
		succes = false;
	}

	// foreground
	if (!m_pForeGroundMap->LoadFromFile(m_LevelKey + "/Map/foreGround.morrog"))
	{
		succes = false;
	}

	if (succes)
	{
		std::cout << "All maps loaded!\n";
	}
	else
	{
		std::cout << "Maps where not loaded correct in EditorState::LoadTileMaps()!!\n";
	}
}

void EditorState::AddTile()
{
	if (m_pTextureSelector->GetIsActive())
	{
		m_TextureSrcRect = m_pTextureSelector->GetSrcRect();
	}
	else
	{
		switch (m_SelectedViewMode)
		{
		case EditorState::ViewMode::background:
			std::cout << "Cant add tile in current viewMode!\n";
			break;
		case EditorState::ViewMode::caste:
			m_pCastleMap->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::lightForest:
			m_pLightForestMap->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::darkForestOne:
			m_pDarkForestOneMap->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::darkForestTwo:
			m_pDarkForestTwoMap->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::water:
			m_pWaterTileMaps[int(m_WaterViewMode)]->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::cave:
			m_pCaveMap->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::base:
			m_pBaseMap->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::ladders:
			m_pLadderMap->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::foreGround:
			m_pForeGroundMap->AddTile(m_MousePosGrid, m_TextureSrcRect);
			break;
		case EditorState::ViewMode::all:
			std::cout << "Cant add tile in current viewMode!\n";
			break;
		default:
			break;
		}
	}
}

void EditorState::RemoveTile()
{
	if (!m_pTextureSelector->GetIsActive())
	{
		switch (m_SelectedViewMode)
		{
		case EditorState::ViewMode::background:
			std::cout << "Cant remove tile in current viewMode!\n";
			break;
		case EditorState::ViewMode::caste:
			m_pCastleMap->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::lightForest:
			m_pLightForestMap->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::darkForestOne:
			m_pDarkForestOneMap->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::darkForestTwo:
			m_pDarkForestTwoMap->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::water:
			m_pWaterTileMaps[int(m_WaterViewMode)]->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::cave:
			m_pCaveMap->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::base:
			m_pBaseMap->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::ladders:
			m_pLadderMap->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::foreGround:
			m_pForeGroundMap->RemoveTile(m_MousePosGrid);
			break;
		case EditorState::ViewMode::all:
			std::cout << "Cant remove tile in current viewMode!\n";
			break;
		default:
			break;
		}
	}
}

void EditorState::ChangeSelectMode(const std::string& direction)
{
	bool print{ false };

	// i know, i know hashtable would be nice here but i'm not smart enough something about std::map or something... idk idc 
	if (direction == "Up")
	{
		if (m_SelectedViewMode == ViewMode::all)
		{
			std::cout << "Cant go up anymore..\n";  // didnt want to have a circle here it would make things less efficient (to build maps in game)
			
		}
		else
		{
			m_SelectedViewMode = ViewMode(int(m_SelectedViewMode) + 1);		// nah, i dont want operator overloaders
			print = true;
		}
	}
	else if (direction == "Down")
	{
		if (m_SelectedViewMode == ViewMode::background)
		{
			std::cout << "Cant go down anymore..\n";
		}
		else
		{
			m_SelectedViewMode = ViewMode(int(m_SelectedViewMode) - 1);
			print = true;
		}
	}

	if (m_SelectedViewMode == ViewMode::water)		// only can change this when editing water maps
	{
		if (direction == "Left")
		{
			if (m_WaterViewMode == WaterViewMode::one)
			{
				std::cout << "Cant go any lower in this..\n";
			}
			else
			{
				m_WaterViewMode = WaterViewMode(int(m_WaterViewMode) - 1);
				print = true;
			}
		}
		else if (direction == "Right")
		{
			if (m_WaterViewMode == WaterViewMode::three)
			{
				std::cout << "Cant go any higher in this..\n";
			}
			else
			{
				m_WaterViewMode = WaterViewMode(int(m_WaterViewMode) + 1);
				print = true;
			}
		}
	}

	// print the current viewMode
	if (print)
	{
		PrintCurrentViewMode();
		ResetDrawAreas();
	}
}