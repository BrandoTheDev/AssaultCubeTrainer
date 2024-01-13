#include "Features.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int main(void)
{
	Features features;
	features.SetupDrawing();

	while (!GetAsyncKeyState(VK_END) & 1)
	{
		features.DisplayMenu();
		features.HackLoop();
		
		if (GetAsyncKeyState(VK_F1) & 1)
			features.toggle_god_mode();

		if (GetAsyncKeyState(VK_F2) & 1)
			features.toggle_infinite_ammo();

		if (GetAsyncKeyState(VK_F3) & 1)
			features.toggle_aimbot();

		if (GetAsyncKeyState(VK_F4) & 1)
			features.toggle_teleport_to_enemies();

		if (GetAsyncKeyState(VK_F5) & 1)
			features.toggle_esp();

		Sleep(1);
		system("cls");
	}

	system("pause");
	return 0;
}

