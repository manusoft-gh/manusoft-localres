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

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afx.h>
#include "MPath.h"
#include "LocalRes.h"

typedef CString RString;

#pragma warning(push)
#pragma warning(disable:4073)
#pragma init_seg( lib )
#pragma warning(pop)

static const LPCTSTR DEFAULT_LIBRARY_EXTENSION = _T("DLL");

static MPath GetModuleDirectory();

static class DelayedInitMPath
{
	MPath m_pathModule;
	BOOL bInitialized;
public:
	DelayedInitMPath() : bInitialized( FALSE ) {}
	operator MPath&()
	{
		if( !bInitialized )
		{
			m_pathModule = GetModuleDirectory();
			bInitialized = TRUE;
		}
		return m_pathModule;
	}
} g_pathModule;

static MPath g_pathLocalizedResources;
static MPath g_pathLanguageResources;
static MPath g_pathDefaultResources;


MPath GetModuleDirectory()
{
	MEMORY_BASIC_INFORMATION mbi;
	void* pvEIP;
	__asm
	{
		push eax
		call nextline
	nextline:
		pop eax
		mov pvEIP, eax
		pop eax
	}
	::VirtualQuery( pvEIP, &mbi, sizeof(mbi) );

	RString sModule = _T("");
	GetModuleFileName( (HMODULE)mbi.AllocationBase, sModule.GetBuffer( MAX_PATH ), MAX_PATH );
	sModule.ReleaseBuffer();
	MPath pathModuleDirectory( sModule );
	pathModuleDirectory.SetNameExtension( (LPCTSTR)NULL );
	return pathModuleDirectory;
}


BOOL SetResourceLocale( LPCTSTR pszAbbreviatedLanguage,
												LPCTSTR pszDefaultLanguage /*= NULL*/ )
{
	RString sDefaultLanguage = pszDefaultLanguage;
	if( !sDefaultLanguage.IsEmpty() )
	{
		MPath pathDefault( g_pathModule );
		pathDefault.AppendDirectory( sDefaultLanguage );
		if( pathDefault.DirectoryExists() )
			g_pathDefaultResources = pathDefault;
		else
		{
			pathDefault = g_pathModule;
			pathDefault.AppendDirectory( sDefaultLanguage.Left( 2 ) );
			if( pathDefault.DirectoryExists() )
				g_pathDefaultResources = pathDefault;
		}
	}
	RString sLanguage = pszAbbreviatedLanguage;
	if( sLanguage.IsEmpty() )
	{
		g_pathLocalizedResources.Empty();
		g_pathLanguageResources.Empty();
		return FALSE;
	}
	MPath pathGeneral( g_pathModule );
	pathGeneral.AppendDirectory( sLanguage.Left( 2 ) );
	BOOL bGeneralExists = pathGeneral.DirectoryExists();
	MPath pathSpecific( g_pathModule );
	pathSpecific.AppendDirectory( sLanguage );
	BOOL bSpecificExists = (!(pathSpecific == pathGeneral) && pathSpecific.DirectoryExists());
	if( !(bSpecificExists || bGeneralExists) )
		return FALSE;
	g_pathLocalizedResources = bSpecificExists? (LPCTSTR)pathSpecific : (LPCTSTR)NULL;
	g_pathLanguageResources = bGeneralExists? (LPCTSTR)pathGeneral : (LPCTSTR)NULL;
	return TRUE;
}


RString FindPrivateFolder( LPCTSTR pszFolder )
{
	MPath pathTest( g_pathModule );
	pathTest.AppendDirectory( pszFolder );
	if( pathTest.DirectoryExists() )
		return (const RString&)pathTest;
	return _T("");
}


RString FindLocalizedFolder( LPCTSTR pszFolder )
{
	if( !g_pathLocalizedResources.IsEmpty() )
	{
		MPath pathTest( g_pathLocalizedResources );
		pathTest.AppendDirectory( pszFolder );
		if( pathTest.DirectoryExists() )
			return (const RString&)pathTest;
	}
	if( !g_pathLanguageResources.IsEmpty() )
	{
		MPath pathTest( g_pathLanguageResources );
		pathTest.AppendDirectory( pszFolder );
		if( pathTest.DirectoryExists() )
			return (const RString&)pathTest;
	}
	if( !g_pathDefaultResources.IsEmpty() )
	{
		MPath pathTest( g_pathDefaultResources );
		pathTest.AppendDirectory( pszFolder );
		if( pathTest.DirectoryExists() )
			return (const RString&)pathTest;
	}
	return _T("");
}


RString FindLocalizedPrivateFolder( LPCTSTR pszFolder )
{
	RString sTestFilename = FindLocalizedFolder( pszFolder );
	if( !sTestFilename.IsEmpty() )
		return sTestFilename;
	return FindPrivateFolder( pszFolder );
}




RString FindPrivateFile( LPCTSTR pszFilename, LPCTSTR pszDefExt /*= NULL*/ )
{
	RString sFilename( pszFilename );
	if( sFilename.Find( _T('.') ) <= 0 )
		sFilename = sFilename + _T('.') + pszDefExt;
	MPath pathTest( g_pathModule );
	pathTest.SetNameExtension( sFilename );
	if( pathTest.Exists() )
		return (const RString&)pathTest;
	return _T("");
}


RString FindLocalizedFile( LPCTSTR pszFilename, LPCTSTR pszDefExt /*= NULL*/ )
{
	RString sFilename( pszFilename );
	if( sFilename.Find( _T('.') ) <= 0 )
		sFilename = sFilename + _T('.') + pszDefExt;
	if( !g_pathLocalizedResources.IsEmpty() )
	{
		MPath pathTest( g_pathLocalizedResources );
		pathTest.SetNameExtension( sFilename );
		if( pathTest.Exists() )
			return (const RString&)pathTest;
	}
	if( !g_pathLanguageResources.IsEmpty() )
	{
		MPath pathTest( g_pathLanguageResources );
		pathTest.SetNameExtension( sFilename );
		if( pathTest.Exists() )
			return (const RString&)pathTest;
	}
	if( !g_pathDefaultResources.IsEmpty() )
	{
		MPath pathTest( g_pathDefaultResources );
		pathTest.SetNameExtension( sFilename );
		if( pathTest.Exists() )
			return (const RString&)pathTest;
	}
	return _T("");
}


RString FindLocalizedPrivateFile( LPCTSTR pszFilename, LPCTSTR pszDefExt /*= NULL*/ )
{
	RString sTestFilename = FindLocalizedFile( pszFilename, pszDefExt );
	if( !sTestFilename.IsEmpty() )
		return sTestFilename;
	return FindPrivateFile( pszFilename, pszDefExt );
}


RString FindFileCustom( LPCTSTR pszFilename, LPCTSTR pszDefExt /*= NULL*/ )
{
	RString sTestFilename = FindLocalizedPrivateFile( pszFilename, pszDefExt );
	if( !sTestFilename.IsEmpty() )
		return sTestFilename;
	RString sFilename( pszFilename );
	if( sFilename.Find( _T('.') ) <= 0 )
		sFilename = sFilename + _T('.') + pszDefExt;
	RString sDefExt = _T("");
	if( pszDefExt )
		sDefExt = _T('.') + pszDefExt;
	static TCHAR szPath[MAX_PATH];
	DWORD dwLen = SearchPath( NULL,
														pszFilename,
														pszDefExt? (LPCTSTR)sDefExt : NULL,
														MAX_PATH,
														szPath,
														NULL );
	if( dwLen > 0 && dwLen < MAX_PATH )
		return szPath;
	return _T("");
}


RString FindPrivateLibrary( LPCTSTR pszFilename )
{
	return FindPrivateFile( pszFilename, DEFAULT_LIBRARY_EXTENSION );
}


RString FindLocalizedLibrary( LPCTSTR pszFilename )
{
	return FindLocalizedFile( pszFilename, DEFAULT_LIBRARY_EXTENSION );
}


RString FindLocalizedPrivateLibrary( LPCTSTR pszFilename )
{
	return FindLocalizedPrivateFile( pszFilename, DEFAULT_LIBRARY_EXTENSION );
}


RString FindLibraryCustom( LPCTSTR pszFilename )
{
	return FindFileCustom( pszFilename, DEFAULT_LIBRARY_EXTENSION );
}


HINSTANCE LoadPrivateLibrary( LPCTSTR pszFilename )
{
	RString sFilename = pszFilename;
	if( sFilename.Find( _T('.') ) <= 0 )
		sFilename = sFilename + _T('.') + DEFAULT_LIBRARY_EXTENSION;
	MPath pathTest( g_pathModule );
	pathTest.SetNameExtension( sFilename );
	return LoadLibrary( pathTest );
}


HINSTANCE LoadLocalizedLibrary( LPCTSTR pszFilename )
{
	RString sFilename = pszFilename;
	if( sFilename.Find( _T('.') ) <= 0 )
		sFilename = sFilename + _T('.') + DEFAULT_LIBRARY_EXTENSION;
	HINSTANCE hinstReturn = NULL;
	if( !g_pathLocalizedResources.IsEmpty() )
	{
		MPath pathTest( g_pathLocalizedResources );
		pathTest.SetNameExtension( sFilename );
		hinstReturn = LoadLibrary( pathTest );
		if( hinstReturn )
			return hinstReturn;
	}
	if( !g_pathLanguageResources.IsEmpty() )
	{
		MPath pathTest( g_pathLanguageResources );
		pathTest.SetNameExtension( sFilename );
		hinstReturn = LoadLibrary( pathTest );
		if( hinstReturn )
			return hinstReturn;
	}
	if( !g_pathDefaultResources.IsEmpty() )
	{
		MPath pathTest( g_pathDefaultResources );
		pathTest.SetNameExtension( sFilename );
		hinstReturn = LoadLibrary( pathTest );
		if( hinstReturn )
			return hinstReturn;
	}
	return NULL;
}


HINSTANCE LoadLocalizedPrivateLibrary( LPCTSTR pszFilename )
{
	HINSTANCE hinstReturn = NULL;
	hinstReturn = LoadLocalizedLibrary( pszFilename );
	if( hinstReturn )
		return hinstReturn;
	return LoadPrivateLibrary( pszFilename );
}


HINSTANCE LoadLibraryCustom( LPCTSTR pszFilename )
{
	HINSTANCE hinstReturn = LoadLocalizedPrivateLibrary( pszFilename );
	if( hinstReturn )
		return hinstReturn;
	RString sFilename = pszFilename;
	if( sFilename.Find( _T('.') ) <= 0 )
		sFilename = sFilename + _T('.') + DEFAULT_LIBRARY_EXTENSION;
	return LoadLibrary( sFilename );
}
