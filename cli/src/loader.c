#include "loader.h"

BOOLEAN LdrLoadDriver(PCSTR driverName, PCSTR driverPath) {
    HANDLE ntdll = GetModuleHandle("ntdll.dll");
    if (ntdll == INVALID_HANDLE_VALUE)
        return FALSE;

    NtLoadDriverFn NtLoadDriver = (NtLoadDriverFn) GetProcAddress(ntdll, "NtLoadDriver");
    RtlInitUnicodeStringFn RtlInitUnicodeString = (RtlInitUnicodeStringFn) GetProcAddress(ntdll, "RtlInitUnicodeString");
    if (!NtLoadDriver || !RtlInitUnicodeString) {
        ERR("NtLoadDriver or RtlInitUnicodeString not found");
        return FALSE;
    }

    int status = UtlEnablePrivileges();
    if (status != 0) // Error already printed
        return FALSE;

    INFO("SeLoadDriverPrivilege privilege enabled");

    HKEY registryKey;
    if (RegCreateKey(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\SomeDrv", &registryKey) != ERROR_SUCCESS) {
        ERR("RegCreateKey failed: 0x%lX", GetLastError());
        return FALSE;
    }

    DWORD type = SERVICE_KERNEL_DRIVER;
    if (RegSetValueEx(registryKey, "Type", NULL, REG_DWORD, (const BYTE*) &type, sizeof(type)) != ERROR_SUCCESS) {
        ERR("RegSetValueEx (Type) failed: 0x%lX", GetLastError());
        return FALSE;
    }

    DWORD start = SERVICE_DEMAND_START;
    if (RegSetValueEx(registryKey, "Start", NULL, REG_DWORD, (const BYTE*) &start, sizeof(start)) != ERROR_SUCCESS) {
        ERR("RegSetValueEx (Start) failed: 0x%lX", GetLastError());
        return FALSE;
    }

    DWORD errorControl = SERVICE_ERROR_NORMAL;
    if (RegSetValueEx(registryKey, "ErrorControl", NULL, REG_DWORD, (const BYTE*) &errorControl, sizeof(errorControl)) != ERROR_SUCCESS) {
        ERR("RegSetValueEx (ErrorControl) failed: 0x%lX", GetLastError());
        return FALSE;
    }

    if (RegSetValueEx(registryKey, "ImagePath", NULL, REG_SZ, (const BYTE*) driverPath, strlen(driverPath)) != ERROR_SUCCESS) {
        ERR("RegSetValueEx (ImagePath) failed: 0x%lX", GetLastError());
        return FALSE;
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

        return FALSE;
    }

    INFO("Driver started sucessfully");
    return TRUE;
}
