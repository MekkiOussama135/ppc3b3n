## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = c_b_gestion_des_coach

c_b_gestion_des_coach_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	coach.c coach.h 
	
	

c_b_gestion_des_coach_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

