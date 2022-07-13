#ifndef _BASE_CONFIG_H_
#define _BASE_CONFIG_H_

// tolua_begin

struct BASE_CONFIG
{ 
    BOOL bUseTconnd = FALSE;

    BOOL bUseRouter = FALSE;
	int32_t nRouterServerType = 0;

    BOOL bUseConn = FALSE;
	int32_t nConnServerType = 0;

    BOOL bUseDBProxy = FALSE;
	int32_t nDBProxyServerType = 0;

	int32_t nInitShmSize = 100;
	int32_t nInitBtCtxCount = 10;
	int32_t nInitBtEventCount = 10;
	int32_t nInitBtGlobalEventListCount = 10;
	int32_t nInitTimerPoolCount = 10;
    int32_t nInitDefaultClientSessionCount = 10;

	int32_t nServerStopTimeout = 10 * 1000;
	int32_t nServerEndWaitTimeout = 10 * 1000;

    int32_t nClientPingTimeout = 5 * 1000;

    int32_t nResMode = 0;
    
    char    szScriptPath[64];
    char    szResPath[64];

    struct DUMP_CONFIG
    {
        BOOL bIsMiniDump = FALSE;
        int32_t nDumpInterval = 60 * 1000;
    };
    DUMP_CONFIG DumpConfig;

    int32_t nCommonCoroCount = 50;
    int32_t nCoroWaitTimeInterval = 5 * 1000;
};

extern BASE_CONFIG g_BaseConfig;

// tolua_end

#endif	// _BASE_CONFIG_H_
