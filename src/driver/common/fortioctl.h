#ifndef FORTIOCTL_H
#define FORTIOCTL_H

#include "common.h"

#define INITGUID
#include <guiddef.h>

/* GUID-s generated by guidgen.exe */

/* {012B3999-B791-463E-918C-C32B53E9350E} */
DEFINE_GUID(FORT_GUID_PROVIDER, 0x12b3999, 0xb791, 0x463e, 0x91, 0x8c, 0xc3, 0x2b, 0x53, 0xe9, 0x35,
        0xe);

/* {749978E2-CB6A-42F9-9C30-1A3EA9810185} */
DEFINE_GUID(FORT_GUID_CALLOUT_CONNECT_V4, 0x749978e2, 0xcb6a, 0x42f9, 0x9c, 0x30, 0x1a, 0x3e, 0xa9,
        0x81, 0x1, 0x85);

/* {A69AF63F-A10E-4CDC-B473-71D3CAA39929} */
DEFINE_GUID(FORT_GUID_CALLOUT_CONNECT_V6, 0xa69af63f, 0xa10e, 0x4cdc, 0xb4, 0x73, 0x71, 0xd3, 0xca,
        0xa3, 0x99, 0x29);

/* {CED9C0FC-DF22-4F39-9185-B435726CABB2} */
DEFINE_GUID(FORT_GUID_CALLOUT_ACCEPT_V4, 0xced9c0fc, 0xdf22, 0x4f39, 0x91, 0x85, 0xb4, 0x35, 0x72,
        0x6c, 0xab, 0xb2);

/* {ED43ABFB-1AE7-4666-8360-2E9C6895A13D} */
DEFINE_GUID(FORT_GUID_CALLOUT_ACCEPT_V6, 0xed43abfb, 0x1ae7, 0x4666, 0x83, 0x60, 0x2e, 0x9c, 0x68,
        0x95, 0xa1, 0x3d);

/* {DED18B1B-0022-415F-AFDC-C9D59CE6D475} */
DEFINE_GUID(FORT_GUID_CALLOUT_IN_TRANSPORT_V4, 0xded18b1b, 0x22, 0x415f, 0xaf, 0xdc, 0xc9, 0xd5,
        0x9c, 0xe6, 0xd4, 0x75);

/* {07463B63-84A0-4ED6-B51D-8E45A536815F} */
DEFINE_GUID(FORT_GUID_CALLOUT_IN_TRANSPORT_V6, 0x7463b63, 0x84a0, 0x4ed6, 0xb5, 0x1d, 0x8e, 0x45,
        0xa5, 0x36, 0x81, 0x5f);

/* {0004F6D3-8430-4B35-9BF7-23C25C25003E} */
DEFINE_GUID(FORT_GUID_CALLOUT_OUT_TRANSPORT_V4, 0x4f6d3, 0x8430, 0x4b35, 0x9b, 0xf7, 0x23, 0xc2,
        0x5c, 0x25, 0x0, 0x3e);

/* {A2A2C562-BFCD-4D56-9EFD-B2C4C58DCC4B} */
DEFINE_GUID(FORT_GUID_CALLOUT_OUT_TRANSPORT_V6, 0xa2a2c562, 0xbfcd, 0x4d56, 0x9e, 0xfd, 0xb2, 0xc4,
        0xc5, 0x8d, 0xcc, 0x4b);

/* {AFA06CD5-4942-4FDF-8A4A-2EDEB25BBECE} */
DEFINE_GUID(FORT_GUID_SUBLAYER, 0xafa06cd5, 0x4942, 0x4fdf, 0x8a, 0x4a, 0x2e, 0xde, 0xb2, 0x5b,
        0xbe, 0xce);

/* {72AF05F1-4B4D-4485-B852-85407C0BE405} */
DEFINE_GUID(FORT_GUID_BOOT_FILTER_CONNECT_V4, 0x72af05f1, 0x4b4d, 0x4485, 0xb8, 0x52, 0x85, 0x40,
        0x7c, 0xb, 0xe4, 0x5);

/* {20406F5A-0819-471D-8499-BCA022C284F0} */
DEFINE_GUID(FORT_GUID_BOOT_FILTER_CONNECT_V6, 0x20406f5a, 0x819, 0x471d, 0x84, 0x99, 0xbc, 0xa0,
        0x22, 0xc2, 0x84, 0xf0);

/* {87135302-8994-41D0-9DD0-06B28E8409EE} */
DEFINE_GUID(FORT_GUID_BOOT_FILTER_ACCEPT_V4, 0x87135302, 0x8994, 0x41d0, 0x9d, 0xd0, 0x6, 0xb2,
        0x8e, 0x84, 0x9, 0xee);

/* {E789730B-7973-4A62-949F-B2FDEF6D8578} */
DEFINE_GUID(FORT_GUID_BOOT_FILTER_ACCEPT_V6, 0xe789730b, 0x7973, 0x4a62, 0x94, 0x9f, 0xb2, 0xfd,
        0xef, 0x6d, 0x85, 0x78);

/* {1811AD5C-DFBD-485A-A1FF-1018C56EC772} */
DEFINE_GUID(FORT_GUID_PERSIST_FILTER_CONNECT_V4, 0x1811ad5c, 0xdfbd, 0x485a, 0xa1, 0xff, 0x10, 0x18,
        0xc5, 0x6e, 0xc7, 0x72);

/* {E6C87910-11D8-481E-9D1C-0FF0CBF2AEE7} */
DEFINE_GUID(FORT_GUID_PERSIST_FILTER_CONNECT_V6, 0xe6c87910, 0x11d8, 0x481e, 0x9d, 0x1c, 0xf, 0xf0,
        0xcb, 0xf2, 0xae, 0xe7);

/* {46B0FF23-137E-47AF-9689-EE589FE1615D} */
DEFINE_GUID(FORT_GUID_PERSIST_FILTER_ACCEPT_V4, 0x46b0ff23, 0x137e, 0x47af, 0x96, 0x89, 0xee, 0x58,
        0x9f, 0xe1, 0x61, 0x5d);

/* {22006CB4-3723-4C13-9A93-719CE21557F7} */
DEFINE_GUID(FORT_GUID_PERSIST_FILTER_ACCEPT_V6, 0x22006cb4, 0x3723, 0x4c13, 0x9a, 0x93, 0x71, 0x9c,
        0xe2, 0x15, 0x57, 0xf7);

/* {91CF8B80-7BB2-42BA-8A26-8242E14532FC} */
DEFINE_GUID(FORT_GUID_FILTER_CONNECT_V4, 0x91cf8b80, 0x7bb2, 0x42ba, 0x8a, 0x26, 0x82, 0x42, 0xe1,
        0x45, 0x32, 0xfc);

/* {86510E3E-84C0-4E38-BDF5-3395917251C3} */
DEFINE_GUID(FORT_GUID_FILTER_CONNECT_V6, 0x86510e3e, 0x84c0, 0x4e38, 0xbd, 0xf5, 0x33, 0x95, 0x91,
        0x72, 0x51, 0xc3);

/* {544A3E25-7BEB-4970-88EF-B4BCA2CE2482} */
DEFINE_GUID(FORT_GUID_FILTER_ACCEPT_V4, 0x544a3e25, 0x7beb, 0x4970, 0x88, 0xef, 0xb4, 0xbc, 0xa2,
        0xce, 0x24, 0x82);

/* {F2985354-C225-4289-8D01-80A87CF6D7B4} */
DEFINE_GUID(FORT_GUID_FILTER_ACCEPT_V6, 0xf2985354, 0xc225, 0x4289, 0x8d, 0x1, 0x80, 0xa8, 0x7c,
        0xf6, 0xd7, 0xb4);

/* {F515D62B-26A3-413E-874C-D65CE70C9AEF} */
DEFINE_GUID(FORT_GUID_FILTER_IN_TRANSPORT_V4, 0xf515d62b, 0x26a3, 0x413e, 0x87, 0x4c, 0xd6, 0x5c,
        0xe7, 0xc, 0x9a, 0xef);

/* {FDBB79A9-773D-4028-B4B4-AD99CAFCAEFC} */
DEFINE_GUID(FORT_GUID_FILTER_IN_TRANSPORT_V6, 0xfdbb79a9, 0x773d, 0x4028, 0xb4, 0xb4, 0xad, 0x99,
        0xca, 0xfc, 0xae, 0xfc);

/* {D284AFE8-4CAF-4432-A753-B6F311BDA2BA} */
DEFINE_GUID(FORT_GUID_FILTER_OUT_TRANSPORT_V4, 0xd284afe8, 0x4caf, 0x4432, 0xa7, 0x53, 0xb6, 0xf3,
        0x11, 0xbd, 0xa2, 0xba);

/* {A14E1FAE-E1D7-42E0-851C-6F3A1B89BEAD} */
DEFINE_GUID(FORT_GUID_FILTER_OUT_TRANSPORT_V6, 0xa14e1fae, 0xe1d7, 0x42e0, 0x85, 0x1c, 0x6f, 0x3a,
        0x1b, 0x89, 0xbe, 0xad);

/* {C2D858F8-2951-4EED-8DA1-A4930F8E5204} */
DEFINE_GUID(FORT_GUID_FILTER_REAUTH_IN_V4, 0xc2d858f8, 0x2951, 0x4eed, 0x8d, 0xa1, 0xa4, 0x93, 0xf,
        0x8e, 0x52, 0x4);

/* {635F7606-E920-4D6D-8222-6C3A514C703E} */
DEFINE_GUID(FORT_GUID_FILTER_REAUTH_IN_V6, 0x635f7606, 0xe920, 0x4d6d, 0x82, 0x22, 0x6c, 0x3a, 0x51,
        0x4c, 0x70, 0x3e);

/* {749709CE-9686-4056-B89A-7A5852DFC898} */
DEFINE_GUID(FORT_GUID_FILTER_REAUTH_OUT_V4, 0x749709ce, 0x9686, 0x4056, 0xb8, 0x9a, 0x7a, 0x58,
        0x52, 0xdf, 0xc8, 0x98);

/* {B3DB1623-C317-4E04-A9D1-5482960EB70C} */
DEFINE_GUID(FORT_GUID_FILTER_REAUTH_OUT_V6, 0xb3db1623, 0xc317, 0x4e04, 0xa9, 0xd1, 0x54, 0x82,
        0x96, 0xe, 0xb7, 0xc);

/* {00000000-0000-0000-0000-000000000000} */
DEFINE_GUID(FORT_GUID_EMPTY, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00);

#define FORT_DEVICE_NAME     "\\\\.\\fortfw"
#define FORT_NT_DEVICE_NAME  L"\\Device\\fortfw"
#define FORT_DOS_DEVICE_NAME L"\\DosDevices\\fortfw"

#define FORT_DEVICE_TYPE 0xD000
#define FORT_IOCTL_BASE  0xD00

#define FORT_CTL_CODE(i, a) CTL_CODE(FORT_DEVICE_TYPE, FORT_IOCTL_BASE + (i), METHOD_BUFFERED, (a))

/* Macro to extract function index out of the device io control code */
#define FORT_CTL_INDEX_FROM_CODE(ctrlCode) ((DWORD) ((ctrlCode >> 2) & 0xFF))

#define FORT_IOCTL_INDEX_VALIDATE    0
#define FORT_IOCTL_INDEX_SETSERVICES 1
#define FORT_IOCTL_INDEX_SETCONF     2
#define FORT_IOCTL_INDEX_SETFLAGS    3
#define FORT_IOCTL_INDEX_GETLOG      4
#define FORT_IOCTL_INDEX_ADDAPP      5
#define FORT_IOCTL_INDEX_DELAPP      6
#define FORT_IOCTL_INDEX_SETZONES    7
#define FORT_IOCTL_INDEX_SETZONEFLAG 8

#define FORT_IOCTL_VALIDATE    FORT_CTL_CODE(FORT_IOCTL_INDEX_VALIDATE, FILE_WRITE_DATA)
#define FORT_IOCTL_SETSERVICES FORT_CTL_CODE(FORT_IOCTL_INDEX_SETSERVICES, FILE_WRITE_DATA)
#define FORT_IOCTL_SETCONF     FORT_CTL_CODE(FORT_IOCTL_INDEX_SETCONF, FILE_WRITE_DATA)
#define FORT_IOCTL_SETFLAGS    FORT_CTL_CODE(FORT_IOCTL_INDEX_SETFLAGS, FILE_WRITE_DATA)
#define FORT_IOCTL_GETLOG      FORT_CTL_CODE(FORT_IOCTL_INDEX_GETLOG, FILE_READ_DATA)
#define FORT_IOCTL_ADDAPP      FORT_CTL_CODE(FORT_IOCTL_INDEX_ADDAPP, FILE_WRITE_DATA)
#define FORT_IOCTL_DELAPP      FORT_CTL_CODE(FORT_IOCTL_INDEX_DELAPP, FILE_WRITE_DATA)
#define FORT_IOCTL_SETZONES    FORT_CTL_CODE(FORT_IOCTL_INDEX_SETZONES, FILE_WRITE_DATA)
#define FORT_IOCTL_SETZONEFLAG FORT_CTL_CODE(FORT_IOCTL_INDEX_SETZONEFLAG, FILE_WRITE_DATA)

#endif // FORTIOCTL_H
