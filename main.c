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

// EFI_SYSTEM_TABLE
typedef struct{
    char _pad[60];
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
} EFI_SYSTEM_TABLE;

// entry point
void efi_main(void *ImageHandle __attribute__((unused)), EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputtString(SystemTable->ConOut, L"Hello UEFI!\n");
    while(1);
}