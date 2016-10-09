#include "stdafx.h"
#include "winreg.h"
#include "ServiceControlWnd.h"
#include "MultiString.h"

//---------------------------------------------------------------------
// definition of default columns widths; 
// SVC_xxx constants are defined in ServiceProperty.h

static int const DefaultWidth[SVC_LAST] = 
{
    100,    // SVC_NAME
    100,    // SVC_INTERNAL_NAME
    100,    // SVC_TYPE
    100,    // SVC_TYPE_REMARKS
    20,     // SVC_INTERACTIVE
    100,    // SVC_STATE
    100,    // SVC_STARTUP
    100,    // SVC_ERROR_CTRL
    100,    // SVC_PATH
    100,    // SVC_LOAD_GROUP
    100,    // SVC_TAG_ID
    100,    // SVC_DEPENDENCIES
    100,    // SVC_ACCOUNT
    100     // SVC_DRIVER_OBJECT
};

//-----------------------------------------------------
// definition for our registry key
static char const KeyName[] = "Software\\Service_Control_Manager_IVK";

//-----------------------------------------------------
static BOOL GetMultiString( HKEY hKey, LPCTSTR ValueName, KMultiString& S )
{
    DWORD Type;
    DWORD DataSize = 0;
    DWORD Result = RegQueryValueEx( hKey, ValueName, NULL, &Type, NULL, &DataSize );
    if (Result != ERROR_SUCCESS)
        return FALSE;

    if (Type != REG_MULTI_SZ)
        return FALSE;

    CString Str;
    TCHAR* pBuffer = Str.GetBuffer(DataSize);

    Result = RegQueryValueEx( hKey, ValueName, NULL, &Type, (BYTE*)pBuffer, &DataSize );
    if (Result != ERROR_SUCCESS)
    {
        Str.ReleaseBuffer();
        return FALSE;
    };

    S.CopyFromBuffer(pBuffer);
    Str.ReleaseBuffer();
    return TRUE;
};

//-----------------------------------------------------
static BOOL SetMultiString( HKEY hKey, LPCTSTR ValueName, KMultiString const& S )
{
    CString Str = S.GetBuffer();
        
    LONG Result = RegSetValueEx( 
                                    hKey, 
                                    ValueName, 
                                    NULL, 
                                    REG_MULTI_SZ, 
                                    (BYTE*)(LPCTSTR)Str, 
                                    (Str.GetLength()+1) * sizeof(TCHAR) 
                               );
    return (Result == ERROR_SUCCESS);
};

//-----------------------------------------------------
static BOOL SetString( HKEY hKey, LPCTSTR ValueName, LPCTSTR Value )
{
    LONG Result = RegSetValueEx(
                                 hKey,
                                 ValueName,
                                 NULL,
                                 REG_SZ,
                                 (BYTE*)Value,
                                 (_tcslen(Value)+1) * sizeof(TCHAR)
                               );
    return (Result == ERROR_SUCCESS);
};

//-----------------------------------------------------
int GetPropertyByName( LPCTSTR Name )
{
    for (int i=0; i<SVC_LAST; ++i)
        if (_tcsicmp( PropertyName[i], Name ) == 0)
            return i;

    return -1;
};

//-----------------------------------------------------
void CServiceControlWnd::LoadRegistry()
{
    HKEY hKey;

    if (RegOpenKey( HKEY_CURRENT_USER, KeyName, &hKey ) != ERROR_SUCCESS)
        hKey = NULL;

    KMultiString Columns;
    if (hKey && GetMultiString( hKey, _T("Columns"), Columns ))
    {
        // read columns from registry
        int nColumns = Columns.GetSize();

        for (int i=0; i<nColumns; ++i)
        {
            int nProperty = GetPropertyByName( Columns[i] );
            if (nProperty != -1)
                m_Columns.Add(nProperty);
        };
    }
    else
    {
        // use default columns
        m_Columns.Add( SVC_INTERNAL_NAME );
        m_Columns.Add( SVC_TYPE );
        m_Columns.Add( SVC_STATE );
        m_Columns.Add( SVC_STARTUP );
        m_Columns.Add( SVC_PATH );
    };

    // initialize widths with default values
    for (int i=0; i<SVC_LAST; ++i)
        m_Widths[i] = DefaultWidth[i];


    KMultiString Widths;
    if (hKey && GetMultiString( hKey, _T("Column Widths"), Widths ))
    {
        // read widths from registry
        int nWidths = Widths.GetSize();
        if (nWidths > SVC_LAST) nWidths = SVC_LAST;

        for (int i=0; i<nWidths; ++i)
        {
            int w = _ttoi(Widths[i]);
            if ((w > 0) || (w<10000))
                m_Widths[i] = w;
        };
    };

    if (hKey)
        RegCloseKey(hKey);
};

//-----------------------------------------------------
void CServiceControlWnd::SaveRegistry()
{
    HKEY hKey;

    if (RegCreateKey( HKEY_CURRENT_USER, KeyName, &hKey ) != ERROR_SUCCESS)
        return;

    SetString( hKey, _T("Version"), _T("1.01"));

    KMultiString Columns;
    int nColumns = m_Columns.GetSize();

    for (int i=0; i<nColumns; ++i)
    {
        Columns.Add( PropertyName[ m_Columns[i] ] );
    };

    SetMultiString( hKey, _T("Columns"), Columns );

    KMultiString Widths;
    for (i=0; i<SVC_LAST; ++i)
    {
        CString S;
        S.Format("%d", m_Widths[i]);
        Widths.Add(S);
    };

    SetMultiString( hKey, _T("Column Widths"), Widths );

    RegCloseKey(hKey);
};
