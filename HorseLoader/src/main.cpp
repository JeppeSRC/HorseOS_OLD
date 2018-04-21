#include <efi.h>
#include "util.h"
#include "cmd/commandline.h"

EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systable) {
	InitializeLibrary(handle, systable);

	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = 0;
	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	systable->BootServices->LocateProtocol(&gopGuid, 0, &gop);

	if (gop == 0) {
		print(L"Failed to locate EFI_GRAPHICS_OUTPUT_PROTOCOL!\n");
		WaitEscapeAndExit();
	}

	UINT32 width = 1280;
	UINT32 height = 0;
	EFI_GRAPHICS_PIXEL_FORMAT format = PixelRedGreenBlueReserved8BitPerColor;

	UINT32 gopModeIndex = GetGraphicsMode(gop, &width, &height, &format);

	if (format == PixelBltOnly) {
		println(L"No physical framebuffer support!");
		WaitEscapeAndExit();
	}

	gop->SetMode(gop, gopModeIndex);

	UINTN col = 0;
	UINTN row = 0;

	UINTN textModeIndex = GetTextMode(systable->ConOut, &col, &row);
	systable->ConOut->SetMode(systable->ConOut, textModeIndex);

	print(L"Booting.....\n");
	printf(L"Setting physical framebuffer to %u -> %ux%u %s\n", gopModeIndex, width, height, GetGraphicsPixelFormatString(format));
	printf(L"Setting text mode to %u -> %ux%u\n", textModeIndex, col, row);

	EFI_INPUT_KEY key;

	systable->ConIn->ReadKeyStroke(systable->ConIn, &key);

	EnterPreBootCommandLine(handle, systable);

	WaitEscapeAndExit(); 
	
	return EFI_SUCCESS;
}