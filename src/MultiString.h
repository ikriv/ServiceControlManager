#ifndef __MULTI_STRING_H__
#define __MULTI_STRING_H__

// class that encapsulates array of null-terminated strings,
// terminated by two null characters
class KMultiString: public CStringArray
{
    public:

        // CopyFromBuffer()
        //
        // lpszMultiString must point to array of null-terminated strings, 
        // terminated by two  null characters;
        //
        // CopyFromBuffer() copies substrings from lpszMultiString to the
        // string array; any strings that were in the array before the call
        // are deleted
        void CopyFromBuffer( LPCTSTR lpszMultiString );

        // CopyFromList()
        //
        // lpszList must point to a single null-termianted strings that
        // contains substrings separated by chSeparator character;
        //
        // CopyFromList() copies substrings from lpszList to the
        // string array; any strings that were in the array before 
        // the call are deleted
        void CopyFromList( LPCTSTR lpszList, TCHAR chSeparator  = _T(';') );

        // GetBuffer() creates CString object that contains array of 
        // null-terminated strings, terminated by two null characters; 
        // this CString object may be converted to LPCTSTR in order to 
        // pass it to Win32 function that accepts multi-string as parameter
        CString GetBuffer() const;

        // GetList() creates CString object that contains "normal" string
        // where substrings of the multi-string are separated by chSeparator 
        // character
        CString GetList( TCHAR chSeparator = _T(';') ) const;
};
        
#endif        