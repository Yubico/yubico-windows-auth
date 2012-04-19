/* Copyright (c) 2012, Yubico AB.  All rights reserved.
   
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
   TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
   THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   SUCH DAMAGE.
*/

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
