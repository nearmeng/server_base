#ifndef _BASE_CONFIG_H_
#define _BASE_CONFIG_H_

// tolua_begin

struct BASE_CONFIG
{ 
    BASE_CONFIG() 
    {
	    nInitShmSize = 10 * 1024 * 1024;
    	nInitTimerPoolCount = 10;
	    nServerStopTimeout = 10 * 1000;
		strncpy(szServerConfigName, "server_config", sizeof(szServerConfigName));

        bUseTconnd = FALSE;

        bUseRouter = FALSE;
	    nRouterServerType = 0;

        bUseConn = FALSE;
	    nConnServerType = 0;

        bUseDBProxy = FALSE;
	    nDBProxyServerType = 0;

		bEventEnable = FALSE;
    	nInitBtEventCount = 0;
	    nInitBtGlobalEventListCount = 0;

		bResEnable = FALSE;
        nResMode = 0;
        szResPath[0] = '\0';

        DumpConfig.bIsMiniDump = FALSE;
        DumpConfig.nDumpInterval = 60 * 1000;

		bCoroEnable = FALSE;
        nCommonCoroCount = 50;
    };

	int32_t nInitShmSize;
	int32_t nInitTimerPoolCount;
	char szServerConfigName[64];
	
	int32_t nServerStopTimeout;

    BOOL bUseTconnd;

    BOOL bUseRouter;
	int32_t nRouterServerType;

    BOOL bUseConn;
	int32_t nConnServerType;

    BOOL bUseDBProxy;
	int32_t nDBProxyServerType;

	BOOL bEventEnable;
	int32_t nInitBtEventCount;
	int32_t nInitBtGlobalEventListCount;

	BOOL bResEnable;
    int32_t nResMode;
    char    szResPath[64];

    struct DUMP_CONFIG
    {
        BOOL bIsMiniDump;
        int32_t nDumpInterval;
    };
    DUMP_CONFIG DumpConfig;

	BOOL bCoroEnable;
    int32_t nCommonCoroCount;
};

extern BASE_CONFIG g_BaseConfig;

// tolua_end

#endif	// _BASE_CONFIG_H_
