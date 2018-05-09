#pragma once

#include "../rtl/FilterLib.h"
#include "../rtl/referencedImpl.h"

#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

/*
 * Filter class for consuming inputs
 */
class CConsumer_Filter : public glucose::IFilter, public virtual refcnt::CReferenced {
protected:
	// input pipe
	glucose::SFilter_Pipe mInput;
public:
	CConsumer_Filter(glucose::SFilter_Pipe input_pipe);

	virtual HRESULT Run(const refcnt::IVector_Container<glucose::TFilter_Parameter> *configuration) override final;
};

#pragma warning( pop )
