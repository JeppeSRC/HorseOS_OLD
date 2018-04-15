#include <efi.h>


EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systable) {
	InitializeLibrary(handle, systable);

	print(L"Booting.....");



	systable->BootServices->Stall(10000000);

	return EFI_SUCCESS;
}