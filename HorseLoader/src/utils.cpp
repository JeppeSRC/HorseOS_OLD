#include "util.h"

CONST CHAR16* GetGraphicsPixelFormatString(EFI_GRAPHICS_PIXEL_FORMAT format) {
	switch (format) {
		case PixelRedGreenBlueReserved8BitPerColor:
			return L"PixelRedGreenBlueReserved8BitPerColor";
		case PixelBlueGreenRedReserved8BitPerColor:
			return L"PixelBlueGreenRedReserved8BitPerColor";
		case PixelBitMask:
			return L"PixelBitMask";
		case PixelBltOnly:
			return L"PixelBltOnly";
	}

	return L"Error";
}