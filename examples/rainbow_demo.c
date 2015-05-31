#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <prisma.h>


int main(void) {
    uint32_t i, step, l, r, count;
    struct PrismaEscapeData fg, bg, reset;
    uint32_t resolution = 42; //5
    const uint32_t *gradient;
    prisma_adapt(PRISMA__ADAPT__GUESS);
    fg.value = PRISMA__FG_EXTENDED;
    bg.value = PRISMA__BG_EXTENDED;
    reset.value = PRISMA__RESET;
    for(step=0 ; step<6 ; step++) {
	switch(step) {
	case 0: l=0xff0000; r=0xffff00; break;
	case 1: l=r; r=0x00ff00; break;
	case 2: l=r; r=0x00ffff; break;
	case 3: l=r; r=0x0000ff; break;
	case 4: l=r; r=0xff00ff; break;
	case 5: l=r; r=0xff0000; break;
	}
	gradient = prisma_gradient(resolution, l, r);
	for(i=0 ; i<resolution ; i++) {
	    prisma_argbtobytes(gradient[i], NULL, &bg.r, &bg.g, &bg.b);
	    prisma_argbtobytes(gradient[i], NULL, &fg.r, &fg.g, &fg.b);
	    printf("%s",  prismas_dtoe(bg));
	    printf("%s0", prismas_dtoe(fg));
	}
	printf("%s\n", prismas_dtoe(reset));
    }
    exit(EXIT_SUCCESS);
}
