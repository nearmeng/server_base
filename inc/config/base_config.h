#ifndef _BASE_CONFIG_H_
#define _BASE_CONFIG_H_

// tolua_begin

struct BASE_CONFIG
{ 
    BASE_CONFIG() 
    {
        bUseTconnd = FALSE;

        bUseRouter = FALSE;
	    nRouterServerType = 0;

        bUseConn = FALSE;
	    nConnServerType = 0;

        bUseDBProxy = FALSE;
	    nDBProxyServerType = 0;

	    nInitShmSize = 100;
    	nInitBtCtxCount = 10;
    	nInitBtEventCount = 10;
	    nInitBtGlobalEventListCount = 10;
    	nInitTimerPoolCount = 10;
        nInitDefaultClientSessionCount = 10;

	    nServerStopTimeout = 10 * 1000;
	    nServerEndWaitTimeout = 10 * 1000;

        nClientPingTimeout = 5 * 1000;

        nResMode = 0;
    
        szScriptPath[0] = '\0';
        szResPath[0] = '\0';

        DumpConfig.bIsMiniDump = FALSE;
        DumpConfig.nDumpInterval = 60 * 1000;

        nCommonCoroCount = 50;
        nCoroWaitTimeInterval = 5 * 1000;
    };

    BOOL bUseTconnd;

    BOOL bUseRouter;
	int32_t nRouterServerType;

    BOOL bUseConn;
	int32_t nConnServerType;

    BOOL bUseDBProxy;
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

	char szServerConfigName[64];
};

extern BASE_CONFIG g_BaseConfig;

// tolua_end

#endif	// _BASE_CONFIG_H_
