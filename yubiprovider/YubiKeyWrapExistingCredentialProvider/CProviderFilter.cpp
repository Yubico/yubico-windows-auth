#include "CProviderFilter.h"
#include <wincred.h>

CProviderFilter::CProviderFilter(void)
{
}

CProviderFilter::~CProviderFilter(void)
{
}


HRESULT CProviderFilter::Filter(CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus,DWORD dwFlags,GUID* rgclsidProviders,BOOL* rgbAllow,DWORD cProviders)
{
	UNREFERENCED_PARAMETER(dwFlags);
	switch (cpus)
	{
	case CPUS_CREDUI:
		if(dwFlags & CREDUIWIN_GENERIC) {
			return S_OK;
		}
	case CPUS_LOGON:
	case CPUS_UNLOCK_WORKSTATION:
	case CPUS_CHANGE_PASSWORD:
		//Filters out the default Windows provider (only for Logon and Unlock scenarios)
		for (DWORD i = 0; i < cProviders; i++)
		{
			if (IsEqualGUID(rgclsidProviders[i], CLSID_PasswordCredentialProvider)) {
				rgbAllow[i] = FALSE;
			}
		}
		return S_OK;
	default:
		return E_INVALIDARG;
	}
}

HRESULT CProviderFilter::UpdateRemoteCredential(const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcsIn, CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcsOut)

{
	UNREFERENCED_PARAMETER(pcpcsOut);
	UNREFERENCED_PARAMETER(pcpcsIn);
    return E_NOTIMPL;
}


// Boilerplate code to create our provider.
HRESULT CFilter_CreateInstance(__in REFIID riid, __deref_out void** ppv)
{
    HRESULT hr;

	CProviderFilter* pFilter = new CProviderFilter();

    if (pFilter)
    {
        hr = pFilter->QueryInterface(riid, ppv);
		// TODO: this release is probably needed to not leak memory, but right now we crash with it.
        //pFilter->Release();
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }
    
    return hr;
}
