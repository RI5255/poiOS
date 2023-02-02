#ifndef _EFI_H_
#define _EFI_H_

// basic data types (defined in p.20)
typedef unsigned long long UINTN;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef unsigned short CHAR16;
typedef void VOID;
typedef unsigned char  BOOLEAN;
typedef UINTN EFI_STATUS;
typedef VOID* EFI_HANDLE;
typedef VOID* EFI_EVENT;
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

typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8 Data4[8];
} EFI_GUID;

typedef 
EFI_STATUS
(EFIAPI *EFI_LOCATE_PROTOCOL) (
    IN EFI_GUID *Protocol,
    IN VOID *Registration OPTIONAL,
    OUT VOID **Interface
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
    char _pad4[16];
    EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;

    char _pad5[40];

    // Library Services
    char _pad6[16];
    EFI_LOCATE_PROTOCOL LocateProtocol;

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

// EFI_GRAPHICS_OUTPUT_PROTOCOL
typedef struct {
    UINT8 Blue;
    UINT8 Green;
    UINT8 Red;
    UINT8 Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef UINT64 EFI_PHYSICAL_ADDRESS;

typedef struct {
    UINT32 RedMask;
    UINT32 GreenMask;
    UINT32 BlueMask;
    UINT32 ReservedMask;
} EFI_PIXEL_BITMASK;

typedef enum {
    PixelRedGreenBlueReserved8BitPerColor, 
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBitOnly,
    PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
    UINT32 Version;
    UINT32 HorizontalResolution;
    UINT32 VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
    EFI_PIXEL_BITMASK PixelInformation;
    UINT32 PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
    UINT32 MaxMode;
    UINT32 Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    UINTN SizeOfInfo;
    EFI_PHYSICAL_ADDRESS FrameBufferBase;
    UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct {
    char _pad[24];
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

// EFI_SIMPLE_POINTER_PROTOCOL
struct _EFI_SIMPLE_POINTER_PROTOCOL;

typedef struct {
    UINT64 ResolutionX;
    UINT64 ResolutionY;
    UINT64 ResolutionZ;
    BOOLEAN LeftButton;
    BOOLEAN RightButton;
} EFI_SIMPLE_POINTER_MODE;

typedef struct {
    INT32 RelativeMovementX;
    INT32 RelativeMovementY;
    INT32 RelativeMovementZ;
    BOOLEAN LeftButton;
    BOOLEAN RightButton;
} EFI_SIMPLE_POINTER_STATE;

typedef 
EFI_STATUS
(EFIAPI *EFI_SIMPLE_POINTER_GET_STATE) (
    IN struct _EFI_SIMPLE_POINTER_PROTOCOL *This,
    OUT EFI_SIMPLE_POINTER_STATE *State
);

typedef 
EFI_STATUS
(EFIAPI *EFI_SIMPLE_POINTER_RESET) (
    IN struct _EFI_SIMPLE_POINTER_PROTOCOL *This,
    IN BOOLEAN ExtendedVerification
);

typedef struct _EFI_SIMPLE_POINTER_PROTOCOL{
    EFI_SIMPLE_POINTER_RESET Reset;
    EFI_SIMPLE_POINTER_GET_STATE GetState;
    EFI_EVENT WaitForInput;
    EFI_SIMPLE_POINTER_MODE *Mode;
} EFI_SIMPLE_POINTER_PROTOCOL;

extern EFI_SYSTEM_TABLE *ST;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
extern EFI_SIMPLE_POINTER_PROTOCOL *SPP;
void efi_init(EFI_SYSTEM_TABLE *SystemTable);

#endif