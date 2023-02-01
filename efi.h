#ifndef _EFI_H_
#define _EFI_H_

// basic data types (defined in p.20)
#define UINTN unsigned long 
#define UINT8 unsigned char 
#define UINT16 unsigned short 
#define INT32 int 
#define UINT32 unsigned int 
#define UINT64 unsigned long 
#define CHAR16 unsigned short
#define VOID void 
#define EFI_STATUS UINTN 
#define EFI_HANDLE VOID *
#define EFI_EVENT VOID *
#define EFIAPI 
#define IN 
#define OUT 
#define OPTIONAL

// EFI_SYMPLE_TEXT_OUTPUT_PROTOCOL
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef 
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING) (
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN CHAR16 *String
);

typedef 
EFI_STATUS
(EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    char _pad1[8];
    EFI_TEXT_STRING OutputtString;
    char _pad2[32];
    EFI_TEXT_CLEAR_SCREEN ClearScreen;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// EFI_SIMPLE_TEXT_INPUT_PROTOCOL
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct {
    UINT16 ScanCode;
    CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef 
EFI_STATUS
(EFIAPI *EFI_INPUT_READ_KEY) (
    IN struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
    OUT EFI_INPUT_KEY *Key
);

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    char pad[8];
    EFI_INPUT_READ_KEY ReadKeyStroke;
    EFI_EVENT WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// EIF_BOOT_SERVICES
typedef 
EFI_STATUS
(EFIAPI *EFI_WAIT_FOR_EVENT) (
    IN UINTN NumberOfEvents,
    IN EFI_EVENT *Event,
    OUT UINTN *Index
);

typedef 
EFI_STATUS
(EFIAPI *EFI_SET_WATCHDOG_TIMER) (
    IN UINTN Timeout, // これを0にすると無効化できる。その他の引数は0かNULLでいい。
    IN UINT64 WatchdogCode,
    IN UINTN DataSize,
    IN CHAR16 *WatchdogData OPTIONAL
);

typedef struct {
    char _pad1[96];
    
    // Event & Timer Services
    EFI_WAIT_FOR_EVENT WaitForEvent;
    char _pad2[24];

    char _pad3[112];

    // Miscellaeous Services 
    char _pad[16];
    EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;

} EFI_BOOT_SERVICES;

// EFI_SYSTEM_TABLE
typedef struct{
    char _pad1[44];
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    char _pad2[8];
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    char _pad3[24];
    EFI_BOOT_SERVICES *BootServices;
} EFI_SYSTEM_TABLE;


extern EFI_SYSTEM_TABLE *ST;
void efi_init(EFI_SYSTEM_TABLE *SystemTable);

#endif