//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// CWrappedCredentialEvents is our implementation of ICredentialProviderCredentialEvents (ICPCE).
// Most credential provider authors will not need to implement this interface,
// but a credential provider that wraps another (as this sample does) must.
// The wrapped credential will pass its "this" pointer into any calls to ICPCE,
// but LogonUI will not recognize the wrapped "this" pointer as a valid credential.
// Our implementation translates from the wrapped "this" pointer to the wrapper "this".

/* This file is based on sample code from the Microsoft Windows SDK
    version 7.1. All changes are Copyright (c) 2012, Yubico AB.
    All rights reserved.
   
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


#pragma once

#include <windows.h>
#include <strsafe.h>
#include <shlguid.h>
#include "helpers.h"
#include "dll.h"

class CWrappedCredentialEvents : public ICredentialProviderCredentialEvents
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

    IFACEMETHODIMP QueryInterface(__in REFIID riid, __in void** ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(CWrappedCredentialEvents, ICredentialProviderCredentialEvents), // IID_ICredentialProviderCredentialEvents
            {0},
        };
        return QISearch(this, qit, riid, ppv);
    }
    
    // ICredentialProviderCredentialEvents
    IFACEMETHODIMP SetFieldState(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in CREDENTIAL_PROVIDER_FIELD_STATE cpfs);
    IFACEMETHODIMP SetFieldInteractiveState(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in CREDENTIAL_PROVIDER_FIELD_INTERACTIVE_STATE cpfis);
    IFACEMETHODIMP SetFieldString(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in PCWSTR psz);
    IFACEMETHODIMP SetFieldCheckbox(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in BOOL bChecked, __in PCWSTR pszLabel);
    IFACEMETHODIMP SetFieldBitmap(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in HBITMAP hbmp);
    IFACEMETHODIMP SetFieldComboBoxSelectedItem(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in DWORD dwSelectedItem);
    IFACEMETHODIMP DeleteFieldComboBoxItem(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in DWORD dwItem);
    IFACEMETHODIMP AppendFieldComboBoxItem(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in PCWSTR pszItem);
    IFACEMETHODIMP SetFieldSubmitButton(__in ICredentialProviderCredential *pcpc, __in DWORD dwFieldID, __in DWORD dwAdjacentTo);
    IFACEMETHODIMP OnCreatingWindow(__out HWND *phwndOwner);

    // Local
    CWrappedCredentialEvents();

    void Initialize(__in ICredentialProviderCredential* pWrapperCredential, __in ICredentialProviderCredentialEvents* pEvents);
    void Uninitialize();

private:
    LONG                                 _cRef;
    ICredentialProviderCredential*       _pWrapperCredential;
    ICredentialProviderCredentialEvents* _pEvents;
};
