#ifndef _SERVER_SHM_DEF_H_
#define _SERVER_SHM_DEF_H_

enum SERVER_SHM_TYPE_DEF
{
	stdInvalid = -1,

	stdType1,
	stdType2,
	stdBtCtx,
	stdBtMgrData,
	stdBtEvent,
	stdBtGlobalEventList,
	stdTimeMgr,
	stdTimerPool,
	stdGuidMgr,
	stdRouterAliveData,
	stdRouterObj,
	stdRouterServiceMgr,
	stdRouterServiceData,
	stdMsgQueue,
	stdSMMsgData,
	stdSMSObj,
	stdSMSServiceData,
	stdSMSRouterMgr,
	stdSMSTrans,
	stdSession,

	stdStacklessGlobalMgr,
	stdStacklessCoreBegin,
	stdStacklessCoreEnd = stdStacklessCoreBegin + 256,

	stdGlobalResMgr,
	stdResBegin,
	stdResEnd = stdResBegin + 256,

	stdUserDefined,
};

#endif