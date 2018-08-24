#include "filter_chain.h"

#include "../rtl/FilterLib.h"

CFilter_Configuration::CFilter_Configuration() noexcept : std::vector<glucose::TFilter_Parameter>() {
}

CFilter_Configuration::CFilter_Configuration(const CFilter_Configuration &other) : std::vector<glucose::TFilter_Parameter>(other) {
	Traverse_Configuration([](refcnt::IReferenced *obj) { obj->AddRef(); });
}

CFilter_Configuration::~CFilter_Configuration() {
	Traverse_Configuration([](refcnt::IReferenced *obj) { obj->Release(); });	
}

void CFilter_Configuration::Traverse_Configuration(std::function<void(refcnt::IReferenced *obj)> func) {	
	for (auto &param : *this) 
		glucose::Visit_Filter_Parameter(param, func);
}

CFilter_Configuration& CFilter_Configuration::operator=(const CFilter_Configuration& other) {	
	Traverse_Configuration([](refcnt::IReferenced *obj) { obj->Release(); });	//release what we are about to replace

	std::vector<glucose::TFilter_Parameter>::operator=(other);				//replace the raw content

	Traverse_Configuration([](refcnt::IReferenced *obj) { obj->AddRef(); });	//and add ref on new content

	return *this;
}


void CFilter_Configuration::push_back(const glucose::TFilter_Parameter &parameter) {
	std::vector<glucose::TFilter_Parameter>::push_back(parameter);
	glucose::Visit_Filter_Parameter(*rbegin(), [](refcnt::IReferenced *obj) { obj->AddRef(); });
}