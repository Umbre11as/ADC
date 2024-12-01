#include "util.h"

int UtlEnablePrivileges() {
    HANDLE token;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
        ERR("OpenProcessToken failed: 0x%lX", GetLastError());
        return 1;
    }

    LUID luid;
    if (!LookupPrivilegeValue(NULL, "SeLoadDriverPrivilege", &luid)) {
        ERR("LookupPrivilegeValue failed: 0x%lX", GetLastError());
        CloseHandle(token);
        return 1;
    }

    TOKEN_PRIVILEGES privileges;
    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Luid = luid;
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(token, FALSE, &privileges, sizeof(privileges), NULL, NULL)) {
        ERR("AdjustTokenPrivileges failed: 0x%lX", GetLastError());
        CloseHandle(token);
        return 1;
    }

    CloseHandle(token);
    return 0;
}
