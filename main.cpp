
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <random>
#include <string>
#include "global.hpp"
#include "cuby.hpp"
#include "texty.hpp"
#include "highscore.hpp"
#include "other.hpp"
#include "game.hpp"
#include "timer.hpp"
Uint32 generator_timeout = 500; 
Uint32 cubi_update_timeout = 2;

Timer generator_timer;
Timer cubi_update_timer;
int points = 0;
int lastPoints = 0;
string mode = "title";
SDL_Window *gwindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Texture *gHelloWorld = NULL;
SDL_Event e;
SDL_Rect imgRect;
SDL_Renderer* gRenderer = NULL;
SDL_Rect playerRect;
TTF_Font* font = NULL;
string hs_text = "";
enum cubi_types {
  CUBI_UP,
  CUBI_RIGHT,
  CUBI_DOWN,
  CUBI_LEFT
};
int cubis_type[5];

SDL_Rect pointsRect = {
  x: 0,
  y: 0,
  w: SCREEN_WIDTH/8,
  h: SCREEN_HEIGHT/3
};
SDL_Rect hsRect = {
  x: SCREEN_WIDTH-SCREEN_WIDTH/3,
  y: 0,
  w: SCREEN_WIDTH/4,
  h: SCREEN_HEIGHT/4
};
SDL_Rect timer_rect {
  x: 0,
  y: SCREEN_HEIGHT- SCREEN_HEIGHT/4,
  w: SCREEN_WIDTH,
  h: SCREEN_HEIGHT/4
};
SDL_Color timer_color = {0xB8, 0x9A, 0xFE}; //B89AFE
bool newhs = false;
int current_cubi = 0;
bool quit = false;
bool init();
bool loadMedia();
void close();
void render();
void restart();
int main(int argc, char *args[]) {
  if (init()) {
    if (loadMedia()) {
      while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
          if (e.type == SDL_QUIT) {
            quit = true;
          }
            const Uint8* keyboard = SDL_GetKeyboardState(NULL);
            if (e.type == SDL_KEYDOWN) {
            if (mode == "game") {
            
              if (keyboard[SDL_SCANCODE_UP]) {
                if (!player_state) {
                  player_change_state();
                }
              } else if (keyboard[SDL_SCANCODE_DOWN]){
                if (player_state) {
                  player_change_state();
                }
              }
            }

            if (mode == "title") {
                if(keyboard[SDL_SCANCODE_H]) {
                  mode = "help";
                } else {
                  mode = "game";
                }
                
              }
              if (mode == "help") {
                if(keyboard[SDL_SCANCODE_RETURN]) {
                  mode = "title";
                }
              }
              if (mode == "over" && keyboard[SDL_SCANCODE_SPACE]) mode = "title";
              
              
            } 
            
          }
          
        
        render();
        if (mode == "game") {
        }
      }
    }
  }
  close();
  return 0;
}
bool init() {
  //Initialize SDL
  bool success = true;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    gwindow = SDL_CreateWindow("CubiGrav", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gwindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer( gwindow, -1, SDL_RENDERER_ACCELERATED );
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      gScreenSurface = SDL_GetWindowSurface(gwindow);
      TTF_Init();
      font = TTF_OpenFont("./PixelOperator-Bold.ttf", 16);
      hs_text = "High score: " + to_string(get_highscore());
      //GAME MODE INIT
      mode_game_init();

      //TIMER INIT
      cubi_update_timer.reset_timer(cubi_update_timeout);
      generator_timer.reset_timer(generator_timeout);
    }
  }
  return success;
}

bool loadMedia() {
  bool success = true;
  return success;
}




void render() {
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(gRenderer);
  if (mode == "game") {
      player.render(gRenderer);
      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
      const SDL_Rect rect_array[2] = {rect_ceiling, rect_roof};
      SDL_RenderFillRects(gRenderer, rect_array, 2);
      for(int i=0; i < cubis.size(); i++) {
        if (cubi_update_timer.get_timer_status()) {
          cubis[i].move_by(-1, 0);
          
        }
        cubis[i].render(gRenderer);
        
      }
      if (cubi_update_timer.get_timer_status()) {
        bool kill = check_cubis();
        cubi_update_timer.reset_timer(cubi_update_timeout);
        if (kill) {
          restart();
        }
      }
      if (generator_timer.get_timer_status()) {
        generator_timer.reset_timer(generator_timeout);
        points++;
        make_random_cubi();
      }
      render_text(gRenderer, font, to_string(points), points_color, rect_points);
  }
  if (mode == "title") {
    SDL_Rect titleR {
      x: SCREEN_WIDTH/2-SCREEN_WIDTH/5,
      y: SCREEN_HEIGHT/10,
      w: SCREEN_WIDTH/3,
      h: SCREEN_HEIGHT/5
    };
    render_text(gRenderer, font, "CUBI GRAV", {0,0,0}, titleR);
    titleR.y = SCREEN_HEIGHT - SCREEN_HEIGHT/4;
    render_text(gRenderer, font, "Press H for help, press any other key to start", {0,0,0}, titleR);
  }
  if (mode == "over") {
    SDL_Rect titleR {
      x: SCREEN_WIDTH/2-SCREEN_WIDTH/5,
      y: SCREEN_HEIGHT/10,
      w: SCREEN_WIDTH/3,
      h: SCREEN_HEIGHT/5
    };
    SDL_Rect pointsR {
      x: SCREEN_WIDTH/2-SCREEN_WIDTH/5,
      y: SCREEN_HEIGHT/3,
      w: SCREEN_WIDTH,
      h: SCREEN_HEIGHT/3
    };
    render_text(gRenderer, font, "GAME OVER", {0,0,0}, titleR);
    titleR.y = SCREEN_HEIGHT - SCREEN_HEIGHT/4;
    titleR.w = SCREEN_WIDTH/2;
    titleR.x = SCREEN_WIDTH/4;
    render_text(gRenderer, font, "Press space to go to the title screen", {0,0,0}, titleR);
    string pointsTXT = "Points: " + to_string(lastPoints);
    if (newhs) {
      pointsTXT+= "\nNEW RECORD!";
    } else {
      pointsTXT+= "\nRecord: " + to_string(get_highscore());
    }
    render_text(gRenderer, font, pointsTXT, {0,0,0}, pointsR);
  }
  if (mode == "help") {
    SDL_Rect keyR {
      x: SCREEN_WIDTH/4,
      y: SCREEN_HEIGHT - SCREEN_HEIGHT/4,
      w: SCREEN_WIDTH/2,
      h: SCREEN_HEIGHT/5
    };
    SDL_Rect helpR {
      x: 0,
      y: 0,
      w: SCREEN_WIDTH,
      h: SCREEN_HEIGHT/2
    };
    render_text(gRenderer, font, HELP, {0,0,0}, helpR);
    render_text(gRenderer, font, "Press Return/Enter to go to the title screen", {0xF,0xF,0xF}, keyR);
  }
  SDL_RenderPresent(gRenderer);

}
void close() {
  SDL_DestroyTexture(gHelloWorld);
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gwindow);
  SDL_Quit();
}

void restart() {
  current_cubi = -1;
  newhs = update_highscore(points);
  hs_text = "High score: " + to_string(get_highscore());
  lastPoints = points;
  points = 0;
  mode = "over";
  cubis.clear();
  cubis_state.clear();
  render();
}