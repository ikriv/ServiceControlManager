#include "stdafx.h"
#include "ServiceProperty.h"

LPCTSTR const PropertyName[SVC_LAST] =
{
    _T("Name"),
    _T("Internal Name"),
    _T("Type"),
    _T("TypeEx"),
    _T("Interactive"),
    _T("State"),
    _T("Startup"),
    _T("Error Control"),
    _T("Path"),
    _T("Load Group"),
    _T("Tag ID"),
    _T("Depends On"),
    _T("Account"),
    _T("Driver Object")
};

BOOL const Property_Returned_By_EnumServicesStatus[SVC_LAST] = 
{
    TRUE,   // name
    TRUE,   // internal name
    TRUE,   // type
    TRUE,   // extended type (shared, file system)
    TRUE,   // interactive
    TRUE,   // state
    FALSE,  // startup
    FALSE,  // error control
    FALSE,  // path
    FALSE,  // load group
    FALSE,  // Tag ID
    FALSE,  // depends on
    FALSE,  // account
    FALSE   // driver object
};
    