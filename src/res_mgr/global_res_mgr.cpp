#include "stdafx.h"
#include "res_mgr/global_res_mgr.h"
#include "res_mgr/res_mgr.h"

#include "config/base_config.h"

#include "lua/embed_source.h"

extern int tolua_res_base_open (lua_State* tolua_S);

CGlobalResMgr* CGlobalResMgr::ms_Instance = NULL;

struct FAKE_RES : RES_BASE
{
};

CGlobalResMgr::CGlobalResMgr()
{
    m_nResMode = 0;
    m_ResMgr.clear();
}

BOOL CGlobalResMgr::init(int32_t nResMode, int32_t nShmType, int32_t nResShmTypeBegin, int32_t nResShmTypeEnd,
		std::vector<TOLUA_FUNC> vFuncs, BOOL bResume)
{
    int32_t nRetCode = 0;
    char szScriptPath[RES_NAME_LEN];
    CResMgr<FAKE_RES>* pResMgr = NULL;
    GLOBAL_RES_MGR_DATA* pMgrData = NULL;

    LOG_PROCESS_ERROR(nResMode > rlmInvalid && nResMode < rlmTotal);

    m_nResMode = nResMode;

    nRetCode = m_MgrData.init(nShmType, bResume);
    LOG_PROCESS_ERROR(nRetCode);

    pMgrData = m_MgrData.get_obj();
    LOG_PROCESS_ERROR(pMgrData);

    nRetCode = m_LuaScript.init();
    LOG_PROCESS_ERROR(nRetCode);

    vFuncs.push_back(tolua_res_base_open);
	for (int32_t i = 0; i < vFuncs.size(); i++)
	{
		vFuncs[i](m_LuaScript.get_lua_state());
	}

    nRetCode = m_LuaScript.load_from_buffer("read_res.lua", pcszReadResLuaSource, strlen(pcszReadResLuaSource));
    LOG_PROCESS_ERROR(nRetCode);
    
    if (!bResume)
    {
        nRetCode = _init_shm_type_for_all_mgr(nResShmTypeBegin, nResShmTypeEnd);
        LOG_PROCESS_ERROR(nRetCode);
    }

    for (std::map<std::string, RES_INFO>::iterator it = m_ResMgr.begin(); it != m_ResMgr.end(); it++)
    {
        std::string ResName = it->first;
        RES_INFO& rResInfo = it->second;
        int32_t nShmType = 0;

        pResMgr = (CResMgr<FAKE_RES>*)rResInfo.pResMgr;
        LOG_PROCESS_ERROR(pResMgr);

        nShmType = _get_shm_type_by_mgr_name(ResName.c_str());
        LOG_PROCESS_ERROR(nShmType > 0);

        INF("res mgr init, name %s shm_type %d", ResName.c_str(), nShmType);

        nRetCode = pResMgr->init(nShmType, ResName.c_str(), g_BaseConfig.szResPath, rResInfo.ResObjName.c_str(), bResume);
        LOG_PROCESS_ERROR(nRetCode);
    }

    for (std::map<std::string, RES_INFO>::iterator it = m_ResMgr.begin(); it != m_ResMgr.end(); it++)
    {
        std::string ResName = it->first;
        RES_INFO& rResInfo = it->second;

        pResMgr = (CResMgr<FAKE_RES>*)rResInfo.pResMgr;
        LOG_PROCESS_ERROR(pResMgr);

        nRetCode = pResMgr->post_check();
        LOG_PROCESS_ERROR(nRetCode);
    }

    return TRUE;
Exit0:
    return FALSE;
}

BOOL CGlobalResMgr::uninit(void)
{
    int32_t nRetCode = 0;

    nRetCode = m_LuaScript.uninit();
    LOG_CHECK_ERROR(nRetCode);
    
    for (std::map<std::string, RES_INFO>::iterator it = m_ResMgr.begin(); it != m_ResMgr.end(); it++)
    {
        std::string ResName = it->first;
        RES_INFO& rResInfo = it->second;

        CResMgr<FAKE_RES>* pResMgr = (CResMgr<FAKE_RES>*)rResInfo.pResMgr;
        LOG_PROCESS_ERROR(pResMgr);

        nRetCode = pResMgr->uninit();
        LOG_PROCESS_ERROR(nRetCode);
    }

    return TRUE;
Exit0:
    return FALSE;
}

BOOL CGlobalResMgr::add_res_mgr(const char* pcszResName, void* pResMgr)
{
    int32_t nRetCode = 0;
    const char* pcszStart = NULL;
    RES_INFO stInfo;
    char szParsedResName[RES_NAME_LEN];
    char szParsedObjName[OBJ_NAME_LEN];

    LOG_PROCESS_ERROR(pcszResName);
    LOG_PROCESS_ERROR(pResMgr);

#if defined(WIN32) || defined(WIN64)
	pcszStart = strstr(pcszResName, " ");
	PROCESS_ERROR(pcszStart);

	snprintf(szParsedObjName, OBJ_NAME_LEN, pcszStart + 1);
	snprintf(szParsedResName, RES_NAME_LEN, pcszStart + 1);
#else
	pcszStart = pcszResName;
	while (isdigit(*pcszStart))
		pcszStart++;

	snprintf(szParsedObjName, OBJ_NAME_LEN, pcszStart);
	snprintf(szParsedResName, RES_NAME_LEN, pcszStart);
#endif	// WIN32 || WIN64
    
	*strrchr(szParsedResName, '_') = 0;
	strxtolower(szParsedResName, RES_NAME_LEN);

    stInfo.pResMgr = pResMgr;
    stInfo.ResObjName = szParsedObjName;

    m_ResMgr.insert(std::make_pair(szParsedResName, stInfo));

    return TRUE;
Exit0:
    return FALSE;
}

BOOL CGlobalResMgr::reload(BOOL bForce)
{
    int32_t nRetCode = 0;

    for (std::map<std::string, RES_INFO>::iterator it = m_ResMgr.begin(); it != m_ResMgr.end(); it++)
    {
        std::string ResName = it->first;
        RES_INFO& rResInfo = it->second;

        CResMgr<FAKE_RES>* pResMgr = (CResMgr<FAKE_RES>*)rResInfo.pResMgr;
        LOG_PROCESS_ERROR(pResMgr);

        nRetCode = pResMgr->reload(bForce);
        LOG_PROCESS_ERROR(nRetCode);
    }
    
    for (std::map<std::string, RES_INFO>::iterator it = m_ResMgr.begin(); it != m_ResMgr.end(); it++)
    {
        std::string ResName = it->first;
        RES_INFO& rResInfo = it->second;

        CResMgr<FAKE_RES>* pResMgr = (CResMgr<FAKE_RES>*)rResInfo.pResMgr;
        LOG_PROCESS_ERROR(pResMgr);

        nRetCode = pResMgr->post_check();
        LOG_PROCESS_ERROR(nRetCode);
    }

    return TRUE;
Exit0:
    return FALSE;
}
    
int32_t CGlobalResMgr::_get_shm_type_by_mgr_name(const char* pcszMgrName)
{
    int32_t nRetCode = 0;
    GLOBAL_RES_MGR_DATA* pMgrData = NULL;

    pMgrData = m_MgrData.get_obj();
    LOG_PROCESS_ERROR(pMgrData);

    for (int32_t i = 0; i < pMgrData->nShmTypeInfoCount; i++)
    {
        if (strncmp(pMgrData->stShmTypeInfo[i].szResMgrName, pcszMgrName, sizeof(pMgrData->stShmTypeInfo[i].szResMgrName)) == 0)
        {
            return pMgrData->stShmTypeInfo[i].nShmType;
        }
    }

Exit0:
    return 0;
}

BOOL CGlobalResMgr::_init_shm_type_for_all_mgr(int32_t nResShmTypeBegin, int32_t nResShmTypeEnd)
{
    int32_t nRetCode = 0;
    GLOBAL_RES_MGR_DATA* pMgrData = NULL;

    pMgrData = m_MgrData.get_obj();
    LOG_PROCESS_ERROR(pMgrData);
    
    {
        int32_t& nShmInfoCount = pMgrData->nShmTypeInfoCount;
        LOG_PROCESS_ERROR(nShmInfoCount == 0);

        for (std::map<std::string, RES_INFO>::iterator it = m_ResMgr.begin(); it != m_ResMgr.end(); it++)
        {
            std::string ResName = it->first;

            strxcpy(pMgrData->stShmTypeInfo[nShmInfoCount].szResMgrName, ResName.c_str(), RES_NAME_LEN);
            pMgrData->stShmTypeInfo[nShmInfoCount].nShmType = nResShmTypeBegin + nShmInfoCount;
            LOG_PROCESS_ERROR(pMgrData->stShmTypeInfo[nShmInfoCount].nShmType <= nResShmTypeEnd);

            nShmInfoCount++;
        }
    }

    return TRUE;
Exit0:
    return FALSE;
}
