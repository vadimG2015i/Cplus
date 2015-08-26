/* ------------------------------------------------------------------------ */
/* LHa for UNIX    															*/
/*				append.c -- append to archive								*/
/*																			*/
/*		Modified          		Nobutaka Watazaki							*/
/*																			*/
/*	Ver. 1.14 	Source All chagned				1995.01.14	N.Watazaki		*/
/* ------------------------------------------------------------------------ */
#include "lha.h"
//#include "..\unlhadlg.h"

/* ------------------------------------------------------------------------ */
int
encode_lzhuf(FILE *infp, FILE *outfp, long size, 
			 long *original_size_var,
			 long *packed_size_var,
			 char *name, char *hdr_method)
{
	static int      method = -1;

	if (method < 0) {
		method = compress_method;
		if (method > 0)
			method = encode_alloc(method);
	}

	interface.method = method;

	if (interface.method > 0) {
		interface.infile = infp;
		interface.outfile = outfp;
		interface.original = size;
		// size = �t�@�C���T�C�Y�H
//		dlgSetFileSize(size, -1);
		start_indicator(name, size, "Freezing", 1 << dicbit);
		LHAencode(&interface);
		*packed_size_var = interface.packed;
		*original_size_var = interface.original;
	} else {
		copyfile(infp, outfp, size, 1);
		*packed_size_var = size;
		*original_size_var = size;
	}
	bcopy("-lh -", hdr_method, 5);
	hdr_method[3] = interface.method + '0';

	finish_indicator2(name, "Frozen",
		    (int) ((*packed_size_var * 100L) / *original_size_var));
	return crc;
}
/* ------------------------------------------------------------------------ */
void
start_indicator(char *name, long size,
				char *msg, long def_indicator_threshold)
{
#ifdef NEED_INCREMENTAL_INDICATOR
	long            i;
	int             m;
#endif

	if (quiet)
		return;

#ifdef NEED_INCREMENTAL_INDICATOR
	switch (quiet_mode) {
	case 0:
		m = MAX_INDICATOR_COUNT - strlen(name);
		if (m < 1)		/* Bug Fixed by N.Watazaki */
			m = 3;		/* (^_^) */
		// name = �i�[�Ώۃt�@�C��
//		printf("\r%s\t- %s :  ", name, msg);
	//	dlgSetFileName(name);
//		dlgSetFileSize(size, -1);
		indicator_threshold =
			((size + (m * def_indicator_threshold - 1)) /
			 (m * def_indicator_threshold) *
			 def_indicator_threshold);
		if (indicator_threshold)
			i = ((size + (indicator_threshold - 1)) / indicator_threshold);
		else
			i = 0;
		// i = . �̐�
//		progressSetRange(0, i);
//		while (i--)
//			putchar('.');
		indicator_count = 0;
//		printf("\r%s\t- %s :  ", name, msg);
		break;
	case 1:
//		printf("\r%s :", name);
		break;
	}
#else
//	printf("%s\t- ", name);
#endif
	fflush(stdout);
	reading_size = 0L;
}
/* ------------------------------------------------------------------------ */
void
finish_indicator2(char *name, char *msg, int pcnt)
{
	if (quiet)
		return;

	if (pcnt > 100)
		pcnt = 100;	/* (^_^) */
#ifdef NEED_INCREMENTAL_INDICATOR
//	printf("\r%s\t- %s(%d%%)\n", name,  msg, pcnt);
#else
//	printf("%s\n", msg);
#endif
	fflush(stdout);
}
/* ------------------------------------------------------------------------ */
void
finish_indicator(char *name, char *msg)
{
	if (quiet)
		return;

#ifdef NEED_INCREMENTAL_INDICATOR
//	printf("\r%s\t- %s\n", name, msg);
#else
//	printf("%s\n", msg);
#endif
	fflush(stdout);
}
