
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <random>
#include <string>
#include <cmath>
#include "global.hpp"
#include "cuby.hpp"
#include "texty.hpp"
#include "highscore.hpp"
#include "other.hpp"
#include "game.hpp"
#include "timer.hpp"
Uint32 generator_timeout = 365 * 5;
float cubi_update_timeout = 0;
Uint32 animation_timeout = 60;
Timer generator_timer;
Timer cubi_update_timer;
Timer animation_timer;
bool be_nice_and_dont_burn_the_cpu = true;
Uint32 time_step_ms = 1000 / 60;
Uint32 next_game_step = SDL_GetTicks(); // initial value

int lastPoints = 0;
int madness = 1;
int points = 0;
float cubi_speed = 18;

enum game_mode{
  TITLE, HELP, GAME, OVER
};

game_mode mode = TITLE;
SDL_Window *gwindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Texture *gHelloWorld = NULL;
SDL_Event e;
SDL_Rect imgRect;
SDL_Renderer *gRenderer = NULL;
SDL_Rect playerRect;
TTF_Font *font = NULL;
std::string hs_text = "";


SDL_Rect pointsRect, hsRect, timer_rect, rect_roof, rect_ceiling, rect_points;
SDL_Color madness_colors[] = {
    {124, 250, 72, 255},
    {114, 159, 207, 255},
    {252, 233, 79, 255},
    {252, 175, 62, 255},
    {239, 41, 41, 255},
    {166, 61, 183, 255},
    {136, 138, 133, 255},
    {0, 0, 0, 255}};

void update_rects()
{
  pointsRect = {
    x : 0,
    y : 0,
    w : SCREEN_WIDTH / 8,
    h : SCREEN_HEIGHT / 3
  };
  hsRect = {
    x : SCREEN_WIDTH - SCREEN_WIDTH / 3,
    y : 0,
    w : SCREEN_WIDTH / 4,
    h : SCREEN_HEIGHT / 4
  };
  timer_rect = {
    x : 0,
    y : SCREEN_HEIGHT - SCREEN_HEIGHT / 4,
    w : SCREEN_WIDTH,
    h : SCREEN_HEIGHT / 4
  };
  rect_roof = {x : 0, y : 0, w : SCREEN_WIDTH, h : SCREEN_HEIGHT / 5};
  rect_points = {x : 0, y : 0, w : SCREEN_WIDTH / 5, h : SCREEN_HEIGHT / 5};
  rect_ceiling = {x : 0, y : SCREEN_HEIGHT - SCREEN_HEIGHT / 5, w : SCREEN_WIDTH, h : SCREEN_HEIGHT / 5};
  roof_start = SCREEN_HEIGHT / 5;
  ceiling_start = SCREEN_HEIGHT - SCREEN_HEIGHT / 5;
  PLAYER_POSX = SCREEN_WIDTH / 3;
  ENEMY_WIDTH = 64;
  player.set_size(64, SCREEN_HEIGHT / 5);
  update_player_pos();
}

SDL_Color timer_color = {0xB8, 0x9A, 0xFE}; // B89AFE
bool newhs = false;
int current_cubi = 0;
bool loadMedia();
void close();
void render();
void restart();
SDL_DisplayMode dm;
int GLOBAL_SCREEN_WIDTH = 1;
int GLOBAL_SCREEN_HEIGHT = 1;


int init()
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return -1;
  } else {
    gwindow = SDL_CreateWindow("CubiGrav", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gwindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      return -2;
    } else {
      gRenderer = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      gScreenSurface = SDL_GetWindowSurface(gwindow);
      TTF_Init();
      font = TTF_OpenFont("./PixelOperator-Bold.ttf", 16);
      hs_text = "High score: " + std::to_string(get_highscore());
      // GAME MODE INIT
      mode_game_init();

      // TIMER INIT
      cubi_update_timer.reset_timer(cubi_update_timeout);
      generator_timer.reset_timer(generator_timeout);
    }
  }

  //  Initialize Game Mechanics
  update_rects();
    if (loadMedia()) 
    {
      if (SDL_GetDesktopDisplayMode(0, &dm) != 0) 
      {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return -3;
      }
    } else
        return -4;
    

    GLOBAL_SCREEN_WIDTH = dm.w;
    GLOBAL_SCREEN_HEIGHT = dm.h;


  return 0;
}

bool loadMedia()
{
  bool success = true;
  return success;
}

void update_screen_size()
{
  update_rects();
  SDL_SetWindowSize(gwindow, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (madness < 2)
    SDL_SetWindowPosition(gwindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void reset_screen_size()
{
  SCREEN_HEIGHT = INITIAL_SCREEN_HEIGHT;
  SCREEN_WIDTH = INITIAL_SCREEN_WIDTH;
  update_screen_size();
}
void set_difficulty_highway()
{
  generator_timeout = 300/cubi_speed;
}

int difficulty_timeout = 0;
int madness2_offset = 25;



void adjust_difficulty() {
  if (madness > 1) {
    SCREEN_HEIGHT = (200 - (points - madness2_offset) > 100 ? 200 - (points - madness2_offset) : 100);
    SCREEN_WIDTH = ((points - madness2_offset) + INITIAL_SCREEN_WIDTH < GLOBAL_SCREEN_WIDTH ? (points - madness2_offset) + INITIAL_SCREEN_WIDTH : GLOBAL_SCREEN_WIDTH);
  }
  update_screen_size();
  cubi_speed = powf(points+50, (1/(float)2)) < 18 ? powf(points+50, (1/(float)2)) : 18;
  generator_timeout = (300 - (int)std::round(powf((points+50)*50, (0.5))))/cubi_speed;//(200 - points > 100 ? 200 - points : 100);
  if (generator_timeout < 150/cubi_speed) generator_timeout = 150/cubi_speed;
  if (points % 25 == 0 && points > 0)
  {
    madness++;
  }
  if (madness <= 8)
  {
    cubis_color = madness_colors[madness - 1];
  }
  cubi_update_timeout = 1;
}

const bool UP   = 1;
const bool DOWN = 0;
void adjust_window_position(const bool direction){
  int separation = (7 - madness > 1 ? 7 - madness : 1);

  if(direction == UP){
    if (madness > 3 && madness < 8)
      SDL_SetWindowPosition(gwindow, GLOBAL_SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2,
                          GLOBAL_SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / separation));
    if (madness >= 8)
      SDL_SetWindowPosition(gwindow, GLOBAL_SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, 0);
  } else{
    if (madness > 3 && madness < 8)
      SDL_SetWindowPosition(gwindow, GLOBAL_SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2,
                        GLOBAL_SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT / separation));
    if (madness >= 8)
      SDL_SetWindowPosition(gwindow, GLOBAL_SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2,
                          GLOBAL_SCREEN_HEIGHT - SCREEN_HEIGHT);
  } 
}



void render_frame(){
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(gRenderer);
  if (mode == GAME)
  {
    player.render(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, cubis_color.r, cubis_color.g, cubis_color.b, 0xFF);
    const SDL_Rect rect_array[2] = {rect_ceiling, rect_roof};
    SDL_RenderFillRects(gRenderer, rect_array, 2);
    for (int i = 0; i < (int)cubis.size(); i++)
    {
      cubis[i].move_by(-cubi_speed, 0);
      cubis[i].render(gRenderer);
    }
    if (cubi_update_timer.get_timer_status())
    {
      bool kill = check_cubis();
      cubi_update_timer.reset_timer(cubi_update_timeout);
      if (kill)
      {
        restart();
      }
    }
    if (animation_timer.get_timer_status())
    {
      animation_timer.reset_timer(animation_timeout);
    }
    if (generator_timer.get_timer_status())
    {
      points++;
      adjust_difficulty();
      generator_timer.reset_timer(generator_timeout);
      make_random_cubi();
    }
    render_text(gRenderer, font, std::to_string(points), points_color, rect_points);
  }
  if (mode == TITLE)
  {
    SDL_Rect titleR{
      x : SCREEN_WIDTH / 2 - SCREEN_WIDTH / 5,
      y : SCREEN_HEIGHT / 10,
      w : SCREEN_WIDTH / 3,
      h : SCREEN_HEIGHT / 5
    };
    render_text(gRenderer, font, "CUBI GRAV", {0, 0, 0}, titleR);
    titleR.y = SCREEN_HEIGHT - SCREEN_HEIGHT / 4;
    render_text(gRenderer, font, "Press H for help, press any other key to start", {0, 0, 0}, titleR);
  }
  if (mode == OVER)
  {
    SDL_Rect titleR{
      x : SCREEN_WIDTH / 2 - SCREEN_WIDTH / 5,
      y : SCREEN_HEIGHT / 10,
      w : SCREEN_WIDTH / 3,
      h : SCREEN_HEIGHT / 5
    };
    SDL_Rect pointsR{
      x : SCREEN_WIDTH / 2 - SCREEN_WIDTH / 5,
      y : SCREEN_HEIGHT / 3,
      w : SCREEN_WIDTH,
      h : SCREEN_HEIGHT / 3
    };
    render_text(gRenderer, font, "GAME OVER", {0, 0, 0}, titleR);
    titleR.y = SCREEN_HEIGHT - SCREEN_HEIGHT / 4;
    titleR.w = SCREEN_WIDTH / 2;
    titleR.x = SCREEN_WIDTH / 4;
    render_text(gRenderer, font, "Press space to go to the title screen", {0, 0, 0}, titleR);
    std::string pointsTXT = "Points: " + std::to_string(lastPoints);
    if (newhs)
    {
      pointsTXT += "\nNEW RECORD!";
    }
    else
    {
      pointsTXT += "\nRecord: " + std::to_string(get_highscore());
    }
    render_text(gRenderer, font, pointsTXT, {0, 0, 0}, pointsR);
  }
  if (mode == HELP)
  {
    SDL_Rect keyR{
      x : SCREEN_WIDTH / 4,
      y : SCREEN_HEIGHT - SCREEN_HEIGHT / 4,
      w : SCREEN_WIDTH / 2,
      h : SCREEN_HEIGHT / 5
    };
    SDL_Rect helpR{
      x : 0,
      y : 0,
      w : SCREEN_WIDTH,
      h : SCREEN_HEIGHT / 2
    };
    render_text(gRenderer, font, HELP_STR, {0, 0, 0}, helpR);
    render_text(gRenderer, font, "Press Return/Enter to go to the title screen", {0xF, 0xF, 0xF}, keyR);
  }
  SDL_RenderPresent(gRenderer);
}

void render()
{
  Uint32 now = SDL_GetTicks();

  if(next_game_step <= now /*|| vsync_enabled*/) {

      int computer_is_too_slow_limit = 10; // max number of advances per render, adjust this according to your minimum playable fps

      // Loop until all steps are executed or computer_is_too_slow_limit is reached
      while((next_game_step <= now) && (computer_is_too_slow_limit--)){
        //AdvanceGameLogicBy1Step();
        generator_timer.update_timer();
        cubi_update_timer.update_timer();
        next_game_step += time_step_ms; // count 1 game tick done
      }

      render_frame();

    } else {
      // we're too fast, wait a bit.
      if(be_nice_and_dont_burn_the_cpu){
        SDL_Delay(next_game_step - now);
      }
      }


  
}
void close()
{
  SDL_DestroyTexture(gHelloWorld);
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gwindow);
  SDL_Quit();
}

void restart()
{
  reset_screen_size();
  current_cubi = -1;
  newhs = update_highscore(points);
  hs_text = "High score: " + std::to_string(get_highscore());
  lastPoints = points;
  points = 0;
  madness = 1;
  mode = OVER;
  cubis.clear();
  cubis_state.clear();
  render();
}



int main(int argc, char *args[])
{
  if(init() < 0){
    close();
    return -1;
  }

  while (e.type != SDL_QUIT){
    
    SDL_PollEvent(&e);

    render();


    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

    if (e.type == SDL_KEYDOWN) {

      switch(mode){
        case GAME:
          if (keyboard[SDL_SCANCODE_UP]) {
            if (!player_state) {
              adjust_window_position(UP);
              player_change_state();
            }
          } else if (keyboard[SDL_SCANCODE_DOWN]) {
            if (player_state) {
              adjust_window_position(DOWN);
              player_change_state();
            }
          }
          break;

        case TITLE:
          if (keyboard[SDL_SCANCODE_H])
            mode = HELP;
          else
          {
            adjust_difficulty();
            cubi_update_timer.reset_timer(cubi_update_timeout);
            generator_timer.reset_timer(generator_timeout);
            animation_timer.reset_timer(animation_timeout);
            mode = GAME;
          }
          break;

        case HELP:
          if (keyboard[SDL_SCANCODE_RETURN])
            mode = TITLE;
          break;

        case OVER:
          if(keyboard[SDL_SCANCODE_SPACE])
            mode = TITLE;
          break;
      }



    }
  
  }
  
  
  
  close();
  return 0;
}

/* Everything is a smoke machine if you operate ir wrong enough */
