
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
Uint32 generator_timeout = 365*2; 
float cubi_update_timeout = 0;
Uint32 animation_timeout = 60;
Timer generator_timer;
Timer cubi_update_timer;
Timer animation_timer;

int lastPoints = 0;
int madness = 1;
int points = 0;
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

SDL_Rect pointsRect, hsRect, timer_rect, rect_roof, rect_ceiling, rect_points;
SDL_Color madness_colors[] = {
  {124, 250, 72, 255},
  {114, 159, 207, 255},
  {252, 233, 79, 255},
  {252, 175, 62, 255},
  {239, 41, 41, 255},
  {166, 61, 183, 255},
  {136, 138, 133, 255},
  {0, 0, 0, 255}
};

void update_rects() {
  pointsRect = {
    x: 0,
    y: 0,
    w: SCREEN_WIDTH/8,
    h: SCREEN_HEIGHT/3
  };
  hsRect = {
    x: SCREEN_WIDTH-SCREEN_WIDTH/3,
    y: 0,
    w: SCREEN_WIDTH/4,
    h: SCREEN_HEIGHT/4
  };
  timer_rect = {
    x: 0,
    y: SCREEN_HEIGHT- SCREEN_HEIGHT/4,
    w: SCREEN_WIDTH,
    h: SCREEN_HEIGHT/4
  };
  rect_roof = {x: 0, y: 0,
							w: SCREEN_WIDTH,
							h: SCREEN_HEIGHT/5
						};
  rect_points = {x: 0, y: 0,
                w: SCREEN_WIDTH/5,
                h: SCREEN_HEIGHT/5
              };
  rect_ceiling = {x: 0, y: SCREEN_HEIGHT-SCREEN_HEIGHT/5,
                w: SCREEN_WIDTH,
                h: SCREEN_HEIGHT/5
              };
  roof_start = SCREEN_HEIGHT/5;
  ceiling_start = SCREEN_HEIGHT-SCREEN_HEIGHT/5;
  PLAYER_POSX = SCREEN_WIDTH/3;
  ENEMY_WIDTH = SCREEN_WIDTH/10;
  player.set_size(SCREEN_WIDTH/10, SCREEN_HEIGHT/5);
  update_player_pos();
}

SDL_Color timer_color = {0xB8, 0x9A, 0xFE}; //B89AFE
bool newhs = false;
int current_cubi = 0;
bool quit = false;
bool init();
bool loadMedia();
void close();
void render();
void restart();
SDL_DisplayMode dm;
int GLOBAL_SCREEN_WIDTH = 1;
int GLOBAL_SCREEN_HEIGHT = 1;


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

void update_screen_size() {
  update_rects();
  SDL_SetWindowSize(gwindow, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (madness < 2) SDL_SetWindowPosition(gwindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void reset_screen_size() {
  SCREEN_HEIGHT = INITIAL_SCREEN_HEIGHT; 
  SCREEN_WIDTH = INITIAL_SCREEN_WIDTH;
  update_screen_size();
}
void set_difficulty_highway() {
  generator_timeout = 1200;

}

int difficulty_timeout = 0;
int madness2_offset = 25;
void adjust_difficulty() {
    if (points%50 >= 40 && points > 0) {
      if (points%50 == 40 && madness > 2) {
          SCREEN_HEIGHT = 100;
          SCREEN_WIDTH = 800;
          update_screen_size();
      } else {
          if (madness > 1) {
            SCREEN_HEIGHT = (200 - (points - madness2_offset) > 100 ? 200 - (points - madness2_offset) : 100);
            SCREEN_WIDTH = ((points - madness2_offset) + INITIAL_SCREEN_WIDTH < 1000 ? (points - madness2_offset) + INITIAL_SCREEN_WIDTH : 1000);
            update_screen_size();
          }   
      }
      set_difficulty_highway();
    } else {
      if (madness > 1) {
        SCREEN_HEIGHT = (200 - (points - madness2_offset) > 100 ? 200 - (points - madness2_offset) : 100);
        SCREEN_WIDTH = ((points - madness2_offset) + INITIAL_SCREEN_WIDTH < 1000 ? (points - madness2_offset) + INITIAL_SCREEN_WIDTH : 1000);
      }
      update_screen_size();
      generator_timeout = ( 500 - points*2 > 200 ? 500 - points*2 : 200 );
    }
    if (points%25 == 0 && points > 0) {
      madness++;
      SDL_Log("madness %i", madness);
    }
    if (madness <= 8) {
      cubis_color = madness_colors[madness-1];
    }
    cubi_update_timeout = 1;
}

void render() {
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(gRenderer);
  if (mode == "game") {
      player.render(gRenderer);
      SDL_SetRenderDrawColor(gRenderer, cubis_color.r, cubis_color.g, cubis_color.b, 0xFF);
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
      if (animation_timer.get_timer_status()) {
        animation_timer.reset_timer(animation_timeout);
      }
      if (generator_timer.get_timer_status()) {
        points++;
        adjust_difficulty();
        generator_timer.reset_timer(generator_timeout);
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
  reset_screen_size();
  current_cubi = -1;
  newhs = update_highscore(points);
  hs_text = "High score: " + to_string(get_highscore());
  lastPoints = points;
  points = 0;
  madness = 1;
  mode = "over";
  cubis.clear();
  cubis_state.clear();
  render();
}

int main(int argc, char *args[]) {
  update_rects();
  if (init()) {
    if (loadMedia()) {
      if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
      {
          SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
          return 1;
      }
      GLOBAL_SCREEN_WIDTH = dm.w;
      GLOBAL_SCREEN_HEIGHT = dm.h;
      while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
          if (e.type == SDL_QUIT) {
            quit = true;
          }
            const Uint8* keyboard = SDL_GetKeyboardState(NULL);
            if (e.type == SDL_KEYDOWN) {
            if (mode == "game") {
              int separation = (7 - madness > 1? 7 - madness : 1);
              if (keyboard[SDL_SCANCODE_UP]) {
                if (!player_state) {
                  if (madness > 3 && madness < 8) SDL_SetWindowPosition(gwindow, GLOBAL_SCREEN_WIDTH/2-SCREEN_WIDTH/2, 
                                      GLOBAL_SCREEN_HEIGHT/2-(SCREEN_HEIGHT/2)-(SCREEN_HEIGHT/separation));
                  if (madness >= 8) SDL_SetWindowPosition(gwindow, GLOBAL_SCREEN_WIDTH/2-SCREEN_WIDTH/2, 
                                      0);
                  player_change_state();
                }
              } else if (keyboard[SDL_SCANCODE_DOWN]){
                if (player_state) {
                  if (madness > 3 && madness < 8) SDL_SetWindowPosition(gwindow, GLOBAL_SCREEN_WIDTH/2-SCREEN_WIDTH/2, 
                                      GLOBAL_SCREEN_HEIGHT/2-(SCREEN_HEIGHT/2)+(SCREEN_HEIGHT/separation));
                  if (madness >= 8) SDL_SetWindowPosition(gwindow, GLOBAL_SCREEN_WIDTH/2-SCREEN_WIDTH/2, 
                                      GLOBAL_SCREEN_HEIGHT-SCREEN_HEIGHT);
                  player_change_state();
                }
              }
            }

            if (mode == "title") {
                if(keyboard[SDL_SCANCODE_H]) {
                  mode = "help";
                } else {
                  adjust_difficulty();
                  cubi_update_timer.reset_timer(cubi_update_timeout);
                  generator_timer.reset_timer(generator_timeout);
                  animation_timer.reset_timer(animation_timeout);
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