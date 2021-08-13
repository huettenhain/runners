#include <windows.h>
#pragma warning(disable: 4091)
#include <imagehlp.h>
#include "shared.h"

typedef union {
	WCHAR* argument;
	ULONGLONG integer;
	BYTE* bytes;
} multi_t;

BOOL ParseFunctionArguments(int argi, int argc, multi_t* argv) {
	HANDLE Heap = GetProcessHeap();

	for (int i = argi; i < argc; i++) {
		int number = -1;
		int length = 1;
		if (StrToIntExW(argv[i].argument, STIF_SUPPORT_HEX, &number) && number >= 0) {
			argv[i].integer = number;
			continue;
		}
		for (WCHAR* p = argv[i].argument; *p; p++)
			length += 1;
		DWORD size = length;
		BYTE* WorkBench = HeapAlloc(Heap, HEAP_ZERO_MEMORY, length);
		if (!WorkBench)
			return FALSE;
		if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, argv[i].argument, -1, WorkBench, length, NULL, NULL)) {
			HeapFree(Heap, 0, WorkBench);
			return FALSE;
		}
		if (!CryptStringToBinaryA(WorkBench, 0, CRYPT_STRING_HEX_ANY, argv[i].bytes, &size, NULL, NULL)) {
			for (int j = 0; j < length; j++)
				argv[i].bytes[j] = WorkBench[j];
		}
		HeapFree(Heap, 0, WorkBench);
	}
	return TRUE;
}

void usage() {
	DWORD err = GetLastError();
#   define _USAGE \
	"USAGE: dllrun <dll> [<base>|0] [[_stdcall] <proc>] [<args>]\n"\
	"  dll: The path to the DLL.\n"\
	" base: The base to which the image should be rebased. Specify 0 to omit rebasing.\n"\
	" proc: Name of a procedure to execute from the loaded library.\n"\
	" args: Arguments to be passed to the procedure. Arguments are parsed as follows:\n"\
	"       - integers in decimal or hexadecimal (0x-prefix) notation are are passed as integers.\n"\
	"       - hex strings are decoded and passed as pointers to the decoded buffer.\n"\
	"       - all other strings are passed as ASCII strings.\n"
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), _USAGE, sizeof(_USAGE), NULL, NULL);
#	undef _USAGE
	SetLastError(err);
}

void _declspec(noreturn) main() {
	int argc; 
	int argi = 1;
	multi_t *argv = (multi_t*) CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argc < 2) {
		usage();
		goto EPILOGUE;
	}
	HMODULE dll = LoadLibraryW(argv[argi++].argument);
	if (!dll) {
		usage();
		goto EPILOGUE;
	}
	HANDLE neverm0re = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (argc >= 3) {
		ULONG_PTR Base = 0;
		if (StrToIntExW(argv[argi++].argument, STIF_SUPPORT_HEX, (int*) &Base) && Base) {
			ULONG OldSize, NewSize;
			ULONG_PTR OldBase, NewBase = Base;
			ParseFunctionArguments(1, 2, argv);
			if (!ReBaseImage(argv[1].bytes, NULL, TRUE, FALSE, FALSE, 0, &OldSize, &OldBase, &NewSize, &NewBase, 0) || NewBase != Base)
				goto EPILOGUE;
		}
	}
	ParseFunctionArguments(argi, argc, argv);
	if (dll && argc >= 3) {
		FARPROC proc;
		BOOL x64cc = TRUE;
		if (!StrNCmp(argv[argi].bytes, "_stdcall", 9)) {
			x64cc = FALSE;
			argi += 1;
		}
		if (proc = GetProcAddress(dll, argv[argi++].bytes)) {
#if _WIN64
			extern int RunProc64(
				FARPROC proc, int remaining, multi_t * argv, BOOL x64cc);
			RunProc64(proc, argc - argi, argv + argc - 1, x64cc);
#else
			__asm {
				mov esi, argv
				mov ebx, argc
				mov ecx, argi
				dec ebx
			_more_args :
				cmp ebx, ecx
				jl _call_fp
				push [esi + 4 * ebx]
				dec ebx
				jmp _more_args
			_call_fp :
				call proc
			}
#endif
		}
	}
	__assume(neverm0re);
	WaitForSingleObject(neverm0re, INFINITE);

EPILOGUE:
	epilogue();
}