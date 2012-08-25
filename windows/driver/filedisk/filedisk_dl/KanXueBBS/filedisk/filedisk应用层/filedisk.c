///////////////////////////////////////////////////////
//该版本的ARG PARSER部分存在问题
//在带有5个参数，但是缺少文件名的状况没有办法进行解析
//BUG一个
////////////////////////////////////////////////////////

#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include "filedisk.h"

int Syntax(void)
{
    fprintf(stderr, "syntax:\n");
    fprintf(stderr, "filedisk /mount  devicenumber filename [size[k|M|G] | /ro | /cd] driveletter\n");
    fprintf(stderr, "filedisk /umount driveletter\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "example:\n");
    fprintf(stderr, "filedisk /mount  0 c:\\temp\\filedisk.img 8M f:\n");
    fprintf(stderr, "filedisk /mount  1 c:\\temp\\cdimage.iso /cd i:\n");
    fprintf(stderr, "filedisk /umount f:\n");
    fprintf(stderr, "filedisk /umount i:\n");

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
Mount(
    int                     DeviceNumber,
    POPEN_FILE_INFORMATION  OpenFileInformation,
    char                    DriveLetter
	)
{
    char    VolumeName[] = "\\\\.\\ :";
    char    DeviceName[255];
    HANDLE  Device;
    DWORD   BytesReturned;

    VolumeName[4] = DriveLetter; //处理结果类似"\\\\.\\D:"

	////////////////////////////////////////////////////////////////
	//测试该虚拟卷是否已经存在，
	//如果已经存在
	//则输出错误
	////////////////////////////////////////////////////////////////
    Device = CreateFile(
        VolumeName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );

    if (Device != INVALID_HANDLE_VALUE)	//这里判断的是合法的情况
    {
        SetLastError(ERROR_BUSY);
        PrintLastError(&VolumeName[4]);
        return -1;
    }
	
	////////////////////////////////////////////////////////////////
	//如果不存在
	//则开始建立虚拟盘符
	//DEVICE_NAME_PREFIX=“\Device\FileDisk\FileDisk”
	////////////////////////////////////////////////////////////////
    
	sprintf(DeviceName, DEVICE_NAME_PREFIX "%u", DeviceNumber);

    ////////////////////////////////////////////////////////////////
	//所有到VolumeName的IO请求都将被重定向到DeviceName
	//执行到这里，在系统中应该可以看见一个虚拟的盘符出现
	//但是还不能对该盘符做任何操作
	////////////////////////////////////////////////////////////////

	if (!DefineDosDevice(
        DDD_RAW_TARGET_PATH,
        &VolumeName[4],
        DeviceName
        ))
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }
    
	////////////////////////////////////////////////////////////////
	//打开虚拟盘符
	//如果打开设备成功，则向其发送IOCTL_FILE_DISK_OPEN_FILE IRP
	//其中IOCTL_FILE_DISK_OPEN_FILE在filedisk.h里面进行定义
	////////////////////////////////////////////////////////////////
    Device = CreateFile(
        VolumeName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );

    if (Device == INVALID_HANDLE_VALUE)
    {
        PrintLastError(&VolumeName[4]);
        DefineDosDevice(DDD_REMOVE_DEFINITION, &VolumeName[4], NULL);
        return -1;
    }

    if (!DeviceIoControl(
        Device,
        IOCTL_FILE_DISK_OPEN_FILE,
        OpenFileInformation,
        sizeof(OPEN_FILE_INFORMATION) + OpenFileInformation->FileNameLength - 1,
        NULL,
        0,
        &BytesReturned,
        NULL
        ))
    {
        PrintLastError("FileDisk:");
        DefineDosDevice(DDD_REMOVE_DEFINITION, &VolumeName[4], NULL);
        return -1;
    }

    return 0;

	//mount操作到这里结束
}

int Umount(char DriveLetter)
{
    char    VolumeName[] = "\\\\.\\ :";
    HANDLE  Device;
    DWORD   BytesReturned;

    VolumeName[4] = DriveLetter;

    Device = CreateFile(
        VolumeName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING,
        NULL
        );
	//////////////////////////////////////////////////////////////////
	//如果返回的是INVALID_HANDLE_VALUE
	//则意味着该虚拟盘不存在，不能执行/umount的动作
	//输出错误提示以后返回
	//////////////////////////////////////////////////////////////////
    if (Device == INVALID_HANDLE_VALUE)
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }
	//////////////////////////////////////////////////////////////////
	//如果返回其他值，则虚拟的盘符存在
	//开始执行/umount的动作
	//包括以下几个步骤：
	//	1、锁定当前卷，通过发送FSCTL_LOCK_VOLUME到设备驱动实现
	//	2、关闭所有该卷上打开的所有文件，通过发送IOCTL_FILE_DISK_CLOSE_FILE到设备驱动实现
	//	3、卸载该卷，通过发送FSCTL_DISMOUNT_VOLUME到设备驱动实现
	//	4、解除该卷的锁定，通过发送FSCTL_UNLOCK_VOLUME到设备驱动实现
	//	5、关闭设备
	//	6、删除虚拟盘符
	//////////////////////////////////////////////////////////////////
    
	
	//锁定需要卸载的卷
	if (!DeviceIoControl(
        Device,
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
	
	//关闭该卷上打开的所有文件
    if (!DeviceIoControl(
        Device,
        IOCTL_FILE_DISK_CLOSE_FILE,
        NULL,
        0,
        NULL,
        0,
        &BytesReturned,
        NULL
        ))
    {
        PrintLastError("FileDisk:");
        return -1;
    }
	//卸载该卷
    if (!DeviceIoControl(
        Device,
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
	//解除对该卷的锁定
    if (!DeviceIoControl(
        Device,
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
	////////////////////////////////////////////////////////////
	//关闭设备HANDLE
	//执行到这里，系统中应该还是可以看到一个虚拟的盘符
	//但是已经不能对该盘符做任何操作了
	////////////////////////////////////////////////////////////
    CloseHandle(Device);

	///////////////////////////////////////////////////////////
	//删除虚拟盘符
	//执行完该操作，系统中看到的盘符应该消失
	///////////////////////////////////////////////////////////
    if (!DefineDosDevice(
        DDD_REMOVE_DEFINITION,
        &VolumeName[4],
        NULL
        ))
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }

    return 0;
}
//filedisk的主程序

int 
main(int argc, char* argv[])
{
    char*                   Command;				//所要进行的操作，/mount || /umount
    int                     DeviceNumber;			//0--4
    char*                   FileName;				//用来虚拟磁盘所用的全文件名
    char*                   Option;					//选项/ro|/cd
    char                    DriveLetter;			//被虚拟的盘符
    POPEN_FILE_INFORMATION  OpenFileInformation;	//打开文件的信息，
													//在filedisk.h中定义
	printf("DEVICE_BASE_NAME\t=\t %s\n",DEVICE_BASE_NAME);
	printf("DEVICE_DIR_NAME \t=\t %s\n",DEVICE_DIR_NAME);
	printf("DEVICE_NAME_PREFIX\t=\t %s\n",DEVICE_NAME_PREFIX);

//////////////////////////////////////////////////////////////////////////////
//				对程序的参数进行解析；
//				程序最多带6个参数，其中/MOUNT时OPTION可选的
//				程序/UMOUNT只带一个参数
//////////////////////////////////////////////////////////////////////////////

	
	Command = argv[1];								//第一个参数

    if ((argc == 5 || argc == 6) && !strcmp(Command, "/mount"))
    {
        DeviceNumber = atoi(argv[2]);				//第二个参数
        FileName = argv[3];							//第三个参数

        OpenFileInformation =
            malloc(sizeof(OPEN_FILE_INFORMATION) + strlen(FileName) + 4);

        memset(
            OpenFileInformation,
            0,
            sizeof(OPEN_FILE_INFORMATION) + strlen(FileName) + 4
            );

        strcpy(OpenFileInformation->FileName, "\\??\\");
        strcat(OpenFileInformation->FileName, FileName);
        OpenFileInformation->FileNameLength =
            strlen(OpenFileInformation->FileName);

        if (argc > 5)
        {
            Option = argv[4];				//第四个参数是选项
            DriveLetter = argv[5][0];		//第五个参数只取一个字母，
											//如C：则DriveLetter=C

           //解析选项
			if (!strcmp(Option, "/ro"))		//只读方式MOUNT
            {
                OpenFileInformation->ReadOnly = TRUE;
            }
            else if (!strcmp(Option, "/cd"))//虚拟CD_ROM
            {
                OpenFileInformation->DeviceType = FILE_DEVICE_CD_ROM;
            }
            else							//解析虚拟磁盘的容量单位
            {
                if (Option[strlen(Option) - 1] == 'G')
                {
                    OpenFileInformation->FileSize.QuadPart =
                        _atoi64(Option) * 1024 * 1024 * 1024;
                }
                else if (Option[strlen(Option) - 1] == 'M')
                {
                    OpenFileInformation->FileSize.QuadPart =
                        _atoi64(Option) * 1024 * 1024;
                }
                else if (Option[strlen(Option) - 1] == 'k')
                {
                    OpenFileInformation->FileSize.QuadPart =
                        _atoi64(Option) * 1024;
                }
                else
                {
                    OpenFileInformation->FileSize.QuadPart =
                        _atoi64(Option);
                }
            }
        }
        else
        {
            DriveLetter = argv[4][0];
        }
        return Mount(DeviceNumber, OpenFileInformation, DriveLetter);
    }
    else if (argc == 3 && !strcmp(Command, "/umount"))
    {
        DriveLetter = argv[2][0];
        return Umount(DriveLetter);
    }
    else
    {
        return Syntax();
    }
}
