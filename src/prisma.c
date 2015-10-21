/*
 * prisma is released under the CC0 license.
 * (see http://creativecommons.org/about/cc0)
 * Anyone may freely build upon, enhance and reuse prisma for any 
 * purposes without restriction under copyright or database law.
 *
 * Bug reports should be sent to yoanlecoq.io at gmail.com.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <_prisma.h>
#include <prisma.h>

#ifdef _PRISMA_LINUX
#include <unistd.h>
#elif defined _PRISMA_WINDOWS
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#include <windows.h>
#endif

int8_t _prisma_adaptation = PRISMA__ADAPT__FULLSUPPORT;

#ifdef _PRISMA_LINUX
/* Taken from https://gist.github.com/fclairamb/a16a4237c46440bdb172. */
pid_t getppidof(const pid_t pid) {
    int ppid;
    char buffer[BUFSIZ];
    size_t size;
    FILE* fp;

    sprintf(buffer, "/proc/%d/stat", pid);
    fp = fopen(buffer, "r");
    if (fp) {
    size = fread(buffer, sizeof (char), sizeof (buffer), fp);
    if (size > 0) {
        strtok(buffer, " ");
        strtok(NULL, " ");
        strtok(NULL, " ");
        ppid = atoi(strtok(NULL, " "));
    }
    fclose(fp);
    }
    return ppid;
}

char* getnameof(const pid_t pid) {
    static char buffer[BUFSIZ];
    char *old_buffer, *new_buffer;
    FILE *fp;
    size_t size;
    sprintf(buffer, "/proc/%d/cmdline", pid);
    fp = fopen(buffer, "r");
    if (fp) {
    size = fread(buffer, sizeof (char), sizeof (buffer), fp);
    if (size > 0) {
        new_buffer = strtok(buffer, "/");
        while(new_buffer != NULL) {
        old_buffer = new_buffer;
        new_buffer = strtok(NULL, "/");
        }
    }
    fclose(fp);
    }
    return old_buffer;
}
#endif /* _PRISMA_LINUX */

char* _prisma_trytogettermname() {
    static char _prisma_termname[_PRISMA_TERMNAME_LENGTH];

#ifdef _PRISMA_LINUX 
    char *tmp = getnameof(getppidof(getsid(getpid())));
    if(tmp) {
        strncpy(_prisma_termname, tmp, _PRISMA_TERMNAME_LENGTH);
        if(!strncmp(_prisma_termname, "initdline", _PRISMA_TERMNAME_LENGTH)
        || !strncmp(_prisma_termname, "konsole", _PRISMA_TERMNAME_LENGTH))
            return _prisma_termname;
    } 

#ifdef _GNU_SOURCE
    strncpy(_prisma_termname, secure_getenv("TERM"), _PRISMA_TERMNAME_LENGTH);
#else
    strncpy(_prisma_termname, getenv("TERM"), _PRISMA_TERMNAME_LENGTH);
#endif

#elif defined _PRISMA_WINDOWS
    if(GetConsoleOriginalTitleA(&_prisma_termname[9], 
                _PRISMA_TERMNAME_LENGTH-9)) {
        strncpy(_prisma_termname, "windows, ", 9);
    } else
        strncpy(_prisma_termname, "windows, unknown", _PRISMA_TERMNAME_LENGTH);
#else
    strncpy(_prisma_termname, "unknown", _PRISMA_TERMNAME_LENGTH);
#endif

    return _prisma_termname;
}

int8_t prisma_adapt(int8_t adaptation) {
    char *termname;
    if(adaptation==PRISMA__ADAPT__GUESS) {
        termname = _prisma_trytogettermname();
        if(!strcmp(termname, "xterm"))
            _prisma_adaptation = PRISMA__ADAPT__XTERM;
        else if(!strcmp(termname, "konsole"))
            _prisma_adaptation = PRISMA__ADAPT__FULLSUPPORT;
        else if(!strcmp(termname, "linux")
             || !strcmp(termname, "initdline"))
            _prisma_adaptation = PRISMA__ADAPT__LINUXTTY;
        else if(!strncmp(termname, "windows", 7))
            _prisma_adaptation = PRISMA__ADAPT__CMD;
        else _prisma_adaptation = PRISMA__ADAPT__NOSUPPORT;
    } else _prisma_adaptation = adaptation;
    return _prisma_adaptation;
}

void prisma_argbtobytes(uint32_t argb, uint8_t *a, uint8_t *r, uint8_t *g, uint8_t *b) {
    if(a) *a = prisma_abyte(argb);
    if(r) *r = prisma_rbyte(argb);
    if(g) *g = prisma_gbyte(argb);
    if(b) *b = prisma_bbyte(argb);
}

uint8_t prisma_argbto8color(uint32_t argb) {
    argb = ((argb&0xff0000)>=0x7f0000 ? 0xff0000 : 0 ) 
         | ((argb&0x00ff00)>=0x007f00 ? 0x00ff00 : 0 ) 
         | ((argb&0x0000ff)>=0x00007f ? 0x0000ff : 0 );
    switch(argb) {
    case 0x000000: return 0;
    case 0xff0000: return 1;
    case 0x00ff00: return 2;
    case 0xffff00: return 3;
    case 0x0000ff: return 4;
    case 0xff00ff: return 5;
    case 0x00ffff: return 6;
    case 0xffffff: return 7;
    }
}

uint32_t prisma_8colortoargb(uint8_t src) {
    switch(src) {
    case 0: return 0xff000000;
    case 1: return 0xffff0000;
    case 2: return 0xff00ff00;
    case 3: return 0xffffff00;
    case 4: return 0xff0000ff;
    case 5: return 0xffff00ff;
    case 6: return 0xff00ffff;
    case 7: return 0xffffffff;
    default: return 0;
    }
}

uint8_t prisma_argbtoxterm(uint32_t argb) {
    uint8_t r,g,b;
    prisma_argbtobytes(argb, NULL, &r, &g, &b);
    return ( 
          (r>=0x04 && r<=0xf6 && r==g && g==b) 
        ? ( 232 + ((r-0x08)*24/0xee) )
        : ( 16 + (36*(r/51)) + (6*(g/51)) + (b/51) )    
    );
}

uint32_t prisma_xtermtoargb(uint8_t index) {
    uint32_t argb;
    if(index<8) {
        argb = prisma_8colortoargb(index);
        if(argb==0xffffffff)
            argb = 0xffc0c0c0;
        else
            argb = 0xff000000 
            | ((argb&0xff0000)!=0 ? 0x800000: 0) 
            | ((argb&0x00ff00)!=0 ? 0x008000: 0) 
            | ((argb&0x0000ff)!=0 ? 0x000080: 0);
    } else if(index<16) {
        index -= 8;
        argb = prisma_8colortoargb(index);
        if(argb==0)
            argb = 0xff808080;
    } else if(index<232) {
        index -= 16;
        argb = 0xff000000 
            | ((51*(index/36))<<16) 
            | ((51*((index%36)/6))<<8) 
            | (51*(index%6));
    } else {
        index -= 232;
        argb = (0x08+(10*index));
        argb |= 0xff000000 | (argb<<16) | (argb<<8);
    }
    return argb;
}

struct PrismaEscapeData prisma_etod(const void* src_string, size_t char_size) {
    uint32_t src[PRISMA_ESCAPE_CAPACITY];
    uint32_t i;
    int32_t stock[6], k;
    char tmp[4], p;
    struct PrismaEscapeData res;

    memset(&res, 0, sizeof(struct PrismaEscapeData));
    res.value = PRISMA__UNSUPPORTED;

    for(i=0 ; i<PRISMA_ESCAPE_CAPACITY ; i++) {
        switch(char_size) {
        case 1: src[i] = ((const uint8_t*)  src_string)[i]; break;
        case 2: src[i] = ((const uint16_t*) src_string)[i]; break;
        case 4: src[i] = ((const uint32_t*) src_string)[i]; break;
        default: return res; break;
        }
        if(i==1) {
            if(!(src[0]=='\033' && src[1]=='['))
                return res;
            else continue;
        }
        if(src[i] == '\0' || (src[i] >= '@' && src[i] <= '~')) {
            res.length = i+1;
            break;
        }
    }
    if(i>=PRISMA_ESCAPE_CAPACITY) {
        res.length = PRISMA_ESCAPE_CAPACITY;
        return res;
    }
    
    memset(tmp, 0, 4);
    memset(stock, 0, 24);

    for(p=0, k=0, i=2 ; i<res.length && k<5 && p<4; i++) {
        switch(src[i]) {
        case ';': 
            stock[k] = atoi(tmp);
            p=0;
            k++;
            memset(tmp, 0, 4*sizeof(char));
            break;
        case '0': case '1': case '2': case '3': case '4': 
        case '5': case '6': case '7': case '8': case '9': 
            tmp[p] = (char) src[i];
            p++;
            break;
        default: 
            stock[k] = atoi(tmp);
            p=-1; 
            break;
        }
        if(p==-1)
            break;
    }
    
    switch(src[i]) {
    case 'm': 
        res.value = stock[0];
        if(res.value == PRISMA__FG_EXTENDED 
        || res.value == PRISMA__BG_EXTENDED) {
            switch(stock[1]) {
            case 2: 
                res.r = stock[2];
                res.g = stock[3];
                res.b = stock[4];
                break;
            case 5: 
                prisma_argbtobytes(prisma_xtermtoargb(stock[2]), NULL, &res.r, &res.g, &res.b);
            break;
            }
        }
        break;
    case 'J': 
        res.value = stock[0]+_PRISMA__CLEAR__FIRST;
        break;
    case 'H': 
        res.value = PRISMA__CURSOR_POSITION;
        res.n = stock[0] > 0 ? stock[0] : 1;
        res.m = stock[1] > 0 ? stock[1] : 1;
        break;
    }
    return res;
}

uint32_t _prisma_cmd_fg;
uint32_t _prisma_cmd_bg;

const void* prisma_dtoe(struct PrismaEscapeData src, size_t char_size) {
    static uint32_t res32[PRISMA_ESCAPE_CAPACITY];
    static uint16_t res16[PRISMA_ESCAPE_CAPACITY];
    static uint8_t  res8[PRISMA_ESCAPE_CAPACITY];
    uint32_t i;

    memset(res8, 0, PRISMA_ESCAPE_CAPACITY*sizeof(uint8_t));

    if(_prisma_adaptation != PRISMA__ADAPT__NOSUPPORT) {

        if(char_size!=1 && char_size!=2 && char_size!=4)
            return NULL;
        if(src.value >= _PRISMA__SGR__FIRST && src.value <= _PRISMA__SGR__LAST) 
        {
            if(src.value == PRISMA__FG_EXTENDED 
            || src.value == PRISMA__BG_EXTENDED) 
            {
                switch(_prisma_adaptation) {
                case PRISMA__ADAPT__FULLSUPPORT: 
                    snprintf((char*) res8, PRISMA_ESCAPE_CAPACITY, "\033[%u;2;%u;%u;%um", src.value, src.r, src.g, src.b);
                    break;
                case PRISMA__ADAPT__XTERM: 
                    snprintf((char*) res8, PRISMA_ESCAPE_CAPACITY, "\033[%u;5;%um", src.value, prisma_argbtoxterm(prisma_bytestoargb(0, src.r, src.g, src.b)));
                    break;
                case PRISMA__ADAPT__LINUXTTY: 
                    snprintf((char*) res8, PRISMA_ESCAPE_CAPACITY, "\033[%u;%um", 1, src.value-8+prisma_argbto8color(prisma_bytestoargb(0, src.r, src.g, src.b)));
                    break;
                case PRISMA__ADAPT__CMD: 
#ifdef _PRISMA_WINDOWS
                    if(src.value == PRISMA__FG_EXTENDED) {
                        _prisma_cmd_fg = 
                          (src.r>127 ? FOREGROUND_RED : 0)
                        | (src.g>127 ? FOREGROUND_GREEN : 0)
                        | (src.b>127 ? FOREGROUND_BLUE : 0);
                        if(src.r>127 || src.g>127 || src.b>127)
                            _prisma_cmd_fg |= FOREGROUND_INTENSITY;
                    }
                    if(src.value == PRISMA__BG_EXTENDED) {
                        _prisma_cmd_bg = 
                          (src.r>127 ? BACKGROUND_RED : 0)
                        | (src.g>127 ? BACKGROUND_GREEN : 0)
                        | (src.b>127 ? BACKGROUND_BLUE : 0);
                        if(src.r>127 || src.g>127 || src.b>127)
                            _prisma_cmd_bg |= BACKGROUND_INTENSITY;
                    }
#endif
                    break;    
                }
            } else {
                if(_prisma_adaptation == PRISMA__ADAPT__CMD) {
#ifdef _PRISMA_WINDOWS
                    if(src.value >= PRISMA__FG_BLACK 
                    && src.value <= PRISMA__FG_WHITE) 
                    {
                        _prisma_cmd_fg = prisma_8colortoargb(src.value - PRISMA__FG_BLACK);
                        _prisma_cmd_fg = 
                              (prisma_rbyte(_prisma_cmd_fg)!=0 ? FOREGROUND_RED : 0)
                            | (prisma_gbyte(_prisma_cmd_fg)!=0 ? FOREGROUND_GREEN : 0)
                            | (prisma_bbyte(_prisma_cmd_fg)!=0 ? FOREGROUND_BLUE : 0);
                    } else if(src.value >= PRISMA__BG_BLACK && src.value <= PRISMA__BG_WHITE) {
                        _prisma_cmd_bg = prisma_8colortoargb(src.value - PRISMA__BG_BLACK);
                        _prisma_cmd_bg = 
                              (prisma_rbyte(_prisma_cmd_bg)!=0 ? BACKGROUND_RED : 0)
                            | (prisma_gbyte(_prisma_cmd_bg)!=0 ? BACKGROUND_GREEN : 0)
                            | (prisma_bbyte(_prisma_cmd_bg)!=0 ? BACKGROUND_BLUE : 0);
                    } else if(src.value == PRISMA__RESET) {
                        _prisma_cmd_bg = 0;
                        _prisma_cmd_fg = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                    } else if(src.value == PRISMA__FG_DEFAULT) {
                        _prisma_cmd_fg = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                    } else if(src.value == PRISMA__BG_DEFAULT) {
                        _prisma_cmd_bg = 0;
                    } else if(src.value == PRISMA__BOLD) {
                        _prisma_cmd_fg |= FOREGROUND_INTENSITY;
                        _prisma_cmd_bg |= BACKGROUND_INTENSITY;
                    } else if(src.value == PRISMA__BOLD_OFF || src.value == PRISMA__DEFAULT_INTENSITY) {
                        _prisma_cmd_fg &= ~FOREGROUND_INTENSITY;
                        _prisma_cmd_bg &= ~BACKGROUND_INTENSITY;
                    }
#endif
                } else {
                    snprintf((char*) res8, PRISMA_ESCAPE_CAPACITY, "\033[%um", src.value);
                }
            } /* END SGR */
        } else if(src.value >= _PRISMA__CLEAR__FIRST && src.value <= _PRISMA__CLEAR__LAST) {
            snprintf((char*) res8, PRISMA_ESCAPE_CAPACITY, "\033[%uJ", src.value-_PRISMA__CLEAR__FIRST);
        } else if(src.value >= PRISMA__CURSOR_POSITION) {
            snprintf((char*) res8, PRISMA_ESCAPE_CAPACITY, "\033[%u;%uH", src.n, src.m);
        } else {
            return NULL;
        }

#ifdef _PRISMA_WINDOWS
        if(_prisma_adaptation == PRISMA__ADAPT__CMD) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _prisma_cmd_bg | _prisma_cmd_fg);
            memset(res8, 0, PRISMA_ESCAPE_CAPACITY*sizeof(uint8_t));
        }
#endif
    }

    switch(char_size) {
    case 2: 
        for(i=0 ; i<PRISMA_ESCAPE_CAPACITY ; i++)
            res16[i] = res8[i]; 
        return res16;
    case 4: 
        for(i=0 ; i<PRISMA_ESCAPE_CAPACITY ; i++)
            res32[i] = res8[i]; 
        return res32;
    }
    return res8;
}


const uint32_t* prisma_gradient(uint8_t resolution, uint32_t argb_first, uint32_t argb_last) {
    static uint32_t gradient[256];
    float i, a, r, g, b, offset;

    memset(gradient, 0, 256*sizeof(uint32_t));

    if(resolution==0)
        return gradient;
    if(resolution==1) {
        gradient[0] = argb_first;
        return gradient;
    }

    gradient[0] = argb_first;
    i = 1.0f;
    do {
        if(prisma_abyte(argb_first) < prisma_abyte(argb_last)) {
            offset = ((float) (prisma_abyte(argb_last)-prisma_abyte(argb_first)))/(float)resolution;
            a = prisma_abyte(argb_first) + ceilf((i + i/((float)resolution-1))*offset);
        } else {
            offset = ((float) (prisma_abyte(argb_first)-prisma_abyte(argb_last)))/(float)resolution;
            a = prisma_abyte(argb_last) - ceilf((i + i/((float)resolution-1))*offset);
        }

        if(prisma_rbyte(argb_first) < prisma_rbyte(argb_last)) {
            offset = ((float) (prisma_rbyte(argb_last)-prisma_rbyte(argb_first)))/(float)resolution;
            r = prisma_rbyte(argb_first) + ceilf((i + i/((float)resolution-1))*offset);
        } else {
            offset = ((float) (prisma_rbyte(argb_first)-prisma_rbyte(argb_last)))/(float)resolution;
            r = prisma_rbyte(argb_last) - ceilf((i + i/((float)resolution-1))*offset);
        }

        if(prisma_gbyte(argb_first) < prisma_gbyte(argb_last)) {
            offset = ((float) (prisma_gbyte(argb_last)-prisma_gbyte(argb_first)))/(float)resolution;
            g = prisma_gbyte(argb_first) + ceilf((i + i/((float)resolution-1))*offset);
        } else {
            offset = ((float) (prisma_gbyte(argb_first)-prisma_gbyte(argb_last)))/(float)resolution;
            g = prisma_gbyte(argb_last) - ceilf((i + i/((float)resolution-1))*offset);
        }

        if(prisma_bbyte(argb_first) < prisma_bbyte(argb_last)) {
            offset = ((float) (prisma_bbyte(argb_last)-prisma_bbyte(argb_first)))/(float)resolution;
            b = prisma_bbyte(argb_first) + ceilf((i + i/((float)resolution-1))*offset);
        } else {
            offset = ((float) (prisma_bbyte(argb_first)-prisma_bbyte(argb_last)))/(float)resolution;
            b = prisma_bbyte(argb_last) - ceilf((i + i/((float)resolution-1))*offset);
        }

        gradient[(uint8_t) i] = prisma_bytestoargb(a, r, g, b);
        i++;
    } while(i <= resolution && i != 0.0f);

    gradient[((uint8_t) i)-2] = argb_last;

    return gradient;
}


void* prisma_textonly(const void* src_string, void* dst_string, size_t char_size) {
    struct PrismaEscapeData esc;
    uint32_t i, j;
    const uint8_t *src8;
    uint8_t *dst8;
    const uint16_t *src16;
    uint16_t *dst16;
    const uint32_t *src32;
    uint32_t *dst32;

    switch(char_size) {
    case 1: 
        src8 = (const uint8_t*) src_string;
        dst8 = (uint8_t*) dst_string;
        for(j=0, i=0 ; src8[i] != '\0' ; i++) {
            esc = prisma_etod(&src8[i], char_size);
            if(esc.value == PRISMA__UNSUPPORTED) {
                dst8[j++] = src8[i];
            } else {
                i += esc.length-1;
            }
        }
        dst8[j] = '\0';
        return dst8;
    case 2: 
        src16 = (const uint16_t*) src_string;
        dst16 = (uint16_t*) dst_string;
        for(j=0, i=0 ; src16[i] != '\0' ; i++) {
            esc = prisma_etod(&src16[i], char_size);
            if(esc.value == PRISMA__UNSUPPORTED) {
                dst16[j++] = src16[i];
            } else {
                i += esc.length-1;
            }
        }
        dst16[j] = '\0';
        return dst16;
    case 4: 
        src32 = (const uint32_t*) src_string;
        dst32 = (uint32_t*) dst_string;
        for(j=0, i=0 ; src32[i] != '\0' ; i++) {
            esc = prisma_etod(&src32[i], char_size);
            if(esc.value == PRISMA__UNSUPPORTED) {
                dst32[j++] = src32[i];
            } else {
                i += esc.length-1;
            }
        }
        dst32[j] = '\0';
        return dst32;
    }
    return dst_string;
}
