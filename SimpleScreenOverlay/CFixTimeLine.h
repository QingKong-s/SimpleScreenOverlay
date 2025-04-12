#pragma once
struct CFixTimeLine : public eck::ITimeLine
{
	ULONG STDMETHODCALLTYPE AddRef() { return 1; }
	ULONG STDMETHODCALLTYPE Release() { return 1; }
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) { return E_NOINTERFACE; }

	BOOL STDMETHODCALLTYPE IsValid() { return TRUE; }
	int STDMETHODCALLTYPE GetCurrTickInterval() { return 0; }
};