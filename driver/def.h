#ifndef DEF
#define DEF

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <libusb-1.0/libusb.h>
#include <sys/mman.h>

typedef unsigned char BYTE;

#pragma pack (1)

// Scanner is big endian...
#define WX(a) ((uint16_t)(((a)&0xFF)<<8)+(((a)>>8)&0xFF))
#define UX(a) ((uint32_t)(((a)<<24)&0xFF000000)+(((a)<<8)&0xFF0000)+(((a)>>8)&0xFF00)+(((a)>>24)&0xFF))

// Pixel type...
enum EPIXELTYPE
{
	PIXELTYPE_BW = 0,
	PIXELTYPE_GRAY = 1,
	PIXELTYPE_COLOR = 2
};

//
// SCSI: Inquiry
//
struct ScsiInquiry
{
	// SCSI Common...
	BYTE	sPeriph_dev_type	:		5;		// 0:		Peripheral Device Type
	BYTE	sPeriph_qual		:		3;		// 0:		Peripheral Qualifier

	BYTE	sDev_type_qual		:		7;		// 1:		Device-Type Qualifier
	BYTE	sRmb				:		1;		// 1:		RMB

	BYTE	sAnsi_ver			:		3;		// 2:		ANSI-Approved Version
	BYTE	sEcma_ver			:		3;		// 2:		ECMA Version
	BYTE	sIso_ver			:		2;		// 2:		ISO Version

	BYTE	sResp_data_form		:		4;		// 3:		Response Data Format
	BYTE	sReserved_1			:		3;		// 3:		Reserved
	BYTE	sAenc				:		1;		// 3:		AENC

	BYTE	sAdd_len;							// 4:		Additional Length (n-4)

	BYTE	sReserved_2[2];						// 5-6:		Reserved

	BYTE	sSft_rst			:		1;		// 7:		Soft Reset
	BYTE	sCmdque				:		1;		// 7:		CmdQue
	BYTE	sReserved_3			:		1;		// 7:		Reserved
	BYTE	sLinked				:		1;		// 7:		Linked
	BYTE	sSync				:		1;		// 7:		Sync
	BYTE	sWbus16				:		1;		// 7:		WBus16
	BYTE	sWbus32				:		1;		// 7:		WBus32
	BYTE	sReladr				:		1;		// 7:		RelAdr
	
	BYTE	sVend_id[8];						// 8-15:	Vendor Identification
	BYTE	sProd_id[16];						// 16-31:	Product Identification
	BYTE	sProd_revis_lev[4];					// 32-35:	Product Revision Level

	BYTE	sReserve[60];
};

//
// SCSI: Sense
//
struct ScsiSense
{
	BYTE	Error_Code			:		7;		// 0:		Error Code
	BYTE	Valid				:		1;		// 0:		Valid bit

	BYTE	Segment_Number;						// 1:		Segment Number

	BYTE	Sense_Key			:		4;		// 2:		Sense Key
	BYTE	Reserved1			:		1;		// 2:		Reserved
	BYTE	ILI					:		1;		// 2:		Incorrent Length Indicator
	BYTE	EOM					:		1;		// 2:		End of Media
	BYTE	Filemark			:		1;		// 2:		Filemark

	BYTE	Information_BITFIELDs[4];			// 3-6:		Info
	BYTE	Additional_Sense_Length;			// 7:		Additional Sense Length
	BYTE	Command_Specific_Information[4];	// 8-11:	Command Specific Info
	BYTE	Additional_Sense_Code;				// 12:		Sense Code
	BYTE	Additional_Sense_Code_Qualifier;	// 13:		Sense Code Qualifier
	BYTE	FRU;								// 14:		FRU

	BYTE	Sense_Key_Specific1	:		7;		// 15:		Sense Key Specific 1
	BYTE	SKSV				:		1;		// 15:		SKSV

	BYTE	Sense_Key_Specific2;				// 16:		Sense Key Specific 2
	BYTE	Sense_Key_Specific3;				// 17:		Sense Key Specific 3

	BYTE	Reserve[4];
};

//
// SCSI: Get/Define Window
//
struct ScsiWindow
{
	BYTE	 WindowDescLength[2];				// 0-1:		Window description length
	BYTE	 Reserved_0[4];						// 2-5:		Reserved
	BYTE	 WindowDataLength[2];				// 6-7:		Window description length

	BYTE     WindowId;							// 8:		Window ID
	BYTE     Reserved_1;						// 9:		Reserved
	ushort   X_resolution;						// 10-11:	X Resolution
	ushort   Y_resolution;						// 12-13:	Y Resolution
	BYTE     UpperLeft_X[4];					// 14-17:	Upper Left X
	BYTE     UpperLeft_Y[4];					// 18-21:	Upper Left Y
	uint     Width;								// 22-25:	Width
	uint     Length;							// 26-29:	Length
	BYTE     Brightness;						// 30:		Brightness
	BYTE     Threshold;							// 31:		Threshold
	BYTE     Contrast;							// 32:		Contrast
	BYTE     ImageComposition;					// 33:		Image Composition
	BYTE     BitsPerPixel;						// 34:		Bits Per Pixel
	BYTE     HalftonePattern[2];				// 35-36:	Halftone Pattern

	BYTE	 PaddingType	:	3;				// 37:		Padding Type
	BYTE	 Reserved_2		:	4;				// 37:		Reserved
	BYTE	 Rif			:	1;				// 37:		Reverse Image Format

	BYTE     BitOrdering[2];					// 38-39:	Bit Ordering
	BYTE     CompressionType;					// 40:		Compression
	BYTE     CompressionArgument;				// 41:		K-Factor

	BYTE	 PaperLength[2];					// 42-43:	Page Length Detection
	BYTE     Reserved_3[4];						// 44-47:	Reserved

	BYTE	 AvisionFF;							// 48:		Avision ID (FF)
	BYTE	 AddedLength;						// 49:		Number of extra bytes

	BYTE	 ScanSpeed		:	3;				// 50:		0-slowest / 4-fastest
	BYTE	 Color			:	3;				// 50:		0-none/1-red/2-green/3-blue/4-rgb/5-cmyk/6-gray
	BYTE	 Source			:	1;				// 50:		0-get line width/1-set line width
	BYTE	 ADF			:	1;				// 50:		0-flatbed/1-ADF

	BYTE	 Highlight;							// 51:		Highlight snap
	BYTE	 Shadow;							// 52:		Shadow snap
	ushort	 LineWidth;							// 53-54:	Line width
	ushort	 LineCount;							// 55-56:	Line count

	BYTE	 BoxCar			:	1;				// 57:		0-no Box Car compression
	BYTE	 NoHome			:	1;				// 57:		0-check home sensor
	BYTE	 ScanDir		:	1;				// 57:		0-scan from home / 1-bottom to home
	BYTE	 CalType		:	1;				// 57:		0-quality cal / 1-speed cal
	BYTE	 ScanType		:	1;				// 57:		0-speed mode / 1-quality mode
	BYTE	 MSBLSB			:	1;				// 57:		0-msb
	BYTE	 ByteOrder		:	1;				// 57:		0-Intel,low byte first / 1-Motorola
	BYTE	 Trans			:	1;				// 57):		0-normal / 1-transparency

	BYTE	 IrExposureTimeFactor;				// 58:		Film?
	BYTE	 RedExposureTimeFactor[2];			// 59-60:	Film?
	BYTE	 GreenExposureTimeFactor[2];		// 61-62:	Film?
	BYTE	 BlueExposureTimeFactor[2];			// 63-64:	Film?

	BYTE	 Reserved_4		:	3;				// 65:		reserved
	BYTE	 ADFMode		:	2;				// 65:		0-simplex/1-duplex
	BYTE	 CMYK			:	1;				// 65:		0-enable/1-disable
	BYTE	 Reserved_5		:	2;				// 65:		reserved

	BYTE	 AutoFocusLinesPerSteps;			// 66:		Film?
	BYTE	 LineWidthHigh;						// 67:		High byte
	BYTE	 LineCountHigh;						// 68:		High byte
	BYTE	 BackgroundRasterLines;				// 69:		Number of background rasters to read
	BYTE	 One;						// 70:		Number of background rasters to read
};

// A TIFF header is composed of tags...
struct TiffTag
{
	ushort			u16Tag;
	ushort			u16Type;
	unsigned int	u32Count;
	unsigned int	u32Value;
};

// TIFF header for BITONAL images...
struct TiffBitonal
{
	// Header...
	unsigned char	u8ByteOrder[2];
	ushort			u16Version;
	unsigned int	u32OffsetFirstIFD;
	// First IFD...
	ushort			u16IFD;
	// Tags...
	TiffTag			tifftag[16];
	unsigned int	u32NextIFD;
	unsigned int	u32XResolution[2];
	unsigned int	u32YResolution[2];
};

// TIFF header for Grayscale images...
struct TiffGray
{
	// Header...
	unsigned char	u8ByteOrder[2];
	ushort			u16Version;
	unsigned int	u32OffsetFirstIFD;
	// First IFD...
	ushort			u16IFD;
	// Tags...
	TiffTag			tifftag[14];
	unsigned int	u32NextIFD;
	unsigned int	u32XResolution[2];
	unsigned int	u32YResolution[2];
};

// TIFF header for Color uncompressed images...
struct TiffColor
{
	// Header...
	unsigned char	u8ByteOrder[2];
	ushort			u16Version;
	unsigned int	u32OffsetFirstIFD;
	// First IFD...
	ushort			u16IFD;
	// Tags...
	TiffTag			tifftag[14];
	unsigned int	u32NextIFD;
	ushort			u16BitsPerSample[3];
	unsigned int	u32XResolution[2];
	unsigned int	u32YResolution[2];
	unsigned int	u32Stuff[4];
};

#pragma pack ()

#endif // DEF

