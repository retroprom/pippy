#ifndef LIBC_SEGMENTS_H
#define LIBC_SEGMENTS_H

#ifdef USE_SEGMENTS
#define SEG_LIBC __attribute__ ((section (Q_SEG_NAME)))
#define AS_SEG_LIBC .section SEG_NAME,"x"
#else
#define SEG_LIBC
#define AS_SEG_LIBC
#endif


#endif /* LIBC_SEGMENTS_H */
