#include <windows.h>
#define Malloc(s) CustomMalloc(s)
#define Free(s) CustomFree(s)

LPVOID
WINAPIV
CustomMalloc(
        _Out_ SIZE_T dwSize
) {
        LPVOID lpMemory = VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (lpMemory == NULL) {
                DWORD dwError = GetLastError();
                WriteErrorToConsole(L"VirtualAlloc failed with error", dwError);
                return NULL;
        }
        return lpMemory;
}

VOID
WINAPIV
CustomFree(
        _In_ LPVOID lpMemory
) {
        if (lpMemory == NULL) {
                return;
        }
        if (!VirtualFree(lpMemory, 0, MEM_RELEASE)) {
                DWORD dwError = GetLastError();
                WriteErrorToConsole(L"VirtualFree failed with error", dwError);
        }
}

VOID
WINAPIV
WriteToConsole(
        _In_ LPCWSTR lpMessage
) {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdout == INVALID_HANDLE_VALUE) {
                DWORD dwError = GetLastError();
                WriteErrorToConsole(L"GetStdHandle failed with error", dwError);
                return;
        }

        DWORD dwBytesWritten;
        BOOL bSuccess = WriteConsoleW(hStdout, lpMessage, lstrlenW(lpMessage), &dwBytesWritten, NULL);
        if (!bSuccess) {
                DWORD dwError = GetLastError();
                WriteErrorToConsole(L"WriteConsoleW failed with error", dwError);
        }
}

LPWSTR
WINAPIV
GetLastErrorMessage(
        _In_ DWORD dwError
) {
        LPWSTR lpMsgBuf = NULL;
        FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dwError,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf,
                0,
                NULL
        );
        return lpMsgBuf;
}

VOID
WINAPIV
WriteErrorToConsole(
        _In_ LPCWSTR lpContext,
        _In_ DWORD dwError
) {
        LPWSTR lpErrorMessage = GetLastErrorMessage(dwError);
        WCHAR szBuffer[256];
        wsprintfW(szBuffer, L"%s %lu: %s\n", lpContext, dwError, lpErrorMessage);
        WriteToConsole(szBuffer);
        LocalFree(lpErrorMessage);
}
