#include "stdafx.h"

CDrawing cDraw;
void CDrawing::TextW(bool cent, int font, int x, int y, Color color, wchar_t *pszString)
{
	if(cent)
	{
		int wide, tall;
		pSurface->GetTextSize(font, pszString, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}
	pSurface->DrawSetTextColor( color );
	pSurface->DrawSetTextFont(font);
	pSurface->DrawSetTextPos(x, y);
	pSurface->DrawPrintText(pszString, (int)wcslen(pszString), FONT_DRAW_DEFAULT);
}
void CDrawing::DrawString(int FONT, bool bCenter, int x, int y, Color c, const char *fmt, ...) 
{
	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(fmt) + 1) * sizeof(wchar_t)));
	
	mbstowcs(pszStringWide, fmt, (strlen(fmt) + 1) * sizeof(wchar_t));

	TextW(bCenter, FONT, x, y, c, pszStringWide);

	free(pszStringWide);
}
void CDrawing::OutlinedRect(int x, int y, int w, int h, Color& color)
{
	pSurface->DrawSetColor(color);
	pSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void CDrawing::FillRGBA( int x, int y, int w, int h, Color colour )
{
	pSurface->DrawSetColor( colour );
	pSurface->DrawFilledRect( x, y, x + w, y + h );
}
void CDrawing::DrawHealthBar(int x, int y, float health, int w, int h, Color Draw)
{
	x -= w / 2;
	y -= h / 2;
	FillRGBA( x, y, w, h+1, Color(0,0,0,255) );
	UINT hw = (UINT)( ( ( w - 2 ) * health ) / 100 );
	FillRGBA( x+1, y+1, hw, h-1, Draw );
}

void CDrawing::DrawLine(int x, int y, int xx, int yy, Color color)
{
	pSurface->DrawSetColor(color);
	pSurface->DrawLine(x, y, xx, yy);
}

void CDrawing::DrawCircle(int x, int y, float radius, int r, int g, int b, int a)
{
	pSurface->DrawCircle(x, y, radius, r, g, b, a);
	pSurface->DrawCircle(x, y, radius + 0.1f, r, g, b, a);
	pSurface->DrawCircle(x, y, radius + 0.2f, r, g, b, a);
	pSurface->DrawCircle(x, y, radius + 0.3f, r, g, b, a);
	pSurface->DrawCircle(x, y, radius + 0.4f, r, g, b, a);
	pSurface->DrawCircle(x, y, radius + 0.5f, r, g, b, a);
}