//========= Copyright Valve Corporation, All rights reserved. ============//

//

// Purpose: 

//

// $NoKeywords: $

//===========================================================================//

#include "cbase.h"

#include <crtmemdebug.h>

#include "vgui_int.h"

#include "clientmode.h"

#include "iinput.h"

#include "iviewrender.h"

#include "ivieweffects.h"

#include "ivmodemanager.h"

#include "prediction.h"

#include "clientsideeffects.h"

#include "particlemgr.h"

#include "steam/steam_api.h"

#include "initializer.h"

#include "smoke_fog_overlay.h"

#include "view.h"

#include "ienginevgui.h"

#include "iefx.h"

#include "enginesprite.h"

#include "networkstringtable_clientdll.h"

#include "voice_status.h"

#include "filesystem.h"

#include "c_te_legacytempents.h"

#include "c_rope.h"

#include "engine/ishadowmgr.h"

#include "engine/IStaticPropMgr.h"

#include "hud_basechat.h"

#include "hud_crosshair.h"

#include "view_shared.h"

#include "env_wind_shared.h"

#include "detailobjectsystem.h"

#include "clienteffectprecachesystem.h"

#include "soundenvelope.h"

#include "c_basetempentity.h"

#include "materialsystem/imaterialsystemstub.h"

#include "VGuiMatSurface/IMatSystemSurface.h"

#include "materialsystem/imaterialsystemhardwareconfig.h"

#include "c_soundscape.h"

#include "engine/ivdebugoverlay.h"

#include "vguicenterprint.h"

#include "iviewrender_beams.h"

#include "tier0/vprof.h"

#include "engine/IEngineTrace.h"

#include "engine/ivmodelinfo.h"

#include "physics.h"

#include "usermessages.h"

#include "gamestringpool.h"

#include "c_user_message_register.h"

#include "IGameUIFuncs.h"

#include "saverestoretypes.h"

#include "saverestore.h"

#include "physics_saverestore.h"

#include "igameevents.h"

#include "datacache/idatacache.h"

#include "datacache/imdlcache.h"

#include "kbutton.h"

#include "tier0/icommandline.h"

#include "gamerules_register.h"

#include "vgui_controls/AnimationController.h"

#include "bitmap/tgawriter.h"

#include "c_world.h"

#include "perfvisualbenchmark.h"	

#include "SoundEmitterSystem/isoundemittersystembase.h"

#include "hud_closecaption.h"

#include "colorcorrectionmgr.h"

#include "physpropclientside.h"

#include "panelmetaclassmgr.h"

#include "c_vguiscreen.h"

#include "imessagechars.h"

#include "game/client/IGameClientExports.h"

#include "client_factorylist.h"

#include "ragdoll_shared.h"

#include "rendertexture.h"

#include "view_scene.h"

#include "iclientmode.h"

#include "con_nprint.h"

#include "inputsystem/iinputsystem.h"

#include "appframework/IAppSystemGroup.h"

#include "scenefilecache/ISceneFileCache.h"

#include "tier2/tier2dm.h"

#include "tier3/tier3.h"

#include "ihudlcd.h"

#include "toolframework_client.h"

#include "hltvcamera.h"

#if defined( REPLAY_ENABLED )

#include "replay/replaycamera.h"

#include "replay/replay_ragdoll.h"

#include "qlimits.h"

#include "replay/replay.h"

#include "replay/ireplaysystem.h"

#include "replay/iclientreplay.h"

#include "replay/ienginereplay.h"

#include "replay/ireplaymanager.h"

#include "replay/ireplayscreenshotmanager.h"

#include "replay/iclientreplaycontext.h"

#include "replay/vgui/replayconfirmquitdlg.h"

#include "replay/vgui/replaybrowsermainpanel.h"

#include "replay/vgui/replayinputpanel.h"

#include "replay/vgui/replayperformanceeditor.h"

#endif

#include "vgui/ILocalize.h"

#include "vgui/IVGui.h"

#include "ixboxsystem.h"

#include "ipresence.h"

#include "engine/imatchmaking.h"

#include "cdll_bounded_cvars.h"

#include "matsys_controls/matsyscontrols.h"

#include "gamestats.h"

#include "particle_parse.h"

#if defined( TF_CLIENT_DLL )

#include "rtime.h"

#include "tf_hud_disconnect_prompt.h"

#include "../engine/audio/public/sound.h"

#include "tf_shared_content_manager.h"

#endif

#include "clientsteamcontext.h"

#include "renamed_recvtable_compat.h"

#include "mouthinfo.h"

#include "sourcevr/isourcevirtualreality.h"

#include "client_virtualreality.h"

#include "mumble.h"



// NVNT includes

#include "hud_macros.h"

#include "haptics/ihaptics.h"

#include "haptics/haptic_utils.h"

#include "haptics/haptic_msgs.h"



#if defined( TF_CLIENT_DLL )

#include "abuse_report.h"

#endif



#ifdef USES_ECON_ITEMS

#include "econ_item_system.h"

#endif // USES_ECON_ITEMS



#if defined( TF_CLIENT_DLL )

#include "econ/tool_items/custom_texture_cache.h"

#endif



#ifdef WORKSHOP_IMPORT_ENABLED

#include "fbxsystem/fbxsystem.h"

#endif



extern vgui::IInputInternal *g_InputInternal;



//=============================================================================

// HPE_BEGIN

// [dwenger] Necessary for stats display

//=============================================================================



#include "achievements_and_stats_interface.h"



//=============================================================================

// HPE_END

//=============================================================================





#ifdef PORTAL

#include "PortalRender.h"

#endif



#ifdef SIXENSE

#include "sixense/in_sixense.h"

#endif



#if defined( GAMEPADUI )

#include "../gamepadui/igamepadui.h"

#endif // GAMEPADUI



// memdbgon must be the last include file in a .cpp file!!!

#include "tier0/memdbgon.h"



extern IClientMode *GetClientModeNormal();



// IF YOU ADD AN INTERFACE, EXTERN IT IN THE HEADER FILE.

IVEngineClient	*engine = NULL;

IVModelRender *modelrender = NULL;

IVEfx *effects = NULL;

IVRenderView *render = NULL;

IVDebugOverlay *debugoverlay = NULL;

IMaterialSystemStub *materials_stub = NULL;

IDataCache *datacache = NULL;

IVModelInfoClient *modelinfo = NULL;

IEngineVGui *enginevgui = NULL;

INetworkStringTableContainer *networkstringtable = NULL;

ISpatialPartition* partition = NULL;

IFileSystem *filesystem = NULL;

IShadowMgr *shadowmgr = NULL;

IStaticPropMgrClient *staticpropmgr = NULL;

IEngineSound *enginesound = NULL;

IUniformRandomStream *random = NULL;

static CGaussianRandomStream s_GaussianRandomStream;

CGaussianRandomStream *randomgaussian = &s_GaussianRandomStream;

ISharedGameRules *sharedgamerules = NULL;

IEngineTrace *enginetrace = NULL;

IGameUIFuncs *gameuifuncs = NULL;

IGameEventManager2 *gameeventmanager = NULL;

ISoundEmitterSystemBase *soundemitterbase = NULL;

IInputSystem *inputsystem = NULL;

ISceneFileCache *scenefilecache = NULL;

IXboxSystem *xboxsystem = NULL;	// Xbox 360 only

IMatchmaking *matchmaking = NULL;

IUploadGameStats *gamestatsuploader = NULL;

IClientReplayContext *g_pClientReplayContext = NULL;

#if defined( REPLAY_ENABLED )

IReplayManager *g_pReplayManager = NULL;

IReplayMovieManager *g_pReplayMovieManager = NULL;

IReplayScreenshotManager *g_pReplayScreenshotManager = NULL;

IReplayPerformanceManager *g_pReplayPerformanceManager = NULL;

IReplayPerformanceController *g_pReplayPerformanceController = NULL;

IEngineReplay *g_pEngineReplay = NULL;

IEngineClientReplay *g_pEngineClientReplay = NULL;

IReplaySystem *g_pReplay = NULL;

#endif



#if defined(GAMEPADUI)

IGamepadUI* g_pGamepadUI = nullptr;

#endif // GAMEPADUI



IHaptics* haptics = NULL;// NVNT haptics system interface singleton



//=============================================================================

// HPE_BEGIN

// [dwenger] Necessary for stats display

//=============================================================================



AchievementsAndStatsInterface* g_pAchievementsAndStatsInterface = NULL;



//=============================================================================

// HPE_END

//=============================================================================



IGameSystem *SoundEmitterSystem();

IGameSystem *ToolFrameworkClientSystem();



// Engine player info, no game related infos here

BEGIN_BYTESWAP_DATADESC( player_info_s )

	DEFINE_ARRAY( name, FIELD_CHARACTER, MAX_PLAYER_NAME_LENGTH ),

	DEFINE_FIELD( userID, FIELD_INTEGER ),

	DEFINE_ARRAY( guid, FIELD_CHARACTER, SIGNED_GUID_LEN + 1 ),

	DEFINE_FIELD( friendsID, FIELD_INTEGER ),

	DEFINE_ARRAY( friendsName, FIELD_CHARACTER, MAX_PLAYER_NAME_LENGTH ),

	DEFINE_FIELD( fakeplayer, FIELD_BOOLEAN ),

	DEFINE_FIELD( ishltv, FIELD_BOOLEAN ),

#if defined( REPLAY_ENABLED )

	DEFINE_FIELD( isreplay, FIELD_BOOLEAN ),

#endif

	DEFINE_ARRAY( customFiles, FIELD_INTEGER, MAX_CUSTOM_FILES ),

	DEFINE_FIELD( filesDownloaded, FIELD_INTEGER ),

END_BYTESWAP_DATADESC()



static bool g_bRequestCacheUsedMaterials = false;

void RequestCacheUsedMaterials()

{

	g_bRequestCacheUsedMaterials = true;

}



void ProcessCacheUsedMaterials()

{

	if ( !g_bRequestCacheUsedMaterials )

		return;



	g_bRequestCacheUsedMaterials = false;

	if ( materials )

	{

        materials->CacheUsedMaterials();

	}

}



// String tables

INetworkStringTable *g_pStringTableParticleEffectNames = NULL;

INetworkStringTable *g_StringTableEffectDispatch = NULL;

INetworkStringTable *g_StringTableVguiScreen = NULL;

INetworkStringTable *g_pStringTableMaterials = NULL;

INetworkStringTable *g_pStringTableInfoPanel = NULL;

INetworkStringTable *g_pStringTableClientSideChoreoScenes = NULL;

INetworkStringTable *g_pStringTableServerMapCycle = NULL;



#ifdef TF_CLIENT_DLL

INetworkStringTable *g_pStringTableServerPopFiles = NULL;

INetworkStringTable *g_pStringTableServerMapCycleMvM = NULL;

#endif



static CGlobalVarsBase dummyvars( true );

// So stuff that might reference gpGlobals during DLL initialization won't have a NULL pointer.

// Once the engine calls Init on this DLL, this pointer gets assigned to the shared data in the engine

CGlobalVarsBase *gpGlobals = &dummyvars;

class CHudChat;

class CViewRender;

extern CViewRender g_DefaultViewRender;



extern void StopAllRumbleEffects( void );



static C_BaseEntityClassList *s_pClassLists = NULL;

C_BaseEntityClassList::C_BaseEntityClassList()

{

	m_pNextClassList = s_pClassLists;

	s_pClassLists = this;

}

C_BaseEntityClassList::~C_BaseEntityClassList()

{

}



// Any entities that want an OnDataChanged during simulation register for it here.

class CDataChangedEvent

{

public:

	CDataChangedEvent() {}

	CDataChangedEvent( IClientNetworkable *ent, DataUpdateType_t updateType, int *pStoredEvent )

	{

		m_pEntity = ent;

		m_UpdateType = updateType;

		m_pStoredEvent = pStoredEvent;

	}



	IClientNetworkable	*m_pEntity;

	DataUpdateType_t	m_UpdateType;

	int					*m_pStoredEvent;

};



ISaveRestoreBlockHandler *GetEntitySaveRestoreBlockHandler();

ISaveRestoreBlockHandler *GetViewEffectsRestoreBlockHandler();



CUtlLinkedList<CDataChangedEvent, unsigned short> g_DataChangedEvents;

ClientFrameStage_t g_CurFrameStage = FRAME_UNDEFINED;





class IMoveHelper;



void DispatchHudText( const char *pszName );



static ConVar s_CV_ShowParticleCounts("showparticlecounts", "0", 0, "Display number of particles drawn per frame");

static ConVar s_cl_team("cl_team", "default", FCVAR_USERINFO|FCVAR_ARCHIVE, "Default team when joining a game");

static ConVar s_cl_class("cl_class", "default", FCVAR_USERINFO|FCVAR_ARCHIVE, "Default class when joining a game");



#ifdef HL1MP_CLIENT_DLL

static ConVar s_cl_load_hl1_content("cl_load_hl1_content", "0", FCVAR_ARCHIVE, "Mount the content from Half-Life: Source if possible");

#endif





// Physics system

bool g_bLevelInitialized;

bool g_bTextMode = false;

class IClientPurchaseInterfaceV2 *g_pClientPurchaseInterface = (class IClientPurchaseInterfaceV2 *)(&g_bTextMode + 156);



static ConVar *g_pcv_ThreadMode = NULL;



// GAMEPADUI TODO - put this somewhere better. (Madi)

#if defined( GAMEPADUI )

const bool IsSteamDeck()

{

	if ( CommandLine()->FindParm( "-gamepadui" ) )

		return true;



	if ( CommandLine()->FindParm( "-nogamepadui" ) )

		return false;



	const char *pszSteamDeckEnv = getenv( "SteamDeck" );

	if ( pszSteamDeckEnv && *pszSteamDeckEnv )

		return atoi( pszSteamDeckEnv ) != 0;



	return false;

}

#endif





//-----------------------------------------------------------------------------

// Purpose: interface for gameui to modify voice bans

//-----------------------------------------------------------------------------

class CGameClientExports : public IGameClientExports

{

public:

	// ingame voice manipulation

	bool IsPlayerGameVoiceMuted(int playerIndex)

	{

		return GetClientVoiceMgr()->IsPlayerBlocked(playerIndex);

	}



	void MutePlayerGameVoice(int playerIndex)

	{

		GetClientVoiceMgr()->SetPlayerBlockedState(playerIndex, true);

	}



	void UnmutePlayerGameVoice(int playerIndex)

	{

		GetClientVoiceMgr()->SetPlayerBlockedState(playerIndex, false);

	}



	void OnGameUIActivated( void )

	{

		IGameEvent *event = gameeventmanager->CreateEvent( "gameui_activated" );

		if ( event )

		{

			gameeventmanager->FireEventClientSide( event );

		}

	}



	void OnGameUIHidden( void )

	{

		IGameEvent *event = gameeventmanager->CreateEvent( "gameui_hidden" );

		if ( event )

		{

			gameeventmanager->FireEventClientSide( event );

		}

	}



    //=============================================================================

    // HPE_BEGIN

    // [dwenger] Necessary for stats display

    //=============================================================================



    void CreateAchievementsPanel( vgui::Panel* pParent )

    {

        if (g_pAchievementsAndStatsInterface)

        {

            g_pAchievementsAndStatsInterface->CreatePanel( pParent );

        }

    }



    void DisplayAchievementPanel()

    {

        if (g_pAchievementsAndStatsInterface)

        {

            g_pAchievementsAndStatsInterface->DisplayPanel();

        }

    }



    void ShutdownAchievementPanel()

    {

        if (g_pAchievementsAndStatsInterface)

        {

            g_pAchievementsAndStatsInterface->ReleasePanel();

        }

    }



	int GetAchievementsPanelMinWidth( void ) const

	{

        if ( g_pAchievementsAndStatsInterface )

        {

            return g_pAchievementsAndStatsInterface->GetAchievementsPanelMinWidth();

        }



		return 0;

	}



    //=============================================================================

    // HPE_END

    //=============================================================================



	const char *GetHolidayString()

	{

		return UTIL_GetActiveHolidayString();

	}

};



EXPOSE_SINGLE_INTERFACE( CGameClientExports, IGameClientExports, GAMECLIENTEXPORTS_INTERFACE_VERSION );



class CClientDLLSharedAppSystems : public IClientDLLSharedAppSystems

{

public:

	CClientDLLSharedAppSystems()

	{

		AddAppSystem( "soundemittersystem" DLL_EXT_STRING, SOUNDEMITTERSYSTEM_INTERFACE_VERSION );

		AddAppSystem( "scenefilecache" DLL_EXT_STRING, SCENE_FILE_CACHE_INTERFACE_VERSION );

	}



	virtual int	Count()

	{

		return m_Systems.Count();

	}

	virtual char const *GetDllName( int idx )

	{

		return m_Systems[ idx ].m_pModuleName;

	}

	virtual char const *GetInterfaceName( int idx )

	{

		return m_Systems[ idx ].m_pInterfaceName;

	}

private:

	void AddAppSystem( char const *moduleName, char const *interfaceName )

	{

		AppSystemInfo_t sys;

		sys.m_pModuleName = moduleName;

		sys.m_pInterfaceName = interfaceName;

		m_Systems.AddToTail( sys );

	}



	CUtlVector< AppSystemInfo_t >	m_Systems;

};



EXPOSE_SINGLE_INTERFACE( CClientDLLSharedAppSystems, IClientDLLSharedAppSystems, CLIENT_DLL_SHARED_APPSYSTEMS );





//-----------------------------------------------------------------------------

// Helper interface for voice.

//-----------------------------------------------------------------------------

class CHLVoiceStatusHelper : public IVoiceStatusHelper

{

public:

	virtual void GetPlayerTextColor(int entindex, int color[3])

	{

		color[0] = color[1] = color[2] = 128;

	}



	virtual void UpdateCursorState()

	{

	}



	virtual bool			CanShowSpeakerLabels()

	{

		return true;

	}

};

static CHLVoiceStatusHelper g_VoiceStatusHelper;



//-----------------------------------------------------------------------------

// Code to display which entities are having their bones setup each frame.

//-----------------------------------------------------------------------------



ConVar cl_ShowBoneSetupEnts( "cl_ShowBoneSetupEnts", "0", 0, "Show which entities are having their bones setup each frame." );



class CBoneSetupEnt

{

public:

	char m_ModelName[128];

	int m_Index;

	int m_Count;

};



bool BoneSetupCompare( const CBoneSetupEnt &a, const CBoneSetupEnt &b )

{

	return a.m_Index < b.m_Index;

}



CUtlRBTree<CBoneSetupEnt> g_BoneSetupEnts( BoneSetupCompare );





void TrackBoneSetupEnt( C_BaseAnimating *pEnt )

{

#ifdef _DEBUG

	if ( IsRetail() )

		return;

		

	if ( !cl_ShowBoneSetupEnts.GetInt() )

		return;



	CBoneSetupEnt ent;

	ent.m_Index = pEnt->entindex();

	unsigned short i = g_BoneSetupEnts.Find( ent );

	if ( i == g_BoneSetupEnts.InvalidIndex() )

	{

		Q_strncpy( ent.m_ModelName, modelinfo->GetModelName( pEnt->GetModel() ), sizeof( ent.m_ModelName ) );

		ent.m_Count = 1;

		g_BoneSetupEnts.Insert( ent );

	}

	else

	{

		g_BoneSetupEnts[i].m_Count++;

	}

#endif

}



void DisplayBoneSetupEnts()

{

#ifdef _DEBUG

	if ( IsRetail() )

		return;

	

	if ( !cl_ShowBoneSetupEnts.GetInt() )

		return;



	unsigned short i;

	int nElements = 0;

	for ( i=g_BoneSetupEnts.FirstInorder(); i != g_BoneSetupEnts.LastInorder(); i=g_BoneSetupEnts.NextInorder( i ) )

		++nElements;

		

	engine->Con_NPrintf( 0, "%d bone setup ents (name/count/entindex) ------------", nElements );



	con_nprint_s printInfo;

	printInfo.time_to_live = -1;

	printInfo.fixed_width_font = true;

	printInfo.color[0] = printInfo.color[1] = printInfo.color[2] = 1;

	

	printInfo.index = 2;

	for ( i=g_BoneSetupEnts.FirstInorder(); i != g_BoneSetupEnts.LastInorder(); i=g_BoneSetupEnts.NextInorder( i ) )

	{

		CBoneSetupEnt *pEnt = &g_BoneSetupEnts[i];

		

		if ( pEnt->m_Count >= 3 )

		{

			printInfo.color[0] = 1;

			printInfo.color[1] = printInfo.color[2] = 0;

		}

		else if ( pEnt->m_Count == 2 )

		{

			printInfo.color[0] = (float)200 / 255;

			printInfo.color[1] = (float)220 / 255;

			printInfo.color[2] = 0;

		}

		else

		{

			printInfo.color[0] = printInfo.color[0] = printInfo.color[0] = 1;

		}

		engine->Con_NXPrintf( &printInfo, "%25s / %3d / %3d", pEnt->m_ModelName, pEnt->m_Count, pEnt->m_Index );

		printInfo.index++;

	}



	g_BoneSetupEnts.RemoveAll();

#endif

}



//-----------------------------------------------------------------------------

// Purpose: engine to client .dll interface

//-----------------------------------------------------------------------------

class CHLClient : public IBaseClientDLL

{

public:

	CHLClient();



	virtual int						Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals );



	virtual void					PostInit();

	virtual void					Shutdown( void );



	virtual bool					ReplayInit( CreateInterfaceFn fnReplayFactory );

	virtual bool					ReplayPostInit();



	virtual void					LevelInitPreEntity( const char *pMapName );

	virtual void					LevelInitPostEntity();

	virtual void					LevelShutdown( void );



	virtual ClientClass				*GetAllClasses( void );



	virtual int						HudVidInit( void );

	virtual void					HudProcessInput( bool bActive );

	virtual void					HudUpdate( bool bActive );

	virtual void					HudReset( void );

	virtual void					HudText( const char * message );



	// Mouse Input Interfaces

	virtual void					IN_ActivateMouse( void );

	virtual void					IN_DeactivateMouse( void );

	virtual void					IN_Accumulate( void );

	virtual void					IN_ClearStates( void );

	virtual bool					IN_IsKeyDown( const char *name, bool& isdown );

	virtual void					IN_OnMouseWheeled( int nDelta );

	// Raw signal

	virtual int						IN_KeyEvent( int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding );

	virtual void					IN_SetSampleTime( float frametime );

	// Create movement command

	virtual void					CreateMove ( int sequence_number, float input_sample_frametime, bool active );

	virtual void					ExtraMouse
