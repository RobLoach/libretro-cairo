TARGET_NAME := cairo

include Makefile.libretro

ifeq ($(STATIC_LINKING),1)
	FLAGS += -DSTATIC_LINKING
endif

FLAGS += -D__LIBRETRO__ $(COREDEFINES) $(ENDIANNESS_DEFINES) $(PLATFORM_DEFINES) $(WARNINGS) $(fpic)

CXXFLAGS += $(FLAGS)
CFLAGS += $(FLAGS) -std=gnu99

CORE_DIR := $(CURDIR)
DEP_INSTALL_DIR := $(CORE_DIR)/tmp

CFLAGS += -I$(DEP_INSTALL_DIR)/include
LFLAGS := -L$(DEP_INSTALL_DIR)/lib
LIBS := -L$(DEP_INSTALL_DIR)/lib -lcairo -lpixman-1 -lpng -lfreetype -lfontconfig -lz -lm

ifeq ($(platform), win)
	LIBS += -lgdi32 -lmsimg32
endif

with_fpic=
ifneq ($(fpic),)
   with_fpic := --with-pic=yes
endif

host_opts=
ifneq ($(HOST),)
	host_opts := --host=$(HOST)
endif

# Ignore first attempt builds, and re-try for a cleaner dependency chain.
all: $(TARGET)

deps: $(DEP_INSTALL_DIR)/lib/libcairo.a

vendor/libretro-common/include/libretro.h:
	@git submodule update --init --recursive

$(OBJECTS): vendor/libretro-common/include/libretro.h deps

$(TARGET): $(OBJECTS)
	$(CXX) $(fpic) $(SHARED) $(INCLUDES) $(LFLAGS) -o $@ $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(DEP_INSTALL_DIR)/lib/libpixman-1.a: $(DEP_INSTALL_DIR)/lib/libpng.a
	cd $(CORE_DIR)/vendor/pixman && \
		./autogen.sh && \
		./configure $(host_opts) \
			--enable-shared=no \
			--enable-static=yes \
			--enable-libpng \
			--disable-gtk \
			--disable-loongson-mmi \
			PNG_CFLAGS="-I$(DEP_INSTALL_DIR)/include" \
			PNG_LIBS="-L$(DEP_INSTALL_DIR)/lib -lpng" \
			LDFLAGS="-L$(DEP_INSTALL_DIR)/lib" \
			CPPFLAGS="-I$(DEP_INSTALL_DIR)/include" \
			$(with_fpic) CFLAGS="-fno-lto" --prefix=$(DEP_INSTALL_DIR)
	$(MAKE) -C "$(CORE_DIR)/vendor/pixman"
	$(MAKE) -C "$(CORE_DIR)/vendor/pixman" install

$(DEP_INSTALL_DIR)/lib/libz.a:
	cd $(CORE_DIR)/vendor/zlib && \
		CFLAGS="-fPIC" ./configure $(host_opts) --static --sharedlibdir="$(DEP_INSTALL_DIR)/lib" --libdir="$(DEP_INSTALL_DIR)/lib" --includedir="$(DEP_INSTALL_DIR)/include" \
		--prefix=$(DEP_INSTALL_DIR)
	$(MAKE) -C "$(CORE_DIR)/vendor/zlib"
	$(MAKE) -C "$(CORE_DIR)/vendor/zlib" install

$(DEP_INSTALL_DIR)/lib/libpng.a: $(DEP_INSTALL_DIR)/lib/libz.a
	cd $(CORE_DIR)/vendor/libpng && \
		./autogen.sh; \
		./configure $(host_opts) --enable-shared=no --enable-static=yes \
			--enable-hardware-optimizations=no \
			LDFLAGS="-L$(DEP_INSTALL_DIR)/lib" \
			CPPFLAGS="-I$(DEP_INSTALL_DIR)/include" \
			$(with_fpic) CFLAGS="-fno-lto" --prefix=$(DEP_INSTALL_DIR)
	$(MAKE) -C "$(CORE_DIR)/vendor/libpng"
	$(MAKE) -C "$(CORE_DIR)/vendor/libpng" install

# Font Config
# Requires: uuid-dev
#$(DEP_INSTALL_DIR)/lib/libfontconfig.a: $(DEP_INSTALL_DIR)/lib/libfreetype.a
#	cd $(CORE_DIR)/vendor/fontconf && \
#		./autogen.sh && \
#		./configure $(host_opts) --enable-shared=no --enable-static=yes \
#			--enable-docs=no \
#			FREETYPE_CFLAGS="-I$(DEP_INSTALL_DIR)/include/freetype2" FREETYPE_LIBS="-L$(DEP_INSTALL_DIR)/lib -lfreetype" \
#			LDFLAGS="-L$(DEP_INSTALL_DIR)/lib" \
#			$(with_fpic) CFLAGS="-fno-lto" --prefix=$(DEP_INSTALL_DIR) && \
#		$(MAKE) && $(MAKE) install

#$(DEP_INSTALL_DIR)/lib/libfreetype.a: $(DEP_INSTALL_DIR)/lib/libpng.a $(DEP_INSTALL_DIR)/lib/libz.a
#	cd $(CORE_DIR)/vendor/freetype2 && \
#		./autogen.sh && \
#		./configure $(host_opts) --enable-shared=no --enable-static=yes \
#			--with-zlib=yes \
#			--with-bzip2=no \
#			--with-png=yes \
#			--with-harfbuzz=no \
#			ZLIB_CFLAGS="-I$(DEP_INSTALL_DIR)/include" ZLIB_LIBS="-L$(DEP_INSTALL_DIR)/lib -lz" \
#			LIBPNG_CFLAGS="-I$(DEP_INSTALL_DIR)/include -I$(DEP_INSTALL_DIR)/include" LIBPNG_LIBS="-L$(DEP_INSTALL_DIR)/lib -lpng" \
#			LDFLAGS="-L$(DEP_INSTALL_DIR)/lib" \
#			CPPFLAGS="-I$(DEP_INSTALL_DIR)/include" \
#			$(with_fpic) CFLAGS="-fno-lto" --prefix=$(DEP_INSTALL_DIR) && \
#		$(MAKE) && $(MAKE) install

$(DEP_INSTALL_DIR)/lib/libcairo.a: $(DEP_INSTALL_DIR)/lib/libpixman-1.a $(DEP_INSTALL_DIR)/lib/libpng.a # $(DEP_INSTALL_DIR)/lib/libfreetype.a
	cd "$(CORE_DIR)/vendor/cairo" && \
		./autogen.sh && \
		./configure $(host_opts) --enable-static=yes --enable-shared=no \
			--enable-ft=yes \
			--enable-gtk-doc=no \
			--enable-gobject=no \
			--enable-trace=no \
			--enable-interpreter=no \
			--enable-symbol-lookup=no \
			--enable-svg=no \
			--enable-pdf=no \
			--enable-ps=no \
			--enable-glib=no \
			--enable-egl=no \
			--enable-glx=no \
			--enable-wgl=no \
			--enable-script=no \
			--enable-valgrind=no \
			--enable-xml=no \
			--enable-silent-rules \
			--enable-png=yes \
			--enable-xlib=no \
			--enable-pthread=no \
			--enable-drm=no \
			--enable-xcb-drm=no \
			--enable-drm-xr=no \
			--enable-lto=no \
			--enable-qt=no \
			--enable-full-testing=no \
			--with-x=no \
			$(with_fpic) CFLAGS="-fno-lto -DCAIRO_NO_MUTEX=1 -Wl,--verbose" \
			pixman_CFLAGS="-I$(DEP_INSTALL_DIR)/include/pixman-1" pixman_LIBS="-L$(DEP_INSTALL_DIR)/lib -lpixman-1" \
			png_CFLAGS="-I$(DEP_INSTALL_DIR)/include" png_LIBS="-L$(DEP_INSTALL_DIR)/lib -lpng" \
			LDFLAGS="-L$(DEP_INSTALL_DIR)/lib" \
			CPPFLAGS="-I$(DEP_INSTALL_DIR)/include" \
			--prefix=$(DEP_INSTALL_DIR)
	$(MAKE) -C "$(CORE_DIR)/vendor/cairo"
	$(MAKE) -C "$(CORE_DIR)/vendor/cairo" install

			#FREETYPE_CFLAGS="-I$(DEP_INSTALL_DIR)/include/freetype2" FREETYPE_LIBS="-L$(DEP_INSTALL_DIR)/lib -lfreetype" \
			#FONTCONFIG_CFLAGS="-I$(DEP_INSTALL_DIR)/include" FREETYPE_LIBS="-L$(DEP_INSTALL_DIR)/lib -lfontconfig" \

clean_cairo:
	make -C "$(CORE_DIR)/vendor/cairo" distclean || true

clean_pixman:
	make -C "$(CORE_DIR)/vendor/pixman" distclean || true

clean_png:
	make -C "$(CORE_DIR)/vendor/libpng" distclean || true

clean: clean_cairo clean_pixman clean_png
	rm -rf $(TARGET) $(OBJECTS) $(DEP_INSTALL_DIR)
	git clean -xdf
	rm -rf vendor
	git submodule update --init --recursive
	git submodule foreach --recursive git clean -xfd
	git submodule foreach --recursive git reset --hard HEAD

versions: vendor/libretro-common/include/libretro.h
	@git submodule foreach 'git describe --exact-match --tags $(git log -n1 --pretty='%h') || true'

vendor/noarch/noarch: vendor/libretro-common/include/libretro.h
	make -C vendor/noarch

test: vendor/noarch/noarch $(TARGET)
	vendor/noarch/noarch $(CORE_DIR)/$(TARGET) $(TARGET)

PREFIX := /usr
INSTALLDIR := $(PREFIX)/lib/libretro
install: all
	mkdir -p $(DESTDIR)$(INSTALLDIR)
	cp $(TARGET) $(DESTDIR)$(INSTALLDIR)
