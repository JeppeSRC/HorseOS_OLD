#include <efi.h>


EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systable) {
	InitializeLibrary(handle, systable);


	UINTN col = 0;
	UINTN row = 0;

	UINTN textModeIndex = GetTextMode(systable->ConOut, &col, &row);
	systable->ConOut->SetMode(systable->ConOut, textModeIndex);

	print(L"Booting.....\n");
	printf(L"Setting text mode to %u -> %ux%u\n", textModeIndex, col, row);


	systable->BootServices->Stall(100000000);

	return EFI_SUCCESS;
}