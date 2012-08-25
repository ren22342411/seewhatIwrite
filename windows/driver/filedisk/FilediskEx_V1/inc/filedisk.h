/*
    This is a virtual disk driver for Windows NT/2000/XP that uses one or more
    files to emulate physical disks.
    Copyright (C) 1999, 2000, 2001, 2002 Bo Brantén.
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
#ifdef __cplusplus
extern "C"
{
#endif
#include <ntddk.h>
#include <ntdddisk.h>
#include <ntddcdrm.h>
#include <ntverp.h>
#ifdef __cplusplus
}
#endif 

#define PAGEDCODE code_seg("PAGE")
#define LOCKEDCODE code_seg()
#define INITCODE code_seg("INIT")

#define PAGEDDATA data_seg("PAGE")
#define LOCKEDDATA data_seg()
#define INITDATA data_seg("INIT")

#if (VER_PRODUCTBUILD < 2195)

#define FILE_DEVICE_MASS_STORAGE            0x0000002d
#define IOCTL_STORAGE_CHECK_VERIFY2         CTL_CODE(IOCTL_STORAGE_BASE, 0x0200, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000

#endif

#if (VER_PRODUCTBUILD < 2600)

#define IOCTL_DISK_GET_PARTITION_INFO_EX    CTL_CODE(IOCTL_DISK_BASE, 0x0012, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_GET_LENGTH_INFO          CTL_CODE(IOCTL_DISK_BASE, 0x0017, METHOD_BUFFERED, FILE_READ_ACCESS)

typedef enum _PARTITION_STYLE {
    PARTITION_STYLE_MBR,
    PARTITION_STYLE_GPT
} PARTITION_STYLE;

typedef unsigned __int64 ULONG64, *PULONG64;

typedef struct _PARTITION_INFORMATION_MBR {
    UCHAR   PartitionType;
    BOOLEAN BootIndicator;
    BOOLEAN RecognizedPartition;
    ULONG   HiddenSectors;
} PARTITION_INFORMATION_MBR, *PPARTITION_INFORMATION_MBR;

typedef struct _PARTITION_INFORMATION_GPT {
    GUID    PartitionType;
    GUID    PartitionId;
    ULONG64 Attributes;
    WCHAR   Name[36];
} PARTITION_INFORMATION_GPT, *PPARTITION_INFORMATION_GPT;

typedef struct _PARTITION_INFORMATION_EX {
    PARTITION_STYLE PartitionStyle;
    LARGE_INTEGER   StartingOffset;
    LARGE_INTEGER   PartitionLength;
    ULONG           PartitionNumber;
    BOOLEAN         RewritePartition;
    union {
        PARTITION_INFORMATION_MBR Mbr;
        PARTITION_INFORMATION_GPT Gpt;
    };
} PARTITION_INFORMATION_EX, *PPARTITION_INFORMATION_EX;

typedef struct _GET_LENGTH_INFORMATION {
    LARGE_INTEGER Length;
} GET_LENGTH_INFORMATION, *PGET_LENGTH_INFORMATION;

#endif // (VER_PRODUCTBUILD < 2600)

//
// We include some stuff from ntifs.h here so that
// the driver can be compiled with only the DDK.
//

#define MOUNTDEVCONTROLTYPE                         0x0000004D // 'M'
#define IOCTL_MOUNTDEV_QUERY_DEVICE_NAME    CTL_CODE(MOUNTDEVCONTROLTYPE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FTTYPE  ((ULONG)'f')
#define FT_BALANCED_READ_MODE    CTL_CODE(FTTYPE, 6, METHOD_NEITHER,  FILE_ANY_ACCESS)

#define TOKEN_SOURCE_LENGTH 8

typedef enum _TOKEN_TYPE {
    TokenPrimary = 1,
    TokenImpersonation
} TOKEN_TYPE;

typedef struct _TOKEN_SOURCE {
    CCHAR   SourceName[TOKEN_SOURCE_LENGTH];
    LUID    SourceIdentifier;
} TOKEN_SOURCE, *PTOKEN_SOURCE;

typedef struct _TOKEN_CONTROL {
    LUID            TokenId;
    LUID            AuthenticationId;
    LUID            ModifiedId;
    TOKEN_SOURCE    TokenSource;
} TOKEN_CONTROL, *PTOKEN_CONTROL;

typedef struct _SECURITY_CLIENT_CONTEXT {
    SECURITY_QUALITY_OF_SERVICE SecurityQos;
    PACCESS_TOKEN               ClientToken;
    BOOLEAN                     DirectlyAccessClientToken;
    BOOLEAN                     DirectAccessEffectiveOnly;
    BOOLEAN                     ServerIsRemote;
    TOKEN_CONTROL               ClientTokenControl;
} SECURITY_CLIENT_CONTEXT, *PSECURITY_CLIENT_CONTEXT;

#define PsDereferenceImpersonationToken(T)  \
            {if (ARGUMENT_PRESENT(T)) {     \
                (ObDereferenceObject((T))); \
            } else {                        \
                ;                           \
            }                               \
}

#define PsDereferencePrimaryToken(T) (ObDereferenceObject((T)))

extern "C"
NTKERNELAPI
VOID
PsRevertToSelf (
    VOID
);

extern "C"
NTKERNELAPI
NTSTATUS
SeCreateClientSecurity (
    IN PETHREAD                     Thread,
    IN PSECURITY_QUALITY_OF_SERVICE QualityOfService,
    IN BOOLEAN                      RemoteClient,
    OUT PSECURITY_CLIENT_CONTEXT    ClientContext
);

#define SeDeleteClientSecurity(C)  {                                           \
            if (SeTokenType((C)->ClientToken) == TokenPrimary) {               \
                PsDereferencePrimaryToken( (C)->ClientToken );                 \
            } else {                                                           \
                PsDereferenceImpersonationToken( (C)->ClientToken );           \
            }                                                                  \
}

extern "C"
NTKERNELAPI
VOID
SeImpersonateClient (
    IN PSECURITY_CLIENT_CONTEXT ClientContext,
    IN PETHREAD                 ServerThread OPTIONAL
);

extern "C"
NTKERNELAPI
TOKEN_TYPE
SeTokenType (
    IN PACCESS_TOKEN Token
);

//
// For backward compatibility with Windows NT 4.0 by Bruce Engle.
//
#ifndef MmGetSystemAddressForMdlSafe
#define MmGetSystemAddressForMdlSafe(MDL, PRIORITY) MmGetSystemAddressForMdlPrettySafe(MDL)

PVOID
MmGetSystemAddressForMdlPrettySafe (
    PMDL Mdl
    )
{
    CSHORT  MdlMappingCanFail;
    PVOID   MappedSystemVa;

    MdlMappingCanFail = Mdl->MdlFlags & MDL_MAPPING_CAN_FAIL;

    Mdl->MdlFlags |= MDL_MAPPING_CAN_FAIL;

    MappedSystemVa = MmGetSystemAddressForMdl(Mdl);

    if (MdlMappingCanFail == 0)
    {
        Mdl->MdlFlags &= ~MDL_MAPPING_CAN_FAIL;
    }

    return MappedSystemVa;
}
#endif

#include "filedisk_sys_exe_common.h"
//#ifndef _FILE_DISK_
//#define _FILE_DISK_
//
//#ifndef __T
//#ifdef _NTDDK_
//#define __T(x)  L ## x
//#else
//#define __T(x)  x
//#endif
//#endif
//
//#ifndef _T
//#define _T(x)   __T(x)
//#endif
//
//#define DEVICE_BASE_NAME    _T("\\FileDisk")
//#define DEVICE_DIR_NAME     _T("\\Device")      DEVICE_BASE_NAME
//#define DEVICE_NAME_PREFIX  DEVICE_DIR_NAME     DEVICE_BASE_NAME
//
//#define FILE_DEVICE_FILE_DISK       0x8000
//
//#define IOCTL_FILE_DISK_OPEN_FILE   CTL_CODE(FILE_DEVICE_FILE_DISK, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
//#define IOCTL_FILE_DISK_CLOSE_FILE  CTL_CODE(FILE_DEVICE_FILE_DISK, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
//#define IOCTL_FILE_DISK_QUERY_FILE  CTL_CODE(FILE_DEVICE_FILE_DISK, 0x802, METHOD_BUFFERED, FILE_READ_ACCESS)
//
//typedef struct _OPEN_FILE_INFORMATION {
//    LARGE_INTEGER   FileSize;
//    BOOLEAN         ReadOnly;
//    USHORT          FileNameLength;
//    UCHAR           FileName[1];
//} OPEN_FILE_INFORMATION, *POPEN_FILE_INFORMATION;
//
//#endif

#define PARAMETER_KEY           L"\\Parameters"

#define NUMBEROFDEVICES_VALUE   L"NumberOfDevices"

#define DEFAULT_NUMBEROFDEVICES 4

#define SECTOR_SIZE             512

#define TOC_DATA_TRACK          0x04

#define DATA_DEV_MAP_SIZE       32

#define DEVICE_NAME_LENGTH      64

HANDLE dir_handle;

typedef struct _DEVICE_EXTENSION {
    UNICODE_STRING              device_name;
    UNICODE_STRING              symbolic_link;

    BOOLEAN                     ctrldev_or_datadev; // TRUE - Control Device, FALSE - Data Device

    typedef struct _CTRL_DEV_EXT    // Control Device
    {
        LONG                  number_of_device; 

        LONG                  next_device_number; 

        typedef struct _DATA_DEV_MAP_ITEM
        {
            CHAR driver_letter;
            PDEVICE_OBJECT data_dev_obj;
        }DATA_DEV_MAP_ITEM;

        DATA_DEV_MAP_ITEM       dev_map[DATA_DEV_MAP_SIZE];

        KSPIN_LOCK              dev_map_lock;

    }CTRL_DEV_EXT;

    typedef struct _DATA_DEV_EXT    // Data Device
    {
        ULONG                      device_number;    
        BOOLEAN                     media_in_device;
        HANDLE                      file_handle;
        ANSI_STRING                 file_name;
        LARGE_INTEGER               file_size;
        BOOLEAN                     read_only;
    }DATA_DEV_EXT;

    typedef union _SPECIAL_DEV_EXT
    {
        CTRL_DEV_EXT              ctrl_dev_ext; 
        DATA_DEV_EXT              data_dev_ext; 
    }SPECIAL_DEV_EXT;

    SPECIAL_DEV_EXT             union_special_dev_ext;       
    LIST_ENTRY                  list_head;
    KSPIN_LOCK                  list_lock;
    KEVENT                      request_event;
    PVOID                       thread_pointer;
    BOOLEAN                     terminate_thread;
    PSECURITY_CLIENT_CONTEXT    security_client_context;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

extern "C"
NTSTATUS
DriverEntry (
    IN PDRIVER_OBJECT   DriverObject,
    IN PUNICODE_STRING  RegistryPath
);

NTSTATUS
FileDiskCreateDataDevice (
    IN PDEVICE_OBJECT   CtrlDevObject,
    IN DEVICE_TYPE      DeviceType,
    IN ULONG            Number,
    IN UCHAR            DriverLetter,
    OUT PDEVICE_OBJECT  * pDataDevObj
    );

NTSTATUS
FileDiskCreateControlDevice ( IN PDRIVER_OBJECT   DriverObject );

VOID
FileDiskUnload (
    IN PDRIVER_OBJECT   DriverObject
);

PDEVICE_OBJECT
FileDiskDeleteDevice (
    IN PDEVICE_OBJECT   DeviceObject
);

NTSTATUS
FileDiskCreateClose (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
);

NTSTATUS
FileDiskReadWrite (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
);

NTSTATUS
FileDiskDeviceControl (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
);

VOID
FileDiskThread (
    IN PVOID            Context
);

NTSTATUS
FileDiskOpenFile (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
);

NTSTATUS
FileDiskCloseFile (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
);

NTSTATUS
FileDiskMount (
    IN PDEVICE_OBJECT   CtrlDevObj,
    IN PIRP             Irp
);

NTSTATUS
FileDiskUmount (
    IN PDEVICE_OBJECT   CtrlDevObj,
    IN PIRP             Irp
);


NTSTATUS
FindDataDeviceByDriverLetter (
    IN  PDEVICE_OBJECT   CtrlDevObj, 
    IN  CHAR             DriverLetter, 
    OUT PDEVICE_OBJECT * pDataDevObj
);

// Driver letter must be within 'a'-'z' and 'A'-'Z'
// Delete - If the DataDevObj == NULL, it will remove the Item by setting DriverLetter and DataDevObj to 0 of the item found by DriverLetter
// Create - If the DataDevObj != NULL, find the first item which DriverLetter is 0 and set this item with DriverLetter and DataDevObj
NTSTATUS
SetDataDeviceMap (
    IN  PDEVICE_OBJECT   CtrlDevObj, 
    IN  CHAR             DriverLetter, 
    IN  PDEVICE_OBJECT   DataDevObj
);

extern "C" int swprintf(wchar_t *, const wchar_t *, ...);
