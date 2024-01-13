#pragma once
#include <math.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct Vector2
{
    float x, y;
};

struct Vec3 {
    float x, y, z;
};

struct Vector4
{
    float x, y, z, w;
};

struct Rotate
{
	float pitch, yaw;
};

Rotate CalcAngle(Vec3 PlayerPos, Vec3 EntityPos)
{
    Rotate rotate{};

    float deltaX = EntityPos.x - PlayerPos.x;
    float deltaY = EntityPos.y - PlayerPos.y;

    rotate.yaw = (float)(atan2(deltaY, deltaX) * 180 / 3.14159) + 90;

    float deltaZ = EntityPos.z - PlayerPos.z;
    float distance = (float)sqrt(pow(EntityPos.x - PlayerPos.x, 2) + pow(EntityPos.y - PlayerPos.y, 2));

    rotate.pitch = (float)(atan2(deltaZ, distance) * 180 / 3.14159);

    return rotate;
}

RECT GetWindowDimensions(LPCWSTR windowName)
{
    
    HWND hWnd = FindWindow(NULL, windowName);

    if (hWnd != NULL)
    {
        RECT windowRect;
        GetClientRect(hWnd, &windowRect);

        return windowRect;
    }
    else
    {
        // Error window not found
        RECT emptyR = { 0, 0, 0, 0 };
        return emptyR;
    }
}

Vector2 WorldToScreen(float matrix[16], Vec3 pos, int windowWidth, int windowHeight)
{
    Vector4 clipCoords;
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipCoords.w < 0.1f)
        return { -1.0f, -1.0f }; // Indicate failure

    Vec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    RECT windowRect = GetWindowDimensions(L"AssaultCube");

    Vector2 screenCoords;
    screenCoords.x = (windowRect.bottom / 2 * NDC.x) + (NDC.x + windowRect.bottom / 2);
    screenCoords.y = -(windowRect.right / 2 * NDC.y) + (NDC.y + windowRect.right / 2);

    return screenCoords;
}