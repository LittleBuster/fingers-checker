#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>

#include <fstream>

SERVICE_STATUS        g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler (DWORD);
DWORD WINAPI ServiceWorkerThread (LPVOID lpParam);

#define SERVICE_NAME  L"Print Service"

void addLog(char * c,char * mm=0);
void addLog(DWORD  c, char * mm=0 );

int main (int argc, char *argv[])
{
    addLog("Print Service: Main: Entry");

    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
        {NULL, NULL}
    };

    if (StartServiceCtrlDispatcher (ServiceTable) == FALSE)
    {
       addLog("Print Service: Main: StartServiceCtrlDispatcher returned error");
       addLog(GetLastError() );
       return GetLastError ();
    }

    addLog("Print Service: Main: Exit");
    return 0;
}


VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv)
{
    DWORD Status = E_FAIL;

    addLog("Print Service: ServiceMain: Entry");

    g_StatusHandle = RegisterServiceCtrlHandler (SERVICE_NAME, ServiceCtrlHandler);

    if (g_StatusHandle == NULL)
    {
        addLog("Print Service: ServiceMain: RegisterServiceCtrlHandler returned error");
        return;
    }
    ZeroMemory (&g_ServiceStatus, sizeof (g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
    {
        addLog("Print Service: ServiceMain: SetServiceStatus returned error");
    }
   addLog("Print Service: ServiceMain: Performing Service Start Operations");

    g_ServiceStopEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL)
    {
        addLog("Print Service: ServiceMain: CreateEvent(g_ServiceStopEvent) returned error");

        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwWin32ExitCode = GetLastError();
        g_ServiceStatus.dwCheckPoint = 1;

        if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
        {
           addLog("Print Service: ServiceMain: SetServiceStatus returned error");
        }
        return;
    }
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
    {
        addLog("Print Service: ServiceMain: SetServiceStatus returned error");
    }
    HANDLE hThread = CreateThread (NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

    addLog("Print Service: ServiceMain: Waiting for Worker Thread to complete");
    WaitForSingleObject (hThread, INFINITE);
    addLog("Print Service: ServiceMain: Worker Thread Stop Event signaled");
    addLog("Print Service: ServiceMain: Performing Cleanup Operations");

    CloseHandle (g_ServiceStopEvent);

    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 3;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE) {
        addLog("Print Service: ServiceMain: SetServiceStatus returned error");
    }
    addLog("Print Service: ServiceMain: Exit");
    return;
}


VOID WINAPI ServiceCtrlHandler (DWORD CtrlCode)
{
    addLog("Print Service: ServiceCtrlHandler: Entry","serh.txt");

    switch (CtrlCode)
    {
     case SERVICE_CONTROL_STOP :
       addLog("Print Service: ServiceCtrlHandler: SERVICE_CONTROL_STOP Request", "serh.txt");
        if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
           break;

        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        g_ServiceStatus.dwWin32ExitCode = 0;
        g_ServiceStatus.dwCheckPoint = 4;

        if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
        {
            addLog("Print Service: ServiceCtrlHandler: SetServiceStatus returned error", "serh.txt");
        }
        SetEvent (g_ServiceStopEvent);
        break;
     default:
         break;
    }
   addLog("Print Service: ServiceCtrlHandler: Exit","serh.txt");
}


DWORD WINAPI ServiceWorkerThread (LPVOID lpParam)
{
   ShellExecute(NULL, L"open", L"C:\\PrintServer\\PrintServer.exe", NULL, NULL, SW_SHOWNORMAL);
    while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0) {
        Sleep(3000);
    }
    return ERROR_SUCCESS;
}


void addLog(char *c, char *mm )
{
    if(!mm)
    {
        mm="myfile.txt";
    }
    char name[255] = "c:\\PrintServer\\";
    strcat(name,mm);

    std::ofstream file;
    file.open(name,std::ios::app);
    file << c;
    file << std::endl;
    file.close();

}


void addLog(DWORD  c, char * mm )
{
    if(!mm)
    {
        mm = "mylog.log";
    }
    char name[255] = "c:\\PrintServer\\";
    strcat(name,mm);

    std::ofstream file;
    file.open(name,std::ios::app);
    file << "ERROR : "<<c;
    file << std::endl;
    file.close();
}
