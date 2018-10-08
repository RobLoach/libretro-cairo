TARGET_NAME := cairo

include Makefile.libretro

ifeq ($(STATIC_LINKING),1)
	FLAGS += -DSTATIC_LINKING
endif

FLAGS += -D__LIBRETRO__ $(COREDEFINES) $(ENDIANNESS_DEFINES) $(PLATFORM_DEFINES) $(WARNINGS) $(fpic)

CXXFLAGS += $(FLAGS) -std=c++14
CFLAGS += $(FLAGS) -std=gnu99

# Ignore first attempt builds, and re-try for a cleaner dependency chain.
all: $(TARGET)
	$(MAKE) $(TARGET)

$(TARGET): $(OBJECTS) | vendor/libretro-common/include/libretro.h
	-$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp | vendor/libretro-common/include/libretro.h
	-$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.c | vendor/libretro-common/include/libretro.h
	-$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.m | vendor/libretro-common/include/libretro.h
	-$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJECTS)
	git clean -xdf
	rm -rf vendor
	git reset --hard HEAD
	git submodule update -f --init --recursive
	git submodule foreach --recursive git clean -xfd
	git submodule foreach --recursive git reset --hard HEAD

vendor/libretro-common/include/libretro.h:
	@git submodule update --init --recursive

submodules-update:
	@git submodule update --remote

test: all
	@echo "No tests"

PREFIX := /usr
INSTALLDIR := $(PREFIX)/lib/libretro
install: all
	mkdir -p $(DESTDIR)$(INSTALLDIR)
	cp $(TARGET) $(DESTDIR)$(INSTALLDIR)
