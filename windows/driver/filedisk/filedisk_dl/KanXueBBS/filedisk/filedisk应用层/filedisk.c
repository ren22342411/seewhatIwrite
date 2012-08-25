///////////////////////////////////////////////////////
//�ð汾��ARG PARSER���ִ�������
//�ڴ���5������������ȱ���ļ�����״��û�а취���н���
//BUGһ��
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

    VolumeName[4] = DriveLetter; //����������"\\\\.\\D:"

	////////////////////////////////////////////////////////////////
	//���Ը�������Ƿ��Ѿ����ڣ�
	//����Ѿ�����
	//���������
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

    if (Device != INVALID_HANDLE_VALUE)	//�����жϵ��ǺϷ������
    {
        SetLastError(ERROR_BUSY);
        PrintLastError(&VolumeName[4]);
        return -1;
    }
	
	////////////////////////////////////////////////////////////////
	//���������
	//��ʼ���������̷�
	//DEVICE_NAME_PREFIX=��\Device\FileDisk\FileDisk��
	////////////////////////////////////////////////////////////////
    
	sprintf(DeviceName, DEVICE_NAME_PREFIX "%u", DeviceNumber);

    ////////////////////////////////////////////////////////////////
	//���е�VolumeName��IO���󶼽����ض���DeviceName
	//ִ�е������ϵͳ��Ӧ�ÿ��Կ���һ��������̷�����
	//���ǻ����ܶԸ��̷����κβ���
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
	//�������̷�
	//������豸�ɹ��������䷢��IOCTL_FILE_DISK_OPEN_FILE IRP
	//����IOCTL_FILE_DISK_OPEN_FILE��filedisk.h������ж���
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

	//mount�������������
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
	//������ص���INVALID_HANDLE_VALUE
	//����ζ�Ÿ������̲����ڣ�����ִ��/umount�Ķ���
	//���������ʾ�Ժ󷵻�
	//////////////////////////////////////////////////////////////////
    if (Device == INVALID_HANDLE_VALUE)
    {
        PrintLastError(&VolumeName[4]);
        return -1;
    }
	//////////////////////////////////////////////////////////////////
	//�����������ֵ����������̷�����
	//��ʼִ��/umount�Ķ���
	//�������¼������裺
	//	1��������ǰ��ͨ������FSCTL_LOCK_VOLUME���豸����ʵ��
	//	2���ر����иþ��ϴ򿪵������ļ���ͨ������IOCTL_FILE_DISK_CLOSE_FILE���豸����ʵ��
	//	3��ж�ظþ�ͨ������FSCTL_DISMOUNT_VOLUME���豸����ʵ��
	//	4������þ��������ͨ������FSCTL_UNLOCK_VOLUME���豸����ʵ��
	//	5���ر��豸
	//	6��ɾ�������̷�
	//////////////////////////////////////////////////////////////////
    
	
	//������Ҫж�صľ�
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
	
	//�رոþ��ϴ򿪵������ļ�
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
	//ж�ظþ�
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
	//����Ըþ������
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
	//�ر��豸HANDLE
	//ִ�е����ϵͳ��Ӧ�û��ǿ��Կ���һ��������̷�
	//�����Ѿ����ܶԸ��̷����κβ�����
	////////////////////////////////////////////////////////////
    CloseHandle(Device);

	///////////////////////////////////////////////////////////
	//ɾ�������̷�
	//ִ����ò�����ϵͳ�п������̷�Ӧ����ʧ
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
//filedisk��������

int 
main(int argc, char* argv[])
{
    char*                   Command;				//��Ҫ���еĲ�����/mount || /umount
    int                     DeviceNumber;			//0--4
    char*                   FileName;				//��������������õ�ȫ�ļ���
    char*                   Option;					//ѡ��/ro|/cd
    char                    DriveLetter;			//��������̷�
    POPEN_FILE_INFORMATION  OpenFileInformation;	//���ļ�����Ϣ��
													//��filedisk.h�ж���
	printf("DEVICE_BASE_NAME\t=\t %s\n",DEVICE_BASE_NAME);
	printf("DEVICE_DIR_NAME \t=\t %s\n",DEVICE_DIR_NAME);
	printf("DEVICE_NAME_PREFIX\t=\t %s\n",DEVICE_NAME_PREFIX);

//////////////////////////////////////////////////////////////////////////////
//				�Գ���Ĳ������н�����
//				��������6������������/MOUNTʱOPTION��ѡ��
//				����/UMOUNTֻ��һ������
//////////////////////////////////////////////////////////////////////////////

	
	Command = argv[1];								//��һ������

    if ((argc == 5 || argc == 6) && !strcmp(Command, "/mount"))
    {
        DeviceNumber = atoi(argv[2]);				//�ڶ�������
        FileName = argv[3];							//����������

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
            Option = argv[4];				//���ĸ�������ѡ��
            DriveLetter = argv[5][0];		//���������ֻȡһ����ĸ��
											//��C����DriveLetter=C

           //����ѡ��
			if (!strcmp(Option, "/ro"))		//ֻ����ʽMOUNT
            {
                OpenFileInformation->ReadOnly = TRUE;
            }
            else if (!strcmp(Option, "/cd"))//����CD_ROM
            {
                OpenFileInformation->DeviceType = FILE_DEVICE_CD_ROM;
            }
            else							//����������̵�������λ
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
