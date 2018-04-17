#pragma once

#include "../../../common/SimpleIni.h"

#include "filter_chain.h"

class CConfig {
protected:
	std::wstring mFile_Path;
	// stored filename to be used in e.g. window title
	std::wstring mFile_Name;
	CSimpleIniW mIni;
public:
	void Resolve_And_Load_Config_File();

	void Load(CFilter_Chain &filter_chain);
	void Save(const CFilter_Chain &filter_chain);

	const wchar_t* Get_Config_File_Name() const;
};

extern CConfig Configuration;
