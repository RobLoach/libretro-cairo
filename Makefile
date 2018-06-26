TARGET_NAME := cairo

include Makefile.libretro

ifeq ($(STATIC_LINKING),1)
	FLAGS += -DSTATIC_LINKING
endif

FLAGS += -D__LIBRETRO__ $(COREDEFINES) $(ENDIANNESS_DEFINES) $(PLATFORM_DEFINES) $(WARNINGS) $(fpic)

CXXFLAGS += $(FLAGS) -std=c++14
CFLAGS += $(FLAGS) -std=gnu99

CORE_DIR := $(CURDIR)
DEP_INSTALL_DIR := $(CORE_DIR)/tmp

CFLAGS += -I$(DEP_INSTALL_DIR)/include
LFLAGS := -L$(DEP_INSTALL_DIR)/lib
LIBS := $(DEP_INSTALL_DIR)/lib/libcairo.a $(DEP_INSTALL_DIR)/lib/libpixman-1.a -lm

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

$(OBJECTS): vendor/libretro-common/include/libretro.h deps

$(TARGET): $(OBJECTS)
	-$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	-$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.c
	-$(CC) -c -o $@ $< $(CFLAGS)

$(DEP_INSTALL_DIR)/lib/libpixman-1.a:
	cd $(CORE_DIR)/vendor/pixman && \
		./autogen.sh && \
		./configure $(host_opts) --enable-shared=no --enable-static=yes $(with_fpic) CFLAGS="-fno-lto" --prefix=$(DEP_INSTALL_DIR) && \
		$(MAKE) && $(MAKE) install

$(DEP_INSTALL_DIR)/lib/libcairo.a: $(DEP_INSTALL_DIR)/lib/libpixman-1.a
	cd $(CORE_DIR)/vendor/cairo && \
		./autogen.sh && \
		./configure $(host_opts) --enable-static=yes --enable-ft=yes --enable-shared=no \
			--enable-gobject=no --enable-trace=no --enable-interpreter=no \
			--enable-symbol-lookup=no --enable-svg=no --enable-pdf=no --enable-ps=no \
			--enable-wgl=no --enable-glx=no --enable-egl=no --disable-valgrind \
			--enable-silent-rules --enable-png=no  --enable-xlib=no \
			--enable-drm=no --enable-xcb-drm=no --enable-drm-xr=no --disable-lto  \
			$(with_fpic) CFLAGS="-fno-lto" \
			pixman_CFLAGS="-I$(DEP_INSTALL_DIR)/include/pixman-1" pixman_LIBS="-L$(DEP_INSTALL_DIR)/lib -lpixman-1" --prefix=$(DEP_INSTALL_DIR) && \
		$(MAKE) && $(MAKE) install

clean_cairo:
	cd vendor/cairo; [[ -f Makefile ]] && make distclean || true

clean_pixman:
	cd vendor/pixman; [[ -f Makefile ]] && make distclean || true

clean: clean_cairo clean_pixman
	rm -rf $(TARGET) $(OBJECTS) $(DEP_INSTALL_DIR)
	git clean -xdf
	rm -rf vendor
	git submodule update --init --recursive
	git submodule foreach --recursive git clean -xfd
	git submodule foreach --recursive git reset --hard HEAD

vendor/libretro-common/include/libretro.h:
	@git submodule update --init --recursive

PREFIX := /usr
INSTALLDIR := $(PREFIX)/lib/libretro
install: all
	mkdir -p $(DESTDIR)$(INSTALLDIR)
	cp $(TARGET) $(DESTDIR)$(INSTALLDIR)