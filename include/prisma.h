/*
 * prisma is released under the CC0 license.
 * (see http://creativecommons.org/about/cc0)
 * Anyone may freely build upon, enhance and reuse prisma for any purposes without restriction under copyright or database law.
 * 
 * Bug reports with this unaltered source code should be sent to yoanlecoq.io@gmail.com
 */

#ifndef PRISMA_H
#define PRISMA_H

#include <stdint.h>

#define PRISMA__UNSUPPORTED            -1

#define PRISMA__NORMAL                 0
#define PRISMA__RESET                  0
#define PRISMA__BOLD                   1
#define PRISMA__FAINT                  2
#define PRISMA__ITALIC                 3
#define PRISMA__UNDERLINE              4
#define PRISMA__BLINK__SLOW            5
#define PRISMA__BLINK__RAPID           6
#define PRISMA__NEGATIVE               7
#define PRISMA__CONCEAL                8
#define PRISMA__STRIKE                 9
#define PRISMA__DEFAULT_FONT          10
#define PRISMA__ALTERNATE_FONT_1      11
#define PRISMA__ALTERNATE_FONT_2      12
#define PRISMA__ALTERNATE_FONT_3      13
#define PRISMA__ALTERNATE_FONT_4      14
#define PRISMA__ALTERNATE_FONT_5      15
#define PRISMA__ALTERNATE_FONT_6      16
#define PRISMA__ALTERNATE_FONT_7      17
#define PRISMA__ALTERNATE_FONT_8      18
#define PRISMA__ALTERNATE_FONT_9      19
#define PRISMA__FRAKTUR               20
#define PRISMA__BOLD_OFF              21
#define PRISMA__DEFAULT_INTENSITY     22
#define PRISMA__ITALIC_OFF            23
#define PRISMA__UNDERLINE_OFF         24
#define PRISMA__BLINK_OFF             25
#define PRISMA__NEGATIVE_OFF          27
#define PRISMA__CONCEAL_OFF           28
#define PRISMA__STRIKE_OFF            29

#define PRISMA__FG_BLACK     30
#define PRISMA__FG_RED       31
#define PRISMA__FG_GREEN     32
#define PRISMA__FG_YELLOW    33
#define PRISMA__FG_BLUE      34
#define PRISMA__FG_MAGENTA   35
#define PRISMA__FG_CYAN      36
#define PRISMA__FG_WHITE     37
#define PRISMA__FG_EXTENDED  38
#define PRISMA__FG_DEFAULT   39

#define PRISMA__BG_BLACK     40
#define PRISMA__BG_RED       41
#define PRISMA__BG_GREEN     42
#define PRISMA__BG_YELLOW    43
#define PRISMA__BG_BLUE      44
#define PRISMA__BG_MAGENTA   45
#define PRISMA__BG_CYAN      46
#define PRISMA__BG_WHITE     47
#define PRISMA__BG_EXTENDED  48
#define PRISMA__BG_DEFAULT   49

#define PRISMA__CLEAR_BELOW       60
#define PRISMA__CLEAR_ABOVE       61
#define PRISMA__CLEAR             62

#define PRISMA__CURSOR_POSITION   63

struct PrismaEscapeData {
    int32_t value;  /* The escape sequence's meaning */
    uint8_t r;      /* Optional red value */
    uint8_t g;      /* Optional green value */
    uint8_t b;      /* Optional blue value */
    int16_t n;      /* Optional first informative value */
    int16_t m;      /* Optional second informative value */
    uint8_t length; /* The escape sequence's total length as a string */
};


#define prisma_abyte(argb) (((argb) & 0xff000000)>>24)
#define prisma_rbyte(argb) (((argb) & 0x00ff0000)>>16)
#define prisma_gbyte(argb) (((argb) & 0x0000ff00)>>8)
#define prisma_bbyte(argb)  ((argb) & 0x000000ff)

#define prisma_bytestoargb(_a_, _r_, _g_, _b_) \
    (((((uint8_t) _a_)<<24)&0xff000000) \
    |((((uint8_t) _r_)<<16)&0xff0000) \
    |((((uint8_t) _g_)<<8)&0xff00) \
    | (((uint8_t) _b_)&0xff))
void prisma_argbtobytes(uint32_t argb, uint8_t *a, uint8_t *r, uint8_t *g, uint8_t *b);
uint8_t  prisma_argbtoxterm(uint32_t argb);
uint32_t prisma_xtermtoargb(uint8_t index);
uint8_t  prisma_argbto8color(uint32_t argb);
uint32_t prisma_8colortoargb(uint8_t src) ;

#define PRISMA_ESCAPE_CAPACITY 32

/* 
  Escape String to Data.
*/
struct PrismaEscapeData prisma_etod(const void* src_string, size_t char_size);
#define prismas_etod(src_string) prisma_etod(src_string, sizeof(char))



#define PRISMA__ADAPT__GUESS       -1
#define PRISMA__ADAPT__NOSUPPORT    0
#define PRISMA__ADAPT__FULLSUPPORT  1
#define PRISMA__ADAPT__XTERM        2
#define PRISMA__ADAPT__LINUXTTY     3
#define PRISMA__ADAPT__CMD          4
/* 
  Adapt Prisma's generated ANSI Escape Strings to a terminal, for visual convenience when printing on this one.
  Prisma does not try to detect it automatically (like with ctermid()), for the user may rather want to print escape string somewhere else than in a terminal.
*/
int8_t prisma_adapt(int8_t adaptation);


/* 
  Data to Escape String. 
  Returns a pointer to the generated ANSI escape sequence string, which data will be overriden on the next call. 
  If you need to store the result, then you should copy the pointer's data to an array of yours (by using, for instance, the memcpy() and strcpy() family of function).
  The pointers's data is of a maximum number of bytes defined by PRISMA_ESCAPE_CAPACITY.
  
  If prisma is adapted to Window's cmd.exe (like after calling prisma_adapt(PRISMA__ADAPT__CMD)), then prisma_dtoe() will immediately apply the expected visual result to the console and return an empty string.
  This is because Windows's cmd.exe does not interpret ANSI Escape Sequences, but prisma_dtoe() can still (and will) call specific WINAPI functions for it to behave like a regular escape sequence was printed.
*/
const void* prisma_dtoe(struct PrismaEscapeData src, size_t char_size);
#define prismas_dtoe(src) ((const char*) prisma_dtoe(src, sizeof(char)))


const uint32_t* prisma_gradient(uint8_t resolution, uint32_t argb_first, uint32_t argb_last);

void* prisma_textonly(const void* src_string, void* dst_string, size_t char_size);


#endif /* PRISMA_H */
