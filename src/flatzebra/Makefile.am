# $Id: Makefile.am,v 1.7 2012/07/08 02:57:14 sarrazip Exp $
# Makefile.am

lib_LTLIBRARIES = libflatzebra-0.1.la

libflatzebra_0_1_la_LDFLAGS = -version-info 2:0:0 -no-undefined

libflatzebra_0_1_la_SOURCES = \
	Couple.h \
	RCouple.cpp \
	RCouple.h \
	GameEngine.cpp \
	GameEngine.h \
	PixmapArray.cpp \
	PixmapArray.h \
	PixmapLoadError.cpp \
	PixmapLoadError.h \
	Sprite.cpp \
	Sprite.h \
	RSprite.cpp \
	RSprite.h \
	SoundMixer.cpp \
	SoundMixer.h \
	Joystick.cpp \
	Joystick.h \
	KeyState.h \
	font_13x7.xpm

libflatzebra_0_1_la_CXXFLAGS = \
	$(SDL_CFLAGS) \
	-I$(top_srcdir)/src

libflatzebra_0_1_la_LIBADD = \
	$(SDL_LIBS)

pkgincludedir = $(includedir)/$(PACKAGE)-$(API)/$(PACKAGE)
pkginclude_HEADERS = \
	Couple.h \
	RCouple.h \
	GameEngine.h \
	PixmapArray.h \
	PixmapLoadError.h \
	Sprite.h \
	RSprite.h \
	SoundMixer.h \
	Joystick.h \
	KeyState.h

MAINTAINERCLEANFILES = Makefile.in
