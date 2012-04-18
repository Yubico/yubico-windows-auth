//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#pragma once

#include "helpers.h"

class CProviderFilter : public ICredentialProviderFilter
{
  public:
    // IUnknown
    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return ++_cRef;
    }
    
    IFACEMETHODIMP_(ULONG) Release()
    {
        LONG cRef = --_cRef;
        if (!cRef)
        {
            delete this;
        }
        return cRef;
    }

	IFACEMETHODIMP QueryInterface(__in REFIID riid, __deref_out void** ppv)
    {
        static const QITAB qit[] =
        {
			QITABENT(CProviderFilter, ICredentialProviderFilter), // IID_ICredentialProviderFilter
            {0},
        };
        return QISearch(this, qit, riid, ppv);
    }

  public:
	IFACEMETHODIMP Filter(CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus,DWORD dwFlags,GUID* rgclsidProviders,BOOL* rgbAllow,DWORD cProviders);
	IFACEMETHODIMP UpdateRemoteCredential(const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcsIn, CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION* pcpcsOut);
    
    friend HRESULT CFilter_CreateInstance(__in REFIID riid, __deref_out void** ppv);

  protected:
    CProviderFilter();
    __override ~CProviderFilter();
    
private:
    LONG                _cRef;
};
