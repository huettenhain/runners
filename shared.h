#pragma once
#include <shlwapi.h>

void _declspec(noreturn) epilogue() {
	int error = GetLastError();
	if (error) {
		wchar_t errbuf[0x200];
		HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
		WriteConsoleW(stdout, L"ERROR: ", 7, NULL, NULL);
		WriteConsoleW(
			stdout, errbuf,
			FormatMessageW(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				errbuf, 0x200, NULL),
			NULL, NULL);
	}
	ExitProcess(error);
}
