#ifndef PALM_SEGMENTS_H
#define PALM_SEGMENTS_H
/* First, define generic names for the code segments.  The main
   segment is the only one accessible to the debugger.  */

#ifdef CODEWARRIOR
#define DEF_SEGMENT(name)
#else
#define DEF_SEGMENT(name) __attribute__ ((section (name)))
#endif /*CODEWARROR */

/* length limit of section names = 8 characters */
#define PALM_SEG_0 "pseg0"
#define PALM_SEG_1 "pseg1"
#define PALM_SEG_2 "pseg2"
#define PALM_SEG_2A "pseg2A"
#define PALM_SEG_2B "pseg2B"
#define PALM_SEG_3 "pseg3"
#define PALM_SEG_4 "pseg4"
#define PALM_SEG_5 "pseg5"
#define PALM_SEG_6 "pseg6"
#define PALM_SEG_7 "pseg7"
#define PALM_SEG_8 "pseg8"
#define PALM_SEG_9 "pseg9"
#define PALM_SEG_10 "pseg10"

#endif /* PALM_SEGMENTS_H */
