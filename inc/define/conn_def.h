#ifndef _CONN_DEF_H_
#define _CONN_DEF_H_

#define MAX_MESSAGE_ID			(65535)

enum CONN_EVENT_TYPE
{
    cetInvalid,

    cetStart,
    cetStop,
    cetTimeout,

    cetTotal
};

#define MAKE_SESSION_ID(__tconnd_addr__, __conn_index__)	((((__tconnd_addr__) + 0ULL) << 32) | (__conn_index__))
#define GET_TCONND_ADDR(__session_id__)		                ((__session_id__) >> 32)
#define GET_CONN_INDEX(__session_id__)		                ((__session_id__) & 0xFFFFFFFF)

#endif