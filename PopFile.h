#pragma once
#include <windows.h>
#include <commdlg.h>
#include<new>
#include<cstdlib>
#include<iostream>
#include<fstream>
using std::ifstream;
using std::ios_base;
using std::ofstream;
static OPENFILENAME ofn;

void PopFileInitialize(HWND hwnd)
{
	static char szFilter[] = "Text Files(*.TXT)\0*.txt\0" \
		"ASCII Files(*.ASC)\0*.asc\0" \
		"All Files(*.*)\0*.*\0\0";
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = NULL; // Set in Open and Close functions
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = NULL; // Set in Open and Close functions
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = 0; // Set in Open and Close functions
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = "txt";
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
}

BOOL PopFileOpenDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName)
{
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	return GetOpenFileName(&ofn);
}

BOOL PopFileSaveDlg(HWND hwnd, PSTR pstrFileName, PSTR pstrTitleName)
{
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	return GetSaveFileName(&ofn);
}

BOOL PopFileRead(HWND hwndEdit, PSTR pstrFileName)
{
	std::streamoff iLength;
	PSTR pstrBuffer;
	ifstream ifile(pstrFileName, ios_base::in | ios_base::ate | ios_base::binary);
	if (!ifile.is_open())
		return FALSE;
	iLength = ifile.tellg();
	ifile.seekg(0, ios_base::beg);
	try {
		pstrBuffer = (PSTR) new char[(long)iLength + 1];  
		ifile.read(pstrBuffer, iLength);
		ifile.close();
		pstrBuffer[(long)iLength] = '\0';
		SetWindowText(hwndEdit, pstrBuffer);
		delete[]pstrBuffer;
		return TRUE;
	}
	catch (std::bad_alloc &ba)
	{
		std::cout << ba.what() << std::endl;
		iLength = NULL;
		return FALSE;
	}
	
}
BOOL PopFileWrite(HWND hwndEdit, PSTR pstrFileName)
{
	ofstream ofile(pstrFileName, ios_base::out | ios_base::binary);
	int iLength;
	PSTR pstrBuffer;
	if (!ofile.is_open())
		return FALSE;
	iLength = GetWindowTextLength(hwndEdit);
	try {
		pstrBuffer = (PSTR)new char[iLength + 1];
		GetWindowText(hwndEdit, pstrBuffer, iLength + 1);
		ofile.write(pstrBuffer, iLength);
		ofile.close();
		delete[]pstrBuffer;
		return TRUE;
	}
	catch (std::bad_alloc &ba)
	{
		std::cout << ba.what() << std::endl;
		iLength = NULL;
		return FALSE;
	}
	
}
