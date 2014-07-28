///////////////////////////////////////////////////////////////////////////////
// Dynamic Audio Normalizer
// Copyright (C) 2014 LoRd_MuldeR <MuldeR2@GMX.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version, but always including the *additional*
// restrictions defined in the "License.txt" file.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// http://www.gnu.org/licenses/gpl-2.0.txt
///////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int main(int argc, char* argv[]);

//===================================================================
// WinMain entry point
//===================================================================

#ifdef _DEBUG
#define RELEASE_BUILD 0
#else
#define RELEASE_BUILD 1
#endif

extern "C"
{
	int mainCRTStartup(void);

	int win32EntryPoint(void)
	{
		if(RELEASE_BUILD)
		{
			BOOL debuggerPresent = TRUE;
			if(!CheckRemoteDebuggerPresent(GetCurrentProcess(), &debuggerPresent))
			{
				debuggerPresent = FALSE;
			}
			if(debuggerPresent || IsDebuggerPresent())
			{
				MessageBoxW(NULL, L"Not a debug build. Unload debugger and try again!", L"Debugger", MB_TOPMOST | MB_ICONSTOP);
				return -1;
			}
			else
			{
				return mainCRTStartup();
			}
		}
		else
		{
			return mainCRTStartup();
		}
	}
}

#endif //_WIN32
