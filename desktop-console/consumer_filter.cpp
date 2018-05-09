#include "consumer_filter.h"


CConsumer_Filter::CConsumer_Filter(glucose::SFilter_Pipe input_pipe) : mInput(input_pipe) {

}

HRESULT CConsumer_Filter::Run(const refcnt::IVector_Container<glucose::TFilter_Parameter> *configuration) {
	glucose::TDevice_Event evt;

	while (mInput->receive(&evt) == S_OK) 
		// just read and do nothing - this effectively consumes any incoming event through pipe
		glucose::Release_Event(evt);

	return S_OK;
};
