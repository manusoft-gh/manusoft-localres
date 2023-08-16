// Copyright 2002 ManuSoft
// https://www.manusoft.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _LOCALRES_H_
#define _LOCALRES_H_


BOOL SetResourceLocale( LPCTSTR pszAbbreviatedLanguage, LPCTSTR pszDefaultLanguage = NULL );
class RString FindPrivateFolder( LPCTSTR pszFolder );
class RString FindLocalizedFolder( LPCTSTR pszFolder );
class RString FindLocalizedPrivateFolder( LPCTSTR pszFolder );
class RString FindPrivateFile( LPCTSTR pszFilename, LPCTSTR pszDefExt = NULL );
class RString FindLocalizedFile( LPCTSTR pszFilename, LPCTSTR pszDefExt = NULL );
class RString FindLocalizedPrivateFile( LPCTSTR pszFilename, LPCTSTR pszDefExt = NULL );
class RString FindFileCustom( LPCTSTR pszFilename, LPCTSTR pszDefExt = NULL );
class RString FindPrivateLibrary( LPCTSTR pszFilename );
class RString FindLocalizedLibrary( LPCTSTR pszFilename );
class RString FindLocalizedPrivateLibrary( LPCTSTR pszFilename );
class RString FindLibraryCustom( LPCTSTR pszFilename );
HINSTANCE LoadPrivateLibrary( LPCTSTR pszFilename );
HINSTANCE LoadLocalizedLibrary( LPCTSTR pszFilename );
HINSTANCE LoadLocalizedPrivateLibrary( LPCTSTR pszFilename );
HINSTANCE LoadLibraryCustom( LPCTSTR pszFilename );


class CAutoLoadLibrary
{
	HINSTANCE m_hinstLibrary;
public:
	CAutoLoadLibrary( LPCTSTR pszFilename )
		:	m_hinstLibrary( pszFilename? LoadLibraryCustom( pszFilename ) : NULL )
	{}
	virtual ~CAutoLoadLibrary() { FreeLibrary( m_hinstLibrary ); }
	operator HINSTANCE() const { return m_hinstLibrary; }
};


class CAutoLoadLocalizedLibrary
{
	HINSTANCE m_hinstLibrary;
public:
	CAutoLoadLocalizedLibrary( LPCTSTR pszFilename )
		:	m_hinstLibrary( pszFilename? LoadLocalizedLibrary( pszFilename ) : NULL )
	{}
	virtual ~CAutoLoadLocalizedLibrary() { FreeLibrary( m_hinstLibrary ); }
	operator HINSTANCE() const { return m_hinstLibrary; }
};


class CAutoLoadPrivateLibrary
{
	HINSTANCE m_hinstLibrary;
public:
	CAutoLoadPrivateLibrary( LPCTSTR pszFilename )
		:	m_hinstLibrary( pszFilename? LoadPrivateLibrary( pszFilename ) : NULL )
	{}
	virtual ~CAutoLoadPrivateLibrary() { FreeLibrary( m_hinstLibrary ); }
	operator HINSTANCE() const { return m_hinstLibrary; }
};


#endif // _LOCALRES_H_
