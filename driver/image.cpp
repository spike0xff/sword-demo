#include "image.h"

// TIFF Bitonal Header Template...
static const TiffBitonal s_tiffbitonal =
{
	{'I', 'I'}, 42, 8, 16,									// 1+1+2+4+2	10		 10
										{
	/*  0 NewSubFileType*/				{254, 4, 1,   0},	// 2+2+4+4		12		 22
	/*  1 SubFileType*/					{255, 3, 1,   1},	// 2+2+4+4		12		 34
	/*  2 ImageWidth*/					{256, 4, 1,   0},	// 2+2+4+4		12		 46
	/*  3 ImageLength*/					{257, 4, 1,   0},	// 2+2+4+4		12		 58
	/*  4 BitsPerSample*/				{258, 3, 1,   1},	// 2+2+4+4		12		 70
	/*  5 Compression*/					{259, 3, 1,   0},	// 2+2+4+4		12		 82
	/*  6 PhotometricInterpretation*/	{262, 3, 1,   1},	// 2+2+4+4		12		 94
	/*  7 FillOrder*/					{266, 3, 1,   1},	// 2+2+4+4		12		106
	/*  8 StripOffsets*/				{273, 4, 1, 222},	// 2+2+4+4		12		118
	/*  9 SamplesPerPixel*/				{277, 3, 1,   1},	// 2+2+4+4		12		130
	/* 10 RowsPerStrip*/				{278, 4, 1,   0},	// 2+2+4+4		12		142
	/* 11 StripByteCounts*/				{279, 4, 1,   0},	// 2+2+4+4		12		154
	/* 12 XResolution*/					{282, 5, 1, 206},	// 2+2+4+4		12		166
	/* 13 YResolution*/					{283, 5, 1, 214},	// 2+2+4+4		12		178
	/* 14 T4/6Options*/					{292, 4, 1,   0},	// 2+2+4+4		12		190
	/* 15 ResolutionUnit*/				{296, 3, 1,   2},	// 2+2+4+4		12		202
										},
	0,									// Closing NULL		// 4			4		206
	{0, 1},								// Place for Xres	// 4+4			8		214				
	{0, 1},								// Place for Yres	// 4+4			8		222
};

// TIFF Grayscale Header Template...
static const TiffGray s_tiffgray =
{
	{'I', 'I'}, 42, 8, 14,									// 1+1+2+4+2	10		 10
										{
	/*  0 NewSubFileType*/				{254, 4, 1,   0},	// 2+2+4+4		12		 22
	/*  1 SubFileType*/					{255, 3, 1,   1},	// 2+2+4+4		12		 34
	/*  2 ImageWidth*/					{256, 4, 1,   0},	// 2+2+4+4		12		 46
	/*  3 ImageLength*/					{257, 4, 1,   0},	// 2+2+4+4		12		 58
	/*  4 BitsPerSample*/				{258, 3, 1,   8},	// 2+2+4+4		12		 70
	/*  5 Compression*/					{259, 3, 1,   0},	// 2+2+4+4		12		 82
	/*  6 PhotometricInterpretation*/	{262, 3, 1,   1},	// 2+2+4+4		12		 94
	/*  7 StripOffsets*/				{273, 4, 1, 198},	// 2+2+4+4		12		106
	/*  8 SamplesPerPixel*/				{277, 3, 1,   1},	// 2+2+4+4		12		118
	/*  9 RowsPerStrip*/				{278, 4, 1,   0},	// 2+2+4+4		12		130
	/* 10 StripByteCounts*/				{279, 4, 1,   0},	// 2+2+4+4		12		142
	/* 11 XResolution*/					{282, 5, 1, 182},	// 2+2+4+4		12		154
	/* 12 YResolution*/					{283, 5, 1, 190},	// 2+2+4+4		12		166
	/* 13 ResolutionUnit*/				{296, 3, 1,   2},	// 2+2+4+4		12		178
										},
	0,									// Closing NULL		// 4			4		182
	{0, 1},								// Place for Xres	// 4+4			8		190
	{0, 1},								// Place for Yres	// 4+4			8		198
};

// TIFF Color Uncompressed Header Template...
static const TiffColor s_tiffcolor =
{
	{'I', 'I'}, 42, 8, 14,									// 1+1+2+4+2	10		 10
										{
	/*  0 NewSubFileType*/				{254, 4, 1,   0},	// 2+2+4+4		12		 22
	/*  1 SubFileType*/					{255, 3, 1,   1},	// 2+2+4+4		12		 34
	/*  2 ImageWidth*/					{256, 4, 1,   0},	// 2+2+4+4		12		 46
	/*  3 ImageLength*/					{257, 4, 1,   0},	// 2+2+4+4		12		 58
	/*  4 BitsPerSample*/				{258, 3, 3, 182},	// 2+2+4+4		12		 70
	/*  5 Compression*/					{259, 3, 1,   0},	// 2+2+4+4		12		 82
	/*  6 PhotometricInterpretation*/	{262, 3, 1,   2},	// 2+2+4+4		12		 94
	/*  7 StripOffsets*/				{273, 4, 1, 204},	// 2+2+4+4		12		106
	/*  8 SamplesPerPixel*/				{277, 3, 1,   3},	// 2+2+4+4		12		118
	/*  9 RowsPerStrip*/				{278, 4, 1,   0},	// 2+2+4+4		12		130
	/* 10 StripByteCounts*/				{279, 4, 1,   0},	// 2+2+4+4		12		142
	/* 11 XResolution*/					{282, 5, 1, 188},	// 2+2+4+4		12		154
	/* 12 YResolution*/					{283, 5, 1, 196},	// 2+2+4+4		12		166
	/* 13 ResolutionUnit*/				{296, 3, 1,   2},	// 2+2+4+4		12		178
										},
	0,									// Closing NULL		// 4			4		182
	{8, 8, 8},							// Bits per sample	// 2+2+2		6		188
	{0, 1},								// Place for Xres	// 4+4			8		196
	{0, 1},								// Place for Yres	// 4+4			8		204
};

CImage::CImage()
{
	memset(&pod,0,sizeof(pod));
}

CImage::~CImage()
{
	Close();
}

void CImage::Close()
{
	if (pod.m_pfile)
	{
		fclose(pod.m_pfile);
		pod.m_pfile = 0;
	}
}

void CImage::Create(const char *name)
{
	pod.m_pfile = fopen(name,"w");
}

void CImage::WriteHeader(ScsiWindow *pscsiwindow, int size, char *szMeta)
{
	TiffBitonal tiffbitonal;
	TiffGray tiffgray;
	TiffColor tiffcolor;

	if (pod.m_pfile)
	{
		fseek(pod.m_pfile,0,SEEK_SET);
		switch (pscsiwindow->ImageComposition)
		{
			default:
			case 1:
				memcpy(&tiffbitonal,&s_tiffbitonal,sizeof(tiffbitonal));
				tiffbitonal.tifftag[2].u32Value   = UX(pscsiwindow->Width) / (1200 / WX(pscsiwindow->X_resolution));
				tiffbitonal.tifftag[3].u32Value   = size / ((tiffbitonal.tifftag[2].u32Value + 7) / 8);
				tiffbitonal.tifftag[5].u32Value   = 1;
				tiffbitonal.tifftag[6].u32Value   = 1;
				tiffbitonal.tifftag[10].u32Value  = size / ((tiffbitonal.tifftag[2].u32Value + 7) / 8);
				tiffbitonal.tifftag[11].u32Value  = size;
				tiffbitonal.tifftag[14].u32Value  = 292;
				tiffbitonal.tifftag[14].u32Value  = 0;
				tiffbitonal.u32XResolution[0]     = WX(pscsiwindow->X_resolution);
				tiffbitonal.u32YResolution[0]     = WX(pscsiwindow->Y_resolution);
				fwrite(&tiffbitonal,1,sizeof(tiffbitonal),pod.m_pfile);
				if (szMeta)
				{
					sprintf
					(
						szMeta,
						"<sword>\n"
						"\t<pixel>bw</pixel>\n"
						"\t<resolution>%d</resolution>\n"
						"\t<size>%d</size>\n"
						"\t<width>%d</width>\n"
						"\t<height>%d</height>\n"
						"\t<bytesperrow>%d</bytesperrow>\n"
						"</sword>\n",
						WX(pscsiwindow->X_resolution),
						sizeof(tiffbitonal) + size,
						UX(tiffbitonal.tifftag[2].u32Value),
						UX(tiffbitonal.tifftag[3].u32Value),
						tiffbitonal.tifftag[10].u32Value
					);
				}
				break;
			case 2:
				memcpy(&tiffgray,&s_tiffgray,sizeof(tiffgray));
				tiffgray.tifftag[2].u32Value   = UX(pscsiwindow->Width) / (1200 / WX(pscsiwindow->X_resolution));
				tiffgray.tifftag[3].u32Value   = size / tiffgray.tifftag[2].u32Value;
				tiffgray.tifftag[5].u32Value   = 1;
				tiffgray.tifftag[9].u32Value   = size / tiffgray.tifftag[2].u32Value;
				tiffgray.tifftag[10].u32Value  = size;
				tiffgray.u32XResolution[0]     = WX(pscsiwindow->X_resolution);
				tiffgray.u32YResolution[0]     = WX(pscsiwindow->Y_resolution);
				fwrite(&tiffgray,1,sizeof(tiffgray),pod.m_pfile);
				if (szMeta)
				{
					sprintf
					(
						szMeta,
						"<sword>\n"
						"\t<pixel>gray</pixel>\n"
						"\t<resolution>%d</resolution>\n"
						"\t<size>%d</size>\n"
						"\t<width>%d</width>\n"
						"\t<height>%d</height>\n"
						"\t<bytesperrow>%d</bytesperrow>\n"
						"</sword>\n",
						WX(pscsiwindow->X_resolution),
						sizeof(tiffgray) + size,
						tiffgray.tifftag[2].u32Value,
						tiffgray.tifftag[3].u32Value,
						tiffgray.tifftag[9].u32Value
					);
				}
				break;
			case 5:
				memcpy(&tiffcolor,&s_tiffcolor,sizeof(tiffcolor));
				tiffcolor.tifftag[2].u32Value   = UX(pscsiwindow->Width) / (1200 / WX(pscsiwindow->X_resolution));
				tiffcolor.tifftag[3].u32Value   = size / (tiffcolor.tifftag[2].u32Value * 3);
				tiffcolor.tifftag[5].u32Value   = 1;
				tiffcolor.tifftag[9].u32Value   = size / (tiffcolor.tifftag[2].u32Value * 3);
				tiffcolor.tifftag[10].u32Value  = size;
				tiffcolor.u32XResolution[0]     = WX(pscsiwindow->X_resolution);
				tiffcolor.u32YResolution[0]     = WX(pscsiwindow->Y_resolution);
				fwrite(&tiffcolor,1,sizeof(tiffcolor),pod.m_pfile);
				if (szMeta)
				{
					sprintf
					(
						szMeta,
						"<sword>\n"
						"\t<pixel>color</pixel>\n"
						"\t<resolution>%d</resolution>\n"
						"\t<size>%d</size>\n"
						"\t<width>%d</width>\n"
						"\t<height>%d</height>\n"
						"\t<bytesperrow>%d</bytesperrow>\n"
						"</sword>\n",
						WX(pscsiwindow->X_resolution),
						sizeof(tiffcolor) + size,
						UX(tiffcolor.tifftag[2].u32Value),
						UX(tiffcolor.tifftag[3].u32Value),
						tiffcolor.tifftag[9].u32Value
					);
				}
				break;
		}
	}
}

void CImage::WriteImage(const unsigned char *data, const int len)
{
	if (pod.m_pfile)
	{
		fseek(pod.m_pfile,0,SEEK_END);
		fwrite(data,1,len,pod.m_pfile);
	}
}

