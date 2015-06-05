#pragma once

#ifndef __GAME_CLASSES_H__
#define __GAME_CLASSES_H__

#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

#define INVALID_EHANDLE_INDEX	0xFFFFFFFF

#define	MAX_EDICT_BITS				11
#define NUM_ENT_ENTRY_BITS		(MAX_EDICT_BITS + 2)
#define NUM_ENT_ENTRIES			(1 << NUM_ENT_ENTRY_BITS)
#define NUM_SERIAL_NUM_BITS		16 
#define NUM_SERIAL_NUM_SHIFT_BITS (32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK			(( 1 << NUM_SERIAL_NUM_BITS) - 1)

//#include "Drawing.h"

class IBaseClientDLL;
class IVEngineClient;
class IPanel;
class IClientEntityList;
class ISurface;
class IVModelInfo;
class CGlobalVarsBase;
class IEngineTrace;
class CInput;
class IMaterialSystem;
class IVModelRender;
class IGameMovement; 
class IPrediction;
class IVEfx;
class HFont;
class IVPhysicsDebugOverlay;
class RecvTable;
class RecvProp;
class CBaseHandle;
class IClientNetworkable;
class IClientEntity;
class IClientThinkable;
class IClientUnknown;
class IClientNetworkable;
class CBaseEntity;
class CBaseEntityList;
//struct cache_user_t;


typedef void*	(*CreateClientClassFn)( int entnum, int serialNum );
typedef void*	(*CreateEventFn)( );

template< typename Function > Function call_vfunc( PVOID Base, DWORD Index )
{
    PDWORD* VTablePointer = ( PDWORD* )Base;
    PDWORD VTableFunctionBase = *VTablePointer;
    DWORD dwAddress = VTableFunctionBase[ Index ];
    return ( Function )( dwAddress );
}

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC			= 0x001,
	FONTFLAG_UNDERLINE		= 0x002,
	FONTFLAG_STRIKEOUT		= 0x004,
	FONTFLAG_SYMBOL			= 0x008,
	FONTFLAG_ANTIALIAS		= 0x010,
	FONTFLAG_GAUSSIANBLUR	= 0x020,
	FONTFLAG_ROTARY			= 0x040,
	FONTFLAG_DROPSHADOW		= 0x080,
	FONTFLAG_ADDITIVE		= 0x100,
	FONTFLAG_OUTLINE		= 0x200,
	FONTFLAG_CUSTOM			= 0x400,
	FONTFLAG_BITMAP			= 0x800,
};

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_VAR_VERTEXFOG = (1 << 31),
};

enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

class Color
{
public:
	// constructors
	Color()
	{
		*((int *)this) = 0;
	}
	Color(int r,int g,int b)
	{
		SetColor(r, g, b, 0);
	}
	Color(int r,int g,int b,int a)
	{
		SetColor(r, g, b, a);
	}
	void SetColor(int r, int g, int b, int a = 0)
	{
		_color[0] = (unsigned char)r;
		_color[1] = (unsigned char)g;
		_color[2] = (unsigned char)b;
		_color[3] = (unsigned char)a;
	}

	void GetColor(int &r, int &g, int &b, int &a) const
	{
		r = _color[0];
		g = _color[1];
		b = _color[2];
		a = _color[3];
	}

	void SetRawColor( int color32 )
	{
		*((int *)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int *)this);
	}

	inline int r() const	{ return _color[0]; }
	inline int g() const	{ return _color[1]; }
	inline int b() const	{ return _color[2]; }
	inline int a() const	{ return _color[3]; }
	
	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color &rhs) const
	{
		return ( *((int *)this) == *((int *)&rhs) );
	}

	bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color &operator=( const Color &rhs )
	{
		SetRawColor( rhs.GetRawColor() );
		return *this;
	}

private:
	unsigned char _color[4];
};

class player_info_t
{
public:
	char unknown0[0x10];
	char name[32];
	char unknown2[0x200];
};

class CGlobalVarsBase
{
public:
	CGlobalVarsBase( bool bIsClient );
	bool IsClient() const;
	int GetNetworkBase( int nTick, int nEntity );
	float            realtime; //correct
	int               framecount; //correct
	float            absoluteframetime;
	float            frametime;
	float            curtime; //correct
	float            interval_per_tick;
	int               maxClients; //correct
	int               tickcount; //correct
	float            interpolation_amount;
	float            unknown;
	int               simTicksThisFrame;
	int               network_protocol;
	void             *pSaveData;
	bool            m_bClient;
	int               nTimestampNetworkingBase;
	int               nTimestampRandomizeWindow;
};

class IBaseClientDLL
{
public:
	void CreateMove( int sequence_number, float frametime, bool active )
	{
		typedef void ( __thiscall* oMove )( PVOID,int,float,bool );
		return call_vfunc< oMove >( this, 22 )( this, sequence_number, frametime, active );
	}

	void HudText(char const* text)
	{
		typedef void(__thiscall* oHudText)(PVOID, char const*);
		return call_vfunc< oHudText >(this, 14)(this, text);
	}
};

class IVEngineClient
{
public:
	void GetScreenSize( int& width, int& height )
	{
		typedef void ( __thiscall* oGetScreenSize )( PVOID, int&, int& );
		return call_vfunc< oGetScreenSize >( this, 5 )( this, width, height );
	}
	bool GetPlayerInfo(int ent_num, player_info_t *pinfo)
	{
		typedef bool ( __thiscall* oGetPlayerInfo )( PVOID, int, player_info_t* );
		return call_vfunc< oGetPlayerInfo >( this, 8 )( this, ent_num, pinfo );
	}
	int GetLocalPlayer(  )
	{
		typedef int ( __thiscall* oLocal )( PVOID );
		return call_vfunc< oLocal >( this, 12 )( this );
	}
	float Time(  )
	{
		typedef float ( __thiscall* oTime )( PVOID );
		return call_vfunc< oTime >( this, 14 )( this );
	}
	void GetViewAngles(Vector& vAngles)
	{
		typedef void(__thiscall* oGetViewAngles)(PVOID, Vector&);
		return call_vfunc< oGetViewAngles >(this, 18)(this, vAngles);
	}
	void SetViewAngles(Vector& vAngles)
	{
		typedef void ( __thiscall* oSetViewAngles )( PVOID, Vector& );
		return call_vfunc< oSetViewAngles >( this, 19 )( this, vAngles );
	}
	int GetMaxClients( )
	{
		typedef bool ( __thiscall* oGetMaxClients )( PVOID );
		return call_vfunc< oGetMaxClients >( this, 20 )( this );
	}
	bool IsConnected(  )
	{
		typedef bool ( __thiscall* oGetScreenSize )( PVOID );
		return call_vfunc< oGetScreenSize >( this, 27 )( this );
	}
	bool IsInGame(  )
	{
		typedef bool ( __thiscall* oLocal )( PVOID );
		return call_vfunc< oLocal >( this, 26 )( this );
	}
	const D3DXMATRIX& WorldToScreenMatrix( )
	{
		typedef const D3DXMATRIX& ( __thiscall* oWorldToScreenMatrix )( PVOID );
		return call_vfunc< oWorldToScreenMatrix >( this, 37 )( this );
	}
};

class C_BaseCombatWeapon
{
public:
	int GetAmmo()
	{
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x15C0);
	}

	PCHAR GetName()
	{
		typedef PCHAR(__thiscall* _GetName)(PVOID);
		return call_vfunc<_GetName>(this, 371)(this);
	}

	PCHAR GetPrintName()
	{
		typedef PCHAR(__thiscall* _GetPrintName)(PVOID);
		return call_vfunc<_GetPrintName>(this, 372)(this);
	}
};

class ISurface
{
public:
	void DrawSetColor(Color col)
	{
		typedef void ( __thiscall* oDrawSetColor )( PVOID, Color );
		return call_vfunc< oDrawSetColor >( this, 14 )( this, col );
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void ( __thiscall* oDrawSetColor )( PVOID, int , int , int , int  );
		return call_vfunc< oDrawSetColor >( this, 16 )( this, x0,  y0,  x1,  y1 );
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void ( __thiscall* oDrawOutlinedRect )( PVOID, int , int , int , int  );
		return call_vfunc< oDrawOutlinedRect >( this, 18 )( this, x0,  y0,  x1,  y1 );
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void ( __thiscall* oDrawLine )( PVOID, int , int , int , int  );
		return call_vfunc< oDrawLine >( this, 19 )( this, x0,  y0,  x1,  y1 );
	}
	void DrawSetTextFont(unsigned long hFont)
	{
		typedef void ( __thiscall* oDrawSetTextFont )( PVOID, unsigned long  );
		return call_vfunc< oDrawSetTextFont >( this, 23 )( this, hFont );
	}
	void DrawSetTextColor(Color col)
	{
		typedef void ( __thiscall* oDrawSetTextColor )( PVOID, Color  );
		return call_vfunc< oDrawSetTextColor >( this, 24 )( this, col );
	}
	void DrawSetTextPos(int x,int y)
	{
		typedef void ( __thiscall* oDrawSetTextPos )( PVOID, int ,int  );
		return call_vfunc< oDrawSetTextPos >( this, 26 )( this,  x, y );
	}
	void DrawPrintText(wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
	{
		typedef void ( __thiscall* oDrawPrintText )( PVOID, wchar_t*, int, FontDrawType_t  );
		return call_vfunc< oDrawPrintText >( this, 28 )( this,  text, textLen, drawType );
	}
	unsigned long FontCreate()
	{
		typedef unsigned long ( __thiscall* oCreateFont )( PVOID );
		return call_vfunc< oCreateFont >( this, 71 )( this );
	}
	bool SetFontGlyphSet(unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0)
	{
		typedef bool( __thiscall* oCreateFont )( PVOID, unsigned long , const char *, int , int , int , int , int, int, int );
		return call_vfunc< oCreateFont >( this, 72 )( this, font, windowsFontName,  tall,  weight,  blur,  scanlines, flags, nRangeMin, nRangeMax );
	}
	bool GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall )
	{
		typedef bool( __thiscall* oGetTextSize )( PVOID, unsigned long, const wchar_t*, int&, int& );
		return call_vfunc< oGetTextSize >( this, 79 )( this, font, text, wide, tall );
	}
	void DrawCircle(int x, int y, float radius, int r, int g, int b, int a)
	{
		typedef void(__thiscall* oDrawCircle)(PVOID, int, int, float, int, int, int, int);
		return call_vfunc< oDrawCircle >(this, 162)(this, x, y, radius, r, g, b, a);
	}
};//Size=0x0004(4)


struct model_t
{
	char        name[255];

	// UNDONE: Choose between these two methods
	//int            registration_sequence;
	int            needload;        // mark loaded/not loaded

	modtype_t    type;
	int            flags;

	//
	// volume occupied by the model graphics
	//        
	Vector mins, maxs;
	float        radius;

	int            extradatasize;
};

class IClientRenderable
{
public:
	virtual void*					GetIClientUnknown() = 0;
	virtual Vector const&			GetRenderOrigin( void ) = 0;
	virtual Vector const&			GetRenderAngles( void ) = 0;
	virtual bool					ShouldDraw( void ) = 0;
	virtual bool					IsTransparent( void ) = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;
	virtual void					GetShadowHandle() const = 0;
	virtual void*					RenderHandle() = 0;
	virtual model_t*				GetModel( ) const = 0;
	virtual int						DrawModel( int flags ) = 0;
	virtual int						GetBody() = 0;
	virtual void					ComputeFxBlend( ) = 0;

	bool SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
	{
		typedef bool( __thiscall* oSetupBones )( PVOID, matrix3x4_t*, int, int, float );
		return call_vfunc< oSetupBones>( this, 13 )( this,pBoneToWorldOut, nMaxBones, boneMask, currentTime );
	}
};

class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;	
	virtual void			Release() = 0;	
	virtual void*			GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void			NotifyShouldTransmit( /* ShouldTransmitState_t state*/ ) = 0;
	virtual void			OnPreDataChanged( /*DataUpdateType_t updateType*/ ) = 0;
	virtual void			OnDataChanged( /*DataUpdateType_t updateType*/ ) = 0;
	virtual void			PreDataUpdate( /*DataUpdateType_t updateType*/ ) = 0;
	virtual void			PostDataUpdate( /*DataUpdateType_t updateType*/ ) = 0;	
	virtual void			unknown();
	virtual bool			IsDormant( void ) = 0;
	virtual int				Index( void ) const = 0;	
	virtual void			ReceiveMessage( int classID /*, bf_read &msg*/ ) = 0;	
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities( void ) = 0;
};

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle &handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class CEntInfo
{
public:
	IHandleEntity	*m_pEntity;
	int				m_SerialNumber;
	CEntInfo		*m_pPrev;
	CEntInfo		*m_pNext;
	std::string		m_iName;
	std::string		m_iClassName;

	void			ClearLinks();
};

class CBaseHandle
{
	friend class CBaseEntityList;

public:

	CBaseHandle();
	//CBaseHandle(const CBaseHandle &other);
	CBaseHandle(unsigned long value);
	CBaseHandle(int iEntry, int iSerialNumber);

	void Init(int iEntry, int iSerialNumber);
	void Term();

	bool IsValid() const;

	int GetEntryIndex() const;
	int GetSerialNumber() const;

	int ToInt() const;
	bool operator !=(const CBaseHandle &other) const;
	bool operator ==(const CBaseHandle &other) const;
	bool operator ==(const IHandleEntity* pEnt) const;
	bool operator !=(const IHandleEntity* pEnt) const;
	bool operator <(const CBaseHandle &other) const;
	bool operator <(const IHandleEntity* pEnt) const;

	// Assign a value to the handle.
	const CBaseHandle& operator=(const IHandleEntity *pEntity);
	const CBaseHandle& Set(const IHandleEntity *pEntity);

	// Use this to dereference the handle.
	// Note: this is implemented in game code (ehandle.h)
	IHandleEntity* Get() const;


protected:
	// The low NUM_SERIAL_BITS hold the index. If this value is less than MAX_EDICTS, then the entity is networkable.
	// The high NUM_SERIAL_NUM_BITS bits are the serial number.
	unsigned long	m_Index;
};

class CBaseEntityList
{
public:
	CBaseEntityList();
	~CBaseEntityList();

	// Add and remove entities. iForcedSerialNum should only be used on the client. The server
	// gets to dictate what the networkable serial numbers are on the client so it can send
	// ehandles over and they work.
	CBaseHandle AddNetworkableEntity(IHandleEntity *pEnt, int index, int iForcedSerialNum = -1);
	CBaseHandle AddNonNetworkableEntity(IHandleEntity *pEnt);
	void RemoveEntity(CBaseHandle handle);

	// Get an ehandle from a networkable entity's index (note: if there is no entity in that slot,
	// then the ehandle will be invalid and produce NULL).
	CBaseHandle GetNetworkableHandle(int iEntity) const;

	// ehandles use this in their Get() function to produce a pointer to the entity.
	IHandleEntity* LookupEntity(const CBaseHandle &handle) const;
	IHandleEntity* LookupEntityByNetworkIndex(int edictIndex) const;

	// Use these to iterate over all the entities.
	CBaseHandle FirstHandle() const;
	CBaseHandle NextHandle(CBaseHandle hEnt) const;
	static CBaseHandle InvalidHandle();

	const CEntInfo *FirstEntInfo() const;
	const CEntInfo *NextEntInfo(const CEntInfo *pInfo) const;
	const CEntInfo *GetEntInfoPtr(const CBaseHandle &hEnt) const;
	const CEntInfo *GetEntInfoPtrByIndex(int index) const;

	// Used by Foundry when an entity is respawned/edited.
	// We force the new entity's ehandle to be the same so anyone pointing at it still gets a valid CBaseEntity out of their ehandle.
	void ForceEntSerialNumber(int iEntIndex, int iSerialNumber);

	// Overridables.
protected:

	// These are notifications to the derived class. It can cache info here if it wants.
	virtual void OnAddEntity(IHandleEntity *pEnt, CBaseHandle handle);

	// It is safe to delete the entity here. We won't be accessing the pointer after
	// calling OnRemoveEntity.
	virtual void OnRemoveEntity(IHandleEntity *pEnt, CBaseHandle handle);


private:

	CBaseHandle AddEntityAtSlot(IHandleEntity *pEnt, int iSlot, int iForcedSerialNum);
	void RemoveEntityAtSlot(int iSlot);


private:

	class CEntInfoList
	{
	public:
		CEntInfoList();

		const CEntInfo	*Head() const { return m_pHead; }
		const CEntInfo	*Tail() const { return m_pTail; }
		CEntInfo		*Head() { return m_pHead; }
		CEntInfo		*Tail() { return m_pTail; }
		void			AddToHead(CEntInfo *pElement) { LinkAfter(NULL, pElement); }
		void			AddToTail(CEntInfo *pElement) { LinkBefore(NULL, pElement); }

		void LinkBefore(CEntInfo *pBefore, CEntInfo *pElement);
		void LinkAfter(CEntInfo *pBefore, CEntInfo *pElement);
		void Unlink(CEntInfo *pElement);
		bool IsInList(CEntInfo *pElement);

	private:
		CEntInfo		*m_pHead;
		CEntInfo		*m_pTail;
	};

	int GetEntInfoIndex(const CEntInfo *pEntInfo) const;


	// The first MAX_EDICTS entities are networkable. The rest are client-only or server-only.
	CEntInfo m_EntPtrArray[NUM_ENT_ENTRIES];
	CEntInfoList	m_activeList;
	CEntInfoList	m_freeNonNetworkableList;
};

inline bool CBaseHandle::IsValid() const
{
	return m_Index != INVALID_EHANDLE_INDEX;
}

inline int CBaseHandle::GetEntryIndex() const
{
	if (!IsValid())
		return NUM_ENT_ENTRIES - 1;
	return m_Index & ENT_ENTRY_MASK;
}

inline int CBaseHandle::GetSerialNumber() const
{
	return m_Index >> NUM_SERIAL_NUM_SHIFT_BITS;
}

inline IHandleEntity* CBaseEntityList::LookupEntity(const CBaseHandle &handle) const
{
	if (handle.m_Index == INVALID_EHANDLE_INDEX)
		return NULL;

	const CEntInfo *pInfo = &m_EntPtrArray[handle.GetEntryIndex()];
	if (pInfo->m_SerialNumber == handle.GetSerialNumber())
		return (IHandleEntity*)pInfo->m_pEntity;
	else
		return NULL;
}

inline IHandleEntity* CBaseHandle::Get() const
{
	extern CBaseEntityList *g_pEntityList;
	return g_pEntityList->LookupEntity(*this);
}

template< class T >
class CHandle : public CBaseHandle
{
public:

	CHandle();
	CHandle(int iEntry, int iSerialNumber);
	CHandle(const CBaseHandle &handle);
	CHandle(T *pVal);

	// The index should have come from a call to ToInt(). If it hasn't, you're in trouble.
	static CHandle<T> FromIndex(int index);

	T*		Get() const;
	void	Set(const T* pVal);

	operator T*();
	operator T*() const;

	bool	operator !() const;
	bool	operator==(T *val) const;
	bool	operator!=(T *val) const;
	const CBaseHandle& operator=(const T *val);

	T*		operator->() const;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual void*		GetCollideable() = 0;
	virtual IClientNetworkable*	GetClientNetworkable() = 0;
	virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual IClientEntity*		GetIClientEntity() = 0;
	virtual IClientEntity*		GetBaseEntity() = 0;
	virtual IClientThinkable*	GetClientThinkable() = 0;
};

class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle( void* hThink ) = 0;
	virtual void				Release() = 0;
};

class IClientEntityList
{
public:
	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual IClientEntity *		GetClientEntity(int entnum);
	virtual IClientEntity *		GetClientEntityFromHandle(HANDLE hEnt) = 0;
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int					GetHighestEntityIndex(void);
	virtual void				SetMaxEntities(int maxents);
	virtual int					GetMaxEntities();
};



class __declspec ( novtable )IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	public:
		virtual void			Release( void ) = 0;
		//virtual void			blahblahpad( void ) = 0;
		virtual Vector&			GetAbsOrigin( void ) const = 0;//in broken place use GetOrigin Below
		virtual const Vector&	GetAbsAngles( void ) const = 0;

		Vector GetOrigin()	
		{ 
			return *reinterpret_cast< Vector* >( ( DWORD)this + ( DWORD )0x134 );
		}
		int GetTeamNum( )		
		{ 
			return *reinterpret_cast< int* >( ( DWORD)this + ( DWORD )0xF0 );
		}
		int GetFlags()		
		{ 
			return *reinterpret_cast< int* >( ( DWORD)this + ( DWORD )0x100 );
		}
		int CrosshairID()
		{
			return *reinterpret_cast< int* >((DWORD)this + (DWORD)0x2400);
		}
		Vector GetPunchAngles()
		{
			return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x13E8);
		}
		void SetVisualPunch()
		{
			Vector VisualPunch;
			*reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x13E8) = VisualPunch;
		}
		int GetFovLevel()
		{
			return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x159C);
		}
		int GetHealth()
		{
			return *reinterpret_cast<int*>((DWORD)this + (DWORD)0xFC);
		}
		C_BaseCombatWeapon* GetActiveWeapon(IClientEntityList* pClientEntityList)
		{
			HANDLE hActiveWeapon = *(PHANDLE)((PUCHAR)this + 0x12C0);
			if (!hActiveWeapon)
				return NULL;

			return (C_BaseCombatWeapon*)pClientEntityList->GetClientEntityFromHandle(hActiveWeapon);
		}
		bool isHoldingThrowable(IClientEntityList* pClientList)
		{
			PCHAR pszWeapon = NULL;
			C_BaseCombatWeapon * pActiveCombatWeapon = this->GetActiveWeapon(pClientList);
			if (pActiveCombatWeapon)
			{
				pszWeapon = pActiveCombatWeapon->GetName();
				
			}
			std::stringstream weaponStream;
			weaponStream << pszWeapon;
			if (weaponStream.str() == "weapon_molotov" ||
				weaponStream.str() == "weapon_incgrenade" ||
				weaponStream.str() == "weapon_decoy" ||
				weaponStream.str() == "weapon_flashbang" ||
				weaponStream.str() == "weapon_hegrenade" ||
				weaponStream.str() == "weapon_smokegrenade" ||
				weaponStream.str() == "weapon_c4")
			{
				return true;
			}
			return false;
		}
		int ammoLeft(IClientEntityList* pClientList)
		{
			C_BaseCombatWeapon * pActiveCombatWeapon = this->GetActiveWeapon(pClientList);
			if (pActiveCombatWeapon)
			{
				return pActiveCombatWeapon->GetAmmo();
			}
			return 1;
		}

};



class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char *( __thiscall* oGetName )( PVOID, unsigned int );
		return call_vfunc< oGetName >( this, 36 )( this, vguiPanel );
	}
	void PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce = true)
	{
		typedef void( __thiscall* oPaintTraverse )( PVOID, unsigned int, bool, bool );
		return call_vfunc< oPaintTraverse >( this, 41 )( this, vguiPanel, forceRepaint, allowForce );
	}
};

struct cplane_t
{
	Vector	normal;
	float	dist;
	byte	type;			// for fast side tests
	byte	signbits;		// signx + (signy<<1) + (signz<<1)
	byte	pad[2];

#ifdef VECTOR_NO_SLOW_OPERATIONS
	cplane_t() {}

private:
	// No copy constructors allowed if we're in optimal mode
	cplane_t(const cplane_t& vOther);
#endif
};

struct csurface_t
{
	const char	*name;
	short		surfaceProps;
	unsigned short	flags;		// BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

class CBaseTrace
{
public:

	// Displacement flags tests.
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	// these members are aligned!!
	Vector    startpos;    // start position
	Vector    endpos;    // final position
	cplane_t    plane;    // surface normal at impact

	float    fraction;    // time completed, 1.0 = didn't hit anything

	int    contents;    // contents on other side of surface hit
	unsigned short    dispFlags;    // displacement flags for marking surfaces with data

	bool    allsolid;    // if true, plane is not valid
	bool    startsolid;    // if true, the initial point was in a solid area

	CBaseTrace() {}

private:
	// No copy constructors allowed
	CBaseTrace(const CBaseTrace& vOther);
};

class CGameTrace : public CBaseTrace
{
public:

	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int GetEntityIndex() const;
	bool DidHit() const;

#if defined( ENGINE_DLL )
	void SetEdict(edict_t *pEdict);
	edict_t* GetEdict() const;
#endif  

public:

	float		fractionleftsolid;		
	csurface_t	surface;				

	int			hitgroup;				
	short		physicsbone;			

	IClientEntity *m_pEnt;

	int			hitbox;

	CGameTrace() {}

private:
	// No copy constructors allowed
	CGameTrace(const CGameTrace& vOther);
};

inline bool CGameTrace::DidHit() const
{
	return fraction < 1 || allsolid || startsolid;
}

//typedef CGameTrace trace_t;

struct trace_t
{
	Vector	start;
	Vector	endpos;
	cplane_t	plane;
	float		fraction;
	int			contents;
	WORD		dispFlags;
	bool		allSolid;
	bool		startSolid;
	float		fractionLeftSolid;
	csurface_t	surface;
	int			hitgroup;
	short		physicsBone;
	WORD		m_nWorldSurfaceIndex;
	IClientEntity* m_pEnt;
	int			hitbox;
};

struct Ray_t
{
	VectorAligned  m_Start;    // starting point, centered within the extents
	VectorAligned  m_Delta;    // direction + length of the ray
	VectorAligned  m_StartOffset;    // Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;    // Describes an axis aligned box extruded along a ray
	const matrix3x4_t *m_pWorldAxisTransform;
	bool    m_IsRay;    // are the extents zero?
	bool    m_IsSwept;    // is delta != 0?

	Ray_t() : m_pWorldAxisTransform(NULL)    {}

	void Init(Vector const& start, Vector const& end)
	{
		//Assert( &end );
		//m_Delta = end - start;
		m_Delta.X = end.X - start.X;
		m_Delta.Y = end.Y - start.Y;
		m_Delta.Z = end.Z - start.Z;
		
		m_IsSwept = ((m_Delta.X*m_Delta.X + m_Delta.Y*m_Delta.Y + m_Delta.Z*m_Delta.Z) != 0);

		m_Extents.X = m_Extents.Y = m_Extents.Z = 0;
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		m_StartOffset.X = m_StartOffset.Y = m_StartOffset.Z = 0;
		m_Start.X = start.X;
		m_Start.Y = start.Y;
		m_Start.Z = start.Z;
	}


private:
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter
{
public:
	virtual bool			ShouldHitEntity(IClientEntity* pEntity, int mask) = 0;
	virtual TraceType_t            GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	IClientEntity* pSkip;

	bool ShouldHitEntity(IClientEntity* pEntity, int mask)
	{
		return !(pEntity == pSkip);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

class IEngineTrace
{
public:
	virtual int		function1() = 0;
	virtual int		function2() = 0;
	virtual int		function3() = 0;
	virtual void	function4() = 0;
	virtual void	function5() = 0;
	virtual void	TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace) = 0;
};

class IClientMode
{
	// Misc.
public:

	virtual			~IClientMode() {}

	// Called before the HUD is initialized.
	virtual void	InitViewport() = 0;

	// One time init when .dll is first loaded.
	virtual void	Init() = 0;

	// Called when vgui is shutting down.
	virtual void	VGui_Shutdown() = 0;

	// One time call when dll is shutting down
	virtual void	Shutdown() = 0;

	// Called when switching from one IClientMode to another.
	// This can re-layout the view and such.
	// Note that Enable and Disable are called when the DLL initializes and shuts down.
	virtual void	Enable() = 0;

	// Called when it's about to go into another client mode.
	virtual void	Disable() = 0;

	// Called when initializing or when the view changes.
	// This should move the viewport into the correct position.
	virtual void	Layout() = 0;

	// Gets at the viewport, if there is one...
	virtual void	function1() = 0;

	// Gets at the viewports vgui panel animation controller, if there is one...
	virtual void	function2() = 0;

	// called every time shared client dll/engine data gets changed,
	// and gives the cdll a chance to modify the data.
	virtual void	ProcessInput(bool bActive) = 0;

	// The mode can choose to draw/not draw entities.
	virtual bool	ShouldDrawDetailObjects() = 0;
	virtual bool	function3() = 0;
	virtual bool	function4() = 0;
	virtual bool	ShouldDrawParticles() = 0;

	// The mode can choose to not draw fog
	virtual bool	ShouldDrawFog(void) = 0;

	virtual void	function5() = 0;
	virtual int		function6() = 0;
	virtual void	StartMessageMode(int iMessageModeType) = 0;
	virtual void	function7() = 0;
	virtual void	OverrideMouseInput(float *x, float *y) = 0;
	virtual bool	function8() = 0;

	virtual void	LevelInit(const char *newmap) = 0;
	virtual void	LevelShutdown(void) = 0;

	// Certain modes hide the view model
	virtual bool	ShouldDrawViewModel(void) = 0;
	virtual bool	ShouldDrawCrosshair(void) = 0;

	// Let mode override viewport for engine
	virtual void	AdjustEngineViewport(int& x, int& y, int& width, int& height) = 0;

	// Called before rendering a view.
	virtual void	function9() = 0;

	// Called after everything is rendered.
	virtual void	PostRender(void) = 0;

	virtual void	PostRenderVGui() = 0;

	virtual void	function10() = 0;
	virtual void	DeactivateInGameVGuiContext() = 0;
	virtual float	GetViewModelFOV(void) = 0;

	virtual bool	CanRecordDemo(char *errorMsg, int length) const = 0;

	virtual void	function11() = 0;

	//=============================================================================
	// HPE_BEGIN:
	// [menglish] Save server information shown to the client in a persistent place
	//=============================================================================

	virtual wchar_t* GetServerName() = 0;
	virtual void SetServerName(wchar_t* name) = 0;
	virtual wchar_t* GetMapName() = 0;
	virtual void SetMapName(wchar_t* name) = 0;

	//=============================================================================
	// HPE_END
	//=============================================================================

	virtual bool	function12() = 0;

	virtual void	DisplayReplayMessage(const char *pLocalizeName, float flDuration, bool bUrgent,
		const char *pSound, bool bDlg) = 0;

	// Updates.
public:

	// Called every frame.
	virtual void	Update() = 0;

	// Returns true if VR mode should black out everything around the UI
	virtual bool	ShouldBlackoutAroundHUD() = 0;

	// Returns true if VR mode should black out everything around the UI
	virtual void	function13() = 0;

	virtual bool	IsInfoPanelAllowed() = 0;
	virtual void	InfoPanelDisplayed() = 0;
	virtual bool	IsHTMLInfoPanelAllowed() = 0;
};

enum MaterialPropertyTypes_t
{
	MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0,					// bool
	MATERIAL_PROPERTY_OPACITY,								// int (enum MaterialPropertyOpacityTypes_t)
	MATERIAL_PROPERTY_REFLECTIVITY,							// vec3_t
	MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS				// bool
};

class IMaterial
{
public:
	// Get the name of the material.  This is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char *	GetName() const = 0;
	virtual const char *	GetTextureGroupName() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual void/*PreviewImageRetVal_t*/ GetPreviewImageProperties(int *width, int *height,
		DWORD/*ImageFormat*/ *imageFormat, bool* isTranslucent) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual void/*PreviewImageRetVal_t*/ GetPreviewImage(unsigned char *data,
		int width, int height,
		DWORD/*ImageFormat*/ imageFormat) const = 0;
	// 
	virtual int				GetMappingWidth() = 0;
	virtual int				GetMappingHeight() = 0;

	virtual int				GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool			InMaterialPage(void) = 0;
	virtual	void			GetMaterialOffset(float *pOffset) = 0;
	virtual void			GetMaterialScale(float *pScale) = 0;
	virtual IMaterial		*GetMaterialPage(void) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual void/*IMaterialVar*/ *	FindVar(const char *varName, bool *found, bool complain = true) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to 
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void			IncrementReferenceCount(void) = 0;
	virtual void			DecrementReferenceCount(void) = 0;

	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int 			GetEnumerationID(void) const = 0;

	virtual void			GetLowResColorSample(float s, float t, float *color) const = 0;

	// This computes the state snapshots for this material
	virtual void			RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool			IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool			IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool			IsVertexLit() = 0;

	// Gets the vertex format
	virtual void	GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool			HasProxy(void) const = 0;

	virtual bool			UsesEnvCubemap(void) = 0;

	virtual bool			NeedsTangentSpace(void) = 0;

	virtual bool			NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool			NeedsSoftwareSkinning(void) = 0;

	// Apply constant color or alpha modulation
	virtual void			AlphaModulate(float alpha) = 0;
	virtual void			ColorModulate(float r, float g, float b) = 0;

	// Material Var flags...
	virtual void			SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool			GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;

	// Gets material reflectivity
	virtual void			GetReflectivity(Vector& reflect) = 0;

	// Gets material property flags
	virtual bool			GetPropertyFlag(MaterialPropertyTypes_t type) = 0;

	// Is the material visible from both sides?
	virtual bool			IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void			SetShader(const char *pShaderName) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetNumPasses(void) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetTextureMemoryBytes(void) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void			Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool			NeedsLightmapBlendAlpha(void) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool			NeedsSoftwareLighting(void) = 0;

	// Gets at the shader parameters
	virtual int				ShaderParamCount() const = 0;
	virtual void/*IMaterialVar*/	**GetShaderParams(void) = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool			IsErrorMaterial() const = 0;

	virtual void			Unused() = 0;

	// Gets the current alpha modulation
	virtual float			GetAlphaModulation() = 0;
	virtual void			GetColorModulation(float *r, float *g, float *b) = 0;

	// Is this translucent given a particular alpha modulation?
	virtual bool			IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual void/*IMaterialVar*/ *	FindVarFast(char const *pVarName, unsigned int *pToken) = 0;

	// Sets new VMT shader parameters for the material
	virtual void			SetShaderAndParams(/*KeyValues *pKeyValues*/) = 0;
	virtual const char *	GetShaderName() const = 0;

	virtual void			DeleteIfUnreferenced() = 0;

	virtual bool			IsSpriteCard() = 0;

	virtual void			CallBindProxy(void *proxyData) = 0;

	virtual void			RefreshPreservingMaterialVars() = 0;

	virtual bool			WasReloadedFromWhitelist() = 0;

	virtual bool			SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;

	virtual int				GetReferenceCount() const = 0;
};

class IMaterialSystem
{
public:
	IMaterial *FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial*(__thiscall* oFindMaterial)(PVOID, char const*, const char*, bool, const char*);
		return call_vfunc< oFindMaterial>(this, 83)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
};

class NDebugOverlay
{
public:
	void Line(const Vector &origin, const Vector &target, int r, int g, int b, bool noDepthTest, float flDuration)
	{
		typedef void(__thiscall* oLine)(PVOID, const Vector, const Vector, int, int, int, bool, float);
		return call_vfunc< oLine >(this, 5)(this, origin, target, r, g, b, noDepthTest, flDuration);
	}
};

class IStudioRender
{
public:
	void BeginFrame()
	{
		typedef void(__thiscall* oBeginFrame)(PVOID);
		return call_vfunc< oBeginFrame >(this, 8)(this);
	}
};

extern IMaterialSystem*			pMat				;
extern IBaseClientDLL*			pClient				;
extern IVEngineClient*			pEngine				;
extern IPanel*					pPanel				;
extern IClientEntityList*		pClientEntList		;
extern ISurface*				pSurface			;
extern CGlobalVarsBase*			pGlobals			;
extern IEngineTrace*			pEngineTrace		;
extern unsigned long			espfont				;
extern NDebugOverlay*			pDebugOverlay		;
extern IStudioRender*			pStudioRender		;
#endif