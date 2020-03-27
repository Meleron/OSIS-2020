#pragma once
#include <string>
#include <vector>
#define MAX_KEY_LENGTH 2048
#define MAX_VALUE_NAME 16383
#define MAX_LOADSTRING 100
#define ID_LIST_RESULTS 1001
#define ID_EDIT_QUERY 1002
#define ID_BTN_SEARCH 1003
#define ID_BTN_CANCEL 1004
#define ID_CHECK_HKLM 1005
#define ID_CHECK_HKCR 1006
#define ID_CHECK_HKCU 1007
#define ID_CHECK_HKU 1008
#define ID_CHECK_HKCC 1009

class RegistryService {
private:
	std::vector<std::wstring> searchResults;
	std::wstring query;
	HWND resultList;

	int deep = 0;

	void searchInKey(HKEY hKey, std::wstring path);

	std::wstring getKeyPath(HKEY key);
	bool isMatch(std::wstring str);
	std::wstring toLower(std::wstring str);
	void addToList(HWND hList, std::wstring str);
public:
	void search(std::wstring query, std::vector<HKEY> searchKeys);
	RegistryService(HWND hList);
};

