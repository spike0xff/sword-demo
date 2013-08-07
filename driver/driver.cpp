#include "driver.h"
#include <string.h>

// Clear the pod, give ourselves some aligned memory for
// all communication with the scanner, instantiate our
// image object so that we can create tiff files...
CScanner::CScanner()
{
	memset(&pod,0,sizeof(pod));
	pod.m_pbBuffer = (unsigned char*)memalign(sysconf(_SC_PAGESIZE),0x200000);
	pod.m_pimage = new CImage();
}

// Make sure we clean everything up on the way out...
CScanner::~CScanner()
{
	Close();
	if (pod.m_pbBuffer)
	{
		free(pod.m_pbBuffer);
		pod.m_pbBuffer = 0;
	}
	if (pod.m_pimage)
	{
		delete pod.m_pimage;
		pod.m_pimage = 0;
	}
}

// Close the scanner...
void CScanner::Close()
{
	if (pod.m_handle)
	{
		libusb_release_interface(pod.m_handle,0);
		libusb_close(pod.m_handle);
		pod.m_handle = 0;
	}

	if (pod.m_blLibUsbExit)
	{
		pod.m_blLibUsbExit = false;
		libusb_exit(0);
	}
}

// Get inquiry data from the scanner...
void CScanner::Inquiry()
{
	int iXfer;
	unsigned char bSts;
	unsigned char abCmd[10];
	ScsiInquiry scsiinquiry;
	Log("\n",0,0);

	// Send command...
	memcpy(abCmd,"\x12\x00\x00\x00\x60\x00\x00\x00\x00\x00",sizeof(abCmd));
	Log("inquiry.cmd: ",abCmd,sizeof(abCmd));
	Out(abCmd,sizeof(abCmd),&iXfer);

	// Read data...
	In((unsigned char*)&scsiinquiry,sizeof(scsiinquiry),&iXfer);
	Log("inquiry.inp: ",&scsiinquiry,iXfer);

	// Read status...
	In(&bSts,1,&iXfer);
	Log("inquiry.srb: ",&bSts,1);

	// Handle sense...
	if (bSts == 0x02)
	{
		RequestSense();
	}
}

// Make sure the lamps are on...
void CScanner::LampCheck()
{
	int ii;
	int iXfer;
	unsigned char bSts;
	unsigned char abCmd[16];
	unsigned char bLamp;

	// Always turn them on...
    memcpy(abCmd,"\x2a\x00\xa0\x00\x0a\x0d\x00\x00\x01\x00",10);
    Log("lampcheck.cmd: ",abCmd,10);
    Out(abCmd,10,&iXfer);

    // Read data...
	bLamp = 1;
    Log("lampcheck.Out: ",&bLamp,1);
    Out(&bLamp,sizeof(bLamp),&iXfer);

    // Read status...
    In(&bSts,1,&iXfer);
	Log("lampcheck.srb: ",&bSts,1);
	
	// Handle sense...
	if (bSts == 0x02)
	{
		RequestSense();
		return;
	}

	// Wait until the lamps are ready...
	for (ii = 0; ii < 30; ii++)
	{
		Log("\n",0,0);

		// Send command...
		memcpy(abCmd,"\x28\x00\xa0\x00\x0a\x0d\x00\x00\x01\x00",10);
		Log("lampcheck.cmd: ",abCmd,10);
		Out(abCmd,10,&iXfer);

		// Read data...
		In(&bLamp,sizeof(bLamp),&iXfer);
		Log("lampcheck.Inp: ",&bLamp,1);

		// Read status...
		In(&bSts,1,&iXfer);
		Log("lampcheck.srb: ",&bSts,1);

		// Handle sense...
		if (bSts == 0x02)
		{
			RequestSense();
		}

		// Got it...
		if (bLamp == 1)
		{
			break;
		}

		// Try again...
		sleep(1);
	}
}

// Check for paper in the ADF...
void CScanner::MediaCheck()
{
	int ii;
	int iXfer;
	unsigned char bSts;
	unsigned char abCmd[16];
	unsigned char bMedia;

	// Until we see paper...
	for (ii = 0; ii < 30; ii++)
	{
		Log("\n",0,0);

        // Send command...
        memcpy(abCmd,"\x08\x00\x00\x00\x01\x00\x00\x00\x00\x00",10);
        Log("mediacheck.cmd: ",abCmd,10);
        Out(abCmd,10,&iXfer);

        // Read data...
        In(&bMedia,sizeof(bMedia),&iXfer);
        Log("mediacheck.Inp: ",&bMedia,1);

        // Read status...
        In(&bSts,1,&iXfer);
		Log("mediacheck.srb: ",&bSts,1);

		// Handle sense...
		if (bSts == 0x02)
		{
			RequestSense();
		}

		// Got it...
		if (bMedia)
		{
			break;
		}

		// Try again...
		sleep(1);
	}
}

// Open the scanner...
bool CScanner::Open(bool blHtml)
{
	if (pod.m_blLibUsbExit)
	{
		Log("already open...\n",0,0);
		return (false);
	}

	libusb_init(0);
	pod.m_blLibUsbExit = true;

	pod.m_handle = libusb_open_device_with_vid_pid(0,0x40a,0x6013);
	if (!pod.m_handle)
	{
		Log("not found...\n",0,0);
		return (false);
	}
	libusb_claim_interface(pod.m_handle,0);
	if (!blHtml)
	{
		Log("found...\n",0,0);
	}
	pod.m_blHtml = blHtml;
	return (true);
}

// I have no idea what this is doing, getting meter data?
void CScanner::GetCalibrationFormat()
{
	int iXfer;
	unsigned char bSts;
	unsigned char abCmd[16];
	unsigned char ab32[32];
	Log("\n",0,0);

	// Send command...
	iXfer = 0;
	memcpy(abCmd,"\x28\x00\x60\x00\x0a\x0d\x00\x00\x20\x00",10);
	Log("read32.cmd: ",abCmd,10);
	Out(abCmd,10,&iXfer);

	// Read data...
	memset(ab32,0,sizeof(ab32));
	In(ab32,sizeof(ab32),&iXfer);
	Log("read32.inp: ",ab32,iXfer);

	// Read status...
	In(&bSts,1,&iXfer);
	Log("read32.srb: ",&bSts,1);

	// Handle sense...
	if (bSts == 0x02)
	{
		RequestSense();
	}
}

// Read an sheet from the scanner...
void CScanner::ReadImage()
{
	int iXfer;
	int iTotal;
	int iStripSize;
	unsigned char bSts;
	unsigned char abCmd[16];
	unsigned char *abImage;
	ScsiWindow scsiwindow;

	// Init stuff and get some memory...
	iStripSize = pod.m_iStripeSize;
	abImage = (unsigned char*)malloc(iStripSize);
	if (!abImage)
	{
		Log("readimage.error: malloc",0,0);
		return;
	}

	// Create the file...
	pod.m_pimage->Create("image.tif");

	// Write the header, so we get the proper offset...
	pod.m_pimage->WriteHeader(&pod.m_scsiwindow,0,0);

	// We can setup the read command once and reuse it...
	memcpy(abCmd,"\x28\x00\x00\x00\x0a\x0d\x03\xf4\xe0\x00",10);
	iTotal = UX(iStripSize);
	memcpy(&abCmd[6],&((unsigned char*)&iTotal)[1],3);

	// Loop through the image data...
	bSts = 0;
	iTotal = 0;
	while (bSts == 0)
	{ 
		Log("\n",0,0);

		// Send command...
		Log("readimage.cmd: ",abCmd,10);
		Out(abCmd,10,&iXfer);

		// Read data...
		memset(abImage,0,sizeof(abImage));
		In(abImage,iStripSize,&iXfer);
		Log("readimage.inp: ",abImage,iXfer);

		// Write the data...
		if (iXfer)
		{
			iTotal += iXfer;
			pod.m_pimage->WriteImage(abImage,iXfer);
		}

		// Read status...
		In(&bSts,1,&iXfer);
		Log("readimage.srb: ",&bSts,1);

		// We've lost the connection, so bail...
		if (iXfer == 0)
		{
			break;
		}
	}

	// Rewrite the header, so we get the proper meta-data...
	memcpy(&scsiwindow,&pod.m_scsiwindow,sizeof(scsiwindow));
	pod.m_pimage->WriteHeader(&scsiwindow,iTotal,pod.m_szMeta);
	printf("%s",pod.m_szMeta);
	fflush(stdout);

	// Close the file...
	pod.m_pimage->Close();

	// Handle sense...
	if (bSts == 0x02)
	{
		RequestSense();
	}

	// Cleanup...
	free(abImage);
	abImage = 0;
}

// Release after a sheet...
void CScanner::ReleaseUnit()
{
    int iXfer;
    unsigned char bSts;
    unsigned char abCmd[16];
	Log("\n",0,0);

    // Send command...
    memcpy(abCmd,"\x17\x00\x00\x00\x00\x00\x00\x00\x00\x00",10);
    Log("releaseunit.cmd: ",abCmd,10);
    Out(abCmd,10,&iXfer);

    // Read status...
    In(&bSts,1,&iXfer);
	Log("releaseunit.srb: ",&bSts,1);

	// Handle sense...
	if (bSts == 0x02)
	{
		RequestSense();
	}
}

// Get more data on an error return...
void CScanner::RequestSense()
{
    int iXfer;
    unsigned char bSts;
    unsigned char abCmd[16];
    ScsiSense scsisense;
	Log("\n",0,0);

    // Send command...
    memcpy(abCmd,"\x03\x00\x00\x00\x16\x00\x00\x00\x00\x00",10);
	Log("requestsense.cmd: ",abCmd,10);
    Out(abCmd,10,&iXfer);

    // Read data...
    memset(&scsisense,0,sizeof(scsisense));
    In((unsigned char*)&scsisense,sizeof(scsisense),&iXfer);
	Log("requestsense.inp: ",&scsisense,iXfer);

    // Read status...
    In(&bSts,1,&iXfer);
	Log("requestsense.srb: ",&bSts,1);
}

// Acquire the scanner...
void CScanner::ReserveUnit()
{
    int iXfer;
    unsigned char bSts;
    unsigned char abCmd[16];
	Log("\n",0,0);

    // Send command...
    memcpy(abCmd,"\x16\x00\x00\x00\x00\x00\x00\x00\x00\x00",10);
    Log("reserveunit.cmd: ",abCmd,10);
    Out(abCmd,10,&iXfer);

    // Read status...
    In(&bSts,1,&iXfer);
	Log("reserveunit.srb: ",&bSts,1);

	// Handle sense...
	if (bSts == 0x02)
	{
		RequestSense();
	}
}

// Start scanning...
void CScanner::Scan()
{
    int iXfer;
    unsigned char bSts;
    unsigned char abCmd[16];
	Log("\n",0,0);

    // Send command...
    memcpy(abCmd,"\x1b\x00\x00\x00\x01\x00\x00\x00\x00\x00",10);
    Log("scan.cmd: ",abCmd,10);
    Out(abCmd,10,&iXfer);

    // Read status...
    In(&bSts,1,&iXfer);
	Log("scan.srb: ",&bSts,1);

	// Handle sense...
	if (bSts == 0x02)
	{
		RequestSense();
	}
}

// Set up color...
void CScanner::Send3x3ColorMatrix()
{
    int iXfer;
    unsigned char bSts;
    unsigned char abCmd[16];
    unsigned char ab3x3ColorMatrix[18];
	Log("\n",0,0);

    // Send command...
    memcpy(abCmd,"\x2a\x00\x83\x00\x00\x00\x00\x00\x12\x00",10);
    Log("send3x3colormatrix.cmd: ",abCmd,10);
    Out(abCmd,10,&iXfer);

    // Send data...
	memcpy(ab3x3ColorMatrix,"\x04\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x04\x00",18);
    Log("send3x3colormatrix.out: ",ab3x3ColorMatrix,sizeof(ab3x3ColorMatrix));
    Out(ab3x3ColorMatrix,sizeof(ab3x3ColorMatrix),&iXfer);

    // Read status...
    In(&bSts,1,&iXfer);
    Log("send3x3colormatrix.srb: %d\n",&bSts,1);

	// Handle sense...
	if (bSts == 0x02)
	{
		RequestSense();
	}
}

// Setup gamma...
void CScanner::SendGammaTables()
{
	int ii;
    int iXfer;
    unsigned char bSts;
    unsigned char abCmd[16];
    unsigned char abGammaTable[512];

	// Three of them...
	for (ii = 0; ii < 3; ii++)
	{
		Log("\n",0,0);

        // Send command...
        memcpy(abCmd,"\x2a\x00\x81\x00\x00\x00\x00\x02\x00\x00",10);
		abCmd[5] = ii;
        Log("sendgammatable.cmd: ",abCmd,10);
        Out(abCmd,10,&iXfer);

       	// Send data...
		memcpy
		(
			abGammaTable,
			"\x00\x00\x00\x01\x00\x02\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07"
			"\x00\x08\x00\x09\x00\x0a\x00\x0b\x00\x0c\x00\x0d\x00\x0e\x00\x0f"
			"\x00\x10\x00\x11\x00\x12\x00\x13\x00\x14\x00\x15\x00\x16\x00\x17"
			"\x00\x18\x00\x19\x00\x1a\x00\x1b\x00\x1c\x00\x1d\x00\x1e\x00\x1f"
			"\x00\x20\x00\x21\x00\x22\x00\x23\x00\x24\x00\x25\x00\x26\x00\x27"
			"\x00\x28\x00\x29\x00\x2a\x00\x2b\x00\x2c\x00\x2d\x00\x2e\x00\x2f"
			"\x00\x30\x00\x31\x00\x32\x00\x33\x00\x34\x00\x35\x00\x36\x00\x37"
			"\x00\x38\x00\x39\x00\x3a\x00\x3b\x00\x3c\x00\x3d\x00\x3e\x00\x3f"
			"\x00\x40\x00\x41\x00\x42\x00\x43\x00\x44\x00\x45\x00\x46\x00\x47"
			"\x00\x48\x00\x49\x00\x4a\x00\x4b\x00\x4c\x00\x4d\x00\x4e\x00\x4f"
			"\x00\x50\x00\x51\x00\x52\x00\x53\x00\x54\x00\x55\x00\x56\x00\x57"
			"\x00\x58\x00\x59\x00\x5a\x00\x5b\x00\x5c\x00\x5d\x00\x5e\x00\x5f"
			"\x00\x60\x00\x61\x00\x62\x00\x63\x00\x64\x00\x65\x00\x66\x00\x67"
			"\x00\x68\x00\x69\x00\x6a\x00\x6b\x00\x6c\x00\x6d\x00\x6e\x00\x6f"
			"\x00\x70\x00\x71\x00\x72\x00\x73\x00\x74\x00\x75\x00\x76\x00\x77"
			"\x00\x78\x00\x79\x00\x7a\x00\x7b\x00\x7c\x00\x7d\x00\x7e\x00\x7f"
			"\x00\x80\x00\x81\x00\x82\x00\x83\x00\x84\x00\x85\x00\x86\x00\x87"
			"\x00\x88\x00\x89\x00\x8a\x00\x8b\x00\x8c\x00\x8d\x00\x8e\x00\x8f"
			"\x00\x90\x00\x91\x00\x92\x00\x93\x00\x94\x00\x95\x00\x96\x00\x97"
			"\x00\x98\x00\x99\x00\x9a\x00\x9b\x00\x9c\x00\x9d\x00\x9e\x00\x9f"
			"\x00\xa0\x00\xa1\x00\xa2\x00\xa3\x00\xa4\x00\xa5\x00\xa6\x00\xa7"
			"\x00\xa8\x00\xa9\x00\xaa\x00\xab\x00\xac\x00\xad\x00\xae\x00\xaf"
			"\x00\xb0\x00\xb1\x00\xb2\x00\xb3\x00\xb4\x00\xb5\x00\xb6\x00\xb7"
			"\x00\xb8\x00\xb9\x00\xba\x00\xbb\x00\xbc\x00\xbd\x00\xbe\x00\xbf"
			"\x00\xc0\x00\xc1\x00\xc2\x00\xc3\x00\xc4\x00\xc5\x00\xc6\x00\xc7"
			"\x00\xc8\x00\xc9\x00\xca\x00\xcb\x00\xcc\x00\xcd\x00\xce\x00\xcf"
			"\x00\xd0\x00\xd1\x00\xd2\x00\xd3\x00\xd4\x00\xd5\x00\xd6\x00\xd7"
			"\x00\xd8\x00\xd9\x00\xda\x00\xdb\x00\xdc\x00\xdd\x00\xde\x00\xdf"
			"\x00\xe0\x00\xe1\x00\xe2\x00\xe3\x00\xe4\x00\xe5\x00\xe6\x00\xe7"
			"\x00\xe8\x00\xe9\x00\xea\x00\xeb\x00\xec\x00\xed\x00\xee\x00\xef"
			"\x00\xf0\x00\xf1\x00\xf2\x00\xf3\x00\xf4\x00\xf5\x00\xf6\x00\xf7"
			"\x00\xf8\x00\xf9\x00\xfa\x00\xfb\x00\xfc\x00\xfd\x00\xfe\x00\xff",
			sizeof(abGammaTable)
		);
        Log("sendgammatablecmd.out: ",abGammaTable,sizeof(abGammaTable));
        Out(abGammaTable,sizeof(abGammaTable),&iXfer);

        // Read status...
        In(&bSts,1,&iXfer);
        Log("sendgammatable.srb: ",&bSts,1);

		// Handle sense...
		if (bSts == 0x02)
		{
			RequestSense();
		}
	}
}

// Set most things of interest...
void CScanner::SetWindow(EPIXELTYPE epixeltype, int iResolution, bool blDuplex)
{
	int iXfer;
	ushort u16Tmp;
	unsigned char bSts;
	unsigned char abCmd[16];
	ScsiWindow scsiwindow;
	Log("\n",0,0);

	// Send command...
	memcpy(abCmd,"\x24\x00\x00\x00\x00\x00\x00\x00\x00\x00",10);
	abCmd[8] = sizeof(scsiwindow);
	Log("setwindow.cmd: ",abCmd,10);
	Out(abCmd,10,&iXfer);

	// Pixeltype...
	switch (epixeltype)
	{
		default:
		case PIXELTYPE_BW:
			memcpy
			(
				&scsiwindow,
				"\x00\x00\x00\x00\x00\x00\x00\x3f\x00\x00"
				"\x00\xc8\x00\xc8\x00\x00\x00\x00\x00\x00"
				"\x00\x00\x00\x00\x29\x10\x00\x00\x9f\x60"
				"\x00\x00\x00\x02\x08\x00\x7f\x03\x00\x00"
				"\x00\x00\x00\x00\x00\x00\x00\x00\xff\x15"
				"\xf0\xff\x00\x06\xd8\x1a\x90\x00\x00\x00"
				"\x00\x00\x00\x00\x00\x80\x00\x00\x00\x14"
				"\x00",
				sizeof(scsiwindow)
			);

			// Width in bytes...
			u16Tmp = (ushort)(8.76 * iResolution);
			u16Tmp = WX(u16Tmp);
			memcpy(&scsiwindow.LineWidth,&u16Tmp,sizeof(u16Tmp));
			break;

		case PIXELTYPE_GRAY:
			memcpy
			(
				&scsiwindow,
				"\x00\x00\x00\x00\x00\x00\x00\x3f\x00\x00"
				"\x00\xc8\x00\xc8\x00\x00\x00\x00\x00\x00"
				"\x00\x00\x00\x00\x29\x10\x00\x00\x9f\x60"
				"\x00\x00\x00\x02\x08\x00\x7f\x03\x00\x00"
				"\x00\x00\x00\x00\x00\x00\x00\x00\xff\x15"
				"\xf0\xff\x00\x06\xd8\x1a\x90\x00\x00\x00"
				"\x00\x00\x00\x00\x00\x80\x00\x00\x00\x14"
				"\x01",
				sizeof(scsiwindow)
			);
			pod.m_iStripeSize = 259296;

			// Width in bytes...
			u16Tmp = (ushort)((double)8.76 * (double)iResolution);
			pod.m_iStripeSize = ((262144 / u16Tmp) & 0xFFFFFFFE) * u16Tmp;
			u16Tmp = WX(u16Tmp);
			memcpy(&scsiwindow.LineWidth,&u16Tmp,sizeof(u16Tmp));
			break;

		case PIXELTYPE_COLOR:
			memcpy
			(
				&scsiwindow,
				"\x00\x00\x00\x00\x00\x00\x00\x3f\x00\x00"
				"\x00\xc8\x00\xc8\x00\x00\x00\x00\x00\x00"
				"\x00\x00\x00\x00\x29\x10\x00\x00\x9f\x60"
				"\x00\x00\x00\x05\x08\x00\x7f\x03\x00\x00"
				"\x00\x00\x00\x00\x00\x00\x00\x00\xff\x15"
				"\xe0\xff\x00\x14\x88\x1a\x90\x00\x00\x00"
				"\x00\x00\x00\x00\x00\x80\x00\x00\x00\x14"
				"\x01",
				sizeof(scsiwindow)
			);
			pod.m_iStripeSize = 515088;

			// Width in bytes...
			u16Tmp = (ushort)((8.76 * iResolution) * 3);
			//pod.m_iStripeSize = ((524288 / u16Tmp) & 0xFFFFFFFE) * u16Tmp;
			u16Tmp = WX(u16Tmp);
			memcpy(&scsiwindow.LineWidth,&u16Tmp,sizeof(u16Tmp));
			break;
	}

	// Set duplex/simplex...
	scsiwindow.ADFMode = blDuplex?0x9:0x8;

	// Set resolution...
	u16Tmp = (ushort)iResolution;
	u16Tmp = WX(u16Tmp);
	memcpy(&scsiwindow.X_resolution,&u16Tmp,sizeof(u16Tmp));
	memcpy(&scsiwindow.Y_resolution,&u16Tmp,sizeof(u16Tmp));

	// Set cropping...
	//scsiwindow.UpperLeft_X[4];			// 14-17:	Upper Left X
	//scsiwindow.UpperLeft_Y[4];			// 18-21:	Upper Left Y
	//scsiwindow.Width;						// 22-25:	Width
	//scsiwindow.Length;					// 26-29:	Length
	//scsiwindow.PaperLength[2];			// 42-43:	Page Length Detection

	// Height...
	u16Tmp = (ushort)(34 * iResolution);
	u16Tmp = WX(u16Tmp);
	memcpy(&scsiwindow.LineCount,&u16Tmp,sizeof(u16Tmp));

	// Write data...
	Log("setwindow.out: ",&scsiwindow,sizeof(scsiwindow));
	Out((unsigned char*)&scsiwindow,sizeof(scsiwindow),&iXfer);
	memcpy(&pod.m_scsiwindow,&scsiwindow,sizeof(scsiwindow));

	// Read status...
	In(&bSts,1,&iXfer);
	Log("setwindow.srb: ",&bSts,1);
}

// is the scanner good to go?
void CScanner::TestUnitReady()
{
    int iXfer;
    unsigned char bSts;
    unsigned char abCmd[16];

	// Loopy...
	for (;;)
	{
		Log("\n",0,0);

		// Send command...
		memcpy(abCmd,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",10);
		Log("testunitready.cmd: ",abCmd,10);
		Out(abCmd,10,&iXfer);

		// Read status...
		In(&bSts,1,&iXfer);
		Log("testunitready.srb: ",&bSts,1);

		// We're good...
		if (bSts == 0x00)
		{
			break;
		}

		// Handle sense...
		if (bSts == 0x02)
		{
			RequestSense();
		}
	}
}

// Read data from the scanner...
void CScanner::In(unsigned char *buffer, int size, int *xfer)
{
	int iSts;
	*xfer = 0;
	memset(buffer,0,size);
	mlock(pod.m_pbBuffer,size);
	iSts = libusb_bulk_transfer(pod.m_handle,0x83,pod.m_pbBuffer,size,xfer,0);
	munlock(pod.m_pbBuffer,size);
	if (*xfer)
	{
		if ((*xfer < 0) || (*xfer > size))
		{
			*xfer = 1;
		}
		memcpy(buffer,pod.m_pbBuffer,*xfer);
	}
}

// Write data to the scanner...
void CScanner::Out(unsigned char *buffer, int size, int *xfer)
{
	int iSts;
	*xfer = 0;
	memcpy(pod.m_pbBuffer,buffer,size);
	mlock(pod.m_pbBuffer,size);
	iSts = libusb_bulk_transfer(pod.m_handle,0x01,pod.m_pbBuffer,size,xfer,0);
	munlock(pod.m_pbBuffer,size);
}

// Log a scanner transaction...
void CScanner::Log(const char *title, const void *data, int len)
{
	int ii;
	const int limit = 100;

	// Skip it...
	if (pod.m_blHtml)
	{
		return;
	}

	// Print text...
	if (!data)
	{
		printf("%s",title);
		fflush(stdout);
		return;
	}

	// Display it...
	printf("%s",title);
	if (data)
	{
		for (ii = 0; (len >= 0) && (ii < len) && (ii < limit); ii++)
		{
			printf("%02x ",((unsigned char*)data)[ii]);
		}
		if (len >= limit)
		{
			printf("...");
		}
	}
	printf("(%d)\n",len);
	fflush(stdout);
}

// Main control...
int main(int argc, char *argv[])
{
	int ii;
	CScanner scanner;
	EPIXELTYPE epixeltype = PIXELTYPE_GRAY;
	int iResolution = 200;
	bool blDuplex = false;
	bool blHtml = false;

	// Parse for arguments...
	for (ii = 1; ii < argc; ii++)
	{
		if (!strncasecmp(argv[ii],"pixeltype=",10))
		{
			if (!strcasecmp(&argv[ii][10],"bw"))
			{
				epixeltype = PIXELTYPE_BW;
			}
			else if (!strcasecmp(&argv[ii][10],"gray"))
			{
				epixeltype = PIXELTYPE_GRAY;
			}
			else if (!strcasecmp(&argv[ii][10],"color"))
			{
				epixeltype = PIXELTYPE_COLOR;
			}
			else
			{
				printf("Unrecognized pixeltype...\n");
				return (1);
			}
		}
		else if (!strncasecmp(argv[ii],"resolution=",11))
		{
			iResolution = atoi(&argv[ii][11]);
			switch (iResolution)
			{
				default:
					printf("Unrecognized resolution...\n");
					return (1);
				case 75:
				case 100:
				case 150:
				case 200:
				case 300:
				case 600:
					// No action, we're good...
					break;
			}
		}
		else if (!strncasecmp(argv[ii],"duplex=",7))
		{
			if (!strcasecmp(&argv[ii][7],"true"))
			{
				blDuplex = true;
			}
			else if (!strcasecmp(&argv[ii][7],"false"))
			{
				blDuplex = false;
			}
			else
			{
				printf("Unrecognized duplex...\n");
				return (1);
			}
		}
		else if (!strncasecmp(argv[ii],"html",4))
		{
			blHtml = true;
		}
	}
	
	// Scan...
	if (!scanner.Open(blHtml)) return(1);
	scanner.Inquiry();
	scanner.LampCheck();
	scanner.MediaCheck();
	scanner.TestUnitReady();
	scanner.SetWindow(epixeltype,iResolution,blDuplex);
	scanner.GetCalibrationFormat();
	scanner.Send3x3ColorMatrix();
	scanner.SendGammaTables();
	scanner.MediaCheck();
	scanner.ReserveUnit();
	scanner.Scan();
	scanner.ReadImage();
	scanner.ReleaseUnit();
	scanner.Close();

	// All done...
	return(0);
}

