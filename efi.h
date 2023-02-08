#ifndef _EFI_H_
#define _EFI_H_

// basic data types (defined in p.20)
typedef unsigned long long UINTN;
typedef unsigned char UINT8;
typedef short INT16;
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
#define CONST const 

// status codes 
#define EFI_SUCCESS     0
#define EFI_ERROR	    0x8000000000000000
#define EFI_UNSPPORTED  (EFI_ERROR | 3)

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
(EFIAPI *EFI_TEXT_TEST_STRING) (
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN CHAR16 *String
);

/*
@retval ModeNumberがsupportされていないものだと、EFI_UNSPPORTEDを返す。
*/
typedef 
EFI_STATUS
(EFIAPI *EFI_TEXT_QUERY_MODE) (
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN UINTN ModeNumber,
    OUT UINTN *Columns,
    OUT UINTN *Rows
);

typedef 
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_MODE) (
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN UINTN ModeNumber
);

// Attributes
#define EFI_BLACK		    0x00
#define EFI_BLUE		    0x01
#define EFI_GREEN		    0x02
#define EFI_CYAN		    0x03
#define EFI_RED			    0x04
#define EFI_MAGENTA		    0x05
#define EFI_BROWN		    0x06
#define EFI_LIGHTGRAY	    0x07
#define EFI_BRIGHT		    0x08
#define EFI_DARKGRAY	    0x08
#define EFI_LIGHTBLUE	    0x09
#define EFI_LIGHTGREEN	    0x0A
#define EFI_LIGHTCYAN	    0x0B
#define EFI_LIGHTRED	    0x0C
#define EFI_LIGHTMAGENTA    0x0D
#define EFI_YELLOW		    0x0E
#define EFI_WHITE		    0x0F

#define EFI_BACKGROUND_BLACK	    0x00
#define EFI_BACKGROUND_BLUE		    0x10
#define EFI_BACKGROUND_GREEN	    0x20
#define EFI_BACKGROUND_CYAN		    0x30
#define EFI_BACKGROUND_RED		    0x40
#define EFI_BACKGROUND_MAGENTA	    0x50
#define EFI_BACKGROUND_BROWN		0x60
#define EFI_BACKGROUND_LIGHTGRAY	0x70

typedef 
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_ATTRIBUTE) (
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN UINTN Attribute
);

typedef 
EFI_STATUS
(EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
    IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
);

typedef struct {
    UINT32 MaxMode;
    // current settings
    INT32 Mode;
    INT32 Attribute;
    INT32 CursorColumn;
    INT32 CoursorRow;
    BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    char _pad1[8];
    EFI_TEXT_STRING OutputString;
    EFI_TEXT_TEST_STRING TestString;
    EFI_TEXT_QUERY_MODE QueryMode;
    EFI_TEXT_SET_MODE SetMode;
    EFI_TEXT_SET_ATTRIBUTE SetAttribute;
    EFI_TEXT_CLEAR_SCREEN ClearScreen;
    char _pad2[16];
    SIMPLE_TEXT_OUTPUT_MODE *Mode;
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

// EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL
struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;

typedef UINT8 EFI_KEY_TOGGLE_STATE;

typedef struct {
    UINT32 KeyShiftState; // Shiftキーの押下状態
    EFI_KEY_TOGGLE_STATE KeyToggleState; // ScrollLock, NumLock, CapsLockの状態
} EFI_KEY_STATE;

typedef struct {
    EFI_INPUT_KEY Key;
    EFI_KEY_STATE KeyState;
} EFI_KEY_DATA;

typedef 
EFI_STATUS
(EFIAPI *EFI_INPUT_RESET_EX) (
    IN struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN BOOLEAN ExtendedVerification
);

typedef 
EFI_STATUS
(EFIAPI *EFI_INPUT_READ_KEY_EX) (
    IN struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    OUT EFI_KEY_DATA *KeyData
);

typedef 
EFI_STATUS
(EFIAPI *EFI_SET_STATE) (
    IN struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN EFI_KEY_TOGGLE_STATE *KeyToggleState
);

typedef 
EFI_STATUS
(EFIAPI *EFI_KEY_NOTIFY_FUNCTION) (
    IN EFI_KEY_DATA *KeyData
);

/*
@param KeyData どのキーを押したら関数を呼び出すかを指定する
@prarm KeyNotificationFunction 関数へのポインタ
@param NotifyHandle uniqueなhandleが変える。登録解除用に使用する。
*/
typedef 
EFI_STATUS
(EFIAPI *EFI_REGISTER_KEYSTROKE_NOTIFY) (
    IN struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN EFI_KEY_DATA *KeyData,
    IN EFI_KEY_NOTIFY_FUNCTION KeyNotificationFunction,
    OUT VOID **NotifyHandle
);

typedef 
EFI_STATUS
(EFIAPI *EFI_UNREGISTER_KEYSTROKE_NOTIFY) (
    IN struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN VOID *NotificationHandle
);

typedef struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL{
    EFI_INPUT_RESET_EX Reset;
    EFI_INPUT_READ_KEY_EX ReadKeyStrokeEx;
    EFI_EVENT WaitForKeyEx;
    EFI_SET_STATE SetState;
    EFI_REGISTER_KEYSTROKE_NOTIFY RegisterKeyNotify;
    EFI_UNREGISTER_KEYSTROKE_NOTIFY UnregisterKeyNotify;
} EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;

// EFI_DEVICE_PATH_PROTOCOL
typedef struct {
    UINT8 Type;
    UINT8 SubType;
    UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

// EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL 
typedef 
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_NODE) (
    IN CONST CHAR16 *TextDeviceNode
);

typedef 
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_FROM_TEXT_PATH) (
    IN CONST CHAR16 *TextDevicePath
);

typedef struct {
    EFI_DEVICE_PATH_FROM_TEXT_NODE ConvertTextToDeviceNode;
    EFI_DEVICE_PATH_FROM_TEXT_PATH ConvertTextToDevicePath;
} EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;

// EFI_DEVICE_PATH_TO_TEXT_PROTOCOL
typedef 
CHAR16 *
(EFIAPI *EFI_DEVICE_PATH_TO_TEXT_PATH) (
    IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN BOOLEAN DisplayOnly,
    IN BOOLEAN AllowShortcuts
);

typedef struct {
    char _pad[8];
    EFI_DEVICE_PATH_TO_TEXT_PATH ConvertDevicePathToText;
} EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;

// EFI_DEVICE_PATH_UTILITIES_PROTOCOL
typedef 
EFI_DEVICE_PATH_PROTOCOL *
(EFIAPI *EFI_DEVICE_PATH_UTILS_APPEND_NODE) (
    IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN CONST EFI_DEVICE_PATH_PROTOCOL *DeviceNode
);

typedef struct {
    char _pad[24];
    EFI_DEVICE_PATH_UTILS_APPEND_NODE AppendDeviceNode;
} EFI_DEVICE_PATH_UTLITIES_PROTOCOL;

// EFI_BOOT_SERVICES
typedef 
EFI_STATUS
(EFIAPI *EFI_FREE_POOL) (
    IN VOID *Buffer
);

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

/* 
@param BootPolicy ブートマネージャから呼ばれたことを表す。SourceBufferがNULLなら無視される。
@param ParentImageHandle この関数を呼んだimageのhadnle
@param DevicePath ロードするimageのDevicePath
@param SourceBuffer NULLでなければロードされるイメージのコピーがあるメモリアドレス
*/
typedef  
EFI_STATUS
(EFIAPI *EFI_IMAGE_LOAD) (
    IN BOOLEAN BootPolicy,
    IN EFI_HANDLE ParentImageHandle,
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN VOID *SourceBuffer OPTIONAL,
    IN UINTN SourceSize,
    OUT EFI_HANDLE *ImageHandle
);

/*
@param ImageHandle 実行するimageのhandle
@param ExitDataSize ExitDataのサイズへのポインタ。ExitDataがNULLの場合はULLを指定する
@param ExitData BootServices->Exit()で終了した場合に呼び出し元へ返されるデータへのポインタ
*/
typedef 
EFI_STATUS
(EFIAPI *EFI_IMAGE_START) (
    IN EFI_HANDLE ImageHandle,
    OUT UINTN *ExitDataSize,
    OUT CHAR16 **ExitData OPTIONAL
);

typedef 
EFI_STATUS
(EFIAPI *EFI_SET_WATCHDOG_TIMER) (
    IN UINTN Timeout, // これを0にすると無効化できる。その他の引数は0かNULLでいい。
    IN UINT64 WatchdogCode,
    IN UINTN DataSize,
    IN CHAR16 *WatchdogData OPTIONAL
);

typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef 
EFI_STATUS 
(EFIAPI *EFI_LOCATE_HANDLE_BUFFER) (
    IN EFI_LOCATE_SEARCH_TYPE SearchType,
    IN EFI_GUID *Protocol OPTIONAL,
    IN VOID *SearchKey OPTIONAL,
    OUT UINTN *NumHandles,
    OUT EFI_HANDLE **Buffer
);

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL    0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL           0x00000002

/* 
@param Handle オープンするプロトコルで扱う対象のハンドルを指定
@param Protocol プロトコルのGUID
@param Interface プロトコル構造体のポインタを格納するための変数
@param AgentHandle  OpenProtocolを呼んでいるイメージのハンドル。つまり自分自身のイメージハンドル
@param ConrtollerHandle OpenProtocolを呼んでいるのがUEFI driverである場合に指定する。それ以外はNULL 

*/
typedef 
EFI_STATUS 
(EFIAPI *EFI_OPEN_PROTOCOL) (
    IN EFI_HANDLE Handle,
    IN EFI_GUID *Protocol,
    OUT VOID **Interface OPTIONAL,
    IN EFI_HANDLE AgentHandle,
    IN EFI_HANDLE ControllerHandle, 
    IN UINT32 Attributes 
);

typedef struct {
    char _pad1[40];

    // Memory Services
    char _pad2[32];
    EFI_FREE_POOL FreePool;

    // Event & Timer Services
    char _pad3[16];
    EFI_WAIT_FOR_EVENT WaitForEvent;
    char _pad4[24];

    // Protocol Handler Services
    char _pad5[72];

    // Image Services
    EFI_IMAGE_LOAD LoadImage;
    EFI_IMAGE_START StartImage;
    char _pad7[24];

    // Miscellaeous Services 
    char _pad8[16];
    EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;

    // DriverSupport Services
    char _pad9[16];

    // Open and Close Protocol Services
    EFI_OPEN_PROTOCOL OpenProtocol;
    char _pad10[16];

    // Library Services
    char _pad11[8];
    EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer;
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

// EFI_FILE_PROTOCOL
struct _EFI_FILE_PROTOCOL;

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

typedef
EFI_STATUS
(EFIAPI *EFI_FILE_OPEN) (
    IN struct _EFI_FILE_PROTOCOL *This,
    OUT struct _EFI_FILE_PROTOCOL **NewHandle,
    IN CHAR16 *FileName,
    IN UINT64 OpenMode,
    IN UINT64 Attributes
);

typedef 
EFI_STATUS
(EFIAPI *EFI_FILE_CLOSE) (
    IN struct _EFI_FILE_PROTOCOL *This
);

typedef 
EFI_STATUS
(EFIAPI *EFI_FILE_READ) (
    IN struct _EFI_FILE_PROTOCOL *This,
    IN OUT UINTN *BufferSize,
    OUT VOID *Buffer
);

typedef 
EFI_STATUS
(EFIAPI *EFI_FILE_WRITE) (
    IN struct _EFI_FILE_PROTOCOL *This,
    IN OUT UINTN *BufferSize,
    IN VOID *Buffer
);

typedef 
EFI_STATUS
(EFIAPI *EFI_FILE_FLUSH) (
    IN struct _EFI_FILE_PROTOCOL *This
);

typedef struct _EFI_FILE_PROTOCOL {
    UINT64 Revision;
    EFI_FILE_OPEN Open;
    EFI_FILE_CLOSE Close;
    char _pad1[8];
    EFI_FILE_READ Read;
    EFI_FILE_WRITE Write;
    char _pad2[32];
    EFI_FILE_FLUSH Flush;
}EFI_FILE_PROTOCOL;

// EFI_SIMLE_FILE_SYSTEM_PROTOCOL
struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef 
EFI_STATUS
(EFIAPI *EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME) (
    IN struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
    OUT EFI_FILE_PROTOCOL **Root
);

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL{
    UINT64 Revision;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME OpenVolume;
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

// EFI_FILE_INFO
typedef struct {
    UINT16 Year;
    UINT8 Month;
    UINT8 Day;
    UINT8 Hour;
    UINT8 Minute;
    UINT8 Second;
    UINT8 Pad1;
    UINT32 Nanosecond;
    INT16 TimeZone;
    UINT8 Daylight;
    UINT8 Pad2;
} EFI_TIME;

typedef struct {
    UINT64 Size;
    UINT64 FileSize;
    UINT64 PhysicalSize;
    EFI_TIME CreateTime;
    EFI_TIME LastAccessTime;
    EFI_TIME ModificationTime;
    UINT64 Attribute;
    CHAR16 FileName[];
} EFI_FILE_INFO;

// EFI_LOADED_IMAGE_PROTOCOL 
typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiUnacceptedMemoryType,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef 
EFI_STATUS
(EFIAPI *EFI_IMAGE_UNLOAD) (
    IN EFI_HANDLE ImageHandle
);

typedef struct {
    UINT32 Revision;
    EFI_HANDLE ParentHandle;
    EFI_SYSTEM_TABLE *SystemTable;
    
    // Source location of the image
    EFI_HANDLE DeviceHandle;
    EFI_DEVICE_PATH_PROTOCOL *FilePath;
    VOID *Reserved;

    // Image`s load options
    UINT32 LoadOptionsSize;
    VOID *LoadOptions;

    // Location where image was loaded
    VOID *ImageBase;
    UINT64 ImageSize;
    EFI_MEMORY_TYPE  ImageCodeType;
    EFI_MEMORY_TYPE ImageDataType;
    EFI_IMAGE_UNLOAD Unload;
} EFI_LOADED_IMAGE_PROTOCOL;

extern EFI_SYSTEM_TABLE *ST;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
extern EFI_SIMPLE_POINTER_PROTOCOL *SPP;
extern EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *DPTTP;
extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *STIEP;
extern EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *DPFTP;
extern EFI_DEVICE_PATH_UTLITIES_PROTOCOL *DPUP;

extern EFI_GUID GOP_GUID, SPP_GUID, SFSP_GUID, LIP_GUID, DPP_GUID, DPTTP_GUID, STIEP_GUID, DPFTP_GUID, DPUP_GUID;

void efi_init(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table);

#endif