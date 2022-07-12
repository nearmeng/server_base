#ifndef _BASE_CONFIG_H_
#define _BASE_CONFIG_H_

// tolua_begin

struct BASE_CONFIG
{
    BOOL bUseTconnd;
	int32_t nConnServerType;

    BOOL bUseRouter;
    BOOL bUseConn;
    BOOL bUseDBProxy;
	
	int32_t nRouterServerType;
	int32_t nDBProxyServerType;

	int32_t nInitShmSize;
	int32_t nInitBtCtxCount;
	int32_t nInitBtEventCount;
	int32_t nInitBtGlobalEventListCount;
	int32_t nInitTimerPoolCount;
    int32_t nInitDefaultClientSessionCount;

	int32_t nServerStopTimeout;
	int32_t nServerEndWaitTimeout;

    int32_t nClientPingTimeout;

    int32_t nResMode;
    
    char    szScriptPath[64];
    char    szResPath[64];

    struct DUMP_CONFIG
    {
        BOOL bIsMiniDump;
        int32_t nDumpInterval;
    };
    DUMP_CONFIG DumpConfig;

    int32_t nCommonCoroCount;
    int32_t nCoroWaitTimeInterval;
};

extern BASE_CONFIG g_BaseConfig;

// tolua_end

#endif	// _BASE_CONFIG_H_
