# Makefile minimal pour CodeManager (CLI/GUI)
APP := CodeManager
SRC =	Main.cpp	\
        src/application/Application.cpp \
        src/data/Data.cpp	\
        src/software/Software.cpp	\
        src/audio/Audio.cpp	\
        src/graphic/HeaderGraphic.cpp   \
        src/graphic/BodyGraphic.cpp
OBJS := $(SRCS:.cpp=.o)

# Linux build natif
CXX := g++
CXXFLAGS := -O2 -pipe -Wall -Wextra -DNDEBUG
LDFLAGS := 
LIBS :=    # ex: -lstdc++fs -lX11 -lQt5Core -lQt5Widgets

# Installation
PREFIX ?= /usr
DESTDIR ?= 
INSTALL ?= install

# Cross-compilation Windows (64-bit)
MX := x86_64-w64-mingw32
WCXX := $(MX)-g++
WRC := $(MX)-windres
WIN_CXXFLAGS := -O2 -pipe -static-libgcc -static-libstdc++ -DNDEBUG
WIN_LDFLAGS := 
WIN_LIBS :=   # ex: -lws2_32 -luser32 -lgdi32

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Installation Linux dans $(DESTDIR)$(PREFIX)
install: $(APP)
	$(INSTALL) -d "$(DESTDIR)$(PREFIX)/bin"
	$(INSTALL) -m 0755 "$(APP)" "$(DESTDIR)$(PREFIX)/bin/$(APP)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/$(APP)"

clean:
	rm -f $(APP) $(APP).exe $(OBJS) $(OBJS:.o=.d)
	find . -name "*.o" -o -name "*.d" -delete || true
	rm -rf dist

-include $(OBJS:.o=.d)

# Paquet tar.gz "portable" Linux (bin + script de lancement)
package: all
	mkdir -p dist/$(APP)/bin dist/$(APP)/lib
	cp "$(APP)" dist/$(APP)/bin/
	printf '%s\n' '#!/usr/bin/env bash' 'DIR=$$(cd "$$(dirname "$$0")" && pwd)' \
	'export LD_LIBRARY_PATH="$$DIR/../lib:$$LD_LIBRARY_PATH"' 'exec "$$DIR/../bin/$(APP)" "$$@"' > dist/$(APP)/run.sh
	chmod +x dist/$(APP)/run.sh
	tar -C dist -czf $(APP)-linux-x86_64.tar.gz $(APP)

# Cross-build Windows .exe (nécessite mingw-w64)
win: $(SRCS)
	$(WCXX) $(WIN_CXXFLAGS) $(WIN_LDFLAGS) -o $(APP).exe $(SRCS) $(WIN_LIBS)

# Zip Windows basique
win-zip: win
	mkdir -p dist/win
	cp "$(APP).exe" dist/win/
	cd dist && zip -r ../$(APP)-windows-x86_64.zip win
