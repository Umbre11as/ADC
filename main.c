#include <Windows.h>
#include <stdio.h>

#define ERR(x, ...) PrintError(x, __VA_ARGS__)
#define INFO(x, ...) PrintInfo(x, __VA_ARGS__)

void PrintError(IN PCSTR Format, ...) {
    va_list args;
    va_start(args, Format);

    char buffer[256];
    vsprintf_s(buffer, sizeof(buffer), Format, args);
    fprintf(stderr, "[error]: %s\n", buffer);

    va_end(args);
}

void PrintInfo(IN PCSTR Format, ...) {
    va_list args;
    va_start(args, Format);

    char buffer[256];
    vsprintf_s(buffer, sizeof(buffer), Format, args);
    printf("[info]: %s\n", buffer);

    va_end(args);
}

PCSTR driverName = "SomeDrv";
PCSTR driverPath = "HelloWorld.sys";

int main() {
    SC_HANDLE serviceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (serviceManager == INVALID_HANDLE_VALUE) {
        ERR("Cannot open service manager: 0x%lX", GetLastError());
        return 1;
    }

    SC_HANDLE service = CreateService(
        serviceManager,
        driverName, driverName,
        SERVICE_ALL_ACCESS,
        SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE,
        driverPath,
        NULL, NULL, NULL, NULL, NULL
    );
check:
    if (service == NULL) {
        DWORD error = GetLastError();
        if (error == ERROR_SERVICE_EXISTS) {
            service = OpenService(serviceManager, driverName, SERVICE_ALL_ACCESS);
            goto check;
        }

        ERR("Cannot create kernel driver service: 0x%lX", error);
        CloseServiceHandle(serviceManager);
        return 1;
    }

    if (!StartService(service, 0, NULL)) {
        DWORD error = GetLastError();
        if (error == ERROR_INVALID_IMAGE_HASH)
            ERR("Driver signature enforcement not disabled");
        else if (error == ERROR_SERVICE_MARKED_FOR_DELETE)
            ERR("Service marked for delete, restart your PC");
        else
            ERR("Cannot start kernel driver: 0x%lX", GetLastError());

        CloseServiceHandle(service);
        CloseServiceHandle(serviceManager);
        return 1;
    }

    INFO("Driver started");

    CloseServiceHandle(service);
    CloseServiceHandle(serviceManager);
    return 0;
}
