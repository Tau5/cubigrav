OBJS = main.cpp
CC = g++
SDL_VERSION=2.0.9
SDLTTF_VERSION=2.0.15
COMPILER_FLAGS = 
LINKER_FLAGS = -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf
OBJ_NAME = cubigrav
MINGW= x86_64-w64-mingw32
all : $(OBJS) 
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

dist : all
	mkdir -p build
	cp -r installer build
	cp cubigrav build/installer/data/cubigrav
	cd build/installer; tar -cvzf ../cubigrav.tar.gz .


win:
	$(MINGW)-g++ main.cpp -o $(OBJ_NAME).exe -lmingw32 -ISDL2-$(SDL_VERSION)/$(MINGW)/include/SDL2 -LSDL2-$(SDL_VERSION)/$(MINGW)/lib/ -ISDL2_ttf-$(SDLTTF_VERSION)/$(MINGW)/include/SDL2 -LSDL2_ttf-$(SDLTTF_VERSION)/$(MINGW)/lib -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive
	
	mkdir -p windows
	cp SDL2-$(SDL_VERSION)/$(MINGW)/bin/*.dll windows/
	cp SDL2_ttf-$(SDLTTF_VERSION)/$(MINGW)/bin/*.dll windows/
	cp PixelOperator-Bold.ttf windows/
	cp cubigrav.exe windows/
