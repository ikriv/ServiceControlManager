#ifndef __MISC_UTIL_H__
#define __MISC_UTIL_H__

#ifndef _WINSVC_
    #include "winsvc.h"
#endif

// retrieves standard message for given error code
CString GetSystemMessage( DWORD Code );

//---------------------------------------------------------------------
// class that automatically frees service handle in destructor
class KServiceHandle
{
    public:
        KServiceHandle( SC_HANDLE Handle_ ) : Handle(Handle_) {};
        ~KServiceHandle() { if (Handle) CloseServiceHandle(Handle); };

        operator SC_HANDLE() { return Handle; };

    private:
        void operator= (KServiceHandle const&); // disabled - not implemented
        SC_HANDLE Handle;
};



#endif