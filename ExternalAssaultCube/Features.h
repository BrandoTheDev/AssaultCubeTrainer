#pragma once
#include "MemMan.h"

class Features
{
private:
	bool b_god_mode = false;
	bool b_infinite_ammo = false;
	bool b_aimbot = false;
	bool b_teleport_to_enemies = false;
	bool b_esp = false;

	DWORD player;
	const char* on_or_off(bool status);

	// Cheat Logic
	void ESP(DWORD entity);
	void GodMode(DWORD player);
	void InfiniteAmmo(DWORD player);
	void Aimbot(DWORD entity, DWORD player);
	void TeleportToEnemies(DWORD entity, DWORD player);
	
	// ESP Specific Section
	HBRUSH hBrushEnemy = CreateSolidBrush(RGB(255, 0, 0));
	COLORREF enemyColor = RGB(255, 0, 0);
	HDC HDC_Desktop = nullptr;
	HFONT Font = nullptr;
	HWND TargetWnd = nullptr;
	HWND HANDLE = nullptr;
	COLORREF TextCOLOR = NULL;
	RECT m_Rect{};

public:
	Features();
	void DisplayMenu();

	// Cheat Switches
	void toggle_esp();
	void toggle_aimbot();
	void toggle_god_mode();
	void toggle_infinite_ammo();
	void toggle_teleport_to_enemies();

	// Cheat Logic
	void HackLoop();

	// Rendering
	void SetupDrawing();
	void DrawFilledRect(int x, int y, int w, int h, HBRUSH brushColor);
	void DrawBorderBox(int x, int y, int w, int h, int thickness, HBRUSH brushColor);
	void DrawString(int x, int y, COLORREF color, const char* text);
};

