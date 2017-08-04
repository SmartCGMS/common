#pragma once

#include "../iface/referencedIface.h"

#include <atomic>


class CReferenced : public virtual IReferenced {    
protected:
	std::atomic<ULONG> mCounter;
public:
	CReferenced() : mCounter(0) {};
	virtual ~CReferenced() {};

	virtual HRESULT IfaceCalling QueryInterface(/*REFIID */ void*  riid, void ** ppvObj);
	virtual ULONG IfaceCalling AddRef();
	virtual ULONG IfaceCalling Release();
};


class CNotReferenced : public CReferenced {
public:
	virtual ULONG IfaceCalling AddRef();
	virtual ULONG IfaceCalling Release();
};
