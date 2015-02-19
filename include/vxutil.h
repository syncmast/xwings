//---------------------------------------------------------------------------
#ifndef vxutilH
#define vxutilH

#include <wctype.h>

#define BO_UNKNOWN			-1
#define BO_LITTLE_ENDIAN	 1
#define BO_BIG_ENDIAN		 2
//---------------------------------------------------------------------------
extern "C"
{
    int byteorder();
    char *getexepath(char *buf);
    int convert_charset(const char *from, const char *to, char *str, size_t n, char *outbuf);
    int ipower(int bas, int pot);
    void *memsetx(void *d, void *s, size_t nd, size_t n);
    int mbs2wcs(wchar_t **dest, const char *src);
    int wcs2mbs(char **dest, const wchar_t *ws);
}
//---------------------------------------------------------------------------
#endif
