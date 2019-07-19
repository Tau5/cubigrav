#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

void render_text(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color fg, SDL_Rect rect) {
    SDL_Surface* txtsurface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), fg, rect.w);
    SDL_Texture* text_ure = SDL_CreateTextureFromSurface(renderer, txtsurface);
    SDL_FreeSurface(txtsurface);
    SDL_RenderCopy(renderer, text_ure, NULL, &rect);
    SDL_DestroyTexture(text_ure);
    txtsurface = NULL;
    text_ure = NULL;
}