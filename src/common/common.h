#ifndef COMMON_H
#define COMMON_H

#include "version.h"

#define INITGUID
#include <guiddef.h>

/* GUID-s generated by guidgen.exe */

/* {012B3999-B791-463E-918C-C32B53E9350E} */
DEFINE_GUID(FORT_GUID_PROVIDER,
	0x12b3999, 0xb791, 0x463e, 0x91, 0x8c, 0xc3, 0x2b, 0x53, 0xe9, 0x35, 0xe);

/* {749978E2-CB6A-42F9-9C30-1A3EA9810185} */
DEFINE_GUID(FORT_GUID_CALLOUT_CONNECT_V4,
	0x749978e2, 0xcb6a, 0x42f9, 0x9c, 0x30, 0x1a, 0x3e, 0xa9, 0x81, 0x1, 0x85);

/* {CED9C0FC-DF22-4F39-9185-B435726CABB2} */
DEFINE_GUID(FORT_GUID_CALLOUT_ACCEPT_V4,
	0xced9c0fc, 0xdf22, 0x4f39, 0x91, 0x85, 0xb4, 0x35, 0x72, 0x6c, 0xab, 0xb2);

/* {1F50005D-CDBC-42A0-A0C0-53E43081FABE} */
DEFINE_GUID(FORT_GUID_CALLOUT_STREAM_V4,
	0x1f50005d, 0xcdbc, 0x42a0, 0xa0, 0xc0, 0x53, 0xe4, 0x30, 0x81, 0xfa, 0xbe);

/* {5F1A7B3C-3E88-41C9-A442-61CFE6A48806} */
DEFINE_GUID(FORT_GUID_CALLOUT_DATAGRAM_V4,
	0x5f1a7b3c, 0x3e88, 0x41c9, 0xa4, 0x42, 0x61, 0xcf, 0xe6, 0xa4, 0x88, 0x6);

/* {DED18B1B-0022-415F-AFDC-C9D59CE6D475} */
DEFINE_GUID(FORT_GUID_CALLOUT_IN_TRANSPORT_V4,
	0xded18b1b, 0x22, 0x415f, 0xaf, 0xdc, 0xc9, 0xd5, 0x9c, 0xe6, 0xd4, 0x75);

/* {0004F6D3-8430-4B35-9BF7-23C25C25003E} */
DEFINE_GUID(FORT_GUID_CALLOUT_OUT_TRANSPORT_V4,
	0x4f6d3, 0x8430, 0x4b35, 0x9b, 0xf7, 0x23, 0xc2, 0x5c, 0x25, 0x0, 0x3e);

/* {AFA06CD5-4942-4FDF-8A4A-2EDEB25BBECE} */
DEFINE_GUID(FORT_GUID_SUBLAYER,
	0xafa06cd5, 0x4942, 0x4fdf, 0x8a, 0x4a, 0x2e, 0xde, 0xb2, 0x5b, 0xbe, 0xce);

/* {91CF8B80-7BB2-42BA-8A26-8242E14532FC} */
DEFINE_GUID(FORT_GUID_FILTER_CONNECT_V4,
	0x91cf8b80, 0x7bb2, 0x42ba, 0x8a, 0x26, 0x82, 0x42, 0xe1, 0x45, 0x32, 0xfc);

/* {544A3E25-7BEB-4970-88EF-B4BCA2CE2482} */
DEFINE_GUID(FORT_GUID_FILTER_ACCEPT_V4,
	0x544a3e25, 0x7beb, 0x4970, 0x88, 0xef, 0xb4, 0xbc, 0xa2, 0xce, 0x24, 0x82);

/* {ED0F2527-A787-4CA2-9493-C96320422FCF} */
DEFINE_GUID(FORT_GUID_FILTER_STREAM_V4,
	0xed0f2527, 0xa787, 0x4ca2, 0x94, 0x93, 0xc9, 0x63, 0x20, 0x42, 0x2f, 0xcf);

/* {A3700639-1B50-461C-BE4C-BC350A7FB3A9} */
DEFINE_GUID(FORT_GUID_FILTER_DATAGRAM_V4,
	0xa3700639, 0x1b50, 0x461c, 0xbe, 0x4c, 0xbc, 0x35, 0xa, 0x7f, 0xb3, 0xa9);

/* {F515D62B-26A3-413E-874C-D65CE70C9AEF} */
DEFINE_GUID(FORT_GUID_FILTER_IN_TRANSPORT_V4,
	0xf515d62b, 0x26a3, 0x413e, 0x87, 0x4c, 0xd6, 0x5c, 0xe7, 0xc, 0x9a, 0xef);

/* {D284AFE8-4CAF-4432-A753-B6F311BDA2BA} */
DEFINE_GUID(FORT_GUID_FILTER_OUT_TRANSPORT_V4,
	0xd284afe8, 0x4caf, 0x4432, 0xa7, 0x53, 0xb6, 0xf3, 0x11, 0xbd, 0xa2, 0xba);

/* {C2D858F8-2951-4EED-8DA1-A4930F8E5204} */
DEFINE_GUID(FORT_GUID_FILTER_REAUTH_IN,
	0xc2d858f8, 0x2951, 0x4eed, 0x8d, 0xa1, 0xa4, 0x93, 0xf, 0x8e, 0x52, 0x4);

/* {749709CE-9686-4056-B89A-7A5852DFC898} */
DEFINE_GUID(FORT_GUID_FILTER_REAUTH_OUT,
	0x749709ce, 0x9686, 0x4056, 0xb8, 0x9a, 0x7a, 0x58, 0x52, 0xdf, 0xc8, 0x98);

#define FORT_DEVICE_NAME	"\\\\.\\fortfw"

#define FORT_DEVICE_TYPE	0xD000
#define FORT_IOCTL_BASE		0xD00

#define FORT_CTL_CODE(i,a)	CTL_CODE(FORT_DEVICE_TYPE, FORT_IOCTL_BASE + (i), METHOD_BUFFERED, (a))

#define FORT_IOCTL_VALIDATE	FORT_CTL_CODE(0, FILE_WRITE_DATA)
#define FORT_IOCTL_SETCONF	FORT_CTL_CODE(1, FILE_WRITE_DATA)
#define FORT_IOCTL_SETFLAGS	FORT_CTL_CODE(2, FILE_WRITE_DATA)
#define FORT_IOCTL_GETLOG	FORT_CTL_CODE(3, FILE_READ_DATA)
#define FORT_IOCTL_ADDAPP	FORT_CTL_CODE(4, FILE_WRITE_DATA)
#define FORT_IOCTL_DELAPP	FORT_CTL_CODE(5, FILE_WRITE_DATA)
#define FORT_IOCTL_SETZONES	FORT_CTL_CODE(6, FILE_WRITE_DATA)
#define FORT_IOCTL_SETZONEFLAG	FORT_CTL_CODE(7, FILE_WRITE_DATA)


#ifndef NT_SUCCESS
#define NT_SUCCESS(status)	((LONG) (status) >= 0)
#endif

#define FORT_STATUS_USER_ERROR	STATUS_INVALID_PARAMETER
#define FORT_ERROR_USER_ERROR	ERROR_INVALID_PARAMETER

/* Convert system time to seconds since 1970 */
#define SECSPERDAY		86400
#define SECS_1601_TO_1970	((369 * 365 + 89) * (INT64) SECSPERDAY)  /* 1601 to 1970 is 369 years plus 89 leap days */
#define fort_system_to_unix_time(system_time)	((system_time) / 10000000 - SECS_1601_TO_1970)

#endif COMMON_H
