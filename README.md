# Malware Code Runners

The repository contains two projects to execute code from malware samples that don't just
execute by themselves, i.e. shellcode and DLLs. 

**WARNING.** I am not sure if these tools are working properly right now, they are work in
progress. The code is dumped here so I can share it with others. Once the tools have seen
some use and I am convinced they work, this warning will disappear.

## Compiles With

Microsoft Visual Studio Community 2019, Version 16.10.3

## Does What

The tools included here are the following:

- [binrun](binrun.c) executes shellcode blobs from a file
- [dllrun](dllrun.c) can be used to execute routines from a dynamic link library

The `dllrun` tool has been given a substantially greater amount of love than `binrun`.
It will even display a help message:
```
C:\> dllrun.exe
USAGE: dllrun <dll> [<base>|0] [[_stdcall] <proc>] [<args>]
  dll: The path to the DLL.
 base: The base to which the image should be rebased. Specify 0 to omit rebasing.
 proc: Name of a procedure to execute from the loaded library.
 args: Arguments to be passed to the procedure. Arguments are parsed as follows:
       - integers in decimal or hexadecimal (0x-prefix) notation are are passed as integers.
       - hex strings are decoded and passed as pointers to the decoded buffer.
       - all other strings are passed as ASCII strings.
```
