@echo off
del release.7z
>NUL: 7z a release.7z ^
	Release/x86/dllrun/dllrun.exe ^
	Release/x86/binrun/binrun.exe ^
	Release/x64/dllrun/dllrun.exe ^
	Release/x64/binrun/binrun.exe
>NUL: 7z rn release.7z Release/x86/dllrun/dllrun.exe dllrun32.exe
>NUL: 7z rn release.7z Release/x86/binrun/binrun.exe binrun32.exe
>NUL: 7z rn release.7z Release/x64/dllrun/dllrun.exe dllrun64.exe
>NUL: 7z rn release.7z Release/x64/binrun/binrun.exe binrun64.exe