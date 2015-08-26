AUTOMAKE_OPTIONS = no-dependencies
EXTRA_DIST = cabinfo.c mktime.c getopt.h \
	cabextract.1 debian ja cabextract.spec \
	cabextract.spec.in magic.cabinet

bin_PROGRAMS = cabextract
man_MANS = cabextract.1
cabextract_SOURCES = cabextract.c
cabextract_LDADD = @LIBOBJS@

