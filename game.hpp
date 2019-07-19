#include <SDL.h>
#include <SDL_ttf.h>
#pragma once
SDL_Color cubis_color = {r: 0xFF, g: 0x26, b: 0x26, a: 255};
SDL_Color player_color = {r: 0x5B, g: 0x5B, b: 0xFF, a: 255};
SDL_Color points_color = {r: 0xFF, g: 0xFF, b: 0xFF, a: 255};
SDL_Rect rect_roof = {x: 0, y: 0,
							w: SCREEN_WIDTH,
							h: SCREEN_HEIGHT/5
						};
SDL_Rect rect_points = {x: 0, y: 0,
							w: SCREEN_WIDTH/5,
							h: SCREEN_HEIGHT/5
						};
SDL_Rect rect_ceiling = {x: 0, y: SCREEN_HEIGHT-SCREEN_HEIGHT/5,
							w: SCREEN_WIDTH,
							h: SCREEN_HEIGHT/5
						};
int roof_start = SCREEN_HEIGHT/5;
int ceiling_start = SCREEN_HEIGHT-SCREEN_HEIGHT/5;
int player_state = false;
std::vector<Cuby> cubis;
std::vector<bool> cubis_state;
Cuby player;

void mode_game_init() {
	player.set_sdl_color(player_color);
	player.set_size(SCREEN_WIDTH/10, SCREEN_HEIGHT/5);
	player.set_positon(SCREEN_WIDTH/3, ceiling_start-SCREEN_HEIGHT/5);
	
}

void player_change_state() {
	if (!player_state) {
		player.set_positon(SCREEN_WIDTH/3, roof_start);
		player_state = true;
	} else {
		player.set_positon(SCREEN_WIDTH/3, ceiling_start-SCREEN_HEIGHT/5);
		player_state = false;
	}
}

void make_random_cubi() {
    std::random_device rd; // obtain a random number from hardware
    static std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> random_cubi_state(false, true); // define the range
	std::uniform_int_distribution<> random_cubi_height((SCREEN_HEIGHT-SCREEN_HEIGHT/5*2)/4, (SCREEN_HEIGHT-SCREEN_HEIGHT/5*2)/2); // define the range
    cubis_state.push_back(random_cubi_state(eng));
	Cuby tempcubi;
	tempcubi.set_size(SCREEN_WIDTH/10, random_cubi_height(eng));
	tempcubi.set_sdl_color(cubis_color);
	if (cubis_state[cubis_state.size()-1]) {
		tempcubi.set_positon(SCREEN_WIDTH, roof_start);
	} else {
		tempcubi.set_positon(SCREEN_WIDTH, ceiling_start-tempcubi.get_rect().h);
	}
	cubis.push_back(tempcubi);

}


bool check_cubis() {
	bool end = false;
	int old_index = -1;
	for(auto it = cubis.begin(); it != cubis.end(); ++it) {
		auto i = std::distance(cubis.begin(), it);
		if (cubis[i].get_rect().x < -SCREEN_WIDTH/10 ) {
				cubis.erase(it);
				old_index = i;
			} 
	}
	for(auto it = cubis_state.begin(); it != cubis_state.end(); ++it) {
		auto i = std::distance(cubis_state.begin(), it);
		if (i == old_index) {
				cubis_state.erase(it);
				
			} 
	}
	for(auto it = cubis.begin(); it != cubis.end(); ++it) {
		auto i = std::distance(cubis.begin(), it); 
		if(cubis[i].get_rect().x > SCREEN_WIDTH/3 && cubis[i].get_rect().x < SCREEN_WIDTH/3+SCREEN_WIDTH/10) {
			if (cubis_state[i] == player_state) {
				end = true;
			}
		}
		
		
	}
	
	return end;
}
