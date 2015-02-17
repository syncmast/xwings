//---------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <iconv.h>
#include <wchar.h>
#include "vxutil.h"
//---------------------------------------------------------------------------
int byteorder()
{
    union
    {
        short s;
        char c[sizeof(short)];
    } un;
    un.s = 0x0102;

    if(sizeof(short) != 2)
        return BO_UNKNOWN;

    if(un.c[0] == 1 && un.c[1] == 2)
        return BO_BIG_ENDIAN;
    else if(un.c[0] == 2 && un.c[1] == 1)
        return BO_LITTLE_ENDIAN;
    else
        return BO_UNKNOWN;
}
//---------------------------------------------------------------------------
char *getexepath(char *buf)
{
    sprintf(buf,"/proc/%d/cmdline",getpid());
    FILE *f = fopen(buf,"r");
    fgets(buf,PATH_MAX,f);
    fclose(f);
    return buf;
}
//---------------------------------------------------------------------------
int convert_charset(const char *from, const char *to, char *str, size_t n, char *outbuf)
{
    size_t insize, outsize, bufsize, r;
    iconv_t cd;
    char *buf, *holder;

    cd = iconv_open(to, from);
    if (cd == (iconv_t) -1)
        return 0;
    if(n == 0)
        insize = strlen(str);
    else
        insize = n;
    outsize = bufsize = insize * 4;
    if(outbuf == NULL)
    {
        buf = (char *) malloc(bufsize);
        holder = buf;
        r = iconv(cd, &str, &insize, &buf, &outsize);
        free(holder);
        if (r < 0)
        {
            iconv_close(cd);
            return 0;
        }
    }
    else
    {
        buf = outbuf;
        r = iconv(cd, &str, &insize, &buf, &outsize);
        if (r < 0 || insize != 0)
        {
            iconv_close(cd);
            return 0;
        }
    }
    insize = bufsize - outsize;
    if(outbuf)
    {
        outbuf[insize] = '\0';
    }

    iconv_close(cd);
    return insize;
}
//---------------------------------------------------------------------------
int mbs2wcs(wchar_t **dest, const char *src)
{
    mbstate_t ps;
    memset(&ps, 0, sizeof(ps));
    const char **s = &src;
    size_t  nchar = mbsrtowcs( NULL, s, 0, &ps ) + 1;
    *dest = new wchar_t [sizeof(wchar_t) * nchar];
    mbsrtowcs(*dest, s, nchar, &ps);
    return nchar;
}
//---------------------------------------------------------------------------
int wcs2mbs(char **dest, const wchar_t *ws)
{
    mbstate_t ps;
    memset(&ps, 0, sizeof(ps));
    size_t nchar = wcsrtombs(NULL, &ws, 0, &ps) + 1;
    if(nchar == (size_t) -1)
        return -1;
    *dest = new char[nchar];
    wcsrtombs(*dest, &ws, nchar, &ps);
    return nchar;
}
//---------------------------------------------------------------------------
void *memsetx(void *d, void *s, size_t nd, size_t n)
{
    char *bd;
    bd = (char *)d;
    for(register size_t i = 0; i < n; i += nd, bd += nd)
    {
        memcpy(bd, s, nd);
    };
    return d;
}
//---------------------------------------------------------------------------
int ipower(int bas, int pot)
{
    register int p = 1;
    while(pot > 0)
    {
        if (pot & 1)
        {
            p = p * bas ;
        }
        bas = bas * bas ;
        pot >>= 1;
    }
    return p;
}
//---------------------------------------------------------------------------
