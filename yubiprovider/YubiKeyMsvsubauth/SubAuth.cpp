/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1987 - 2000.  Microsoft Corporation.  All rights reserved.

Module Name:
    subauth.c

Abstract:
    Sample SubAuthentication Package.

Environment:
    User mode only.
    Contains NT-specific code.
    Requires ANSI C extensions: slash-slash comments, long external names.

--*/

//#define _X86_


#if ( _MSC_VER >= 800 )
#pragma warning ( 3 : 4100 ) // enable "Unreferenced formal parameter"
#pragma warning ( 3 : 4219 ) // enable "trailing ',' used for variable argument list"
#endif


#include <Windows.h>
#include <lmcons.h>
#include <lmaccess.h>
#include <lmapibuf.h>
#include <subauth.h>

#include <string>


#include "YkLib.h"

bool DoChallengeResponse(__in BYTE* challenge, __out BYTE* response, __in DWORD len);

bool GetSalt(__in LPWSTR username, __out BYTE* salt, __out DWORD* len);

bool GetNextChallenge(__in LPWSTR username, __out BYTE* challenge, __out DWORD* len);

bool GetNextResponse(__in LPWSTR username, __out BYTE* response, __out DWORD* len);

bool SetNextChallengeAndResponse(__in LPWSTR username, __in BYTE* salt, __in DWORD len);

bool HashData(__inout BYTE* data, __inout DWORD* dataLen, __in BYTE* salt, __in DWORD saltLen, __in int times);

bool CheckEnabled(__in LPWSTR username);

bool GetSafeBootEnabled();

bool GetEnabled();

bool GetLoggingEnabled();

bool GetBoolSetting(__in LPSTR setting);

int GetIterations();

static BOOL WriteLogFile(__in LPWSTR String);

static LPWSTR settingsKey = L"SOFTWARE\\Yubico\\auth\\settings";

NTSTATUS
NTAPI
Msv1_0SubAuthenticationRoutine (
    IN NETLOGON_LOGON_INFO_CLASS LogonLevel,
    IN PVOID LogonInformation,
    IN ULONG Flags,
    IN PUSER_ALL_INFORMATION UserAll,
    OUT PULONG WhichFields,
    OUT PULONG UserFlags,
    OUT PBOOLEAN Authoritative,
    OUT PLARGE_INTEGER LogoffTime,
    OUT PLARGE_INTEGER KickoffTime
)
/*++

Routine Description:

    The subauthentication routine does client/server specific authentication
    of a user. The credentials of the user are passed in addition to all the
    information from SAM defining the user. This routine decides whether to
    let the user log on.


Arguments:

    LogonLevel -- Specifies the level of information given in
        LogonInformation.

    LogonInformation -- Specifies the description for the user
        logging on.  The LogonDomainName field should be ignored.

    Flags - Flags describing the circumstances of the logon.

        MSV1_0_PASSTHRU -- This is a PassThru authenication.  (i.e., the
            user isn't connecting to this machine.)
        MSV1_0_GUEST_LOGON -- This is a retry of the logon using the GUEST
            user account.

    UserAll -- The description of the user as returned from SAM.

    WhichFields -- Returns which fields from UserAllInfo are to be written
        back to SAM.  The fields will only be written if MSV returns success
        to it's caller.  Only the following bits are valid.

        USER_ALL_PARAMETERS - Write UserAllInfo->Parameters back to SAM.  If
            the size of the buffer is changed, Msv1_0SubAuthenticationRoutine
            must delete the old buffer using MIDL_user_free() and reallocate the
            buffer using MIDL_user_allocate().

    UserFlags -- Returns UserFlags to be returned from LsaLogonUser in the
        LogonProfile.  The following bits are currently defined:


            LOGON_GUEST -- This was a guest logon
            LOGON_NOENCRYPTION -- The caller didn't specify encrypted credentials

        SubAuthentication packages should restrict themselves to returning
        bits in the high order byte of UserFlags.  However, this convention
        isn't enforced giving the SubAuthentication package more flexibility.

    Authoritative -- Returns whether the status returned is an
        authoritative status which should be returned to the original
        caller.  If not, this logon request may be tried again on another
        domain controller.  This parameter is returned regardless of the
        status code.

    LogoffTime - Receives the time at which the user should log off the
        system.  This time is specified as a GMT relative NT system time.

    KickoffTime - Receives the time at which the user should be kicked
        off the system. This time is specified as a GMT relative system
        time.  Specify, a full scale positive number if the user isn't to
        be kicked off.

Return Value:

    STATUS_SUCCESS: if there was no error.

    STATUS_NO_SUCH_USER: The specified user has no account.
    STATUS_WRONG_PASSWORD: The password was invalid.

    STATUS_INVALID_INFO_CLASS: LogonLevel is invalid.
    STATUS_ACCOUNT_LOCKED_OUT: The account is locked out
    STATUS_ACCOUNT_DISABLED: The account is disabled
    STATUS_ACCOUNT_EXPIRED: The account has expired.
    STATUS_PASSWORD_MUST_CHANGE: Account is marked as Password must change
        on next logon.
    STATUS_PASSWORD_EXPIRED: The Password is expired.
    STATUS_INVALID_LOGON_HOURS - The user is not authorized to log on at
        this time.
    STATUS_INVALID_WORKSTATION - The user is not authorized to log on to
        the specified workstation.

--*/
{
    *Authoritative = TRUE;
    *UserFlags = 0;
    *WhichFields = 0;

	BYTE response[128];
	wchar_t userName[128];
	memset(userName, 0, 128);
	memcpy(userName, UserAll->UserName.Buffer, UserAll->UserName.Length);

	if(!GetEnabled()) {
		WriteLogFile(L"not enabled.\r\n");
		*Authoritative = FALSE;
		return STATUS_SUCCESS;
	}
	else if(GetSystemMetrics(SM_CLEANBOOT) > 0 && !GetSafeBootEnabled()) {
		WriteLogFile(L"safeboot and not enabled.\r\n");
		*Authoritative = FALSE;
		return STATUS_SUCCESS;
	}
	else if(!CheckEnabled(userName)) {
		WriteLogFile(L"yubikey auth not enabled for user.\r\n");
		*Authoritative = FALSE;
		return STATUS_SUCCESS;
	}
	BYTE challenge[128];
	DWORD chalLen = 128;
	bool res = GetNextChallenge(userName, challenge, &chalLen);
	if(res == false)  {
		WriteLogFile(L"failed to get challenge.\r\n");
		return STATUS_SUCCESS;
	}
	BYTE correctResponse[128];
	DWORD responseLen = 128;
	res = GetNextResponse(userName, correctResponse, &responseLen);
	if(res == false) {
		WriteLogFile(L"failed to get response.\r\n");
		return STATUS_SUCCESS;
	}
	BYTE salt[128];
	DWORD saltLen = 128;
	res = GetSalt(userName, salt, &saltLen);

	if(DoChallengeResponse(challenge, response, chalLen) == false) {
		WriteLogFile(L"Failed challenge response.\r\n");
		return (NTSTATUS)0xFFFF0001L;
	}
	DWORD hashLen = SHA1_DIGEST_SIZE;
	HashData(response, &hashLen, salt, saltLen, GetIterations());
	if(correctResponse != NULL && !memcmp(response, correctResponse, hashLen)) {
		SetNextChallengeAndResponse(userName, salt, saltLen);
		WriteLogFile(L"Successful login.\r\n");
		return STATUS_SUCCESS;
	} else {
		WriteLogFile(L"Failure.\r\n");
		//SetNextChallengeAndResponse(userName); // uncomment this and login to make a new key work.
		//return STATUS_SUCCESS;
		return (NTSTATUS)0xFFFF0002L;
	}
}  // Msv1_0SubAuthenticationRoutine


NTSTATUS
NTAPI
Msv1_0SubAuthenticationFilter (
    IN NETLOGON_LOGON_INFO_CLASS LogonLevel,
    IN PVOID LogonInformation,
    IN ULONG Flags,
    IN PUSER_ALL_INFORMATION UserAll,
    OUT PULONG WhichFields,
    OUT PULONG UserFlags,
    OUT PBOOLEAN Authoritative,
    OUT PLARGE_INTEGER LogoffTime,
    OUT PLARGE_INTEGER KickoffTime
)
{
    return( Msv1_0SubAuthenticationRoutine(
                LogonLevel,
                LogonInformation,
                Flags,
                UserAll,
                WhichFields,
                UserFlags,
                Authoritative,
                LogoffTime,
                KickoffTime
                ) );
}


#define LOGFILE L"C:\\yubikey_logon_log.txt"

static BOOL
WriteLogFile(__in LPWSTR String)
{
	if(!GetLoggingEnabled()) {
		return TRUE;
	}

    HANDLE hFile;
    DWORD dwBytesWritten;

    hFile = CreateFile(LOGFILE,
               GENERIC_WRITE,
               0,
               NULL,
               OPEN_ALWAYS,
               FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,
               NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    return FALSE;

    SetFilePointer(hFile, 0, NULL, FILE_END);

    WriteFile(hFile,
          String,
          (lstrlen(String) * sizeof(WCHAR)),
          &dwBytesWritten,
          NULL);

    CloseHandle(hFile);

    return TRUE;
}


bool DoChallengeResponse(__in BYTE* challenge, __out BYTE* response, __in DWORD len)
{
	CYkLib yk;
	bool res = false;
	unsigned short timer;
	YKLIB_RC rc = yk.openKey();

	if(rc == YKLIB_OK) {
		rc = yk.writeChallengeBegin(YKLIB_SECOND_SLOT, YKLIB_CHAL_HMAC, challenge, len);
		if(rc == YKLIB_OK) {
			for(;;) {
				rc = yk.waitForCompletion(YKLIB_MAX_CHAL_WAIT, response, SHA1_DIGEST_SIZE, &timer);
				if(rc == YKLIB_TIMER_WAIT || rc == YKLIB_PROCESSING) {
					Sleep(100);
					continue;
				} else {
					break;
				}
			}
			if (rc == YKLIB_OK) {
				res = true;
			} else {
				wchar_t foo[128];
				wsprintf(foo, L"failed: %i\r\n", rc);
				WriteLogFile(foo);
			}
		}
		yk.closeKey();
	}
	return res;
}


bool GetSalt(__in LPWSTR username, __out BYTE* salt, __out DWORD* len) {
	std::wstring subKey = L"SOFTWARE\\Yubico\\auth\\users\\";
	subKey += username;

	HKEY key;
	DWORD type = REG_BINARY;

	int res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_READ, &key);
	if(res == ERROR_SUCCESS) {
		res = RegQueryValueExA(key, "salt", NULL, &type, (LPBYTE)salt, len);
		if(res != ERROR_SUCCESS) {
			wchar_t kaka[100];
			wsprintf(kaka, L"kaka: %i\r\n", res);
			WriteLogFile(kaka);
		}
		RegCloseKey(key);
		return true;
	} else {
		WriteLogFile(L"Couldn't open\r\n");
	}
	
	return false;
}

bool GetNextChallenge(__in LPWSTR username, __out BYTE* challenge, __out DWORD* len) {
	std::wstring subKey = L"SOFTWARE\\Yubico\\auth\\users\\";
	subKey += username;

	HKEY key;
	DWORD type = REG_BINARY;

	int res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_READ, &key);
	if(res == ERROR_SUCCESS) {
		res = RegQueryValueExA(key, "nextChallenge", NULL, &type, (LPBYTE)challenge, len);
		if(res != ERROR_SUCCESS) {
			wchar_t kaka[100];
			wsprintf(kaka, L"foo: %i\r\n", res);
			WriteLogFile(kaka);
		}
		RegCloseKey(key);
		return true;
	} else {
		WriteLogFile(L"Couldn't open\r\n");
	}
	
	return false;
}

bool GetNextResponse(__in LPWSTR username, __out BYTE* response, __out DWORD* len) {
	std::wstring subKey = L"SOFTWARE\\Yubico\\auth\\users\\";
	subKey += username;

	HKEY key;
	DWORD type = REG_BINARY;

	int res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_READ, &key);
	if(res == ERROR_SUCCESS) {
		res = RegQueryValueExA(key, "nextResponse", NULL, &type, (LPBYTE)response, len);
		if(res != ERROR_SUCCESS) {
			wchar_t kaka[100];
			wsprintf(kaka, L"foo: %i\r\n", res);
			WriteLogFile(kaka);
		}
		RegCloseKey(key);
		return true;
	} else {
		WriteLogFile(L"Couldn't open\r\n");
	}
	
	return false;
}

bool SetNextChallengeAndResponse(__in LPWSTR username, __in BYTE* salt, __in DWORD saltLen) {
	BYTE challenge[128];
	memset(challenge, 0, 128);

	BCryptGenRandom(NULL, challenge, 63, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	
	std::wstring subKey = L"SOFTWARE\\Yubico\\auth\\users\\";
	subKey += username;

	BYTE response[128];
	bool chalRes = DoChallengeResponse(challenge, response, 63);

	HKEY key;

	DWORD type = REG_BINARY;
	DWORD hashLen = SHA1_DIGEST_SIZE;
	HashData(response, &hashLen, salt, saltLen, GetIterations());
	int res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_ALL_ACCESS, &key);
	if(chalRes == true && res == ERROR_SUCCESS) {
		RegSetValueExA(key, "nextChallenge", 0, type, challenge, 63);
		RegSetValueExA(key, "nextResponse", 0, type, response, hashLen);
		RegCloseKey(key);
	} else if(chalRes == false) {
		WriteLogFile(L"Challenge response failed, keeping old values.\n");
	} else {
		WriteLogFile(L"Failed opening.\r\n");
	}
	return true;
}

bool HashData(__inout BYTE* data, __inout DWORD* dataLen, __in BYTE* salt, __in DWORD saltLen, __in int times) {
	BCRYPT_ALG_HANDLE hAlg;

	BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA1_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG);
	BCryptDeriveKeyPBKDF2(hAlg, data, *dataLen, salt, saltLen, times, data, 128, 0);
	*dataLen = 128;
	BCryptCloseAlgorithmProvider(hAlg, 0);
	return true;
}

bool CheckEnabled(__in LPWSTR username) {
	std::wstring subKey = L"SOFTWARE\\Yubico\\auth\\users\\";
	subKey += username;

	HKEY key;
	DWORD type = REG_DWORD;
	DWORD enabled = 0;
	DWORD dwLen = sizeof(DWORD);

	int res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_READ, &key);
	if(res == ERROR_SUCCESS) {
		res = RegQueryValueExA(key, "enabled", NULL, &type, (LPBYTE)&enabled, &dwLen);
		if(res != ERROR_SUCCESS) {
			wchar_t kaka[100];
			wsprintf(kaka, L"foo: %i\r\n", res);
			WriteLogFile(kaka);
		}
		RegCloseKey(key);
		if(enabled == 1) {
			return true;
		}
	} else {
		WriteLogFile(L"Couldn't open\r\n");
	}
	
	return false;
}

int GetIterations() {
	HKEY key;
	DWORD type = REG_DWORD;
	DWORD iterations = 0;
	DWORD dwLen = sizeof(DWORD);
	int res = 50000;

	int result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, settingsKey, 0, KEY_READ, &key);
	if(result == ERROR_SUCCESS) {
		result = RegQueryValueExA(key, "hashIterations", NULL, &type, (LPBYTE)&iterations, &dwLen);
		if(result == ERROR_SUCCESS) {
			wchar_t foo[128];
			res = iterations;
		}
		RegCloseKey(key);
	}
	return res;
}

bool GetSafeBootEnabled() {
	return GetBoolSetting("safemodeEnabled");
}

bool GetEnabled() {
	return GetBoolSetting("enabled");
}

bool GetLoggingEnabled() {
	return GetBoolSetting("loggingEnabled");

}

bool GetBoolSetting(__in LPSTR setting) {
	HKEY key;
	DWORD type = REG_DWORD;
	DWORD enabled = 0;
	DWORD dwLen = sizeof(DWORD);
	bool res = false;

	int result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, settingsKey, 0, KEY_READ, &key);
	if(result == ERROR_SUCCESS) {
		result = RegQueryValueExA(key, setting, NULL, &type, (LPBYTE)&enabled, &dwLen);
		if(result == ERROR_SUCCESS) {
			if(enabled == 1) {
				res = true;
			}
		}
		RegCloseKey(key);
	}
	return res;
}

// subauth.c eof