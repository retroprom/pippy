/* 
   sscanf stub

   Jeffery D. Collins

*/

#include "libc_segments.h"

#ifndef NULL
/* #define NULL ((void *) 0) */
#define NULL 0
#endif
#ifndef EOF
#define EOF     (-1)
#endif


int sscanf(const char *, const char *, ...) SEG_LIBC;
int sscanf(const char *buf, const char *fmt, ... ){
     /* do nothing but return EOF (could alternatively return a 0 
	to indicate an early failure. */
     return EOF;
}

#define	SPC	01
#define	STP	02

#define	SHORT	0
#define	REGULAR	1
#define	LONG	2
#define	INT	0
#define	FLOAT	1

#define EOF	(-1)

#ifdef USEFLOAT
extern double atof();
#endif

typedef struct _iop {
    char *p;
    int r;
} XFILE;

#define GETC(f) (--(f)->r < 0 ? EOF : *(f)->p++)
#define UNGETC(c, f) (++(f)->r, *--(f)->p)

#define ISDIGIT(c)	(c >= '0' && c <= '9')

static char _sctab[256] = {
    0,0,0,0,0,0,0,0,
    0,SPC,SPC,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    SPC,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

static int _innum(int **ptr, int type, int len, int size, XFILE *iop, int *eofptr) SEG_LIBC;
static int _instr(char *ptr, int type, int len, XFILE *iop, int *eofptr) SEG_LIBC;
static char *_getccl(unsigned char *s) SEG_LIBC;
static int vscanf(XFILE *, char *, int **) SEG_LIBC;
int xsscanf(char *s, char *fmt, int args) SEG_LIBC;

#ifdef TEST
main()
{
    char s1[8], s2[8];
    int i, j;
    long l;

    sscanf("foo barx 1 2 a", "%s %[^x]x %d %ld %x", s1, s2, &i, &l, &j);
    printf("%s %s %d %ld %x\n", s1, s2, i, l, j);
    exit(0);
}
#endif

int
xsscanf(s, fmt, args)
char *s, *fmt;
int args;
{
    XFILE f;

    f.p = s;
    for (f.r = 0; *s++; f.r++)
	;
    return vscanf(&f, fmt, &args);
}

static int
vscanf(iop, fmt, argp)
XFILE *iop;
char *fmt;
int **argp;
{
    register int ch;
    int nmatch = 0, len, ch1;
    int **ptr, fileended = 0, size;

    while (1) {
	switch (ch = *fmt++) {
	case '\0': 
	    return (nmatch);
	case '%': 
	    if ((ch = *fmt++) == '%')
		goto def;
	    ptr = 0;
	    if (ch != '*')
		ptr = argp++;
	    else
		ch = *fmt++;
	    len = 0;
	    size = REGULAR;
	    while (ISDIGIT(ch)) {
		len = len*10 + ch - '0';
		ch = *fmt++;
	    }
	    if (len == 0)
		len = 30000;
	    if (ch == 'l') {
		size = LONG;
		ch = *fmt++;
	    } else if (ch == 'h') {
		size = SHORT;
		ch = *fmt++;
	    } else if (ch == '[')
		fmt = _getccl(fmt);
	    if (ch >= 'A' && ch <= 'Z') {
		ch |= 0x20;
		size = LONG;
	    }
	    if (ch == '\0')
		return(-1);
	    if (_innum(ptr, ch, len, size, iop, &fileended) && ptr)
		nmatch++;
	    if (fileended)
		return(nmatch ? nmatch : -1);
	    break;

	case ' ':
	case '\n':
	case '\t': 
	    while ((ch1 = GETC(iop)) == ' ' || ch1 == '\t' || ch1 == '\n')
		;
	    if (ch1 != EOF)
		UNGETC(ch1, iop);
	    break;

	default: 
	def:
	    ch1 = GETC(iop);
	    if (ch1 != ch) {
		if (ch1 == EOF)
		    return -1;
		UNGETC(ch1, iop);
		return(nmatch);
	    }
	}
    }
}

static int
_innum(int **ptr, int type, int len, int size, XFILE *iop, int *eofptr)
{
    register char *np;
    char numbuf[64];
    register c, base;
    int expseen = 0, scale = INT, negflg = 0, c1, ndigit = 0;
    long lcval = 0;

    if (type == 'c' || type == 's' || type == '[')
	return(_instr(ptr ? *(char **)ptr : NULL, type, len, iop, eofptr));
#ifdef USEFLOAT
    if (type == 'e' || type == 'f')
	scale = FLOAT;
#endif
    if (type == 'o')
	base = 8;
    else if (type == 'x')
	base = 16;
    else
	base = 10;
    np = numbuf;
    while ((c = GETC(iop)) == ' ' || c == '\t' || c == '\n')
	;
    if (c == '-') {
	negflg++;
	*np++ = c;
	c = GETC(iop);
	len--;
    } else if (c == '+') {
	len--;
	c = GETC(iop);
    }
    for ( ; --len >= 0; *np++ = c, c = GETC(iop)) {
	if (ISDIGIT(c) || base == 16 && ('a' <= c && c <= 'f' || 'A' <= c && c <= 'F')) {
	    ndigit++;
	    if (base==8)
		lcval <<=3;
	    else if (base==10)
		lcval = ((lcval<<2) + lcval)<<1;
	    else
		lcval <<= 4;
	    c1 = c;
	    if (ISDIGIT(c))
		c -= '0';
	    else if ('a' <= c && c <= 'f')
		c -= 'a' - 10;
	    else
		c -= 'A' - 10;
	    lcval += c;
	    c = c1;
	    continue;
	} else if (c == '.') {
	    if (base != 10 || scale == INT)
		break;
	    ndigit++;
	    continue;
	} else if ((c == 'e' || c == 'E') && !expseen) {
	    if (base != 10 || scale == INT || ndigit == 0)
		break;
	    expseen++;
	    *np++ = c;
	    c = GETC(iop);
	    if (c != '+' && c != '-' && ('0' > c || c > '9'))
		break;
	} else
	    break;
    }
    if (negflg)
	lcval = -lcval;
    if (c != EOF) {
	UNGETC(c, iop);
	*eofptr = 0;
    } else
	*eofptr = 1;
    if (ptr == NULL || np == numbuf || (negflg && np == numbuf+1))
	return 0;
    *np++ = 0;
    switch ((scale << 4) | size) {

#ifdef USEFLOAT
    case (FLOAT<<4) | SHORT:
    case (FLOAT<<4) | REGULAR:
	**(float **)ptr = atof(numbuf);
	break;

    case (FLOAT<<4) | LONG:
	**(double **)ptr = atof(numbuf);
	break;
#endif

    case (INT<<4) | SHORT:
	**(short **)ptr = lcval;
	break;

    case (INT<<4) | REGULAR:
	**(int **)ptr = lcval;
	break;

    case (INT<<4) | LONG:
	**(long **)ptr = lcval;
	break;
    }
    return 1;
}

static int
_instr(char *ptr, int type, int len, XFILE *iop, int *eofptr)
{
    int ch, ignstp;
    char *optr = ptr;

    *eofptr = 0;
    if (type == 'c' && len == 30000)
	len = 1;
    ignstp = (type == 's') ? SPC : 0;
    while ((ch = GETC(iop)) != EOF && (_sctab[ch] & ignstp))
	;
    if (type == 'c')
	ignstp = 0;
    else if (type == '[')
	ignstp = STP;
    else
	ignstp = SPC;
    while (ch != EOF && !(_sctab[ch] & ignstp)) {
	if (ptr)
	    *ptr++ = ch;
	if (--len <= 0)
	    break;
	ch = GETC(iop);
    }
    if (ch != EOF) {
	if (len > 0)
	    UNGETC(ch, iop);
	*eofptr = 0;
    } else
	*eofptr = 1;
    if (ptr && ptr != optr) {
	if (type != 'c')
	    *ptr++ = '\0';
	return 1;
    }
    return 0;
}

static char *
_getccl(unsigned char *s)
{
    int c, t;

    t = 0;
    if (*s == '^') {
	t++;
	s++;
    }
    for (c = 0; c < (sizeof _sctab / sizeof _sctab[0]); c++)
	if (t)
	    _sctab[c] &= ~STP;
	else
	    _sctab[c] |= STP;
    if ((c = *s) == ']' || c == '-') { /* first char is special */
	if (t)
	    _sctab[c] |= STP;
	else
	    _sctab[c] &= ~STP;
	s++;
    }
    while ((c = *s++) != ']') {
	if (!c)
	    return((char *)--s);
	else if (c == '-' && *s != ']' && s[-2] < *s) {
	    for (c = s[-2] + 1; c < *s; c++)
		if (t)
		    _sctab[c] |= STP;
		else
		    _sctab[c] &= ~STP;
	} else if (t)
	    _sctab[c] |= STP;
	else
	    _sctab[c] &= ~STP;
    }
    return((char *)s);
}
