#pragma once
#include <windows.h>
#include <commdlg.h>
#include<new>
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include<string>
using std::string;
//this function is used when compiler starts 
string writeInString(HWND hwndEdit)
{
	int iLength;
	PSTR pstrBuffer;
	string text;

	iLength = GetWindowTextLength(hwndEdit);
	try {
		pstrBuffer = (PSTR)new char[iLength + 1];
	}
	catch (std::bad_alloc &ba)
	{
		std::cout << ba.what() << std::endl;
		pstrBuffer = NULL;
		iLength = NULL;
	}
	GetWindowText(hwndEdit, pstrBuffer, iLength + 1);
	pstrBuffer[iLength] = '\0';
	text = pstrBuffer;
	delete []pstrBuffer;
	return text;
}

