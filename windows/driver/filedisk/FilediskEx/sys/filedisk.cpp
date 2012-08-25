/*
    This is a virtual disk driver for Windows NT/2000/XP that uses
    one or more files to emulate physical disks.
    Copyright (C) 1999-2004 Bo Brant¨¦n.
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

#include <ntddk.h>
#include <ntdddisk.h>
#include <ntddcdrm.h>
#include <ntverp.h>

//
// We include some stuff from newer DDK:s here so that one
// version of the driver for all versions of Windows can
// be compiled with the Windows NT 4.0 DDK.
//


#include "filedisk.h"

#pragma INITCODE
NTSTATUS
DriverEntry (
    IN PDRIVER_OBJECT   DriverObject,
    IN PUNICODE_STRING  RegistryPath
    )
{
    KdPrint(("Driver Entry Enter\n"));
    NTSTATUS                    status;
    UNICODE_STRING              device_dir_name;
    OBJECT_ATTRIBUTES           object_attributes;

    RtlInitUnicodeString(&device_dir_name, DEVICE_DIR_NAME);

    InitializeObjectAttributes(
        &object_attributes,
        &device_dir_name,
        OBJ_PERMANENT,
        NULL,
        NULL
        );

    status = ZwCreateDirectoryObject(
        &dir_handle,
        DIRECTORY_ALL_ACCESS,
        &object_attributes
        );

    if (!NT_SUCCESS(status))
    {
        return status;
    }

    ZwMakeTemporaryObject(dir_handle);

    status = FileDiskCreateControlDevice(DriverObject);

    DriverObject->MajorFunction[IRP_MJ_CREATE]         = FileDiskCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]          = FileDiskCreateClose;
    DriverObject->MajorFunction[IRP_MJ_READ]           = FileDiskReadWrite;
    DriverObject->MajorFunction[IRP_MJ_WRITE]          = FileDiskReadWrite;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = FileDiskDeviceControl;

    DriverObject->DriverUnload = FileDiskUnload;

    KdPrint(("Driver Entry Leave\n"));
    return STATUS_SUCCESS;
}

NTSTATUS
FileDiskCreateControlDevice ( IN PDRIVER_OBJECT   DriverObject )
{
    UNICODE_STRING      device_name;
    NTSTATUS            status;
    PDEVICE_OBJECT      device_object;
    PDEVICE_EXTENSION   device_extension;
    HANDLE              thread_handle;

    ASSERT(DriverObject != NULL);

    RtlInitUnicodeString(&device_name, DEVICE_NAME_PREFIX);

    status = IoCreateDevice(
        DriverObject,
        sizeof(DEVICE_EXTENSION),
        &device_name,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &device_object
        );

    if (!NT_SUCCESS(status))
    {
        return status;
    }

    device_object->Flags |= DO_BUFFERED_IO;

    device_extension = (PDEVICE_EXTENSION) device_object->DeviceExtension;

    device_extension->ctrldev_or_datadev = TRUE;

    device_extension->device_name.Buffer = (PWSTR)ExAllocatePoolWithTag( PagedPool, DEVICE_NAME_LENGTH * sizeof(WCHAR), 'DFgT' );

    device_extension->device_name.MaximumLength = DEVICE_NAME_LENGTH * sizeof(WCHAR);

    RtlCopyUnicodeString(&(device_extension->device_name),&device_name);

    device_extension->union_special_dev_ext.ctrl_dev_ext.number_of_device = 0; // Should be update with regedit

    device_extension->union_special_dev_ext.ctrl_dev_ext.next_device_number = 0;

    InitializeListHead(&device_extension->list_head);

    KeInitializeSpinLock(&device_extension->list_lock);

    KeInitializeEvent(
        &device_extension->request_event,
        SynchronizationEvent,
        FALSE
        );

    device_extension->terminate_thread = FALSE;

    status = PsCreateSystemThread(
        &thread_handle,
        (ACCESS_MASK) 0L,
        NULL,
        NULL,
        NULL,
        FileDiskThread,
        device_object
        );

    if (!NT_SUCCESS(status))
    {
        IoDeleteDevice(device_object);
        return status;
    }

    status = ObReferenceObjectByHandle(
        thread_handle,
        THREAD_ALL_ACCESS,
        NULL,
        KernelMode,
        &device_extension->thread_pointer,
        NULL
        );

    if (!NT_SUCCESS(status))
    {
        ZwClose(thread_handle);

        device_extension->terminate_thread = TRUE;

        KeSetEvent(
            &device_extension->request_event,
            (KPRIORITY) 0,
            FALSE
            );

        IoDeleteDevice(device_object);

        return status;
    }

    ZwClose(thread_handle);

    UNICODE_STRING      ctrlDevLink;

    RtlInitUnicodeString(&ctrlDevLink,CTRLDEV_SYMLINK_CREATE);

    device_extension->symbolic_link.Buffer = (PWSTR)ExAllocatePoolWithTag( PagedPool, DEVICE_NAME_LENGTH * sizeof(WCHAR), 'DFgT' );

    device_extension->symbolic_link.MaximumLength = DEVICE_NAME_LENGTH * sizeof(WCHAR);

    RtlCopyUnicodeString(&(device_extension->symbolic_link),&ctrlDevLink);

	status = IoCreateSymbolicLink(&(device_extension->symbolic_link),&(device_extension->device_name));

    if (!NT_SUCCESS(status))
    {
        IoDeleteSymbolicLink(&(device_extension->symbolic_link));

        status = IoCreateSymbolicLink(&(device_extension->symbolic_link),&(device_extension->device_name));
		
		if( !NT_SUCCESS(status))
		{
            RtlFreeUnicodeString(&(device_extension->symbolic_link));

            ZwClose(thread_handle);

            device_extension->terminate_thread = TRUE;

            KeSetEvent(
                &device_extension->request_event,
                (KPRIORITY) 0,
                FALSE
                );

            IoDeleteDevice(device_object);

            return status;
		}
    }

    device_object->Flags &= ~DO_DEVICE_INITIALIZING;

    return STATUS_SUCCESS;
}

#pragma PAGEDCODE
VOID
FileDiskUnload (
    IN PDRIVER_OBJECT DriverObject
    )
{
    KdPrint(("FileDiskUnload Enter\n"));

    PDEVICE_OBJECT device_object;

    PAGED_CODE();

    device_object = DriverObject->DeviceObject;

    PDEVICE_EXTENSION device_extension = NULL;

    while (device_object)
    {
        device_extension = (PDEVICE_EXTENSION) device_object->DeviceExtension;

        if ( FALSE == device_extension->ctrldev_or_datadev )
        {
            ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
        
            ZwClose(device_extension->union_special_dev_ext.data_dev_ext.file_handle);
        
            device_extension->union_special_dev_ext.data_dev_ext.media_in_device = FALSE;
        }

        device_object = FileDiskDeleteDevice(device_object);
    }

    ZwClose(dir_handle);

    KdPrint(("FileDiskUnload Leave\n"));
}

PDEVICE_OBJECT
FileDiskDeleteDevice (
    IN PDEVICE_OBJECT DeviceObject
    )
{
    PDEVICE_EXTENSION   device_extension;
    PDEVICE_OBJECT      next_device_object;

    PAGED_CODE();

    ASSERT(DeviceObject != NULL);

    device_extension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    device_extension->terminate_thread = TRUE;

    KeSetEvent(
        &device_extension->request_event,
        (KPRIORITY) 0,
        FALSE
        );

    KeWaitForSingleObject(
        device_extension->thread_pointer,
        Executive,
        KernelMode,
        FALSE,
        NULL
        );

    ObDereferenceObject(device_extension->thread_pointer);

    if (device_extension->security_client_context != NULL)
    {
        SeDeleteClientSecurity(device_extension->security_client_context);
        ExFreePool(device_extension->security_client_context);
    }

    next_device_object = DeviceObject->NextDevice;

    IoDeleteSymbolicLink(&(device_extension->symbolic_link));

    RtlFreeUnicodeString(&(device_extension->symbolic_link));

    RtlFreeUnicodeString(&(device_extension->device_name));

    IoDeleteDevice(DeviceObject);

    return next_device_object;
}

NTSTATUS
FileDiskCreateClose (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
    )
{
    PAGED_CODE();

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = FILE_OPENED;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return STATUS_SUCCESS;
}

#pragma LOCKEDCODE
NTSTATUS
FileDiskReadWrite (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
    )
{
    PDEVICE_EXTENSION   device_extension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    if(device_extension->ctrldev_or_datadev) 
    {

        Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;

        Irp->IoStatus.Information = 0;

        IoCompleteRequest(Irp, IO_NO_INCREMENT);

        return STATUS_SUCCESS;
    }
    else
    {
        PIO_STACK_LOCATION  io_stack;

        KIRQL Irql;

        if (!device_extension->union_special_dev_ext.data_dev_ext.media_in_device)
        {
            Irp->IoStatus.Status = STATUS_NO_MEDIA_IN_DEVICE;

            Irp->IoStatus.Information = 0;
    
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
    
            return STATUS_NO_MEDIA_IN_DEVICE;
        }
    
        io_stack = IoGetCurrentIrpStackLocation(Irp);
    
        if (io_stack->Parameters.Read.Length == 0)
        {
            Irp->IoStatus.Status = STATUS_SUCCESS;

            Irp->IoStatus.Information = 0;
    
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
    
            return STATUS_SUCCESS;
        }
    
        if(Irp->Flags & IRP_BUFFERED_IO)
        {       
            ASSERT((NULL == Irp->MdlAddress));
    
            //KdPrint(("Have Buffered IO\n"));
    
            if( IRP_MJ_READ == io_stack->MajorFunction )
            {
                Irp->IoStatus.Status = ZwReadFile(device_extension->union_special_dev_ext.data_dev_ext.file_handle,
                                                  NULL,NULL,NULL,
                                                  &(Irp->IoStatus),
                                                  Irp->AssociatedIrp.SystemBuffer,
                                                  io_stack->Parameters.Read.Length,
                                                  &(io_stack->Parameters.Read.ByteOffset),
                                                  NULL);
            }
            else
            {
                ASSERT((IRP_MJ_WRITE == io_stack->MajorFunction));
                Irp->IoStatus.Status = ZwWriteFile(device_extension->union_special_dev_ext.data_dev_ext.file_handle,
                                                   NULL,NULL,NULL,
                                                   &(Irp->IoStatus),
                                                   Irp->AssociatedIrp.SystemBuffer,
                                                   io_stack->Parameters.Write.Length,
                                                   &(io_stack->Parameters.Write.ByteOffset),
                                                   NULL);
            }
    
            if(!NT_SUCCESS(Irp->IoStatus.Status))
            {
                IRP_MJ_READ == io_stack->MajorFunction
                    ? DbgPrint("!!!!!! ERROR IRP_MJ_READ\n")
                    : DbgPrint("!!!!!! ERROR IRP_MJ_WRITE\n");
    
                Irql = KeGetCurrentIrql();
    
                DbgPrint("IRQL = 0x%.8x, FILE HANDLE = 0x%.8x\n",Irql, device_extension->union_special_dev_ext.data_dev_ext.file_handle);
    
                DbgPrint("Length = %x, ByteOffset = %x\n", 
                         IRP_MJ_READ == io_stack->MajorFunction 
                         ? io_stack->Parameters.Read.Length 
                         : io_stack->Parameters.Write.Length ,
                         IRP_MJ_READ == io_stack->MajorFunction 
                         ? io_stack->Parameters.Read.ByteOffset.QuadPart
                         : io_stack->Parameters.Write.ByteOffset.QuadPart);
    
                DbgPrint("ERROR Irp->IoStatus.Status = %x, Irp->IoStatus.Information = %x\n",Irp->IoStatus.Status,Irp->IoStatus.Information);
    
            }
    
            IoCompleteRequest(
                Irp,
                (CCHAR) (NT_SUCCESS(Irp->IoStatus.Status) ?
                         IO_DISK_INCREMENT : IO_NO_INCREMENT)
                        );
    
            return Irp->IoStatus.Status;
        }
        else
        {
            //ASSERT((Irp->Flags & IRP_NOCACHE));
    
            //KdPrint(("Have Direct IO\n"));
    
            IoMarkIrpPending(Irp);
    
            ExInterlockedInsertTailList(
                &device_extension->list_head,
                &Irp->Tail.Overlay.ListEntry,
                &device_extension->list_lock
                );
    
            KeSetEvent(
                &device_extension->request_event,
                (KPRIORITY) 0,
                FALSE
                );
    
            return STATUS_PENDING;
        }
    }
}


NTSTATUS
FileDiskDeviceControl (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
    )
{
    PDEVICE_EXTENSION   device_extension;
    PIO_STACK_LOCATION  io_stack;
    NTSTATUS            status;

    device_extension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    io_stack = IoGetCurrentIrpStackLocation(Irp);

    if(device_extension->ctrldev_or_datadev) 
    {
        switch (io_stack->Parameters.DeviceIoControl.IoControlCode)
        {
            case IOCTL_FILE_DISK_OPEN_FILE:
            {
                SECURITY_QUALITY_OF_SERVICE security_quality_of_service;
                /*    
                if (device_extension->media_in_device)
                {
                    KdPrint(("FileDisk: IOCTL_FILE_DISK_OPEN_FILE: Media already opened\n"));
        
                    status = STATUS_INVALID_DEVICE_REQUEST;
                    Irp->IoStatus.Information = 0;
                    break;
                }
                     */    
                if (io_stack->Parameters.DeviceIoControl.InputBufferLength < sizeof(MOUNT_INFORMATION))
                {
                    status = STATUS_INVALID_PARAMETER;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                if (io_stack->Parameters.DeviceIoControl.InputBufferLength <
                    sizeof(MOUNT_INFORMATION) +
                    ((PMOUNT_INFORMATION)Irp->AssociatedIrp.SystemBuffer)->FileNameLength -
                    sizeof(UCHAR))
                {
                    status = STATUS_INVALID_PARAMETER;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                if (device_extension->security_client_context != NULL)
                {
                    SeDeleteClientSecurity(device_extension->security_client_context);
                }
                else
                {
                    device_extension->security_client_context =
                        (PSECURITY_CLIENT_CONTEXT) ExAllocatePoolWithTag(NonPagedPool, sizeof(SECURITY_CLIENT_CONTEXT), 'DFgT');
                }
        
                RtlZeroMemory(&security_quality_of_service, sizeof(SECURITY_QUALITY_OF_SERVICE));
        
                security_quality_of_service.Length = sizeof(SECURITY_QUALITY_OF_SERVICE);
                security_quality_of_service.ImpersonationLevel = SecurityImpersonation;
                security_quality_of_service.ContextTrackingMode = SECURITY_STATIC_TRACKING;
                security_quality_of_service.EffectiveOnly = FALSE;
        
                SeCreateClientSecurity(
                    PsGetCurrentThread(),
                    &security_quality_of_service,
                    FALSE,
                    device_extension->security_client_context
                    );
        
                IoMarkIrpPending(Irp);
        
                ExInterlockedInsertTailList(
                    &device_extension->list_head,
                    &Irp->Tail.Overlay.ListEntry,
                    &device_extension->list_lock
                    );
        
                KeSetEvent(
                    &device_extension->request_event,
                    (KPRIORITY) 0,
                    FALSE
                    );
        
                status = STATUS_PENDING;
                break;
            }

            case IOCTL_FILE_DISK_CLOSE_FILE:
            {
                IoMarkIrpPending(Irp);
        
                ExInterlockedInsertTailList(
                    &device_extension->list_head,
                    &Irp->Tail.Overlay.ListEntry,
                    &device_extension->list_lock
                    );
        
                KeSetEvent(
                    &device_extension->request_event,
                    (KPRIORITY) 0,
                    FALSE
                    );
        
                status = STATUS_PENDING;
     
                break;
            }
    
            default:
            {
                KdPrint(("FileDisk CTRL: IoControlCode %#x\n",
                         io_stack->Parameters.DeviceIoControl.IoControlCode));
        
                status = STATUS_INVALID_DEVICE_REQUEST;
        
                Irp->IoStatus.Information = 0;
        
            }
        }
    }
    else
    {
        if (!device_extension->union_special_dev_ext.data_dev_ext.media_in_device &&
            io_stack->Parameters.DeviceIoControl.IoControlCode != IOCTL_FILE_DISK_OPEN_FILE)
        {
            Irp->IoStatus.Status = STATUS_NO_MEDIA_IN_DEVICE;

            Irp->IoStatus.Information = 0;
    
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
    
            return STATUS_NO_MEDIA_IN_DEVICE;
        }

        switch (io_stack->Parameters.DeviceIoControl.IoControlCode)
        {
        
        case IOCTL_FILE_DISK_QUERY_FILE:
            {
                PMOUNT_INFORMATION mount_information;
        
                if (io_stack->Parameters.DeviceIoControl.OutputBufferLength <
                    sizeof(MOUNT_INFORMATION) + device_extension->union_special_dev_ext.data_dev_ext.file_name.Length - sizeof(UCHAR))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                    Irp->IoStatus.Information = 0;
                    break;
                }
        
                mount_information = (PMOUNT_INFORMATION) Irp->AssociatedIrp.SystemBuffer;
        
                mount_information->FileSize.QuadPart = device_extension->union_special_dev_ext.data_dev_ext.file_size.QuadPart;
                mount_information->ReadOnly = device_extension->union_special_dev_ext.data_dev_ext.read_only;
                mount_information->FileNameLength = device_extension->union_special_dev_ext.data_dev_ext.file_name.Length;
        
                RtlCopyMemory(
                    mount_information->FileName,
                    device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer,
                    device_extension->union_special_dev_ext.data_dev_ext.file_name.Length
                    );
        
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = sizeof(MOUNT_INFORMATION) +
                mount_information->FileNameLength - sizeof(UCHAR);
    
                break;
            }

        case IOCTL_DISK_CHECK_VERIFY:
        case IOCTL_CDROM_CHECK_VERIFY:
        case IOCTL_STORAGE_CHECK_VERIFY:
        case IOCTL_STORAGE_CHECK_VERIFY2:
            {
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = 0;
                break;
            }
    
        case IOCTL_DISK_GET_DRIVE_GEOMETRY:
        case IOCTL_CDROM_GET_DRIVE_GEOMETRY:
            {
                PDISK_GEOMETRY  disk_geometry;
                ULONGLONG       length;
    
                if (io_stack->Parameters.DeviceIoControl.OutputBufferLength <
                    sizeof(DISK_GEOMETRY))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                disk_geometry = (PDISK_GEOMETRY) Irp->AssociatedIrp.SystemBuffer;
    
                length = device_extension->union_special_dev_ext.data_dev_ext.file_size.QuadPart;
    
                disk_geometry->Cylinders.QuadPart = length / SECTOR_SIZE / 32 / 2;
                disk_geometry->MediaType = FixedMedia;
                disk_geometry->TracksPerCylinder = 2;
                disk_geometry->SectorsPerTrack = 32;
                disk_geometry->BytesPerSector = SECTOR_SIZE;
    
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = sizeof(DISK_GEOMETRY);
    
                break;
            }
    
        case IOCTL_DISK_GET_LENGTH_INFO:
            {
                PGET_LENGTH_INFORMATION get_length_information;
    
                if (io_stack->Parameters.DeviceIoControl.OutputBufferLength <
                    sizeof(GET_LENGTH_INFORMATION))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                get_length_information = (PGET_LENGTH_INFORMATION) Irp->AssociatedIrp.SystemBuffer;
    
                get_length_information->Length.QuadPart = device_extension->union_special_dev_ext.data_dev_ext.file_size.QuadPart;
    
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = sizeof(GET_LENGTH_INFORMATION);
    
            break;
            }
    
        case IOCTL_DISK_GET_PARTITION_INFO:
            {
                PPARTITION_INFORMATION  partition_information;
                ULONGLONG               length;
    
                if (io_stack->Parameters.DeviceIoControl.OutputBufferLength <
                    sizeof(PARTITION_INFORMATION))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                partition_information = (PPARTITION_INFORMATION) Irp->AssociatedIrp.SystemBuffer;
    
                length = device_extension->union_special_dev_ext.data_dev_ext.file_size.QuadPart;
    
                partition_information->StartingOffset.QuadPart = 0;
                partition_information->PartitionLength.QuadPart = length;
                partition_information->HiddenSectors = 1;
                partition_information->PartitionNumber = 0;
                partition_information->PartitionType = 0;
                partition_information->BootIndicator = FALSE;
                partition_information->RecognizedPartition = FALSE;
                partition_information->RewritePartition = FALSE;
    
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = sizeof(PARTITION_INFORMATION);
    
                break;
            }
    
        case IOCTL_DISK_GET_PARTITION_INFO_EX:
            {
                PPARTITION_INFORMATION_EX   partition_information_ex;
                ULONGLONG                   length;
    
                if (io_stack->Parameters.DeviceIoControl.OutputBufferLength <
                    sizeof(PARTITION_INFORMATION_EX))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                partition_information_ex = (PPARTITION_INFORMATION_EX) Irp->AssociatedIrp.SystemBuffer;
    
                length = device_extension->union_special_dev_ext.data_dev_ext.file_size.QuadPart;
    
                partition_information_ex->PartitionStyle = PARTITION_STYLE_MBR;
                partition_information_ex->StartingOffset.QuadPart = 0;
                partition_information_ex->PartitionLength.QuadPart = length;
                partition_information_ex->PartitionNumber = 0;
                partition_information_ex->RewritePartition = FALSE;
                partition_information_ex->Mbr.PartitionType = 0;
                partition_information_ex->Mbr.BootIndicator = FALSE;
                partition_information_ex->Mbr.RecognizedPartition = FALSE;
                partition_information_ex->Mbr.HiddenSectors = 1;
    
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = sizeof(PARTITION_INFORMATION_EX);
    
                break;
            }
    
        case IOCTL_DISK_IS_WRITABLE:
            {
                if (!device_extension->union_special_dev_ext.data_dev_ext.read_only)
                {
                    status = STATUS_SUCCESS;
                }
                else
                {
                    status = STATUS_MEDIA_WRITE_PROTECTED;
                }
                Irp->IoStatus.Information = 0;
                break;
            }
    
        case IOCTL_DISK_MEDIA_REMOVAL:
        case IOCTL_STORAGE_MEDIA_REMOVAL:
            {
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = 0;
                break;
            }
    
        case IOCTL_CDROM_READ_TOC:
            {
                PCDROM_TOC cdrom_toc;
    
                if (io_stack->Parameters.DeviceIoControl.OutputBufferLength <
                    sizeof(CDROM_TOC))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                cdrom_toc = (PCDROM_TOC) Irp->AssociatedIrp.SystemBuffer;
    
                RtlZeroMemory(cdrom_toc, sizeof(CDROM_TOC));
    
                cdrom_toc->FirstTrack = 1;
                cdrom_toc->LastTrack = 1;
                cdrom_toc->TrackData[0].Control = TOC_DATA_TRACK;
    
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = sizeof(CDROM_TOC);
    
                break;
            }
    
        case IOCTL_DISK_SET_PARTITION_INFO:
            {
                if (device_extension->union_special_dev_ext.data_dev_ext.read_only)
                {
                    status = STATUS_MEDIA_WRITE_PROTECTED;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                if (io_stack->Parameters.DeviceIoControl.InputBufferLength <
                    sizeof(SET_PARTITION_INFORMATION))
                {
                    status = STATUS_INVALID_PARAMETER;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = 0;
    
                break;
            }
    
        case IOCTL_DISK_VERIFY:
            {
                PVERIFY_INFORMATION verify_information;
    
                if (io_stack->Parameters.DeviceIoControl.InputBufferLength <
                    sizeof(VERIFY_INFORMATION))
                {
                    status = STATUS_INVALID_PARAMETER;
                    Irp->IoStatus.Information = 0;
                    break;
                }
    
                verify_information = (PVERIFY_INFORMATION) Irp->AssociatedIrp.SystemBuffer;
    
                status = STATUS_SUCCESS;
                Irp->IoStatus.Information = verify_information->Length;
    
                break;
            }
    
        case IOCTL_STORAGE_GET_DEVICE_NUMBER :
            {
                KdPrint(("!!! Not handled device request: IOCTL_STORAGE_GET_DEVICE_NUMBER \n"));
                status = STATUS_INVALID_DEVICE_REQUEST;
                Irp->IoStatus.Information = 0;
            }
            break;
    
        case IOCTL_MOUNTDEV_QUERY_DEVICE_NAME:
            {
                KdPrint(("!!! Not handled device request: IOCTL_MOUNTDEV_QUERY_DEVICE_NAME\n"));
                status = STATUS_INVALID_DEVICE_REQUEST;
                Irp->IoStatus.Information = 0;
            }
            break;
    
        case FT_BALANCED_READ_MODE:
            {
                KdPrint(("!!! Not handled device request: FT_BALANCED_READ_MODE\n"));
                status = STATUS_INVALID_DEVICE_REQUEST;
                Irp->IoStatus.Information = 0;
            }
            break;
    
        default:
            {
                KdPrint((
                    "! WARNING FileDisk Data: Not handled device request: IoControlCode %#x\n",
                    io_stack->Parameters.DeviceIoControl.IoControlCode
                    ));
    
                status = STATUS_INVALID_DEVICE_REQUEST;
                Irp->IoStatus.Information = 0;
            }
        }
    }

    if (status != STATUS_PENDING)
    {
        Irp->IoStatus.Status = status;

        IoCompleteRequest(Irp, IO_NO_INCREMENT);
    }

    return status;
}

VOID
FileDiskThread (
    IN PVOID Context
    )
{
    PDEVICE_OBJECT      device_object;
    PDEVICE_EXTENSION   device_extension;
    PLIST_ENTRY         request;
    PIRP                irp;
    PIO_STACK_LOCATION  io_stack;
    PUCHAR              system_buffer;
    PUCHAR              buffer;

	PUCHAR              tempbuffer;
	ULONG i;
    
    NTSTATUS status = STATUS_SUCCESS;
    ULONG mdl_length = 0;
    ULONG parameter_length = 0;
	PVOID mdl_address = NULL;
	ULONG mdl_offset = 0;	
    PVOID kernel_address = NULL;

    LARGE_INTEGER parameter_offset;

    ASSERT(Context != NULL);

    device_object = (PDEVICE_OBJECT) Context;

    device_extension = (PDEVICE_EXTENSION) device_object->DeviceExtension;

    KeSetPriorityThread(KeGetCurrentThread(), LOW_REALTIME_PRIORITY);

    for (;;)
    {
        KeWaitForSingleObject(
            &device_extension->request_event,
            Executive,
            KernelMode,
            FALSE,
            NULL
            );

        if (device_extension->terminate_thread)
        {
            PsTerminateSystemThread(STATUS_SUCCESS);
        }

        while (request = ExInterlockedRemoveHeadList(
            &device_extension->list_head,
            &device_extension->list_lock
            ))
        {
            irp = CONTAINING_RECORD(request, IRP, Tail.Overlay.ListEntry);

            io_stack = IoGetCurrentIrpStackLocation(irp);

            switch (io_stack->MajorFunction)
            {


            case IRP_MJ_DEVICE_CONTROL:
                switch (io_stack->Parameters.DeviceIoControl.IoControlCode)
                {
                case IOCTL_FILE_DISK_OPEN_FILE:

                    SeImpersonateClient(device_extension->security_client_context, NULL);

                    irp->IoStatus.Status = FileDiskMount(device_object, irp);

                    PsRevertToSelf();

                    break;

                case IOCTL_FILE_DISK_CLOSE_FILE:
                    irp->IoStatus.Status = FileDiskUmount(device_object, irp);
                    break;

                default:
                    irp->IoStatus.Status = STATUS_DRIVER_INTERNAL_ERROR;
                }
                break;

            case IRP_MJ_READ: 
            case IRP_MJ_WRITE:
                {
                    ASSERT(!(irp->Flags & IRP_BUFFERED_IO));
                    ASSERT( NULL != irp->MdlAddress );

                    mdl_length = MmGetMdlByteCount(irp->MdlAddress);

                    mdl_address = MmGetMdlVirtualAddress(irp->MdlAddress);

                    mdl_offset = MmGetMdlByteOffset(irp->MdlAddress);

                    parameter_length = IRP_MJ_READ == io_stack->MajorFunction ? io_stack->Parameters.Read.Length : io_stack->Parameters.Write.Length;

                    parameter_offset = IRP_MJ_READ == io_stack->MajorFunction ? io_stack->Parameters.Read.ByteOffset : io_stack->Parameters.Write.ByteOffset;

                    if (mdl_length != parameter_length)
                    {
                        if( mdl_length < parameter_length ) 
                        {
                            DbgPrint("!!!!!! ERROR mdl_length(0x%.8x)< parameter_length(0x%.8x) \n",mdl_length,parameter_length);
                            irp->IoStatus.Information = 0;
                            irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
                            break;
                        }
                        KdPrint(("! WARNING mdl_length(0x%.8x)!= parameter_length(0x%.8x) \n",mdl_length,parameter_length));
                    }

                    kernel_address = MmGetSystemAddressForMdlSafe(irp->MdlAddress,NormalPagePriority);

                    irp->IoStatus.Status = IRP_MJ_READ == io_stack->MajorFunction 
                        ? ZwReadFile(device_extension->union_special_dev_ext.data_dev_ext.file_handle,NULL,NULL,NULL,&(irp->IoStatus),kernel_address,parameter_length,&parameter_offset,NULL)
                        : ZwWriteFile(device_extension->union_special_dev_ext.data_dev_ext.file_handle,NULL,NULL,NULL,&(irp->IoStatus),kernel_address,parameter_length,&parameter_offset,NULL) ;

                    if(!NT_SUCCESS(irp->IoStatus.Status))
                    {
                        DbgPrint("!!!!!! ERROR irp->IoStatus.Status = %x, irp->IoStatus.Information = %x\n",irp->IoStatus.Status,irp->IoStatus.Information);
                    }
                }
                break;

            default:
                irp->IoStatus.Status = STATUS_DRIVER_INTERNAL_ERROR;
            }

            IoCompleteRequest(
                irp,
                (CCHAR) (NT_SUCCESS(irp->IoStatus.Status) ?
                IO_DISK_INCREMENT : IO_NO_INCREMENT)
                );
        }
    }
}

#pragma PAGEDCODE
NTSTATUS
FileDiskOpenFile (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
    )
{
    PDEVICE_EXTENSION               device_extension;
    PMOUNT_INFORMATION              mount_information;
    UNICODE_STRING                  ufile_name;
    NTSTATUS                        status;
    OBJECT_ATTRIBUTES               object_attributes;
    FILE_END_OF_FILE_INFORMATION    file_eof;
    FILE_BASIC_INFORMATION          file_basic;
    FILE_STANDARD_INFORMATION       file_standard;
    FILE_ALIGNMENT_INFORMATION      file_alignment;

    PAGED_CODE();

    ASSERT(DeviceObject != NULL);
    ASSERT(Irp != NULL);

    device_extension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    PDEVICE_OBJECT dataDev = NULL;

    mount_information = (PMOUNT_INFORMATION) Irp->AssociatedIrp.SystemBuffer;

    if (DeviceObject->DeviceType != FILE_DEVICE_CD_ROM)
    {
        device_extension->union_special_dev_ext.data_dev_ext.read_only = mount_information->ReadOnly;
    }

    device_extension->union_special_dev_ext.data_dev_ext.file_name.Length = mount_information->FileNameLength;
    device_extension->union_special_dev_ext.data_dev_ext.file_name.MaximumLength = mount_information->FileNameLength;
    device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer = (PCHAR)ExAllocatePoolWithTag(NonPagedPool, mount_information->FileNameLength, 'DFgT' );

    RtlCopyMemory(
        device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer,
        mount_information->FileName,
        mount_information->FileNameLength
        );

    status = RtlAnsiStringToUnicodeString(
        &ufile_name,
        &device_extension->union_special_dev_ext.data_dev_ext.file_name,
        TRUE
        );

    if (!NT_SUCCESS(status))
    {
        ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
        Irp->IoStatus.Status = status;
        Irp->IoStatus.Information = 0;
        return status;
    }

    InitializeObjectAttributes(
        &object_attributes,
        &ufile_name,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, // OBJ_KERNEL_HANDLE - This flag is used for Buffered IO which will be handled within "Read" Dispatch function 
        NULL,
        NULL
        );

    status = ZwCreateFile(
        &device_extension->union_special_dev_ext.data_dev_ext.file_handle,
        device_extension->union_special_dev_ext.data_dev_ext.read_only ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE,
        &object_attributes,
        &Irp->IoStatus,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        device_extension->union_special_dev_ext.data_dev_ext.read_only ? FILE_SHARE_READ : 0,
        FILE_OPEN,
        FILE_NON_DIRECTORY_FILE |
        FILE_RANDOM_ACCESS |
        FILE_NO_INTERMEDIATE_BUFFERING |
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
        );

    if (status == STATUS_OBJECT_NAME_NOT_FOUND || status == STATUS_NO_SUCH_FILE)
    {
        if (device_extension->union_special_dev_ext.data_dev_ext.read_only || mount_information->FileSize.QuadPart == 0)
        {
            ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
            RtlFreeUnicodeString(&ufile_name);

            Irp->IoStatus.Status = STATUS_NO_SUCH_FILE;
            Irp->IoStatus.Information = 0;

            return STATUS_NO_SUCH_FILE;
        }
        else
        {
            status = ZwCreateFile(
                &device_extension->union_special_dev_ext.data_dev_ext.file_handle,
                GENERIC_READ | GENERIC_WRITE,
                &object_attributes,
                &Irp->IoStatus,
                &mount_information->FileSize,
                FILE_ATTRIBUTE_NORMAL,
                0,
                FILE_OPEN_IF,
                FILE_NON_DIRECTORY_FILE |
                FILE_RANDOM_ACCESS |
                FILE_NO_INTERMEDIATE_BUFFERING |
                FILE_SYNCHRONOUS_IO_NONALERT,
                NULL,
                0
                );

            if (!NT_SUCCESS(status))
            {
                ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
                RtlFreeUnicodeString(&ufile_name);
                return status;
            }

            if (Irp->IoStatus.Information == FILE_CREATED)
            {
                file_eof.EndOfFile.QuadPart = mount_information->FileSize.QuadPart;

                status = ZwSetInformationFile(
                    device_extension->union_special_dev_ext.data_dev_ext.file_handle,
                    &Irp->IoStatus,
                    &file_eof,
                    sizeof(FILE_END_OF_FILE_INFORMATION),
                    FileEndOfFileInformation
                    );

                if (!NT_SUCCESS(status))
                {
                    ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
                    RtlFreeUnicodeString(&ufile_name);
                    ZwClose(device_extension->union_special_dev_ext.data_dev_ext.file_handle);
                    return status;
                }
            }
        }
    }
    else if (!NT_SUCCESS(status))
    {
        ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
        RtlFreeUnicodeString(&ufile_name);
        return status;
    }

    RtlFreeUnicodeString(&ufile_name);

    status = ZwQueryInformationFile(
        device_extension->union_special_dev_ext.data_dev_ext.file_handle,
        &Irp->IoStatus,
        &file_basic,
        sizeof(FILE_BASIC_INFORMATION),
        FileBasicInformation
        );

    if (!NT_SUCCESS(status))
    {
        ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
        ZwClose(device_extension->union_special_dev_ext.data_dev_ext.file_handle);
        return status;
    }

/*
    //
    // The NT cache manager can deadlock if a filesystem that is using the cache
    // manager is used in a virtual disk that stores its file on a filesystem
    // that is also using the cache manager, this is why we open the file with
    // FILE_NO_INTERMEDIATE_BUFFERING above, however if the file is compressed
    // or encrypted NT will not honor this request and cache it anyway since it
    // need to store the decompressed/unencrypted data somewhere, therefor we put
    // an extra check here and don't alow disk images to be compressed/encrypted.
    //
    if (file_basic.FileAttributes & (FILE_ATTRIBUTE_COMPRESSED | FILE_ATTRIBUTE_ENCRYPTED))
    {
        ExFreePool(device_extension->file_name.Buffer);
        ZwClose(device_extension->file_handle);
        Irp->IoStatus.Status = STATUS_ACCESS_DENIED;
        Irp->IoStatus.Information = 0;
        return STATUS_ACCESS_DENIED;
    }
*/

    status = ZwQueryInformationFile(
        device_extension->union_special_dev_ext.data_dev_ext.file_handle,
        &Irp->IoStatus,
        &file_standard,
        sizeof(FILE_STANDARD_INFORMATION),
        FileStandardInformation
        );

    if (!NT_SUCCESS(status))
    {
        ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
        ZwClose(device_extension->union_special_dev_ext.data_dev_ext.file_handle);
        return status;
    }

    device_extension->union_special_dev_ext.data_dev_ext.file_size.QuadPart = file_standard.EndOfFile.QuadPart;

    status = ZwQueryInformationFile(
        device_extension->union_special_dev_ext.data_dev_ext.file_handle,
        &Irp->IoStatus,
        &file_alignment,
        sizeof(FILE_ALIGNMENT_INFORMATION),
        FileAlignmentInformation
        );

    if (!NT_SUCCESS(status))
    {
        ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);
        ZwClose(device_extension->union_special_dev_ext.data_dev_ext.file_handle);
        return status;
    }

    DeviceObject->AlignmentRequirement = file_alignment.AlignmentRequirement;

    if (device_extension->union_special_dev_ext.data_dev_ext.read_only)
    {
        DeviceObject->Characteristics |= FILE_READ_ONLY_DEVICE;
    }
    else
    {
        DeviceObject->Characteristics &= ~FILE_READ_ONLY_DEVICE;
    }

    device_extension->union_special_dev_ext.data_dev_ext.media_in_device = TRUE;

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

	KdPrint(("DeviceObject->AlignmentRequirement : %lu\n",DeviceObject->AlignmentRequirement));

    return STATUS_SUCCESS;
}

NTSTATUS
FileDiskCloseFile (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
    )
{
    PDEVICE_EXTENSION device_extension;

    PAGED_CODE();

    ASSERT(DeviceObject != NULL);
    ASSERT(Irp != NULL);

    device_extension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;

    ExFreePool(device_extension->union_special_dev_ext.data_dev_ext.file_name.Buffer);

    ZwClose(device_extension->union_special_dev_ext.data_dev_ext.file_handle);

    device_extension->union_special_dev_ext.data_dev_ext.media_in_device = FALSE;

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    return STATUS_SUCCESS;
}

NTSTATUS
FileDiskMount (
    IN PDEVICE_OBJECT   CtrlDevObj,
    IN PIRP             Irp
)
{
    ASSERT(CtrlDevObj != NULL);
    ASSERT(Irp != NULL);
    PAGED_CODE();

    PDEVICE_EXTENSION CtrlDevObjExt = (PDEVICE_EXTENSION) CtrlDevObj->DeviceExtension;

    PMOUNT_INFORMATION mount_information = (PMOUNT_INFORMATION) Irp->AssociatedIrp.SystemBuffer;

    PDEVICE_OBJECT DataDevObj = NULL;

    ULONG uDevNumber = InterlockedIncrement(&CtrlDevObjExt->union_special_dev_ext.ctrl_dev_ext.next_device_number);

    NTSTATUS status = mount_information->CdImage ? 
        FileDiskCreateDataDevice(CtrlDevObj, FILE_DEVICE_CD_ROM, uDevNumber-1, mount_information->DriveLetter, &DataDevObj): 
        FileDiskCreateDataDevice(CtrlDevObj, FILE_DEVICE_DISK, uDevNumber-1, mount_information->DriveLetter, &DataDevObj);
    
    if (!NT_SUCCESS(status))
    {
        if (DataDevObj)
        {
            FileDiskDeleteDevice(DataDevObj);            
        }
        Irp->IoStatus.Status = status;
        Irp->IoStatus.Information = 0;
        return status;
    }

    ASSERT(DataDevObj);

    InterlockedIncrement(&CtrlDevObjExt->union_special_dev_ext.ctrl_dev_ext.number_of_device);

    PDEVICE_EXTENSION DataDevObjExt = (PDEVICE_EXTENSION) DataDevObj->DeviceExtension;

    status = FileDiskOpenFile(DataDevObj,Irp);

    if (!NT_SUCCESS(status))
    {
        InterlockedDecrement(&CtrlDevObjExt->union_special_dev_ext.ctrl_dev_ext.number_of_device);

        if (DataDevObj)
        {
            FileDiskDeleteDevice(DataDevObj);            
        }

        Irp->IoStatus.Status = status;
        Irp->IoStatus.Information = 0;

        return status;
    }

//Output
    KdPrint(("str=%wZ len=%u\n",&(DataDevObjExt->device_name),DataDevObjExt->device_name.Length));

    RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,DataDevObjExt->device_name.Buffer,DataDevObjExt->device_name.Length);

    Irp->IoStatus.Status = STATUS_SUCCESS;

    Irp->IoStatus.Information = DataDevObjExt->device_name.Length;

    return STATUS_SUCCESS;
}

NTSTATUS
FileDiskUmount (
    IN PDEVICE_OBJECT   CtrlDevObj,
    IN PIRP             Irp
)
{
    ASSERT(CtrlDevObj);

    CHAR * pDriverLetter = (PCHAR)(Irp->AssociatedIrp.SystemBuffer);

    if(NULL == pDriverLetter) 
    {
        DbgPrint("!!!!!! Error: Invalid Driver Letter Input!\n");

        Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;

        Irp->IoStatus.Information = 0;

        return STATUS_UNSUCCESSFUL;
    }

    PDEVICE_OBJECT DataDevObj = NULL;

    NTSTATUS status =  FindDataDeviceByDriverLetter(CtrlDevObj, *pDriverLetter,&DataDevObj);

    if(!NT_SUCCESS(status)) 
    {
        Irp->IoStatus.Status = STATUS_NO_SUCH_DEVICE;

        Irp->IoStatus.Information = 0;

        return STATUS_NO_SUCH_DEVICE;
    }

    ASSERT(DataDevObj);

    status =  FileDiskCloseFile(DataDevObj,Irp);

    if(!NT_SUCCESS(status)) 
    {
        DbgPrint("!!!!!! Error: Close File Failed!\n");

        Irp->IoStatus.Status = STATUS_NO_SUCH_FILE;

        Irp->IoStatus.Information = 0;

        return STATUS_NO_SUCH_FILE;
    }

    FileDiskDeleteDevice(DataDevObj);

    PDEVICE_EXTENSION CtrlDevObjExt = (PDEVICE_EXTENSION)CtrlDevObj->DeviceExtension;

    InterlockedDecrement(&CtrlDevObjExt->union_special_dev_ext.ctrl_dev_ext.number_of_device);

    Irp->IoStatus.Information = 0;

    Irp->IoStatus.Status = STATUS_SUCCESS;

    return STATUS_SUCCESS;
}

NTSTATUS
FileDiskCreateDataDevice (
    IN PDEVICE_OBJECT   CtrlDevObject,
    IN DEVICE_TYPE      DeviceType,
    IN ULONG            Number,
    IN CHAR             DriverLetter,
    OUT PDEVICE_OBJECT  * pDataDevObj
    )
{
    PAGED_CODE();

    ASSERT(CtrlDevObject != NULL);

    PDRIVER_OBJECT DriverObject = CtrlDevObject->DriverObject;

    PDEVICE_EXTENSION devExtCtrlDev = (PDEVICE_EXTENSION) CtrlDevObject->DeviceExtension;
    
    WCHAR               devNameBuf[DEVICE_NAME_LENGTH] = {0};

    UNICODE_STRING      dataDevName;

    if (DeviceType == FILE_DEVICE_CD_ROM)
    {
        swprintf(
            devNameBuf,
            DEVICE_NAME_PREFIX L"Cd" L"%u",
            Number
            );
    }
    else
    {
        swprintf(
            devNameBuf,
            DEVICE_NAME_PREFIX L"%u",
            Number
            );
    }

    RtlInitUnicodeString(&dataDevName, devNameBuf);

    NTSTATUS status = IoCreateDevice(DriverObject,
        sizeof(DEVICE_EXTENSION),
        &dataDevName,
        DeviceType,
        0,
        FALSE,
        pDataDevObj
        );

    if (!NT_SUCCESS(status))
    {
        return status;
    }

    PDEVICE_OBJECT DataDevObj = *pDataDevObj;

    DataDevObj->Flags |= ( DO_BUFFERED_IO | DO_DIRECT_IO );

    PDEVICE_EXTENSION dataDevExt = (PDEVICE_EXTENSION) DataDevObj->DeviceExtension;

    dataDevExt->ctrldev_or_datadev = FALSE;

    dataDevExt->device_name.Buffer = (PWSTR)ExAllocatePoolWithTag( PagedPool, DEVICE_NAME_LENGTH * sizeof(WCHAR), 'DFgT' );

    dataDevExt->device_name.MaximumLength = DEVICE_NAME_LENGTH * sizeof(WCHAR);

    RtlCopyUnicodeString(&(dataDevExt->device_name),&dataDevName);

    dataDevExt->union_special_dev_ext.data_dev_ext.driver_letter = DriverLetter;

    dataDevExt->union_special_dev_ext.data_dev_ext.media_in_device = FALSE;

    if (DeviceType == FILE_DEVICE_CD_ROM)
    {
        DataDevObj->Characteristics |= FILE_READ_ONLY_DEVICE;

        dataDevExt->union_special_dev_ext.data_dev_ext.read_only = TRUE;
    }

    InitializeListHead(&dataDevExt->list_head);

    KeInitializeSpinLock(&dataDevExt->list_lock);

    KeInitializeEvent(
        &dataDevExt->request_event,
        SynchronizationEvent,
        FALSE
        );

    dataDevExt->terminate_thread = FALSE;

    HANDLE              thread_handle;

    status = PsCreateSystemThread(
        &thread_handle,
        (ACCESS_MASK) 0L,
        NULL,
        NULL,
        NULL,
        FileDiskThread,
        DataDevObj
        );

    if (!NT_SUCCESS(status))
    {
        IoDeleteDevice(DataDevObj);
        return status;
    }

    status = ObReferenceObjectByHandle(
        thread_handle,
        THREAD_ALL_ACCESS,
        NULL,
        KernelMode,
        &dataDevExt->thread_pointer,
        NULL
        );

    if (!NT_SUCCESS(status))
    {
        ZwClose(thread_handle);

        dataDevExt->terminate_thread = TRUE;

        KeSetEvent(
            &dataDevExt->request_event,
            (KPRIORITY) 0,
            FALSE
            );

        IoDeleteDevice(DataDevObj);

        return status;
    }

    ZwClose(thread_handle);


    WCHAR               symLinkBuf[DEVICE_NAME_LENGTH];

    UNICODE_STRING      dataDevLink;

    //StringCchPrintfW()
    swprintf( symLinkBuf, DATADEV_SYMLINK_CREATE, DriverLetter );

	RtlInitUnicodeString(&dataDevLink,symLinkBuf);

    dataDevExt->symbolic_link.Buffer = (PWSTR)ExAllocatePoolWithTag( PagedPool, DEVICE_NAME_LENGTH * sizeof(WCHAR), 'DFgT' );

    dataDevExt->symbolic_link.MaximumLength = DEVICE_NAME_LENGTH * sizeof(WCHAR);

    RtlCopyUnicodeString(&(dataDevExt->symbolic_link),&dataDevLink);

	status = IoCreateSymbolicLink(&(dataDevExt->symbolic_link),&(dataDevExt->device_name));

    if (!NT_SUCCESS(status))
    {
        DbgPrint("IoCreateSymbolicLink failed for 0x%x\n",status);
        IoDeleteSymbolicLink(&(dataDevExt->symbolic_link));

        status = IoCreateSymbolicLink(&(dataDevExt->symbolic_link),&(dataDevExt->device_name));
		
		if( !NT_SUCCESS(status))
		{
            DbgPrint("IoCreateSymbolicLink failed again for 0x%x\n",status);

            RtlFreeUnicodeString(&(dataDevExt->symbolic_link));

            ZwClose(thread_handle);

            dataDevExt->terminate_thread = TRUE;

            KeSetEvent(
                &dataDevExt->request_event,
                (KPRIORITY) 0,
                FALSE
                );

            IoDeleteDevice(DataDevObj);

            return status;
		}
    }

    DataDevObj->Flags &= ~DO_DEVICE_INITIALIZING;

    return STATUS_SUCCESS;
}


NTSTATUS 
FindDataDeviceByDriverLetter(PDEVICE_OBJECT CtrlDevObj, CHAR DriverLetter, PDEVICE_OBJECT * pDataDevObj)
{
    PAGED_CODE();

    ASSERT(CtrlDevObj != NULL);

    ASSERT(pDataDevObj != NULL);

    NTSTATUS status = STATUS_UNSUCCESSFUL;

    if(DriverLetter >= 'a' && DriverLetter <= 'z' || DriverLetter >= 'A' && DriverLetter <= 'Z' )
    {

        PDEVICE_OBJECT device_object = CtrlDevObj->DriverObject->DeviceObject;

        PDEVICE_EXTENSION device_extension = NULL;

        while (device_object)
        {
            device_extension = (PDEVICE_EXTENSION) device_object->DeviceExtension;

            if (device_extension->ctrldev_or_datadev)
            {
                device_object = device_object->NextDevice;

                continue;
            }
            else
            {
                CHAR tmpDL = device_extension->union_special_dev_ext.data_dev_ext.driver_letter;
        
                if( DriverLetter == tmpDL
                    || ( DriverLetter >= 'a' && DriverLetter <= 'z' && (DriverLetter - tmpDL) == ('a'-'A') )
                    || ( DriverLetter >= 'A' && DriverLetter <= 'Z' && (tmpDL - DriverLetter) == ('a'-'A') )
                    )
                {

                    *pDataDevObj = device_object;
                        
                    status = STATUS_SUCCESS;
        
                    break;

                }

                device_object = device_object->NextDevice;
            }
            
        }
    }

    return status;
}
