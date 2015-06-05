#pragma once

#define SCREEN_WIDTH  GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)

#define ESPheight	56000
#define ESPwidth	18100

//Offsets
#define LocalPlayer 0xA6DA14
#define EntityList	0x4A10384
#define EntitySize	0x10
#define InCrosshair	0x23F8
#define teamNumber	0xF0
#define HealthOff	0xFC
#define LifeStatus	0x25B
#define BoneMatrix	0xA78
#define bSpotted	0x935
#define RankAddr	0x2370

#define CPlayers	0x933380 //933380

#define PositionX	0x134
#define PositionY	0x138
#define PositionZ	0x13C

#define viewOffsetX	0x104 //2384
#define viewOffsetY	0x108 //2388

#define PunchX		0x13DC
#define PunchY		0x13E0

#define EPointer	0x50B054
#define EPosition	0x5B2624

#define ViewMatrix	0x4A058D4

#define C4TOffset	0xA7BA14



extern IDirect3D9Ex* dx_Object;
extern IDirect3DDevice9Ex* dx_Device;
extern D3DPRESENT_PARAMETERS dx_Param;
extern ID3DXFont* dx_Font;
extern int Render();
extern int Width;
extern int Height;
extern HWND hWnd;
extern MSG Message;
extern HWND TargetWnd;
extern HINSTANCE hInstance;
extern RECT TargetRect;
extern RECT CheckWnd;
extern HINSTANCE  inj_hModule;
extern const MARGINS Margin;

LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ThreadProc(LPVOID lpParam);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);


void LogError(char* in);
void LogWarning(char* in);
void Log(char* in);

int DirectXInit(HWND hWnd);

float Get3dDistance(Vector myCoords, Vector enemyCoords);
Vector CalculateAngles(IClientEntity* pLocal, Vector src, Vector dst);
VOID WINAPI PaintTraverse_Hooked1(unsigned int vguiPanel, bool forceRepaint, bool allowForce);
Vector Normalize(Vector vAngle);
void DoCheat();