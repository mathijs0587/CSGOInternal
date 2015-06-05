#pragma once
#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "Dwmapi.lib")

class Drawing
{
public:
	Drawing();
	~Drawing();

public:
	//static void DrawFilledBox(int x, int y, int w, int h, int r, int g, int b, int a);
	//static void DrawBox(int x, int y, int width, int height, int px, int r, int g, int b, int a);
	//static int DrawFont(char* String, int x, int y, int r, int g, int b);
	//static int DrawShadowText(char* String, int x, int y, int r, int g, int b);
	//static void DrawLine(float x, float y, float xx, float yy, int r, int g, int b, int a);
	//static void FillRGB(float x, float y, float w, float h, int r, int g, int b, int a);
	//static void DrawBox(float x, float y, float width, float height, float px, int r, int g, int b, int a);
	//static void DrawCircle(float x, float y, float Radius, int r, int g, int b, int a);
	//static void DrawString(int x, int y, int FontSize, DWORD color, LPCSTR Message);
	static void DrawESP(int i, IClientEntity* pLocal, IClientEntity* pEntity);
	//static void DrawRect(int x, int y, int width, int height, D3DCOLOR color);
	static void DrawBone(Color color, int boneID1, int boneID2, IClientEntity* Entity, IClientEntity* localEntity);
	static void DrawBoneESP(Color color, IClientEntity* Entity, IClientEntity* localEntity);
	static void Menu();
};

