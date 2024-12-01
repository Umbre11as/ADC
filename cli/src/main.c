#include "loader.h"

PCSTR driverName = "SomeDrv";
PCSTR driverPath = "\\DosDevices\\C:\\Users\\admin\\Documents\\VBoxShared\\HelloWorld.sys";

int main() {
    if (!LdrLoadDriver(driverName, driverPath)) {
        ERR("Cannot load driver");
        return 1;
    }

    return 0;
}
