// basic data types (defined in p.20)
#define UINTTN unsigned long 
#define UINT8 unsigned char 
#define UINT16 unsigned short 
#define UINT32 unsigned int 
#define UINT64 unsigned long 
#define CHAR16 unsigned short
#define VOID void 
#define EFI_STATUS UINTTN 
#define EFI_HANDLE VOID *
#define EFIAPI 
#define IN 
#define OUT 

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
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// EFI_SYSTEM_TABLE
typedef struct{
    char _pad1[44];
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    char _pad2[8];
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
} EFI_SYSTEM_TABLE;

// entry point
void efi_main(void *ImageHandle __attribute__((unused)), EFI_SYSTEM_TABLE *SystemTable) {
    EFI_INPUT_KEY key;
    UINT16 str[3];

    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    while(1) {
        // ReadKeyStrokeは入力を受け取ると0を返す
        if(!SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key)) {
            if(key.UnicodeChar != L'\r') {
                str[0] = key.UnicodeChar;
                str[1] = L'\0';
            } else {
                // \r: カーソルを先頭に戻す制御文字 \n: カーソルを次の行に移動する制御文字
                str[0] = L'\r';
                str[1] = L'\n';
                str[2] = L'\0';
            }
            SystemTable->ConOut->OutputtString(SystemTable->ConOut, str);
        }
    }
}