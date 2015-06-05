// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "CSGOInternal.h"
#include "Drawing.h"
#include "ConfigFile.h"
#include <sys/stat.h>
#include <ctime>

#include <CkHttp.h>
#include <CkHttpResponse.h>

int Width = SCREEN_WIDTH;
int Height = SCREEN_HEIGHT;

HWND hWnd;
MSG Message;
HWND TargetWnd;
HINSTANCE hInstance;
RECT TargetRect;
RECT CheckWnd;
HINSTANCE  inj_hModule;
const MARGINS Margin = { 0, 0, Width, Height };

Vector SavedPos;

Vector AimNeck, AimNeck2, vNeck1, vNeck2;



RECT m_Rect;
Weapon_ID WeaponID;

bool IsGameAvail = true;

bool DoThisOnce = true;

bool visuals = true;

int drawMenu = 0;

float aimbotSpeed = 0.5f;

float TempAimDistance = 1000000;
float TempTargetDistance = 1000000;

Vector view(50.0f, 50.0f, 0.0f);
Vector tempView(50.0f, 50.0f, 0.0f);

IClientEntity* AimEntity = NULL;

clock_t t;

clock_t aimbotTimerTest;

Vector oldPunch(0, 0, 0);

HMODULE hModEngine = NULL;
HMODULE hModClient = NULL;
HMODULE hModvGuiMat = NULL;
HMODULE hModvGui = NULL;
HMODULE hModMat = NULL;
HMODULE hModStudioRender = NULL;

BOOL bOnce = false;

bool toggle = false;

int debugTestInt = 0;

float Get3dDistance(Vector myCoords, Vector enemyCoords)
{
	return sqrt(
		pow(float(enemyCoords.X - myCoords.X), 2.0) +
		pow(float(enemyCoords.Y - myCoords.Y), 2.0) +
		pow(float(enemyCoords.Z - myCoords.Z), 2.0));
}

bool exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
CreateInterfaceFn CaptureFactory(char *pszFactoryModule)
{
	CreateInterfaceFn fn = NULL;
	while (fn == NULL)
	{
		HMODULE hFactoryModule = GetModuleHandleA(pszFactoryModule);
		if (hFactoryModule)
		{
			fn = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(hFactoryModule, "CreateInterface"));
		}
		Sleep(10);
	}
	return fn;
}

void *CaptureInterface(CreateInterfaceFn fn, char *pszInterfaceName)
{
	unsigned long *ptr = NULL;
	while (ptr == NULL)
	{
		ptr = reinterpret_cast<unsigned long*>(fn(pszInterfaceName, NULL));
		Sleep(10);
	}

	return ptr;
}


LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		DwmExtendFrameIntoClientArea(hWnd, &Margin);

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	return 0;
}

typedef void(__stdcall *BeginFrame_)();
BeginFrame_ oBeginFrame = NULL;

VOID WINAPI BeginFrame_Hooked()
{
	__asm
	{
			mov		ecx, pStudioRender
			call    oBeginFrame
	}
}

bool AllowedToRun()
{
	CkHttp http;
	bool success;

	//  Any string unlocks the component for the 1st 30-days.
	success = http.UnlockComponent("Anything for 30-day trial");
	if (success != true) {
		return false;
	}

	//  Send the HTTP GET and return the content in a string.
	const char *html = 0;
	html = http.quickGetStr("http://www.mathijsparie.nl/csgo/confirm.php");

	if (html == "accept")
	{
		return true;
	}
	

	return false;
}

typedef void(__stdcall *PaintTraverse_)(unsigned int, bool, bool);
PaintTraverse_ oPaintTraverse = NULL;

VOID WINAPI PaintTraverse_Hooked1(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	__asm
	{
			push    allowForce
			push    forceRepaint
			push    vguiPanel
			mov		ecx, pPanel
			call    oPaintTraverse
	}

	if (!bOnce)
	{
		//if (!AllowedToRun())
		//{
		//	exit(EXIT_FAILURE);
		//}

		espfont = pSurface->FontCreate();
		pSurface->SetFontGlyphSet(espfont, "Consolas", 12, 400, 0, 0, FONTFLAG_OUTLINE);

		if (exists("config.txt"))
		{
			ConfigFile cf("config.txt");

			boxESP			= cf.Value("EbolaHacks", "boxESP");
			boneESP			= cf.Value("EbolaHacks", "boneESP");
			aimbotFOV		= cf.Value("EbolaHacks", "aimbotFOV");
			aimbotKey		= cf.Value("EbolaHacks", "aimbotKey");
			ffa				= cf.Value("EbolaHacks", "FreeForAll");
			triggerbot		= cf.Value("EbolaHacks", "triggerbot");
			triggerbotD		= cf.Value("EbolaHacks", "triggerbotDelay");
			aimbotMov		= cf.Value("EbolaHacks", "aimbotSmooth");
			shootOnZoom		= cf.Value("EbolaHacks", "OnlyShootWhenZoomed");
			visuals			= cf.Value("EbolaHacks", "visuals");
			friendlyBoneESP = cf.Value("EbolaHacks", "friendlyBoneESP");
		}
		bOnce = TRUE;
	}

	PCHAR szPanelName = (PCHAR)pPanel->GetName(vguiPanel);
	if (strstr(szPanelName, "MatSystemTopPanel"))
	{
		//cDraw.DrawString(espfont, false, 0, 12, Color(25, 50, 255, 255), "PaintTraverse");
		//cDraw.DrawCircle(100, 100, 100, 100, 100, 255, 255);
		
		DoCheat();
	}
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	wchar_t *pString = reinterpret_cast<wchar_t *> (lpParam);

	do
	{
		hModEngine = GetModuleHandleA("engine.dll");
		hModClient = GetModuleHandleA("client.dll");
		hModvGuiMat = GetModuleHandleA("vguimatsurface.dll");
		hModvGui = GetModuleHandleA("vgui2.dll");
		hModMat = GetModuleHandleA("materialsystem.dll");
		hModStudioRender = GetModuleHandleA("studiorender.dll");
	} while (!hModEngine || !hModClient || !hModvGuiMat || !hModvGui || !hModMat || !hModStudioRender);

	CreateInterfaceFn fnClient = CaptureFactory("client.dll");
	CreateInterfaceFn fnEngine = CaptureFactory("engine.dll");
	CreateInterfaceFn fnVguiMatSurface = CaptureFactory("vguimatsurface.dll");
	CreateInterfaceFn fnVgui2 = CaptureFactory("vgui2.dll");
	CreateInterfaceFn fnMatSystem = CaptureFactory("materialsystem.dll");
	CreateInterfaceFn fnStudioRender = CaptureFactory("studiorender.dll");

	pClient = reinterpret_cast<IBaseClientDLL*>(CaptureInterface(fnClient, "VClient016"));
	pEngine = reinterpret_cast<IVEngineClient*>(CaptureInterface(fnEngine, "VEngineClient013"));
	pSurface = reinterpret_cast<ISurface*>(CaptureInterface(fnVguiMatSurface, "VGUI_Surface031"));
	pClientEntList = reinterpret_cast<IClientEntityList*>(CaptureInterface(fnClient, "VClientEntityList003"));
	pMat = reinterpret_cast<IMaterialSystem*>(CaptureInterface(fnMatSystem, "VMaterialSystem080"));
	pPanel = (IPanel*)fnVgui2("VGUI_Panel009", NULL);
	pEngineTrace = reinterpret_cast<IEngineTrace*>(CaptureInterface(fnEngine, "EngineTraceClient004"));
	pStudioRender = reinterpret_cast<IStudioRender*>(CaptureInterface(fnStudioRender, "VStudioRender026"));
	pDebugOverlay = reinterpret_cast<NDebugOverlay*>(CaptureInterface(fnEngine, "VDebugOverlay004"));

	PDWORD pdwClientVMT = *reinterpret_cast<PDWORD_PTR*>(pClient);
	DWORD dwInitAddr = (DWORD)(pdwClientVMT[0]);
	for (DWORD dwIter = 0; dwIter <= 0xFF; dwIter++)
	{
		if (*(PBYTE)(dwInitAddr + dwIter - 1) == 0x51 && *(PBYTE)(dwInitAddr + dwIter) == 0xA3)
		{
			pGlobals = (CGlobalVarsBase*)*(PDWORD)*(PDWORD)(dwInitAddr + dwIter + 1);
			break;
		}
	}

	pPanelHook = new CVMTHookManager((PDWORD*)pPanel);
	oPaintTraverse = (PaintTraverse_)pPanelHook->dwGetMethodAddress(41);
	pPanelHook->dwHookMethod((DWORD)PaintTraverse_Hooked1, 41);

	pStudioRenderHook = new CVMTHookManager((PDWORD*)pStudioRender);
	oBeginFrame = (BeginFrame_)pStudioRenderHook->dwGetMethodAddress(9);
	pStudioRenderHook->dwHookMethod((DWORD)BeginFrame_Hooked, 9);


	for (;;)
	{
		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return 0;

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{

		inj_hModule = hModule;
		CreateThread(0, NULL, ThreadProc, (LPVOID)L"init", NULL, NULL);
	}
	return TRUE;
}

Vector CalculateAngles(IClientEntity* pLocal, Vector src, Vector dst)
{
	Vector vAngle;
	Vector delta((src.X - dst.X), (src.Y - dst.Y), (src.Z - dst.Z));
	double hyp = sqrt(delta.X*delta.X + delta.Y*delta.Y);

	vAngle.X = (float)(atanf((delta.Z + 64.06f) / (float)hyp) * 57.295779513082f);
	vAngle.Y = (float)(atanf(delta.Y / delta.X) * 57.295779513082f);
	vAngle.Z = 0.0f;

	if (delta.X >= 0.0)
		vAngle.Y += 180.0f;

	if (pLocal->GetPunchAngles().X)
		vAngle.X -= (pLocal->GetPunchAngles().X * 2.0f);

	if (pLocal->GetPunchAngles().Y)
		vAngle.Y -= (pLocal->GetPunchAngles().Y * 2.0f);

	return Normalize(vAngle);
}

Vector Normalize(Vector vAngle)
{
	if (vAngle.X >= 89.0f)
		vAngle.X = 89.0f;
	if (vAngle.X <= -89.0f)
		vAngle.X = -89.0f;

	if (vAngle.Y >= 180.0)
	{
		vAngle.Y -= 360.0;
	}
	else if (vAngle.Y <= -180.0)
	{
		vAngle.Y += 360.0;
	}

	vAngle.Z = 0.0f;

	return vAngle;
}

Vector noRecoil(IClientEntity* pLocal, Vector vAngle)
{
	if (pLocal->GetPunchAngles().X)
		vAngle.X -= ((pLocal->GetPunchAngles().X - oldPunch.X) * 2.0f);

	if (pLocal->GetPunchAngles().Y)
		vAngle.Y -= ((pLocal->GetPunchAngles().Y - oldPunch.Y) * 2.0f);

	oldPunch = pLocal->GetPunchAngles();

	return Normalize(vAngle);
}

bool GotEntity = false;
IClientEntity* targetEntity;
float targetDistance;

/*void Aimbot(IClientEntity* pLocalEntity, IClientEntity* pEntity)
{
	bool PlayerVisible = false;
	bool PlayerAlive = false;

	if (AimEntity != NULL)
	{
		if (AimEntity->IsDormant() || !cEng.IsAlive(AimEntity))
		{
			AimEntity = NULL;
			TempAimDistance = 100000;
		}
	}
	else
	{
		TempAimDistance = 100000;
	}


	PlayerAlive = false;
	if (AimEntity != NULL)
	{
		
		if (cEng.GetBonePosition(AimEntity, AimNeck, 10))
		{
			bool bDucking = pLocalEntity->GetFlags() &(1 << 1);
			if (bDucking)
			{
				Vector renderOriginDucking = pLocalEntity->GetRenderOrigin();
				renderOriginDucking.Z -= 20;
				view = CalculateAngles(pLocalEntity, renderOriginDucking, AimNeck);
			}
			else
			{
				view = CalculateAngles(pLocalEntity, pLocalEntity->GetRenderOrigin(), AimNeck);
			}
			
			if (cMath.WorldToScreen(AimNeck, AimNeck2))
			{
				float xdist = AimNeck2.X - (SCREEN_WIDTH / 2);
				float ydist = AimNeck2.Y - (SCREEN_HEIGHT / 2);
				TempAimDistance = sqrt(pow(ydist, 2) + pow(xdist, 2));

				if (TempAimDistance > aimbotFOV)
				{
					AimEntity = NULL;
					TempAimDistance = 100000;
				}
			}
		}
	}

	if (cEng.GetBonePosition(pEntity, vNeck1, 10))
	{
		if (cMath.WorldToScreen(vNeck1, vNeck2))
		{
			float xdist = vNeck2.X - (SCREEN_WIDTH / 2);
			float ydist = vNeck2.Y - (SCREEN_HEIGHT / 2);
			float AimDistance = sqrt(pow(ydist, 2) + pow(xdist, 2));
			if (AimDistance < TempAimDistance && AimDistance < aimbotFOV)
			{
				TempAimDistance = AimDistance;
				AimEntity = pEntity;
				bool bDucking = pLocalEntity->GetFlags() &(1 << 1);
				if (bDucking)
				{
					Vector renderOriginDucking = pLocalEntity->GetRenderOrigin();
					renderOriginDucking.Z -= 20;
					view = CalculateAngles(pLocalEntity, renderOriginDucking, vNeck1);
				}
				else
				{
					view = CalculateAngles(pLocalEntity, pLocalEntity->GetRenderOrigin(), vNeck1);
				}
			}
		}
	}


	if (AimEntity != NULL)
	{
		if (cEng.IsAlive(AimEntity))
		{
			//cDraw.DrawString(espfont, false, 1920 / 2, 490, Color(50, 255, 100, 255), "isAlive");
			Vector Player = pLocalEntity->GetOrigin();
			Player.Z += 66;
			bool bDuckingPlayer = pLocalEntity->GetFlags() &(1 << 1);
			if (bDuckingPlayer)
			{
				Player.Z -= 20;
			}
			Vector Target = AimEntity->GetOrigin();
			Target.Z += 66;
			bool bDuckingEnemy = pLocalEntity->GetFlags() &(1 << 1);
			if (bDuckingEnemy)
			{
				Target.Z -= 20;
			}
			if (cEng.IsVisible(pLocalEntity, Player, Target))
			{
				Vector myView, daView;
				pEngine->GetViewAngles(myView);

				daView = view - myView;
				//cDraw.DrawString(espfont, false, 1920 / 2, 500, Color(50, 255, 100, 255), "isVisible");
				//myView += (daView / aimbotMov);
				if (clock() - aimbotTimerTest >= ((float)(rand() % 8) + 6.5f))
				{
					daView = Normalize(daView);
					myView += daView / aimbotMov;

					myView = Normalize(myView);
					pEngine->SetViewAngles(myView);

					aimbotTimerTest = clock();
				}
			}
		}
	}
}*/


void Aimbot(IClientEntity* pLocalEntity, IClientEntity* pEntity)
{
	if (cEng.IsAlive(pEntity))
	{
		if (cEng.GetBonePosition(pEntity, vNeck1, 10))
		{
			if (cMath.WorldToScreen(vNeck1, vNeck2))
			{
				bool bDucking = pLocalEntity->GetFlags() &(1 << 1);
				if (bDucking)
				{
					Vector renderOriginDucking = pLocalEntity->GetRenderOrigin();
					renderOriginDucking.Z -= 20;
					view = CalculateAngles(pLocalEntity, renderOriginDucking, vNeck1);
				}
				else
				{
					view = CalculateAngles(pLocalEntity, pLocalEntity->GetRenderOrigin(), vNeck1);
				}

				Vector myView, daView;
				pEngine->GetViewAngles(myView);

				daView = view - myView;

				if (clock() - aimbotTimerTest >= ((float)(rand() % 8) + 6.5f))
				{
					daView = Normalize(daView);
					myView += daView / aimbotMov;

					myView = Normalize(myView);
					pEngine->SetViewAngles(myView);

					aimbotTimerTest = clock();
				}
			}
		}

		
	}
}

void DoCheat()
{
	if (drawMenu)
		cDraw.DrawLine(0, 0, 1, 1, Color(255, 255, 255, 255));

	if (DoThisOnce)
	{
		t = clock();
		aimbotTimerTest = clock();
		DoThisOnce = false;
	}
	if (GetAsyncKeyState(VK_NUMPAD9))
	{
		toggle = false;
	}
	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		toggle = true;
	}
	if (GetAsyncKeyState(VK_NUMPAD6))
	{
		visuals = false;
	}
	if (GetAsyncKeyState(VK_NUMPAD5))
	{
		visuals = true;
	}
	if (GetAsyncKeyState(VK_INSERT) && clock() - t > 300.0f)
	{
		t = clock();
		if (drawMenu == 0)
		{
			drawMenu++;
		}
		else if (drawMenu == 1)
		{
			drawMenu--;
		}
	}

	if (drawMenu)
		Drawing::Menu();

	

	if (!pEngine->IsConnected() && !pEngine->IsInGame())
	{
		AimEntity = NULL;
		TempAimDistance = 100000;
	}

	if (toggle && pEngine->IsConnected() && pEngine->IsInGame())
	{

		IClientEntity* pLocalEntity = pClientEntList->GetClientEntity(pEngine->GetLocalPlayer());

		if (!pLocalEntity)
			return;		
		
		if (cEng.IsAlive(pLocalEntity))
		{
			//pLocalEntity->SetVisualPunch();
			if (pLocalEntity->GetFlags() == FL_ONGROUND && GetAsyncKeyState(0x04))
			{				
				keybd_event(VK_SPACE, 0x39, 0, 0);
				keybd_event(VK_SPACE, 0x39, KEYEVENTF_KEYUP, 0);
			}

			if (aimbotFOV != 0 && drawMenu)
				cDraw.DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, aimbotFOV, 255, 0, 0, 255);


			if (GetAsyncKeyState(aimbotKey) && !pLocalEntity->isHoldingThrowable(pClientEntList) && pLocalEntity->ammoLeft(pClientEntList) > 0)
			{
				if (!GotEntity)
				{
					for (int i = 0; i <= pClientEntList->GetHighestEntityIndex(); i++)
					{
						IClientEntity* pEntity = pClientEntList->GetClientEntity(i);

						if (pEntity == NULL || !pEntity)
							continue;

						if (pEntity->Index() == pLocalEntity->Index())
							continue;

						if (!ffa)
						{
							if (pEntity->GetTeamNum() == pLocalEntity->GetTeamNum())
							{
								continue;
							}
						}

						if (pEntity->IsDormant())
							continue;

						if (!cEng.IsAlive(pEntity))
							continue;

						if (cEng.GetHealth(pEntity) < 1)
							continue;

						
						if (cEng.GetBonePosition(pEntity, AimNeck, 10))
						{
							if (cMath.WorldToScreen(AimNeck, AimNeck2))
							{
								float xdist = AimNeck2.X - (SCREEN_WIDTH / 2);
								float ydist = AimNeck2.Y - (SCREEN_HEIGHT / 2);
								TempAimDistance = sqrt(pow(ydist, 2) + pow(xdist, 2));

								if (TempAimDistance > aimbotFOV)
								{
									continue;
								}
							}
						}

						//cDraw.DrawString(espfont, false, 1920 / 2, 490, Color(50, 255, 100, 255), "isAlive");
						Vector Player = pLocalEntity->GetOrigin();
						Player.Z += 66;
						bool bDuckingPlayer = pLocalEntity->GetFlags() &(1 << 1);
						if (bDuckingPlayer)
						{
							Player.Z -= 20;
						}
						Vector Target = pEntity->GetOrigin();
						Target.Z += 66;
						bool bDuckingEnemy = pEntity->GetFlags() &(1 << 1);
						if (bDuckingEnemy)
						{
							Target.Z -= 20;
						}
						if (cEng.IsVisible(pLocalEntity, Player, Target) && TempAimDistance < targetDistance && TempAimDistance < aimbotFOV)
						{
							targetDistance = TempAimDistance;
							targetEntity = pEntity;
							GotEntity = true;
						}

					}
				}
				
				if (GotEntity)
				{
					Vector Player = pLocalEntity->GetOrigin();
					Player.Z += 66;
					bool bDuckingPlayer = pLocalEntity->GetFlags() &(1 << 1);
					if (bDuckingPlayer)
					{
						Player.Z -= 20;
					}
					Vector Target = targetEntity->GetOrigin();
					Target.Z += 66;
					bool bDuckingEnemy = targetEntity->GetFlags() &(1 << 1);
					if (bDuckingEnemy)
					{
						Target.Z -= 20;
					}
					if (cEng.IsVisible(pLocalEntity, Player, Target))
					{
						Aimbot(pLocalEntity, targetEntity);
					}
				}
			}
			else
			{
				GotEntity = false;
				targetEntity = NULL;
				targetDistance = aimbotFOV;
				view = {0,0,0};
			}

			

			for (int i = 0; i <= pClientEntList->GetHighestEntityIndex(); i++)
			{
				IClientEntity* pEntity = pClientEntList->GetClientEntity(i);
				if (pEntity == NULL || !pEntity)
					continue;
				
				if (pEntity->Index() == pLocalEntity->Index())
					continue;

				if (!ffa)
				{

					if (pEntity->GetTeamNum() == pLocalEntity->GetTeamNum())
					{
						if (friendlyBoneESP)
						{
							Drawing::DrawBoneESP(Color(255, 255, 255, 255), pEntity, pLocalEntity);
						}
						continue;
					}
				}
				
				if (pEntity->IsDormant())
					continue;
				
				if (!cEng.IsAlive(pEntity))
					continue;

				if (cEng.GetHealth(pEntity) < 1)
					continue;
				
				if (boxESP && visuals)
					Drawing::DrawESP(i, pLocalEntity, pEntity);

				if (boneESP && visuals)
					Drawing::DrawBoneESP(Color(255, 0, 0, 255), pEntity, pLocalEntity);

				//pDebugOverlay->Line((const Vector&)pEntity->GetOrigin(), (const Vector&)(pEntity->GetOrigin() + Vector(0, 0, 64)), 255, 50, 50, true, 10.0f);
				//pDebugOverlay->Line((const Vector&)pEntity->GetOrigin(), (const Vector&)(pEntity->GetOrigin() + Vector(10, 0, 64)), 50, 255, 50, false, 10.0f);

				if (triggerbot)
				{

					if (pLocalEntity->CrosshairID() == pEntity->Index() && clock() - t > triggerbotD)
					{
						t = clock();
						if (shootOnZoom)
						{
							if ((pLocalEntity->GetFovLevel() <= 90 && pLocalEntity->GetFovLevel() >= 74) || pLocalEntity->GetFovLevel() == 0){}
							else
							{
								mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
								mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
							}
						}
						else
						{
							mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
							mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						}
						
					}
				}

				
			}
		}
		else
		{
			AimEntity = NULL;
			TempAimDistance = 100000;
		}
	}
	else
	{
		AimEntity = NULL;
		TempAimDistance = 100000;
	}
}
