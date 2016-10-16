#include "platform/CCDevice.h"
#include "CCStdC.h"
#include "CCEGLView.h"
#include "ccUTF8.h"
#include <io.h>

NS_CC_BEGIN

int CCDevice::getDPI()
{
    static int dpi = -1;
    if (dpi == -1)
    {
        HDC hScreenDC = GetDC( NULL );
        int PixelsX = GetDeviceCaps( hScreenDC, HORZRES );
        int MMX = GetDeviceCaps( hScreenDC, HORZSIZE );
        ReleaseDC( NULL, hScreenDC );   
        dpi = 254.0f*PixelsX/MMX/10;
    }
    return dpi;
}

void CCDevice::LogS(const char* str)
{
	int size = 0;
	unsigned short* s = cc_utf8_to_utf16(str, &size);

	FILE* out = stdout;
	if (!_isatty(_fileno(out)))
		fprintf(out, "%s\n", str);
	else {
		HANDLE stream = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD charsWritten;
		WriteConsoleW(stream, s, size, &charsWritten, 0);
		WriteConsoleW(stream, "\n", 1, &charsWritten, 0);
	}
}

void CCDevice::ShowMessageBox(const char * pszMsg, const char * pszTitle)
{
	MessageBoxA(NULL, pszMsg, pszTitle, MB_OK);
}


NS_CC_END