#include <process.h>
#include <windows.h>


#pragma optimize("gy", on)

#pragma comment(lib,"kernel32")
#pragma comment(lib,"user32")

#pragma comment(linker, "/ALIGN:16")
#pragma comment(linker, "/MERGE:.rdata=.data")
#pragma comment(linker, "/MERGE:.text=.data")

#pragma comment(linker,"/SUBSYSTEM:WINDOWS")


void WinMainCRTStartup(void)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    int bWait = 0;
    DWORD exitcode = 0;
    char stopchar = ' ';
    char* lpszCmd = GetCommandLine();

    if (lpszCmd[0] == '"') stopchar = '"';
    do {
        lpszCmd++;
    }
    while ((lpszCmd[0] != stopchar) && (lpszCmd[0] != 0));
    if (lpszCmd[0] != 0)
    {
        do {
            lpszCmd++;
        }
        while ((lpszCmd[0] != 0) && ((lpszCmd[0] == ' ') || (lpszCmd[0] == 't')));
    };
    if (lpszCmd[0] == 0)
    {
        MessageBox(0, "About:\n\nhidec hides console window of started program & waits (opt.) for its termination\n\nUsage:\n\n\thidec [/w] <filename>\n\nWhere:\n\n/w\twait for program termination\nfilename\texecutable file name", "Error: Incorrect usage", 0);
        ExitProcess(0);
    };

    if ((lpszCmd[0] == '/')&&(((lpszCmd[1])|0x20) == 'w')&&(lpszCmd[2] == ' '))
    {
        bWait = 1;
        lpszCmd += 3;
    };
    while ((lpszCmd[0] != 0) && ((lpszCmd[0] == ' ') || (lpszCmd[0] == 't'))) lpszCmd++;

    /* create process with new console */
    unsigned char *ps = (unsigned char*)&si;
    for (unsigned int i = 0; i < sizeof(si); i++) ps[ i ] = 0x00;

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    if( CreateProcess( NULL, lpszCmd,
                       NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
                       NULL, NULL, & si, & pi ) )
    {
        if (bWait) WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
    }
    else
        exitcode = GetLastError();

    /* terminate this */
    ExitProcess(exitcode);
}
