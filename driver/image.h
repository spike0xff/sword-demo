#ifndef IMAGE
#define IMAGE

#include "def.h"

class CImage
{
	public:
		CImage();
		~CImage();
		void Close();
		void Create(const char *name);
		void WriteHeader(ScsiWindow *pscsiwindow, int size);
		void WriteImage(const unsigned char *data, const int len);

	private:
		struct Pod
		{
			FILE *m_pfile;
		} pod;
};

#endif // IMAGE

