#pragma once

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

typedef void(NTAPI *RtlInitUnicodeStringFn)(PUNICODE_STRING, PCWSTR);
typedef NTSTATUS(NTAPI *NtLoadDriverFn)(PUNICODE_STRING);

BOOLEAN LdrLoadDriver(PCSTR driverName, PCSTR driverPath);
