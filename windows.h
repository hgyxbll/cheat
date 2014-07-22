#include <wtypes.h>

WINBASEAPI
UINT
WINAPI
SetErrorMode(
		__in  UINT  uMode
	);

WINBASEAPI
VOID
WINAPI
ZeroMemory(
		_In_  PVOID   Destination,
		_In_  SIZE_T  Length
	);

WINBASEAPI
BOOL
WINAPI
CreatePipe(
		__out_ecount_full(1)  PHANDLE                hReadPipe,
		__out_ecount_full(1)  PHANDLE                hWritePipe,
		__in_opt              LPSECURITY_ATTRIBUTES  lpPipeAttributes,
		__in                  DWORD                  nSize
	);

WINBASEAPI
__checkReturn
DWORD
WINAPI
GetLastError(
		VOID
	);

WINBASEAPI
__out
LPSTR
WINAPI
GetCommandLineA(
		VOID
	);

WINBASEAPI
__out
LPWSTR
WINAPI
GetCommandLineW(
		VOID
	);

WINBASEAPI
BOOL
WINAPI
CreateProcessA(
		__in_opt     LPCSTR                 lpApplicationName,
		__inout_opt  LPSTR                  lpCommandLine,
		__in_opt     LPSECURITY_ATTRIBUTES  lpProcessAttributes,
		__in_opt     LPSECURITY_ATTRIBUTES  lpThreadAttributes,
		__in         BOOL                   bInheritHandles,
		__in         DWORD                  dwCreationFlags,
		__in_opt     LPVOID                 lpEnvironment,
		__in_opt     LPCSTR                 lpCurrentDirectory,
		__in         LPSTARTUPINFOA         lpStartupInfo,
		__out        LPPROCESS_INFORMATION  lpProcessInformation
	);

WINBASEAPI
BOOL
WINAPI
CreateProcessW(
		__in_opt     LPCWSTR                lpApplicationName,
		__inout_opt  LPWSTR                 lpCommandLine,
		__in_opt     LPSECURITY_ATTRIBUTES  lpProcessAttributes,
		__in_opt     LPSECURITY_ATTRIBUTES  lpThreadAttributes,
		__in         BOOL                   bInheritHandles,
		__in         DWORD                  dwCreationFlags,
		__in_opt     LPVOID                 lpEnvironment,
		__in_opt     LPCWSTR                lpCurrentDirectory,
		__in         LPSTARTUPINFOW         lpStartupInfo,
		__out        LPPROCESS_INFORMATION  lpProcessInformation
	);

WINBASEAPI
BOOL
WINAPI
CloseHandle(
		__in  HANDLE  hObject
	);

WINBASEAPI
BOOL
WINAPI
ReadFile(
		__in                       HANDLE        hFile,
		__out_bcount_part_opt(
			nNumberOfBytesToRead,
			*lpNumberOfBytesRead
		) __out_data_source(FILE)  LPVOID        lpBuffer,
		__in                       DWORD         nNumberOfBytesToRead,
		__out_opt                  LPDWORD       lpNumberOfBytesRead,
		__inout_opt                LPOVERLAPPED  lpOverlapped
	);

WINBASEAPI
DWORD
WINAPI
WaitForSingleObject(
		__in  HANDLE  hHandle,
		__in  DWORD   dwMilliseconds
	);

WINBASEAPI
BOOL
WINAPI
GetExitCodeProcess(
		__in   HANDLE   hProcess,
		__out  LPDWORD  lpExitCode
	);

WINBASEAPI
VOID
WINAPI
Sleep(
		__in  DWORD  dwMilliseconds
	);
