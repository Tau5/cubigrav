OBJS = main.cpp
CC = g++
#SDL_VERSION=2.0.9
#SDLTTF_VERSION=2.0.15
COMPILER_FLAGS = -O2 -Wall
LINKER_FLAGS = -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf
OBJ_NAME = cubigrav
MINGW= x86_64-w64-mingw32
all : $(OBJS) 
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug : $(OBJS) 
	$(CC) $(OBJS) -g $(LINKER_FLAGS) -o $(OBJ_NAME) 

.PHONY: install
install:
	mkdir -p $(DESTDIR)/usr/share/applications/
	cp installer/cubigrav.desktop $(DESTDIR)/usr/share/applications/
	mkdir -p $(DESTDIR)/usr/share/icons/hicolor/32x32/apps/
	cp installer/data/cubicolor.png $(DESTDIR)/usr/share/icons/hicolor/32x32/apps/cubigrav.png
	mkdir -p $(DESTDIR)/usr/share/cubigrav
	cp cubigrav $(DESTDIR)/usr/share/cubigrav
	chmod a+x $(DESTDIR)/usr/share/cubigrav/cubigrav
	mkdir -p $(DESTDIR)/usr/share/cubigrav/
	cp installer/data/PixelOperator-Bold.ttf $(DESTDIR)/usr/share/cubigrav/
	mkdir -p $(DESTDIR)/usr/bin/
	cp install/launch.sh $(DESTDIR)/usr/bin/cubigrav
	chmod a+x $(DESTDIR)/usr/bin/cubigrav


clean:
	rm -rf cubigrav cubigrav.exe highscores build windows *.AppImage AppDir appimage-build

win:
	$(MINGW)-g++ main.cpp -o $(OBJ_NAME).exe -lmingw32 -ISDL2-$(SDL_VERSION)/$(MINGW)/include/SDL2 -LSDL2-$(SDL_VERSION)/$(MINGW)/lib/ -ISDL2_ttf-$(SDLTTF_VERSION)/$(MINGW)/include/SDL2 -LSDL2_ttf-$(SDLTTF_VERSION)/$(MINGW)/lib -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive
	
	mkdir -p windows
	cp SDL2-$(SDL_VERSION)/$(MINGW)/bin/*.dll windows/
	cp SDL2_ttf-$(SDLTTF_VERSION)/$(MINGW)/bin/*.dll windows/
	cp PixelOperator-Bold.ttf windows/
	cp cubigrav.exe windows/

appimage: 
	# Requires skip-test because it fails in CentOS 8, could not find the reason why it fails
	# but it works on modern systems
	appimage-builder --skip-test
