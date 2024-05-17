#pragma once

#include <Windows.h>
#include <string>
#include <ShObjIdl.h>
#include <ShlGuid.h>
#include <shlobj_core.h>

bool selectFile(std::string& pPath, std::string title, const char* filter, DWORD flags)
{
	OPENFILENAMEA ofn;
	char fileName[MAX_PATH] = { 0 }, currentDir[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, currentDir);

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = 0;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = (LPSTR)title.data();
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = flags;

	if (GetOpenFileNameA(&ofn))
	{
		pPath = ofn.lpstrFile;
		SetCurrentDirectoryA(currentDir);
		return true;
	}

	SetCurrentDirectoryA(currentDir);
	return false;
}

bool pickFile(std::string& pPath, std::string title = "", const char* filter = nullptr)
{
	return selectFile(pPath, title, filter, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
}

void CreateShortcut(std::string pSrc, std::string pDst, std::string pIconPath = "")
{
	HRESULT hres;
	IShellLinkA* psl = nullptr;

	CoInitialize(NULL);

	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkA, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf = nullptr;

		psl->SetPath(pSrc.data());
		if (pIconPath.size())
			psl->SetIconLocation(pIconPath.data(), 0);
		else
			psl->SetIconLocation(pSrc.data(), 0);

		// Save link
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (SUCCEEDED(hres))
		{
			pDst += ".lnk";
			hres = ppf->Save(std::wstring(pDst.begin(), pDst.end()).data(), TRUE);
			ppf->Release();
		}

		psl->Release();
	}
}

std::string fileNameFromPath(std::string pPath)
{
	size_t pos = pPath.find_last_of('\\');
	if (pos == std::string::npos)
		pos = pPath.find_last_of('/');
	if (pos != std::string::npos)
		return pPath.substr(pos + 1);
	return pPath;
}

std::string fileNameNoExt(std::string pPath)
{
	size_t pos = pPath.find_last_of('.');;
	if (pos != std::string::npos)
		return pPath.substr(0, pos);
	return pPath;
}

std::string getKnownFolder(GUID guid)
{
	std::wstring pathStr;
	PWSTR path[MAX_PATH];
	SHGetKnownFolderPath(guid, 0, NULL, path);
	pathStr = path[0];
	return std::string(pathStr.begin(), pathStr.end());
}