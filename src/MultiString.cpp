#include "stdafx.h"
#include <tchar.h>
#include "MultiString.h"

//----------------------------------------------------------------------
void KMultiString::CopyFromBuffer( LPCTSTR lpszMultiString )
{
    while (*lpszMultiString)
    {
        CStringArray::Add(lpszMultiString); // adds only first substring
        int Length = _tcslen(lpszMultiString) + 1;
        lpszMultiString += Length;
    }
};

//----------------------------------------------------------------------
void KMultiString::CopyFromList( LPCTSTR lpszList, TCHAR chSeparator )
{
    TCHAR Separators[2] = { chSeparator, 0 };

    CString Temp = lpszList;
    TCHAR* Buffer = Temp.GetBuffer(0);

    for ( 
            TCHAR* SubString = _tcstok(Buffer, Separators ); // initial action
            SubString != NULL;                               // terminating condition
            SubString = _tcstok(NULL,Separators)             // moving to next loop
        )
    {
        CStringArray::Add(SubString);
    };

    Temp.ReleaseBuffer();
};

//----------------------------------------------------------------------
CString KMultiString::GetBuffer() const
{
    int const ArraySize = GetSize();

    // special case of empty list
    if (ArraySize == 0)
    {
        CString S;
        TCHAR* Buffer = S.GetBufferSetLength(1);
        *Buffer = 0;
        S.ReleaseBuffer(1);
        return S;
    };

    int BufSize = 0;

    // get sum of lengths of all substrings, including terminating 0s
    for (int Idx=0; Idx < ArraySize; ++Idx)
    {
        BufSize += GetAt(Idx).GetLength() + 1;
    };

    // allocate buffer for BufSize characters (plus final terminating null)
    CString S;
    TCHAR* Buffer = S.GetBufferSetLength(BufSize);

    for (Idx=0; Idx < ArraySize; ++Idx)
    {
        _tcscpy(Buffer, GetAt(Idx));
        Buffer += GetAt(Idx).GetLength() + 1;
    };

    // Cannot call S.ReleaseBuffer() with default length of -1, because
    // our string contains embedded null characters
    S.ReleaseBuffer(BufSize);
    return S;
};

//----------------------------------------------------------------------
CString KMultiString::GetList( TCHAR chSeparator ) const
{
    int const ArraySize = GetSize();

    // special case of empty list
    if (ArraySize == 0)
        return CString(); // return empty string

    int BufSize = 0;

    // get sum of lengths of all substrings + one separator character per string
    for (int Idx=0; Idx < ArraySize; ++Idx)
    {
        BufSize += GetAt(Idx).GetLength() + 1;
    };

    // last string does not require separator character, so decrease overall
    // buffer length by 1
    --BufSize;
    
    // allocate buffer for BufSize characters (plus final terminating null)
    CString S;
    TCHAR* Buffer = S.GetBufferSetLength(BufSize);

    for (Idx=0; Idx < ArraySize; ++Idx)
    {
        if (Idx != 0)
        {
            *Buffer = chSeparator;
            ++Buffer;
        };

        _tcscpy(Buffer, GetAt(Idx));
        Buffer += GetAt(Idx).GetLength();
    };

    S.ReleaseBuffer();
    return S;
};
