#include "stdafx.h"
#include "MiscUtil.h"

//---------------------------------------------------------------------
CString GetSystemMessage( DWORD Code )
{
    CString Msg;
    BOOL bSuccess = FormatMessage(
                                    FORMAT_MESSAGE_FROM_SYSTEM,
                                    NULL,
                                    Code,
                                    0,
                                    Msg.GetBufferSetLength(512),
                                    512,
                                    NULL
                                 );

    Msg.ReleaseBuffer();
    if (!bSuccess)
        Msg.Format( _T("No message for error code %d"), Code );

    return Msg;
};

