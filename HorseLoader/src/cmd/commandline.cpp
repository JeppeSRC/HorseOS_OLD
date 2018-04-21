#include "commandline.h"

#define HorseOS_MAX_CMD_LENGTH 128

CHAR16 currentLine[HorseOS_MAX_CMD_LENGTH+1];

VOID EnterPreBootCommandLine(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systable) {
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* in = systable->ConIn;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* out = systable->ConOut;

	out->EnableCursor(out, 1);

	memset(currentLine, 0, sizeof(currentLine));

	UINT32 currentChar = 0;

	UINTN indices = 0;
	EFI_INPUT_KEY key;

	print(L"CMD: ");
	UINT32 currentRow = out->Mode->CursorRow;
	while (1) {
		systable->BootServices->WaitForEvent(1, &in->WaitForKey, &indices);
		in->ReadKeyStroke(in, &key);
		
		printf(L"%u -> %c\n", (UINT32)key.ScanCode, key.UnicodeChar);
	
		/*if (key.UnicodeChar != 0) {
			if (key.UnicodeChar == 0x08) {
				if (currentChar == 0) {
					currentLine[0] = 0;
					continue;
				} 
				currentLine[--currentChar] = 0;

				CHAR16 tmp[2];

				tmp[0] = key.UnicodeChar;
				tmp[1] = 0;

				print(tmp);
			} else {
				if (currentChar == HorseOS_MAX_CMD_LENGTH-1) continue;
				currentLine[currentChar++] = key.UnicodeChar;
			}
		} 

		print(currentLine);*/
	}

	out->EnableCursor(out, 0);
}