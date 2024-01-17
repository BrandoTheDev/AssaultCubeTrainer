#include "Features.h"
#include "Offsets.h"
#include "Utils.h"

#include <iostream>
#include <string>

MemMan mem;

Features::Features()
{
    std::cout << "Starting up.." << std::endl;
    std::cout << "Created Memory Manager" << std::endl;
    bool gameFound = mem.FindGame(L"ac_client.exe");

    const char* status = gameFound ? "has been found!" : "can NOT be found. Try again.";
    std::cout << "The game " << status << std::endl;

    if (!status)
    {
        Sleep(3000);
        exit(-1);
    }

    DWORD pid = mem.GetProcessId();
    std::cout << "Process id = " << pid << std::endl;

    this->player = mem.Read<DWORD>((DWORD)((BYTE*)Offsets::LOCAL_PLAYER));
    std::cout << this->player << std::endl;
}

const char* Features::on_or_off(bool status)
{
    return (status) ? "[ON]" : "[OFF]";
}

void Features::DisplayMenu()
{
    std::cout << "Assault Cube External" << std::endl << std::endl;
    std::cout << "Menu Options:" << std::endl;
    std::cout << "(F1) God Mode          " << on_or_off(this->b_god_mode) << std::endl;
    std::cout << "(F2) Infinite Ammo     " << on_or_off(this->b_infinite_ammo) << std::endl;
    std::cout << "(F3) Aimbot (SHIFT)    " << on_or_off(this->b_aimbot) << std::endl;
    std::cout << "(F4) Teleport2Enemies  " << on_or_off(this->b_teleport_to_enemies) << std::endl;
    std::cout << "(F5) ESP               " << on_or_off(this->b_esp) << std::endl;
    std::cout << "(END) Key to quit." << std::endl << std::endl << std::endl;
}

// Switches ---------------------------
void Features::toggle_esp()
{
    this->b_esp = !this->b_esp;
}
void Features::toggle_aimbot()
{
    this->b_aimbot = !this->b_aimbot;
}
void Features::toggle_god_mode()
{
    this->b_god_mode = !this->b_god_mode;
}
void Features::toggle_infinite_ammo()
{
    this->b_infinite_ammo = !this->b_infinite_ammo;
}
void Features::toggle_teleport_to_enemies()
{
    this->b_teleport_to_enemies = !this->b_teleport_to_enemies;
}

// Logic ---------------------------
void Features::HackLoop()
{
    int totalPlayers = mem.Read<int>(Offsets::PLAYER_COUNT);
    DWORD entityList = mem.Read<DWORD>((DWORD)((BYTE*)Offsets::ENTITY_LIST));

    for (int i = 1; i < totalPlayers; i++)
    {
        DWORD local_player = player;
        DWORD entity = mem.Read<DWORD>(entityList + (0x4 * i));

        if (!entity) continue;

        DWORD healthAddy = entity + Offsets::HEALTH;
        int health = mem.Read<int>(healthAddy);

        if (health <= 0) continue;

        if (b_teleport_to_enemies)
            Features::TeleportToEnemies(entity, local_player);

        if (b_aimbot)
            Features::Aimbot(entity, local_player);

        if (b_god_mode)
            Features::GodMode(local_player);

        if (b_infinite_ammo)
            Features::InfiniteAmmo(local_player);

        if (b_esp)
            Features::ESP(entity);
    }
}

void Features::GodMode(DWORD player)
{
    DWORD healthAddy = player + Offsets::HEALTH;
    int health = mem.Read<int>(healthAddy);

    if (health < 1337)
        mem.Write(healthAddy, 1337);
}
void Features::InfiniteAmmo(DWORD player)
{
    DWORD ammoAddy = player + Offsets::RIFLE_AMMO;
    int ammo = mem.Read<int>(ammoAddy);

    if (ammo < 1337)
        mem.Write(ammoAddy, 1337);
}
void Features::ESP(DWORD entity)
{
    DWORD userAddy = entity + Offsets::USERNAME;
    const char* user = mem.Read<const char*>(userAddy);

    Vec3 EntityPos{};
    EntityPos.x = mem.Read<float>(entity + Offsets::POS_X);
    EntityPos.y = mem.Read<float>(entity + Offsets::POS_Y);
    EntityPos.z = mem.Read<float>(entity + Offsets::HEAD_Z);

    float matrix[16];
    mem.ReadArray<float>(Offsets::VIEW_MATRIX, matrix, 16);
    
    // My WTS function takes in reso|_ution so you can use your fav 
    // way to get that and put it in for dynamic screen size esp
    // In my updated version I use my GetWindowDim in Uti|_s.h
    Vector2 enemy = WorldToScreen(matrix, EntityPos, 600, 600);
    
    DrawBorderBox(enemy.x - 15, enemy.y - 10, 25, 40, 1, hBrushEnemy);
    DrawString(enemy.x, enemy.y - 25, RGB(255, 0, 0), user);
    free((void*)user);
}
void Features::Aimbot(DWORD entity, DWORD player)
{
    if (!GetAsyncKeyState(VK_SHIFT)) return;

    // Player eyes
    Rotate eyes{};
    DWORD eyesPitchAddy = (player + Offsets::EYES_PITCH);
    DWORD eyesYawAddy = (player + Offsets::EYES_YAW);

    Vec3 PlayerPos{};
    Vec3 EntityPos{};

    Rotate rotate{};

    PlayerPos.x = mem.Read<float>(player + Offsets::POS_X);
    PlayerPos.y = mem.Read<float>(player + Offsets::POS_Y);
    PlayerPos.z = mem.Read<float>(player + Offsets::HEAD_Z);

    EntityPos.x = mem.Read<float>(entity + Offsets::POS_X);
    EntityPos.y = mem.Read<float>(entity + Offsets::POS_Y);
    EntityPos.z = mem.Read<float>(entity + Offsets::HEAD_Z);

    rotate = CalcAngle(PlayerPos, EntityPos);

    mem.Write(eyesPitchAddy, rotate.pitch);
    mem.Write(eyesYawAddy, rotate.yaw);
}
void Features::TeleportToEnemies(DWORD entity, DWORD player)
{
    Vec3 PlayerPos{};
    Vec3 EntityPos{};

     EntityPos.x = mem.Read<float>(entity + Offsets::POS_X);
     EntityPos.y = mem.Read<float>(entity + Offsets::POS_Y);
     EntityPos.z = mem.Read<float>(entity + Offsets::HEAD_Z);

     PlayerPos.x = mem.Write<float>(player + Offsets::POS_X, EntityPos.x);
     PlayerPos.y = mem.Write<float>(player + Offsets::POS_Y, EntityPos.y);
     PlayerPos.z = mem.Write<float>(player + Offsets::POS_Z, EntityPos.z);
}

// Rendering ---------------------------
void Features::SetupDrawing()
{
    this->TargetWnd = FindWindow(0, L"AssaultCube");
    this->HDC_Desktop = GetDC(this->TargetWnd);
    this->TextCOLOR = RGB(0, 255, 0);
}
void Features::DrawFilledRect(int x, int y, int w, int h, HBRUSH brushColor)
{
    RECT rect = { x, y, x + w, y + h };
    FillRect(this->HDC_Desktop, &rect, brushColor);
}
void Features::DrawBorderBox(int x, int y, int w, int h, int thickness, HBRUSH brushColor)
{
    DrawFilledRect(x, y, w, thickness, brushColor);
    DrawFilledRect(x, y, thickness, h, brushColor);
    DrawFilledRect((x + w), y, thickness, h, brushColor);
    DrawFilledRect(x, y + h, w + thickness, thickness, brushColor);
}
void Features::DrawString(int x, int y, COLORREF color, const char* text)
{
    SetTextAlign(this->HDC_Desktop, TA_CENTER | TA_NOUPDATECP);
    SetBkColor(this->HDC_Desktop, RGB(0, 0, 0));
    SetBkMode(this->HDC_Desktop, TRANSPARENT);
    SetTextColor(this->HDC_Desktop, color);
    SelectObject(this->HDC_Desktop, Font);
    TextOutA(this->HDC_Desktop, x, y, text, strlen(text));
    DeleteObject(Font);
}
