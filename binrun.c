#include <windows.h>
#include "shared.h"

#if _WIN64
#	define BOOM(_p) ((void(*)(void*))(_p))(_p);
#else
#	define BOOM(_p) __asm call _p
#endif 

void _declspec(noreturn) main() {
	int argc; LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argc >= 2) {
		HANDLE file = CreateFileW(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (file != INVALID_HANDLE_VALUE) {
			DWORD read;
			DWORD size = GetFileSize(file, NULL);
			BYTE* payload = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			if (payload) {
				if (ReadFile(file, payload, size, &read, NULL) && size == read) {
					unsigned long int num;
					CloseHandle(file);
					file = INVALID_HANDLE_VALUE;
					if (argc > 2 && StrToIntExW(argv[2], STIF_SUPPORT_HEX, &num))
						payload += num;
					BOOM(payload)
				}
				VirtualFree(payload, 0, MEM_RELEASE);
			}
			if (file != INVALID_HANDLE_VALUE) 
				CloseHandle(file);
	} }
	epilogue();
}