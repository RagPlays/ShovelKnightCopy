#pragma once

#include "State.h"

class TileMap;
class PauseMenu;
class TextureSelector;
class Camera;
class Texture;
class Settings;
class TextureManager;

class EditorState final : public State
{
public:
	// Constructors / Destructors
	explicit EditorState(const Rectf& windowBorder, const std::string& levelKey, Settings* settings, TextureManager* textureManager);
	virtual ~EditorState() override;

	EditorState(const EditorState& editorState) = delete;
	EditorState& operator=(const EditorState& editorState) = delete;
	EditorState(EditorState&& editorState) noexcept = delete;
	EditorState& operator=(EditorState&& editorState) noexcept = delete;

	// Update / Draw
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	// Inputs
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:

	// in the editor state i want a tilemap that i can save and then load into the game (because i cant make a tilemap purely out of some numbers in a file)
	const std::string& m_LevelKey;
	const Rectf m_WindowBorder;
	const int m_TileGridSize;

	TextureManager* m_pTextureManager;

	// general tileMap variables
	float m_Scale;
	Point2i m_MapDimentions;

	Rectf m_CloudDestRect;						// where the background needs to be
	TileMap* m_pCastleMap;						// the castle
	TileMap* m_pLightForestMap;					// the light forest
	TileMap* m_pDarkForestOneMap;				// the first dark forest
	TileMap* m_pDarkForestTwoMap;				// the second dark forest
	std::vector < TileMap* > m_pWaterTileMaps;  // water maps (need to be updated!)
	TileMap* m_pCaveMap;						// cave
	TileMap* m_pBaseMap;						// base map where you walk on
	TileMap* m_pLadderMap;						// where you walk on
	TileMap* m_pForeGroundMap;					// draw this before the player
	
	// PauseMenu
	PauseMenu* m_pPauseMenu;

	// TextureRect funtionalities
	Rectf m_HoverRect;
	TextureSelector* m_pTextureSelector;
	Rectf m_TextureSrcRect;

	// Camera
	Camera* m_pCamera;
	std::vector< Rectf > m_CameraMoveRects;		// rects at the side of the windowborder to move camera
	int m_CameraSpeed;					// sets the speed of the camera

	// Default
	Point2f m_MousePos;
	Point2i m_MousePosGrid;

	// SVG BOUNDS
	std::vector< std::vector <Point2f> > m_Vertices;

	// ladders
	std::vector< LadderRectf > m_LadderRects;

	// settings
	Settings* m_pSettings;

	// ViewModes
	enum class ViewMode
	{
		background = 0,
		caste,
		lightForest,
		darkForestOne,
		darkForestTwo,
		water,
		cave,
		base,
		ladders,
		foreGround,
		all
	};
	ViewMode m_SelectedViewMode;

	enum class WaterViewMode
	{
		one = 0,
		two,
		three
	};
	WaterViewMode m_WaterViewMode;
	float m_WaterViewTime;   // to update the water

	bool m_GetMousePos;
	bool m_ShowTileOptimization;

private:

	// Init
	void InitVariables();
	void InitPauseMenu();
	void InitTileMaps();
	void InitGUI();
	void InitCamera();
	void InitVertices();
	void InitLadderRects();

	// Destroy
	void ClearVariables();
	void ClearTileMaps();
	void ClearGUI();
	void ClearCamera();

	// Update
	void UpdateEditor(float elapsedSec);
	void UpdateGUI(float elapsedSec);
	void UpdateCamera(float elapsedSec);
	void UpdateWaterView(float elapsedSec);
	void UpdateMapRenderAreas();
	void ResetDrawAreas();

	//Draw
	void DrawVertices() const;
	void DrawLadderRects() const;
	void DrawTileMaps() const;
	void DrawHoverRect() const;
	void DrawCameraMoveRects() const;
	void DrawGUI() const;
	void DrawCursor() const;

	// Other
	void PrintCurrentViewMode() const;
	void SaveTileMaps();
	void LoadTileMaps();
	void AddTile();
	void RemoveTile();
	void ChangeSelectMode(const std::string& direction);
};