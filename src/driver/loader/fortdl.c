/* Fort Firewall Driver Loader */

#include "fortdl.h"

#include "../fortutl.h"

#include "fortimg.h"

typedef struct fort_loader
{
    DWORD image_size;
    PUCHAR image;

    PDRIVER_UNLOAD driver_unload;
} FORT_LOADER, *PFORT_LOADER;

static FORT_LOADER g_fort_loader;

static void fort_loader_unload(PDRIVER_OBJECT driver)
{
    /* Unload the driver image */
    if (g_fort_loader.driver_unload) {
        g_fort_loader.driver_unload(driver);
    }

    /* Free the allocated driver image */
    fort_mem_free(g_fort_loader.image, FORT_LOADER_POOL_TAG);
}

static NTSTATUS fort_loader_init(PDRIVER_OBJECT driver, PWSTR driverPath)
{
    NTSTATUS status;

    DbgPrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "FORT: Loader Init: %ws irql=%d\n",
            driverPath, KeGetCurrentIrql());

    /* Load the driver file */
    PUCHAR data = NULL;
    DWORD dataSize = 0;
    {
        HANDLE fileHandle;
        status = fort_file_open(driverPath, &fileHandle);

        if (NT_SUCCESS(status)) {
            status = fort_file_read(fileHandle, FORT_LOADER_POOL_TAG, &data, &dataSize);
            ZwClose(fileHandle);
        }

        /* Free the allocated driver path */
        ExFreePool(driverPath);

        if (!NT_SUCCESS(status)) {
            DbgPrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL,
                    "FORT: Loader File Read error: [%ws] status=%d size=%d\n", driverPath, status,
                    dataSize);
            return status;
        }
    }

    /* Prepare the driver image */
    PUCHAR image = NULL;
    DWORD imageSize = 0;
    if (NT_SUCCESS(status)) {
        status = fort_image_load(data, dataSize, &image, &imageSize);

        /* Free the driver file's allocated data */
        fort_mem_free(data, FORT_LOADER_POOL_TAG);
    }

    /* Run the driver entry */
    if (NT_SUCCESS(status)) {
        g_fort_loader.image_size = imageSize;
        g_fort_loader.image = image;

        status = STATUS_INVALID_IMAGE_PROTECT;
    }

    if (!NT_SUCCESS(status)) {
        DbgPrintEx(
                DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "FORT: Loader Init: Error: %x\n", status);
    }

    return status;
}

NTSTATUS
#if defined(FORT_DRIVER)
DriverEntry
#else
DriverLoaderEntry
#endif
        (PDRIVER_OBJECT driver, PUNICODE_STRING reg_path)
{
    NTSTATUS status;

    PWSTR driverPath = NULL;
    status = fort_driver_path(driver, reg_path, &driverPath);

    if (!NT_SUCCESS(status)) {
        DbgPrintEx(DPFLTR_IHVNETWORK_ID, DPFLTR_ERROR_LEVEL, "FORT: Loader Entry: Error: %x\n",
                status);
        return status;
    }

    return fort_loader_init(driver, driverPath);
}
