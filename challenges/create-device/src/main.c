#include <ntifs.h>
#include <CaveHook.h>

PVOID original;

typedef NTSTATUS(*IoCreateDeviceDetourFn)(IN PDRIVER_OBJECT driverObject, IN ULONG extensionSize, IN OPTIONAL PUNICODE_STRING deviceName, IN DEVICE_TYPE deviceType, IN ULONG characteristics, IN BOOLEAN exclusive, OUT PDRIVER_OBJECT* deviceObject);

NTSTATUS IoCreateDeviceDetour(IN PDRIVER_OBJECT driverObject, IN ULONG extensionSize, IN OPTIONAL PUNICODE_STRING deviceName, IN DEVICE_TYPE deviceType, IN ULONG characteristics, IN BOOLEAN exclusive, OUT PDRIVER_OBJECT* deviceObject) {
    NTSTATUS status = ((IoCreateDeviceDetourFn) original)(driverObject, extensionSize, deviceName, deviceType, characteristics, exclusive, deviceObject);
    if (NT_SUCCESS(status)) {
        DbgPrintEx(0, 0, "Device was created\n");
        DbgPrintEx(0, 0, "Device name: %ws\n", deviceName->Buffer);
    }

    return status;
}

NTKERNELAPI NTSTATUS NTAPI IoCreateDriver(IN PUNICODE_STRING, IN PDRIVER_INITIALIZE);

NTSTATUS CreateClose(IN PDEVICE_OBJECT deviceObject, IN OUT PIRP irp) {
    irp->IoStatus.Status = STATUS_SUCCESS;
    irp->IoStatus.Information = 0;

    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return irp->IoStatus.Status;
}

NTSTATUS DeviceControlCallback(IN PDEVICE_OBJECT deviceObject, IN OUT PIRP irp) {
    PIO_STACK_LOCATION io = IoGetCurrentIrpStackLocation(irp);
    DbgPrintEx(0, 0, "[solution]: DeviceIoControl called with code: 0x%lX\n", io->Parameters.DeviceIoControl.IoControlCode);

    return CreateClose(deviceObject, irp);
}

void UnloadDriver(IN PDRIVER_OBJECT driverObject) {
    IoDeleteDevice(driverObject->DeviceObject);
}

NTSTATUS DrvEntry(IN PDRIVER_OBJECT driverObject, IN PUNICODE_STRING registryPath) {
    DbgPrintEx(0, 0, "[solution]: Hi\n");

    UNICODE_STRING deviceName;
    RtlInitUnicodeString(&deviceName, L"\\Device\\shit");

    IoCreateDevice(driverObject, 0, &deviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &driverObject->DeviceObject);
    IoSetDeviceInterfaceState(registryPath, TRUE);

    driverObject->MajorFunction[IRP_MJ_CREATE] = driverObject->MajorFunction[IRP_MJ_CLOSE] = CreateClose;
    driverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControlCallback;
    driverObject->DeviceObject->Flags |= DO_DIRECT_IO;
    driverObject->DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    driverObject->DriverUnload = UnloadDriver;

    OBJECT_ATTRIBUTES objectAttributes;

    UNICODE_STRING driverName;
    RtlInitUnicodeString(&driverName, L"\\Device\\shit");
    InitializeObjectAttributes(&objectAttributes, &driverName, OBJ_CASE_INSENSITIVE, NULL, NULL);

    HANDLE driverHandle;
    IO_STATUS_BLOCK ioStatus;
    ZwCreateFile(&driverHandle, GENERIC_READ | SYNCHRONIZE, &objectAttributes, &ioStatus, NULL, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
    int var = 4;
    ZwDeviceIoControlFile(driverHandle, 0, NULL, NULL, &ioStatus, 0x24, NULL, 0, &var, sizeof(var));
    ZwClose(driverHandle);

    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT driverObject, IN PUNICODE_STRING registryPath) {
    CaveHook((ULONGLONG) IoCreateDevice, IoCreateDeviceDetour, &original);

    UNICODE_STRING driverName;
    RtlInitUnicodeString(&driverName, L"\\Driver\\shit");
    IoCreateDriver(&driverName, &DrvEntry);

    return STATUS_SUCCESS;
}
