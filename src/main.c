#include "util.h"

// From winternl.h
#ifndef NT_SUCCESS
    #define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

PCSTR driverName = "SomeDrv";
PCSTR driverPath = "\\DosDevices\\C:\\Users\\admin\\Documents\\VBoxShared\\HelloWorld.sys";

typedef void(NTAPI *RtlInitUnicodeStringFn)(PUNICODE_STRING, PCWSTR);
typedef NTSTATUS(NTAPI *NtLoadDriverFn)(PUNICODE_STRING);

int main() {
    HANDLE ntdll = GetModuleHandle("ntdll.dll");
    if (ntdll == INVALID_HANDLE_VALUE)
        return 1;

    NtLoadDriverFn NtLoadDriver = (NtLoadDriverFn) GetProcAddress(ntdll, "NtLoadDriver");
    RtlInitUnicodeStringFn RtlInitUnicodeString = (RtlInitUnicodeStringFn) GetProcAddress(ntdll, "RtlInitUnicodeString");
    if (!NtLoadDriver || !RtlInitUnicodeString) {
        ERR("NtLoadDriver or RtlInitUnicodeString not found");
        return 1;
    }

    int status = UtlEnablePrivileges();
    if (status != 0) // Error already printed
        return status;

    INFO("SeLoadDriverPrivilege privilege enabled");

    HKEY registryKey;
    if (RegCreateKey(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\SomeDrv", &registryKey) != ERROR_SUCCESS) {
        ERR("RegCreateKey failed: 0x%lX", GetLastError());
        return 1;
    }

    DWORD type = SERVICE_KERNEL_DRIVER;
    if (RegSetValueEx(registryKey, "Type", NULL, REG_DWORD, (const BYTE*) &type, sizeof(type)) != ERROR_SUCCESS) {
        ERR("RegSetValueEx (Type) failed: 0x%lX", GetLastError());
        return 1;
    }

    DWORD start = SERVICE_DEMAND_START;
    if (RegSetValueEx(registryKey, "Start", NULL, REG_DWORD, (const BYTE*) &start, sizeof(start)) != ERROR_SUCCESS) {
        ERR("RegSetValueEx (Start) failed: 0x%lX", GetLastError());
        return 1;
    }

    DWORD errorControl = SERVICE_ERROR_NORMAL;
    if (RegSetValueEx(registryKey, "ErrorControl", NULL, REG_DWORD, (const BYTE*) &errorControl, sizeof(errorControl)) != ERROR_SUCCESS) {
        ERR("RegSetValueEx (ErrorControl) failed: 0x%lX", GetLastError());
        return 1;
    }

    if (RegSetValueEx(registryKey, "ImagePath", NULL, REG_SZ, (const BYTE*) driverPath, strlen(driverPath)) != ERROR_SUCCESS) {
        ERR("RegSetValueEx (ImagePath) failed: 0x%lX", GetLastError());
        return 1;
    }

    RegCloseKey(registryKey);

    UNICODE_STRING registryPath;
    RtlInitUnicodeString(&registryPath, L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\SomeDrv");

    NTSTATUS loadStatus;
    if (!NT_SUCCESS(loadStatus = NtLoadDriver(&registryPath))) {
        if (loadStatus == 0xC0000428)
            ERR("Driver signature enforcement not disabled");
        else
            ERR("NtLoadDriver failed: 0x%lX", loadStatus);

        return 1;
    }

    INFO("Driver started sucessfully");
    return 0;
}
