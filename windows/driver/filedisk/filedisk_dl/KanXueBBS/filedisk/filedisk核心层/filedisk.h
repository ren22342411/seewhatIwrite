
#ifndef _FILE_DISK_
#define _FILE_DISK_

#ifndef __T
#ifdef _NTDDK_
#define __T(x)  L ## x
#else
#define __T(x)  x
#endif
#endif

#ifndef _T
#define _T(x)   __T(x)
#endif

#define DEVICE_BASE_NAME    _T("\\FileDisk")
#define DEVICE_DIR_NAME     _T("\\Device")      DEVICE_BASE_NAME
#define DEVICE_NAME_PREFIX  DEVICE_DIR_NAME     DEVICE_BASE_NAME

#define FILE_DEVICE_FILE_DISK       0x8000//用户定义范围、0x8000~0xffff

#define IOCTL_FILE_DISK_OPEN_FILE   CTL_CODE(FILE_DEVICE_FILE_DISK, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_FILE_DISK_CLOSE_FILE  CTL_CODE(FILE_DEVICE_FILE_DISK, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_FILE_DISK_QUERY_FILE  CTL_CODE(FILE_DEVICE_FILE_DISK, 0x802, METHOD_BUFFERED, FILE_READ_ACCESS)


typedef struct _OPEN_FILE_INFORMATION {
    LARGE_INTEGER   FileSize;//文件大小
    BOOLEAN         ReadOnly;//只读属性
    USHORT          FileNameLength;//文件名长度
    UCHAR           FileName[1];//文件名
} OPEN_FILE_INFORMATION, *POPEN_FILE_INFORMATION;

//定义控制代码分别是“打开文件”、“关闭文件”、“查询？”

#endif
