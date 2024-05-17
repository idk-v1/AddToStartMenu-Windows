#include "File.h"
#include "Inputbox.h"

std::string titleCase(std::string str)
{
	bool space = true;
	for (char& c : str)
	{
		if (islower(c) && space)
			c -= 32;
		space = (c == ' ');
	}

	return str;
}

std::string removeProgSpace(std::string str)
{
	bool lastLower = false;
	for (int i = 0; i < str.size(); i++)
	{
		char& c = str[i];
		if (c == '-' || c == '_' || c == '.')
			c = ' ';

		if (isupper(c) && lastLower)
			str.insert(i, 1, ' ');

		lastLower = islower(c);
	}

	return str;
}

bool stringContains(std::string str, std::string list)
{
	for (char c : list)
		if (str.find(c) != std::string::npos)
			return true;

	return false;
}

int main()
{
	std::string fileName, iconFileName, displayName, param, startMenu;

	if (pickFile(fileName, "Choose Program", 
		"Programs (*.exe,*.pif,*.com,*.bat)\0*.exe;*.pif;*.com;*.bat\0All files\0*.*"))
	{
		displayName = titleCase(removeProgSpace(fileNameFromPath(fileNameNoExt(fileName))));
		displayName = InputBox(
			(char*)fileNameFromPath(fileName).data(), 
			(char*)"Enter Display Name for", 
			(char*)displayName.data());

		while (stringContains(displayName, "\\/*:?\"<>|") || !displayName.size())
		{
			if (displayName.size())
				displayName = InputBox(
					(char*)("Invalid characters: \\ / * : ? \" < > |\n" + fileNameFromPath(fileName)).data(),
					(char*)"Enter Display Name for",
					(char*)displayName.data());
			else
				displayName = InputBox(
					(char*)("Name cannot be empty\n " + fileNameFromPath(fileName)).data(),
					(char*)"Enter Display Name for",
					(char*)titleCase(removeProgSpace(fileNameFromPath(fileNameNoExt(fileName)))).data());
		}

		if (MessageBoxA(0, "Custom Icon?", displayName.data(), MB_YESNO | MB_ICONQUESTION) == IDYES)
			pickFile(iconFileName, "Choose Icon", 
				"Icon files (*.ico*.icl,*.exe,*.dll)\0*.ico;*.icl;*.exe;*.dll\0All files\0*.*\0");

		if (MessageBoxA(0, "Parameters?", displayName.data(), MB_YESNO | MB_ICONQUESTION) == IDYES)
			param = InputBox((char*)fileNameFromPath(fileName).data(), (char*)"Parameters");

		startMenu = getKnownFolder(FOLDERID_StartMenu);

		CreateShortcut(fileName + ' ' + param, startMenu + '/' + displayName, iconFileName);

		printf("%s\n%s\n%s\n%s\n%s\n", fileName.data(), iconFileName.data(), displayName.data(), startMenu.data(), param.data());
	}

	return 0;
}