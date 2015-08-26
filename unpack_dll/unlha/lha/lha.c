
#include "lha.h"

static int
sort_by_ascii(char **a, char **b)
{
	register char  *p, *q;
	register int    c1, c2;

	p = *a, q = *b;
	if (generic_format) {
		do {
			c1 = *(unsigned char *) p++;
			c2 = *(unsigned char *) q++;
			if (!c1 || !c2)
				break;
			if (islower(c1))
				c1 = toupper(c1);
			if (islower(c2))
				c2 = toupper(c2);
		}
		while (c1 == c2);
		return c1 - c2;
	}
	else {
		while (*p == *q && *p != '\0')
			p++, q++;
		return *(unsigned char *) p - *(unsigned char *) q;
	}
}

/* ------------------------------------------------------------------------ */

extern void
sort_files()
{
#ifndef _WIN32_WCE_EMULATION
	if (cmd_filec > 1)
		qsort(cmd_filev, cmd_filec, sizeof(char *), sort_by_ascii);
#endif
}
