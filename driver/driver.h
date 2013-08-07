#ifndef DRIVER
#define DRIVER

#include "image.h"

class CScanner
{
	public:
		CScanner();
		~CScanner();
		void Close();
		void Inquiry();
		void LampCheck();
		void MediaCheck();
		bool Open(bool blHtml);
		void GetCalibrationFormat();
		void ReadImage();
		void ReleaseUnit();
		void RequestSense();
		void ReserveUnit();
		void Scan();
		void Send3x3ColorMatrix();
		void SendGammaTables();
		void SetWindow(EPIXELTYPE epixeltype, int iResolution, bool blDuplex);
		void TestUnitReady();

	private:
		void In(unsigned char *buffer, int size, int *xfer);
		void Out(unsigned char *buffer, int size, int *xfer);
		void Log(const char *title, const void *data, int len);

	private:
		struct Pod
		{
			bool					m_blLibUsbExit;
			libusb_device_handle	*m_handle;
			unsigned char			*m_pbBuffer;
			CImage					*m_pimage;
			ScsiWindow				m_scsiwindow;
			char					m_szPath[512];
			int						m_iImage;
			int						m_iStripeSize;
			bool					m_blHtml;
			char					m_szMeta[65536];
		} pod;
};

#endif // DRIVER

