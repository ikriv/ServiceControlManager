#ifndef __SERVICE_PROPERTY_H__
#define __SERVICE_PROPERTY_H__

enum KServiceProperty
{
    SVC_DISPLAY_NAME,
    SVC_INTERNAL_NAME,
    SVC_TYPE,
    SVC_TYPE_REMARKS,       // shared, file system
    SVC_INTERACTIVE,        // yes/no
    SVC_STATE,              // running, stopped, etc.
    SVC_STARTUP,            // boot, system, auto...
    SVC_ERROR_CTRL,
    SVC_PATH,
    SVC_LOAD_GROUP,
    SVC_TAG_ID,
    SVC_DEPENDENCIES,
    SVC_ACCOUNT,            // only for services
    SVC_DRIVER_OBJECT_NAME, // only for drivers
    SVC_LAST
};

extern LPCTSTR const PropertyName[SVC_LAST];
extern BOOL const Property_Returned_By_EnumServicesStatus[SVC_LAST];

#endif
