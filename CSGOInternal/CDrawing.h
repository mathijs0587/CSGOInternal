#pragma once
#include "stdafx.h"

class CDrawing
{
public:
	void TextW(bool cent, int font, int x, int y, Color color, wchar_t *pszString);
	void DrawString(int FONT, bool bCenter, int x, int y, Color c, const char *fmt, ...);
	void OutlinedRect(int x, int y, int w, int h, Color& color);
	void DrawHealthBar(int x, int y, float health, int w, int h, Color Draw);
	void FillRGBA( int x, int y, int w, int h, Color colour );
	void DrawLine(int x, int y, int xx, int yy, Color color);
	void DrawCircle(int x, int y, float radius, int r, int g, int b, int a);
};

extern CDrawing cDraw;