/*
    Control program for a virtual disk driver for Windows NT/2000/XP/Vista.
    Copyright (C) 1999-2008 Bo Brantén.
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <windows.h>
#include <winioctl.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>
#include "filedisk_sys_exe_common.h"

int FileDiskUmount(char DriveLetter);

int FileDiskSyntax(void)
{
    fprintf(stderr, "syntax:\n");
    fprintf(stderr, "FileDiskEx.exe /mount <filename> [size[k|M|G] | /ro | /cd] <drive:>\n");
    fprintf(stderr, "FileDiskEx.exe /umount <drive:>\n");
    fprintf(stderr, "FileDiskEx.exe /status <drive:>\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "filename formats:\n");
    fprintf(stderr, "  c:\\path\\filedisk.img\n");
    fprintf(stderr, "  \\Device\\Harddisk0\\Partition1\\path\\filedisk.img\n");
    fprintf(stderr, "  \\\\server\\share\\path\\filedisk.img\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "example:\n");
    fprintf(stderr, "FileDiskEx.exe /mount c:\\temp\\filedisk.img 8M f:\n");
    fprintf(stderr, "FileDiskEx.exe /mount c:\\temp\\cdimage.iso /cd i:\n");
    fprintf(stderr, "FileDiskEx.exe /umount f:\n");
    fprintf(stderr, "FileDiskEx.exe /umount i:\n");

    return -1;
}

void PrintLastError(char* Prefix)
{
    LPVOID lpMsgBuf;

    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        0,
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
        );

    fprintf(stderr, "%s %s", Prefix, (LPTSTR) lpMsgBuf);

    LocalFree(lpMsgBuf);
}

int
FileDiskMount( PMOUNT_INFORMATION MountInformation )
{
    char    DataVolumeName[] = "\\\\.\\ :";
    char    DriveName[] = " :\\";
    
    DataVolumeName[4] = MountInformation->DriveLetter;
    DriveName[0] = MountInformation->DriveLetter;

    HANDLE  hDataDev = CreateFile(
        DataVolumeName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );

    if (hDataDev != INVALID_HANDLE_VALUE) // Has been occupied already
    {
        SetLastError(ERROR_BUSY);
        PrintLastError(&DataVolumeName[4]);
        return -1;
    }

    HANDLE hCtrlDev = CreateFile(
        CTRLDEV_SYMLINK_ACCESS,
        //"\\Device\\FileDisk\\FileDisk",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );

    if (hCtrlDev == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "File Disk Control Device Open Failed!\n");
        return -1;
    }

    DWORD   BytesReturned;

    WCHAR dataDevNameU[255] = {0};
    CHAR dataDevNameA[255] = {0};

    if (!DeviceIoControl(
        hCtrlDev,
        IOCTL_FILE_DISK_OPEN_FILE,
        MountInformation,
        sizeof(MOUNT_INFORMATION) + MountInformation->FileNameLength - 1,
        (PVOID)dataDevNameU,
        sizeof(WCHAR)*255,
        &BytesReturned,
        NULL
        ))
    {
        fprintf(stderr, "Wrong %u\n", BytesReturned);
        CloseHandle(hCtrlDev);
        PrintLastError("FileDiskEx:");
        return -1;
    }

    dataDevNameU[BytesReturned/2] = 0;

    DWORD wchrCnt = WideCharToMultiByte(CP_OEMCP,NULL,dataDevNameU,-1,NULL,0,NULL,FALSE);

    WideCharToMultiByte (CP_OEMCP,NULL,dataDevNameU,-1,dataDevNameA,wchrCnt,NULL,FALSE);

    if (!DefineDosDevice(
        DDD_RAW_TARGET_PATH,
        &DataVolumeName[4],
        dataDevNameA
        ))
    {
        PrintLastError(&DataVolumeName[4]);
        return -1;
    }

     hDataDev = CreateFile(
        DataVolumeName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );

    if (hDataDev == INVALID_HANDLE_VALUE)
    {
        FileDiskUmount(MountInformation->DriveLetter);
        PrintLastError(&DataVolumeName[4]);
        CloseHandle(hDataDev);
        CloseHandle(hCtrlDev);
        return -1;
    }

    //DefineDosDevice(DDD_REMOVE_DEFINITION, &DataVolumeName[4], NULL); Has been done in FileDiskUmount

    SHChangeNotify(SHCNE_DRIVEADD, SHCNF_PATH, DriveName, NULL);

    CloseHandle(hDataDev);

    CloseHandle(hCtrlDev);

    return 0;
}

int FileDiskUmount(char DriveLetter)
{
    char    VolumeName[] = "\\\\.\\ :";
    char    DriveName[] = " :\\";
    HANDLE  hDataDevice;
    DWORD   BytesReturned;

    VolumeName[4] = DriveLetter;
    DriveName[0] = DriveLetter;

    hDataDevice = CreateFile(
        VolumeName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );

    if (hDataDevice == INVALID_HANDLE_VALUE)
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    if (!DeviceIoControl(
        hDataDevice,
        FSCTL_LOCK_VOLUME,
        NULL,
        0,
        NULL,
        0,
        &BytesReturned,
        NULL
        ))
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    if (!DeviceIoControl(
        hDataDevice,
        FSCTL_DISMOUNT_VOLUME,
        NULL,
        0,
        NULL,
        0,
        &BytesReturned,
        NULL
        ))
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    if (!DeviceIoControl(
        hDataDevice,
        FSCTL_UNLOCK_VOLUME,
        NULL,
        0,
        NULL,
        0,
        &BytesReturned,
        NULL
        ))
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    CloseHandle(hDataDevice);

    if (!DefineDosDevice(
        DDD_REMOVE_DEFINITION,
        &VolumeName[4],
        NULL
        ))
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    HANDLE hCtrlDevice = CreateFile(
        CTRLDEV_SYMLINK_ACCESS,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );

    if (hCtrlDevice == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "File Disk Control Device Open Failed!\n");
        return -1;
    }

    if (!DeviceIoControl(
        hCtrlDevice,
        IOCTL_FILE_DISK_CLOSE_FILE,
        &DriveLetter,
        sizeof(char),
        NULL,
        0,
        &BytesReturned,
        NULL
        ))
    {
        CloseHandle(hCtrlDevice);
        PrintLastError("FileDiskEx:");
        return -1;
    }

    CloseHandle(hCtrlDevice);

    SHChangeNotify(SHCNE_DRIVEREMOVED, SHCNF_PATH, DriveName, NULL);

    return 0;
}

int FileDiskStatus(char DriveLetter)
{
    char                    VolumeName[] = "\\\\.\\ :";
    HANDLE                  Device;
    PMOUNT_INFORMATION      MountInformation;
    DWORD                   BytesReturned;

    VolumeName[4] = DriveLetter;

    Device = CreateFile(
        VolumeName,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );

    if (Device == INVALID_HANDLE_VALUE)
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    MountInformation = (PMOUNT_INFORMATION)malloc(sizeof(MOUNT_INFORMATION) + MAX_PATH);

    if (!DeviceIoControl(
        Device,
        IOCTL_FILE_DISK_QUERY_FILE,
        NULL,
        0,
        MountInformation,
        sizeof(MOUNT_INFORMATION) + MAX_PATH,
        &BytesReturned,
        NULL
        ))
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    if (BytesReturned < sizeof(MOUNT_INFORMATION))
    {
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    printf("%c: %.*s %I64u bytes%s\n",
        DriveLetter,
        MountInformation->FileNameLength,
        MountInformation->FileName,
        MountInformation->FileSize,
        MountInformation->ReadOnly ? " ro" : ""
        );

    return 0;
}

int __cdecl main(int argc, char* argv[])
{
    char*                   Command;
    int                     DeviceNumber;
    char*                   FileName;
    char*                   Option;
    char                    DriveLetter;
    PMOUNT_INFORMATION      MountInformation;

    Command = argv[1];

    if ((argc == 4 || argc == 5) && !strcmp(Command, "/mount"))
    {
        //DeviceNumber = atoi(argv[2]);
        FileName = argv[2];

        if (strlen(FileName) < 2)
        {
            return FileDiskSyntax();
        }

        MountInformation =
            (PMOUNT_INFORMATION)malloc(sizeof(MOUNT_INFORMATION) + strlen(FileName) + 7);

        memset(
            MountInformation,
            0, // MountInformation->ReadOnly = FALSE; MountInformation->CdImage = FALSE;
            sizeof(MOUNT_INFORMATION) + strlen(FileName) + 7
            );

        if (FileName[0] == '\\')
        {
            if (FileName[1] == '\\')
                // \\server\share\path\filedisk.img
            {
                strcpy((char *)MountInformation->FileName, "\\??\\UNC");
                strcat((char *)MountInformation->FileName, FileName + 1);
            }
            else
                // \Device\Harddisk0\Partition1\path\filedisk.img
            {
                strcpy((char *)MountInformation->FileName, FileName);
            }
        }
        else
            // c:\path\filedisk.img
        {
            strcpy((char *)MountInformation->FileName, "\\??\\");
            strcat((char *)MountInformation->FileName, FileName);
        }

        MountInformation->FileNameLength =
            (USHORT) strlen((char *)MountInformation->FileName);

        if (argc > 4)
        {
            Option = argv[3];
            DriveLetter = argv[4][0];

            if (!strcmp(Option, "/ro"))
            {
                MountInformation->ReadOnly = TRUE;
            }
            else if (!strcmp(Option, "/cd"))
            {
                MountInformation->CdImage = TRUE;
            }
            else
            {
                if (Option[strlen(Option) - 1] == 'G')
                {
                    MountInformation->FileSize.QuadPart =
                        _atoi64(Option) * 1024 * 1024 * 1024;
                }
                else if (Option[strlen(Option) - 1] == 'M')
                {
                    MountInformation->FileSize.QuadPart =
                        _atoi64(Option) * 1024 * 1024;
                }
                else if (Option[strlen(Option) - 1] == 'k')
                {
                    MountInformation->FileSize.QuadPart =
                        _atoi64(Option) * 1024;
                }
                else
                {
                    MountInformation->FileSize.QuadPart =
                        _atoi64(Option);
                }
            }
        }
        else
        {
            DriveLetter = argv[3][0];
        }
        MountInformation->DriveLetter = DriveLetter;
        return FileDiskMount(MountInformation);
    }
    else if (argc == 3 && !strcmp(Command, "/umount"))
    {
        DriveLetter = argv[2][0];
        return FileDiskUmount(DriveLetter);
    }
    else if (argc == 3 && !strcmp(Command, "/status"))
    {
        DriveLetter = argv[2][0];
        return FileDiskStatus(DriveLetter);
    }
    else
    {
        return FileDiskSyntax();
    }
}
