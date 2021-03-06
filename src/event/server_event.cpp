#include "stdafx.h"
#include "event/server_event.h"

#include "config/base_config.h"
#include "define/server_base_shm_def.h"

#include "bt/bt_mgr.h"


int32_t CBTEventList::ms_nTriggerLayer;
int32_t CBTEventList::ms_nTriggerCounter;

CEventMgr CEventMgr::ms_Instance;
CGlobalEventListMgr CGlobalEventListMgr::ms_Instance;

inline uint64_t MAKE_EVENT_LIST_ID(uint64_t qwStartType, uint64_t qwTemplateID)
{
	return ((qwStartType << 32) | qwTemplateID);
}

BOOL CEventMgr::_init_event_def(void)
{
	int32_t nRetCode = 0;

	//REG_EVENT_DEF(evtRoleSyncData, otRole, evtStaticRoleBegin, evtStaticRoleEnd);
	//REG_EVENT_DEF(evtRoleKillNpc, otRole, evtDynamicBegin, evtDynamicEnd);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CEventMgr::init(BOOL bResume)
{
	int32_t nRetCode = 0;

	nRetCode = m_EventPool.init(stdBtEvent, g_BaseConfig.nInitBtEventCount, bResume);
	LOG_PROCESS_ERROR(nRetCode);
	
	nRetCode = _init_event_def();
	LOG_PROCESS_ERROR(nRetCode);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CEventMgr::uninit(void)
{
	int32_t nRetCode = 0;

	return TRUE;
Exit0:
	return FALSE;
}

int32_t CEventMgr::create_lua_event(const char* pcszEventName, int32_t nEventType, int32_t nTemplateID, int32_t nEventParam,
	int32_t nTreeID, CLuaScript* pScript, uint64_t qwSourceID, int64_t llVar0, int64_t llVar1)
{
	int32_t nRetCode = 0;
	int32_t nEventID = 0;
	EVENT_INFO* pEvent = NULL;
	char szEventName[EVENT_NAME_LEN];

	snprintf(szEventName, EVENT_NAME_LEN, "%s_%s_%d_%d", pcszEventName, pScript->get_script_name(), nTemplateID, nEventParam);
	nEventID = hash_string(szEventName);

	pEvent = m_EventPool.find_object(nEventID);
	LOG_PROCESS_ERROR(pEvent == NULL);

	pEvent = m_EventPool.new_object(nEventID);
	LOG_PROCESS_ERROR(pEvent);

	pEvent->nEventID = nEventID;
	pEvent->nEventType = nEventType;
	pEvent->nEventTemplateID = nTemplateID;
	pEvent->nEventParam = nEventParam;
	pEvent->nTreeID = nTreeID;
	pEvent->qwSourceID = qwSourceID;
	pEvent->llEventVar[0] = llVar0;
	pEvent->llEventVar[1] = llVar1;

	return nEventID;
Exit0:
	return 0;
}
	
int32_t CEventMgr::create_c_event(int32_t nEventType, int32_t nTemplateID, int32_t nEventParam,
    EVENT_CALLBACK pEventCallBack, uint64_t qwSourceID, int64_t llVar0, int64_t llVar1)
{
    int32_t nRetCode = 0;
    int32_t nEventID = 0;
    EVENT_INFO* pEvent = NULL;
    char szEventName[EVENT_NAME_LEN];

	snprintf(szEventName, EVENT_NAME_LEN, "%s_%d_%d_%d", "c_register", nEventType, nTemplateID, nEventParam);
	nEventID = hash_string(szEventName);

	pEvent = m_EventPool.find_object(nEventID);
    if (pEvent == NULL)
    {
        pEvent = m_EventPool.new_object(nEventID);
        LOG_PROCESS_ERROR(pEvent);

        pEvent->nEventID = nEventID;
        pEvent->nEventType = nEventType;
        pEvent->nEventTemplateID = nTemplateID;
        pEvent->nEventParam = nEventParam;
        pEvent->nTreeID = 0;
        pEvent->qwSourceID = qwSourceID;
        pEvent->llEventVar[0] = llVar0;
        pEvent->llEventVar[1] = llVar1;
        pEvent->pEventCallback = pEventCallBack;

        INF("create new event success, eventid %d, event_name %s sourceid %lld val %d %d", nEventID, szEventName, qwSourceID, llVar0, llVar1);
    }
    else
    {
        INF("detect event resume, eventid %d, event_name %s sourceid %lld val %d %d", nEventID, szEventName, qwSourceID, llVar0, llVar1);
        pEvent->pEventCallback = pEventCallBack;
    }

    return nEventID;
Exit0:
    return 0;
}

BOOL CEventMgr::destroy_event(int32_t nEventID)
{
	int32_t nRetCode = 0;
	EVENT_INFO* pEvent = NULL;

	pEvent = m_EventPool.find_object(nEventID);
	LOG_PROCESS_ERROR(pEvent);

	nRetCode = m_EventPool.delete_object(pEvent);
	LOG_PROCESS_ERROR(nRetCode);

	return TRUE;
Exit0:
	return FALSE;
}

EVENT_INFO* CEventMgr::find_event(int32_t nEventID)
{
	return m_EventPool.find_object(nEventID);
}
	
int32_t CEventMgr::get_event_owner(int32_t nEventType)
{
	int32_t nRetCode = 0;
	LOG_PROCESS_ERROR(nEventType > 0 && nEventType < MAX_EVENT_TYPE_COUNT);

	return ms_EventDefList[nEventType].nOwnerType;
Exit0:
	return 0;
}
	
int32_t CEventMgr::get_event_start_type(int32_t nEventType)
{
	LOG_PROCESS_ERROR(nEventType > 0 && nEventType < MAX_EVENT_TYPE_COUNT);

	return ms_EventDefList[nEventType].nStartEventType;
Exit0:
	return 0;
}

int32_t CEventMgr::get_event_end_type(int32_t nEventType)
{
	LOG_PROCESS_ERROR(nEventType > 0 && nEventType < MAX_EVENT_TYPE_COUNT);

	return ms_EventDefList[nEventType].nEndEventType;
Exit0:
	return 0;
}
	
BOOL CEventMgr::register_event_def(int32_t nEventType, const char* pcszEventName,
	int32_t nOwnerType, int32_t nStartType, int32_t nEndType)
{
	int32_t nRetCode = 0;
	
	LOG_PROCESS_ERROR(pcszEventName);
	LOG_PROCESS_ERROR(nEventType > 0 && nEventType < MAX_EVENT_TYPE_COUNT);

	ms_EventDefList[nEventType].nEventType = nEventType;
	ms_EventDefList[nEventType].nOwnerType = nOwnerType;
	ms_EventDefList[nEventType].nStartEventType = nStartType;
	ms_EventDefList[nEventType].nEndEventType = nEndType;
	strxcpy(ms_EventDefList[nEventType].szName, pcszEventName, sizeof(ms_EventDefList[nEventType]));

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CBTEventList::init(int32_t nStartEventType, int32_t nEndEventType)
{
	int32_t nRetCode = 0;

	LOG_PROCESS_ERROR(nEndEventType - nStartEventType < MAX_EVENT_PER_OWNER);

	m_nEventCount = 0;
	memset(m_EventData, 0, sizeof(m_EventData));

	memset(m_byTypeCountList, 0, sizeof(m_byTypeCountList));
	m_nStartEventType = nStartEventType;
	m_nEndEventType = nEndEventType;

	m_nTriggerLayer = 0;
	m_bPendingDelete = FALSE;

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CBTEventList::uninit(void)
{
	return TRUE;
}
	
BOOL CBTEventList::resume(void)
{
	int32_t nRetCode = 0;

	for (int32_t i = 0; i < m_nEventCount; i++)
	{
		m_EventData[i].pEvent = CEventMgr::instance().find_event(m_EventData[i].nEventID);
		LOG_PROCESS_ERROR_DETAIL(m_EventData[i].pEvent, "can not find event %d", m_EventData[i].nEventID);
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CBTEventList::register_event(int32_t nEventID)
{
	int32_t nRetCode = 0;
	EVENT_INFO* pEvent = NULL;

	LOG_PROCESS_ERROR(m_nEventCount < MAX_EVENT_COUNT);

	pEvent = CEventMgr::instance().find_event(nEventID);
	LOG_PROCESS_ERROR(pEvent);
	LOG_PROCESS_ERROR_DETAIL(pEvent->nEventType >= m_nStartEventType, "invalid type %d", pEvent->nEventType);
	LOG_PROCESS_ERROR_DETAIL(pEvent->nEventType < m_nEndEventType, "invalid Type %d", pEvent->nEventType);

	m_EventData[m_nEventCount].nEventID = nEventID;
	m_EventData[m_nEventCount].pEvent = pEvent;
	m_EventData[m_nEventCount].bDeleted = FALSE;
	m_nEventCount++;

	m_byTypeCountList[pEvent->nEventType - m_nStartEventType]++;

	return TRUE;
Exit0:
	return FALSE;
}
	
BOOL CBTEventList::unregister_event(int32_t nEventID)
{
	int32_t nRetCode = 0;

	for (int32_t i = 0; i < m_nEventCount; i++)
	{
		if (m_EventData[i].nEventID == nEventID)
		{
			int32_t nEventType = m_EventData[i].pEvent->nEventType;

			if (m_nTriggerLayer <= 0)
			{
				for (int32_t j = i; j < m_nEventCount; j++)
					m_EventData[j] = m_EventData[j + 1];

				m_nEventCount--;
				m_byTypeCountList[nEventType - m_nStartEventType]--;
			}
			else
			{
				m_EventData[i].bDeleted = TRUE;
				m_bPendingDelete = TRUE;
			}

			return TRUE; 
		}
	}

Exit0:
	return FALSE;
}

int32_t CBTEventList::trigger_event(int32_t nEventType, int32_t nEventTemplateID, int32_t nEventParam,
	void* pOwner, uint64_t qwOwnerID, int64_t llTriggerVar0, int64_t llTriggerVar1, BOOL bRollBack, BOOL bBreakOnFail)
{
	int32_t nRetCode = 0;
	int32_t nOwnerType = 0;
	int32_t nResult = -1;
	BT_CTX Ctx;
		
	LOG_PROCESS_ERROR(nEventType >= m_nStartEventType & nEventType < m_nEndEventType);

	if (m_nEventCount <= 0 || m_byTypeCountList[nEventType - m_nStartEventType] <= 0)
		return brvSuccess;

	LOG_PROCESS_ERROR(ms_nTriggerLayer < MAX_EVENT_RECURSIVE_COUNT);

	ms_nTriggerLayer++;
	m_nTriggerLayer++;

	for (int32_t i = 0; i < m_nEventCount; i++)
	{
        EVENT_INFO* pEventInfo = m_EventData[i].pEvent;

		if (pEventInfo == NULL)
			continue;

		if (m_EventData[i].bDeleted)
			continue;

		if (pEventInfo->nEventType != nEventType)
			continue;

		if (pEventInfo->nEventParam != nEventParam)
			continue;

		if (pEventInfo->nEventTemplateID != nEventTemplateID)
			continue;

		nOwnerType = CEventMgr::instance().get_event_owner(pEventInfo->nEventType);

        if (pEventInfo->nTreeID > 0)
        {
            nRetCode = CBTMgr::instance().start_run(Ctx, pEventInfo->nTreeID,
                nOwnerType, pOwner, qwOwnerID, llTriggerVar0, llTriggerVar1,
                pEventInfo->llEventVar[0], pEventInfo->llEventVar[1], bRollBack);
            LOG_CHECK_ERROR(nRetCode != brvError);
        }
        else if(pEventInfo->pEventCallback)
        {
            EVENT_PARAM stParam;
            stParam.pOwner = pOwner;
            stParam.llTriggerVar0 = llTriggerVar0;
            stParam.llTriggerVar1 = llTriggerVar1;
            stParam.qwOwnerID = qwOwnerID;
            stParam.nOwnerType = nOwnerType;

            pEventInfo->pEventCallback(pEventInfo, stParam);
        }
        else
        {
            WRN("can not trigger event, eventid %d type %d template_id %d param %d", pEventInfo->nEventID, pEventInfo->nEventType,
                pEventInfo->nEventTemplateID, pEventInfo->nEventParam);
            LOG_PROCESS_ERROR(FALSE);
        }

		if (bBreakOnFail)
		{
			nResult = Ctx.nResult;
			if (nRetCode != brvSuccess)
				break;
		}
	}

	LOG_PROCESS_ERROR(ms_nTriggerLayer > 0);
	ms_nTriggerLayer--;
	m_nTriggerLayer--;

	if (m_nTriggerLayer <= 0 && m_bPendingDelete)
	{
		for (int32_t i = m_nEventCount - 1; i >= 0; i--)
		{
			if (m_EventData[i].bDeleted)
			{
				int32_t nEventType = m_EventData[i].pEvent->nEventType;

				for (int32_t j = i; j < m_nEventCount; j++)
					m_EventData[j] = m_EventData[j + 1];

				m_nEventCount--;
				m_byTypeCountList[nEventType - m_nStartEventType]--;
			}
		}

		m_bPendingDelete = FALSE;
	}

	return nResult;

Exit0:
	CRI("failed to trigger event %d", nEventType);
	return -1;
}

BOOL CBTEventList::is_event_registed(int32_t nEventType, int32_t nEventTemplateID, int32_t nEventParam)
{
	int32_t nRetCode = 0;

	LOG_PROCESS_ERROR(nEventType >= m_nStartEventType && nEventType < m_nEndEventType);

	if (m_nEventCount <= 0 || m_byTypeCountList[nEventType - m_nStartEventType] <= 0)
		return FALSE;

	for (int32_t i = 0; i < m_nEventCount; i++)
	{
		EVENT_INFO* pEvent = m_EventData[i].pEvent;
		LOG_PROCESS_ERROR(pEvent);

		if (pEvent->nEventType != nEventType)
			continue;

		if (pEvent->nEventTemplateID != nEventTemplateID)
			continue;

		if (pEvent->nEventParam != nEventParam)
			continue;

		return TRUE;
	}

Exit0:
	return FALSE;
}

BOOL CGlobalEventListMgr::TRAVERSE_BT_EVENT_LIST_RESUME::operator()(uint64_t qwEventListID, CBTEventList* pEventList)
{
	int32_t nRetCode = 0;

	pEventList->resume();

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CGlobalEventListMgr::init(BOOL bResume)
{
	int32_t nRetCode = 0;

	nRetCode = m_GlobalEventListPool.init(stdBtGlobalEventList, g_BaseConfig.nInitBtGlobalEventListCount, bResume);
	LOG_PROCESS_ERROR(nRetCode);

	if (bResume)
	{
		TRAVERSE_BT_EVENT_LIST_RESUME TraverseBtEventListResume;
		m_GlobalEventListPool.traverse(TraverseBtEventListResume);
	}

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CGlobalEventListMgr::uninit(void)
{
	return TRUE;
}

BOOL CGlobalEventListMgr::register_global_event(int32_t nEventID)
{
	int32_t nRetCode = 0;
	uint64_t qwEventListID = 0;
	CBTEventList* pEventList = NULL;
	EVENT_INFO* pEvent = NULL;
	int32_t nStartType = 0;
	int32_t nEndType = 0;

	pEvent = CEventMgr::instance().find_event(nEventID);
	LOG_PROCESS_ERROR(pEvent);

	nStartType = CEventMgr::instance().get_event_start_type(pEvent->nEventType);
	nEndType = CEventMgr::instance().get_event_end_type(pEvent->nEventType);

	qwEventListID = MAKE_EVENT_LIST_ID(nStartType, pEvent->nEventTemplateID);

	pEventList = m_GlobalEventListPool.find_object(qwEventListID);
	LOG_PROCESS_ERROR(pEventList == NULL);

	pEventList = m_GlobalEventListPool.new_object(qwEventListID);
	LOG_PROCESS_ERROR(pEventList);
		
	nRetCode = pEventList->init(nStartType, nEndType);
	LOG_PROCESS_ERROR(nRetCode);

	nRetCode = pEventList->register_event(nEventID);
	LOG_PROCESS_ERROR(nRetCode);

	return TRUE;
Exit0:
	return FALSE;
}

BOOL CGlobalEventListMgr::unregister_global_event(int32_t nEventID)
{
	int32_t nRetCode = 0;
	EVENT_INFO* pEvent = NULL;
	int32_t nStartType = 0;
	uint64_t qwEventListID = 0;
	CBTEventList* pEventList = NULL;
	
	pEvent = CEventMgr::instance().find_event(nEventID);
	LOG_PROCESS_ERROR(pEvent);

	nStartType = CEventMgr::instance().get_event_start_type(pEvent->nEventType);
	qwEventListID = MAKE_EVENT_LIST_ID(nStartType, pEvent->nEventTemplateID);

	pEventList = m_GlobalEventListPool.find_object(qwEventListID);
	LOG_PROCESS_ERROR(pEventList);

	nRetCode = pEventList->unregister_event(nEventID);
	LOG_PROCESS_ERROR(nRetCode);

	return TRUE;
Exit0:
	return FALSE;
}

int32_t CGlobalEventListMgr::trigger_global_event(int32_t nEventType, int32_t nEventTemplateID, int32_t nEventParam, void* pOwner, uint64_t qwOwnerID,
	int64_t llTriggerVar0, int64_t llTriggerVar1, BOOL bRollBack, BOOL bBreakOnFail)
{
	int32_t nRetCode = 0;
	int32_t nResult = -1;
	int32_t nStartType = 0;
	uint64_t qwEventListID = 0;
	CBTEventList* pEventList = NULL;

	LOG_PROCESS_ERROR(nEventType > 0 && nEventType < MAX_EVENT_TYPE_COUNT);

	nStartType = CEventMgr::instance().get_event_start_type(nEventType);
	qwEventListID = MAKE_EVENT_LIST_ID(nStartType, nEventTemplateID);

	pEventList = m_GlobalEventListPool.find_object(qwEventListID);
	LOG_PROCESS_ERROR(pEventList);

	return pEventList->trigger_event(nEventType, nEventTemplateID, nEventParam, pOwner, qwOwnerID, 
				llTriggerVar0, llTriggerVar1, bRollBack, bBreakOnFail);
	
Exit0:
	return nResult;
}

//for lua func
int lua_create_bt_event(lua_State* L)
{
	int32_t nRetCode = 0;
	const char* pcszEventName = NULL;
	int32_t nEventType = 0;
	int32_t nTreeID = 0;
	CLuaScript* pScript = NULL;
	int32_t nEventParam = 0;
	int32_t nEventID = 0;
	uint32_t nTemplateID = 0;
	uint64_t qwSourceID = 0;

	pcszEventName = lua_tostring(L, 1);
	nTemplateID = lua_tointeger(L, 2);
	nEventType = lua_tointeger(L, 3);
	nTreeID = lua_tointeger(L, 4);
	nEventParam = lua_tointeger(L, 5);
	qwSourceID = lua_tointeger(L, 6);

	pScript = CLuaScript::get_script_from_state(L);
	LOG_PROCESS_ERROR(pScript);

	nEventID = CEventMgr::instance().create_lua_event(pcszEventName, nEventType, nTemplateID, nEventParam, nTreeID, pScript, qwSourceID);
	LOG_PROCESS_ERROR(nEventID > 0);

	lua_pushinteger(L, nEventID);
	return 1;
Exit0:
	return 0;
}

int lua_register_global_event(lua_State* L)
{
	int32_t nRetCode = 0;
	int32_t nEventID = lua_tointeger(L, 1);

	nRetCode = CGlobalEventListMgr::instance().register_global_event(nEventID);
	LOG_PROCESS_ERROR(nRetCode);

	lua_pushboolean(L, TRUE);

	return 1;
Exit0:
	lua_pushboolean(L, FALSE);
	return 0;
}