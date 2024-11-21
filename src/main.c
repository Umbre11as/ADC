#include "util.h"

PCSTR driverName = "SomeDrv";
PCWSTR driverPath = L"Z:\\HelloWorld.sys";

int main() {
    HANDLE ntdll = GetModuleHandle("ntdll.dll");
    if (ntdll == INVALID_HANDLE_VALUE)
        return 1;

    int status = UtlEnablePrivileges();
    if (status != 0)
        return status;

    INFO("SeLoadDriverPrivilege privilege enabled");


    return 0;
}
