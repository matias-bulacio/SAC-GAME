LINUX_CC=gcc
WINDOWS_CC=x86_64-w64-mingw32-gcc
CFLAGS=-O2 -Wall -Wextra -MMD -MP
CFLAGS_LINUX= -DPLATFORM_LINUX
CFLAGS_WINDOWS= -DPLATFORM_WINDOWS

SOURCES=$(wildcard Source/*.c Source/Implementation/*.c)
OBJECTS_LINUX=$(patsubst %.c,Build/Linux/%.o,$(SOURCES))
OBJECTS_WINDOWS=$(patsubst %.c,Build/Windows/%.o,$(SOURCES))
DEPENDENCIES_LINUX=$(OBJECTS_LINUX:.o=.d)
DEPENDENCIES_WINDOWS=$(OBJECTS_WINDOWS:.o=.d)

COMMON_INCLUDE=-ISource/Headers/

LINUX_INCLUDE=-ILibraries/Include/Linux/
WINDOWS_INCLUDE=-ILibraries/Include/Windows/

STATIC_LINUX_LIBS = -lraylib
DYNAMIC_LINUX_LIBS= -lm -lGL -lpthread -ldl -lrt -lX11
LINUX_LIBS        = -LLibraries/Static/Linux/ -Wl,-Bstatic $(STATIC_LINUX_LIBS) -Wl,-Bdynamic $(DYNAMIC_LINUX_LIBS)

STATIC_WINDOWS_LIBS = -lraylib
DYNAMIC_WINDOWS_LIBS= -lgdi32 -lwinmm
WINDOWS_LIBS= -LLibraries/Static/Windows/ -Wl,-Bstatic $(STATIC_WINDOWS_LIBS) -Wl,-Bdynamic $(DYNAMIC_WINDOWS_LIBS)

linux: Binaries/Linux/game
windows: Binaries/Windows/game.exe

Binaries/Linux/game: $(OBJECTS_LINUX) | Binaries/Linux
	$(LINUX_CC) -o $@ $^ $(LINUX_LIBS)
	rm -rf Binaries/Linux/Resources
	cp -r Resources Binaries/Linux/Resources

Build/Linux/%.o: %.c | Build/Linux
	mkdir -p $(dir $@)
	$(LINUX_CC) $(CFLAGS) $(CFLAGS_LINUX) $(COMMON_INCLUDE) $(LINUX_INCLUDE) -c -o $@ $< -MF $(@:.o=.d)

Binaries/Windows/game.exe: $(OBJECTS_WINDOWS) | Binaries/Windows
	$(WINDOWS_CC) -o $@ $^ $(WINDOWS_LIBS)
	rm -rf Binaries/Linux/Resources
	cp -r Resources Binaries/Windows/Resources

Build/Windows/%.o: %.c | Build/Windows
	mkdir -p $(dir $@)
	$(WINDOWS_CC) $(CFLAGS) $(CFLAGS_WINDOWS) $(COMMON_INCLUDE) $(WINDOWS_INCLUDE) -c -o $@ $< -MF $(@:.o=.d)

Build/Linux Build/Windows Binaries/Linux Binaries/Windows:
	mkdir -p $@

-include $(DEPENDENCIES_LINUX) $(DEPENDENCIES_WINDOWS)

all: linux

.PHONY: all linux windows

clean:
	rm -rf Build Binaries
	mkdir -p Build/Linux Build/Windows
	mkdir -p Binaries/Linux Binaries/Windows
	touch Build/.gitkeep Binaries/.gitkeep
