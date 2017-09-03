#include "device.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winioctl.h>

Device::Device(QObject *parent) :
    QObject(parent),
    m_handle(INVALID_HANDLE_VALUE)
{
}

Device::~Device()
{
    close();
}

bool Device::open(const QString &filePath)
{
    const DWORD access = GENERIC_READ | GENERIC_WRITE;
    const DWORD share = FILE_SHARE_READ | FILE_SHARE_WRITE
            | FILE_SHARE_DELETE;
    const DWORD creation = OPEN_EXISTING;
    const DWORD attr = FILE_ATTRIBUTE_NORMAL
            | SECURITY_SQOS_PRESENT | SECURITY_IDENTIFICATION;

    m_handle = CreateFileW((LPCWSTR) filePath.utf16(),
                           access, share, NULL, creation, attr, NULL);

    return m_handle != INVALID_HANDLE_VALUE;
}

bool Device::close()
{
    bool res = false;
    if (m_handle != INVALID_HANDLE_VALUE) {
        res = CloseHandle(m_handle);
        m_handle = INVALID_HANDLE_VALUE;
    }
    return res;
}

bool Device::ioctl(int code, char *in, int inSize,
                   char *out, int outSize,
                   int *retSize)
{
    DWORD size = 0;
    const bool res = DeviceIoControl(
                m_handle, code,
                in, inSize, out, outSize,
                &size, NULL);

    if (retSize) {
        *retSize = size;
    }

    return res;
}

QString Device::getLastErrorMessage()
{
    const DWORD err = GetLastError();
    LPWSTR buf = nullptr;

    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER
                   | FORMAT_MESSAGE_FROM_SYSTEM
                   | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, err, 0, (LPWSTR) &buf, 0, NULL);

    if (!buf) {
        return QString("System Error %1").arg(err);
    }

    const QString text = QString::fromUtf16((const ushort *) buf).trimmed();
    LocalFree(buf);
    return text;
}
