#include "WindowsLokinetProcessManager.hpp"

#include <QProcess>
#include <QDebug>
#include <QStandardPaths>

#ifdef Q_OS_WIN

#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

WindowsLokinetProcessManager::WindowsLokinetProcessManager()
{
    ::CreateMutexA(nullptr, FALSE, "lokinet_qt5_ui");
}

bool WindowsLokinetProcessManager::doStartLokinetProcess()
{
    // try searching one level up from CWD
    bool success = success = QProcess::startDetached("net start lokinet");
    if (success)
        qDebug("QProcess::startDetached() failed");
    return success;
}

bool WindowsLokinetProcessManager::doForciblyStopLokinetProcess()
{
    QProcess::startDetached("net stop lokinet");
    return true;
}

bool WindowsLokinetProcessManager::doGetProcessPid(int& pid)
{

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry) != TRUE)
    {
        CloseHandle(snapshot);
        qDebug() << "Process32First() returned false";
        return false;
    }
    else
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, "lokinet.exe") == 0)
            {  
                pid = entry.th32ProcessID;
                qDebug("lokinet pid: %d", pid);
                return true;
            }
        }
    }

    CloseHandle(snapshot);

    qDebug() << "No lokinet.exe found";

    return true;
}

QString WindowsLokinetProcessManager::getDefaultBootstrapFileLocation()
{
    return "C:\\ProgramData\\.lokinet\\bootstrap.signed";
}

#endif // Q_OS_WIN
