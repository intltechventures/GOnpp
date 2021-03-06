//Copyright (C)2014 tike <timflex@gmx.de>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "CmdDlg.h"
#include "PluginDefinition.h"

extern NppData nppData;

BOOL CALLBACK CmdDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_SIZE:
		{
			//::MessageBox(nppData._nppHandle, _T("foo"), _T("WM_SIZE"), MB_OK);
			switch(wParam)
			{
				case SIZE_RESTORED:
				{
					//::MessageBox(nppData._nppHandle, _T("SIZE_RESTORED"), _T("WM_SIZE"), MB_OK);
					RECT rc;
					::GetWindowRect(_hSelf, &rc);
					HWND hEdit = GetDlgItem(_hSelf, ID_DUMP);
					::MoveWindow(hEdit, 0, 0, rc.right - rc.left, rc.bottom - rc.top, TRUE);
					break;
				}
			}
			return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

void CmdDlg::reshape(int width)
{
	RECT rc;
	::GetWindowRect(_hSelf, &rc);
	::MoveWindow(_hSelf, rc.top, rc.left, width, rc.bottom - rc.top, TRUE);
}

LPTSTR CmdDlg::prettyfy(LPTSTR text)
{
	size_t size = _tcslen(text) + 50;
	LPTSTR res = (LPTSTR) calloc(size+1, sizeof(TCHAR));
	
	LPTSTR rest = NULL;
	LPTSTR current = _tcstok_s(text, _T("\n"), &rest);
	while(current != NULL){
		if (_tcslen(current) > _maxLine){
			_maxLine = _tcslen(current);
		}
		if( _tcslen(current) > size - _tcslen(res)){
			size += 500;
			res = (LPTSTR) realloc(res, size * sizeof(TCHAR));
			if (res == NULL){
				return NULL;
			}
		}
		_tcsncat(res, current, __min(size -_tcslen(res), _tcslen(current)));
		_tcsncat(res, _T("\r\n"), __min(size - _tcslen(res), 2 * sizeof(TCHAR)));
		current = _tcstok_s(NULL, _T("\n"), &rest);
	}
	return res;
}


void CmdDlg::setText(LPTSTR text)
{
	LPTSTR lines = prettyfy(text);
	if ( lines == NULL) return;

	::SetDlgItemText(this->_hSelf, ID_DUMP, lines);
	free(lines);
}

void CmdDlg::appendText(LPTSTR text)
{
	LPTSTR pText = this->prettyfy(text);
	if ( pText == NULL) return;

	HWND hEdit = GetDlgItem(this->_hSelf, ID_DUMP);
	int ndx = GetWindowTextLength(hEdit);
	#ifdef WIN32
      SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
   #else
      SendMessage (hEdit, EM_SETSEL, 0, MAKELONG (ndx, ndx));
   #endif
      SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) ((LPSTR) pText));
	  free(pText);
}
