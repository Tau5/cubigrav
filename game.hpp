#include <SDL.h>
#include <SDL_ttf.h>
#pragma once
SDL_Color cubis_color = {r: 0xFF, g: 0x26, b: 0x26, a: 255};
SDL_Color player_color = {r: 0x5B, g: 0x5B, b: 0xFF, a: 255};
SDL_Color points_color = {r: 0xFF, g: 0xFF, b: 0xFF, a: 255};

int roof_start = SCREEN_HEIGHT/5;
int ceiling_start = SCREEN_HEIGHT-SCREEN_HEIGHT/5;
int PLAYER_POSX = SCREEN_WIDTH/3;
int ENEMY_WIDTH = SCREEN_WIDTH/10;
int player_state = false;

std::vector<Cuby> cubis;
std::vector<bool> cubis_state;
Cuby player;

void mode_game_init() {
	player.set_sdl_color(player_color);
	player.set_size(SCREEN_WIDTH/10, SCREEN_HEIGHT/5);
	player.set_positon(PLAYER_POSX, ceiling_start-SCREEN_HEIGHT/5);
	
}

void update_player_pos() {
	if (player_state) {
		player.set_positon(SCREEN_WIDTH/3, roof_start);
	} else {
		player.set_positon(SCREEN_WIDTH/3, ceiling_start-SCREEN_HEIGHT/5);
	}
}
void player_change_state() {
	if (!player_state) {
		player_state = true;
	} else {
		player_state = false;
	}
	update_player_pos();
}

void make_random_cubi() {
    std::random_device rd; // obtain a random number from hardware
    static std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> random_cubi_state(false, true); // define the range
	std::uniform_int_distribution<> random_cubi_height((SCREEN_HEIGHT-SCREEN_HEIGHT/5*2)/4, (SCREEN_HEIGHT-SCREEN_HEIGHT/5*2)/2); // define the range
    cubis_state.push_back(random_cubi_state(eng));
	Cuby tempcubi;
	tempcubi.set_size(ENEMY_WIDTH, random_cubi_height(eng));
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
	if (cubis.size() < 1) return end;
	for(auto it = cubis.begin(); it != cubis.end(); ++it) {
		auto i = std::distance(cubis.begin(), it);
		if (cubis[i].get_rect().x < -1600 ) {
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
		auto target = &cubis[i];
		if(target->get_rect().x-ENEMY_WIDTH < PLAYER_POSX-30 && target->get_rect().x-30 > PLAYER_POSX-ENEMY_WIDTH) {
			if (cubis_state[i] == player_state) {
				end = true;
			}
		}
		
		
	}
	
	return end;
}
