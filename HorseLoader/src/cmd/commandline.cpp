#include "commandline.h"
#include "../util.h"

#define HorseOS_MAX_CMD_LENGTH 128

CHAR16 currentLine[HorseOS_MAX_CMD_LENGTH+1];

void PrintTextModes(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* out) {
	UINTN c = 0;
	UINTN r = 0;

	for (UINTN i = 0; i < out->Mode->MaxMode; i++) {
		EFI_STATUS status = out->QueryMode(out, i, &c, &r);

		printf(L"%u: %ux%u %s\n", i, c, r, status == EFI_UNSUPPORTED ? L"UNSUPPORTED" : L" ");
	}
}

void PrintGraphicsModes(EFI_GRAPHICS_OUTPUT_PROTOCOL* gop) {
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info;

	UINTN size = 0;

	for (UINT32 i = 0; i < gop->Mode->MaxMode; i++) {
		gop->QueryMode(gop, i, &size, &info);

		printf(L"%u: %ux%u %s\n", i, info->HorizontalResolution, info->VerticalResolution, GetGraphicsPixelFormatString(info->PixelFormat));
	}
}

VOID Execute(CONST CHAR16* cmd, EFI_SYSTEM_TABLE* systable, EFI_GRAPHICS_OUTPUT_PROTOCOL* gop) {
	if (startsWith(cmd, L"show text modes")) {
		PrintTextModes(systable->ConOut);
	} else if (startsWith(cmd, L"show graphics modes")) {
		PrintGraphicsModes(gop);
	}
}

VOID EnterPreBootCommandLine(EFI_HANDLE handle, EFI_SYSTEM_TABLE* systable, EFI_GRAPHICS_OUTPUT_PROTOCOL* gop) {
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
	
		if (key.UnicodeChar != 0) {
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
			} else if (key.UnicodeChar == 0x0D) {
				print(L"\n");
				Execute(currentLine, systable, gop);
				memset(currentLine, 0, sizeof(currentLine));
				currentChar = 0;
				currentRow = out->Mode->CursorRow;
				out->SetCursorPosition(out, 0, currentRow);
				print(L"CMD: ");
			} else {
				if (currentChar == HorseOS_MAX_CMD_LENGTH-1) continue;
				currentLine[currentChar++] = key.UnicodeChar;
			}
		} 

		out->SetCursorPosition(out, 5, currentRow);
		print(currentLine);
	}

	out->EnableCursor(out, 0);
}