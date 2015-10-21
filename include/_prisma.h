/*
 * prisma is released under the CC0 license.
 * (see http://creativecommons.org/about/cc0)
 * Anyone may freely build upon, enhance and reuse prisma for any purposes without restriction under copyright or database law.
 *
 * Bug reports with this unaltered source code should be sent to yoanlecoq.io@gmail.com
 */

#ifndef _PRISMA_H
#define _PRISMA_H

#include <stdint.h>

#if defined __linux__ || defined __unix__
#define _PRISMA_LINUX
#elif defined _WIN32
#define _PRISMA_WINDOWS
#endif

#define _PRISMA__SGR__FIRST    0
#define _PRISMA__SGR__LAST    49

#define _PRISMA__CLEAR__FIRST      60
#define _PRISMA__CLEAR__LAST       62

#define _PRISMA_TERMNAME_LENGTH 64

extern int8_t _prisma_adaptation;
char* _prisma_trytogettermname();
extern uint32_t _prisma_cmd_fg;
extern uint32_t _prisma_cmd_bg;

#endif /* _PRISMA_H */
