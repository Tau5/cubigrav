#include <SDL.h>
class Cuby {
    public:
        SDL_Rect get_rect();
        Cuby();
        void set_color(Uint32 r ,Uint32 g ,Uint32 b ,Uint32 a);
        void set_sdl_color(SDL_Color);
        SDL_Color get_color();
        void move_by(Uint32 x, Uint32 y);
        void set_positon(Uint32 x, Uint32 y);
        void set_size(Uint32 w, Uint32 h);
        void render(SDL_Renderer* renderer);
    private:
        SDL_Rect cuby_rect;
        SDL_Color cuby_color;
};
        Cuby::Cuby() {
            cuby_rect.x = 0;
            cuby_rect.y = 0;
            cuby_rect.w = 0;
            cuby_rect.h = 0;
            cuby_color = {r: 0, g: 0, b: 0, a:0};
        }
        void Cuby::set_color(Uint32 r ,Uint32 g ,Uint32 b ,Uint32 a){
            cuby_color.r = r;
            cuby_color.g = g;
            cuby_color.b = b;
            cuby_color.a = a;
        } 
        void Cuby::set_sdl_color(SDL_Color color) {
            cuby_color = color;
        }
        SDL_Color Cuby::get_color(){
            return cuby_color;
        }
        SDL_Rect Cuby::get_rect() {
            return cuby_rect;
        }
        void Cuby::move_by(Uint32 x, Uint32 y) {
            cuby_rect.x += x;
            cuby_rect.y += y;
        }
        void Cuby::set_positon(Uint32 x, Uint32 y) {
            cuby_rect.x = x;
            cuby_rect.y = y;
        }
        void Cuby::set_size(Uint32 w, Uint32 h) {
            cuby_rect.w = w;
            cuby_rect.h = h;
        }
        void Cuby::render(SDL_Renderer* renderer) {
            SDL_SetRenderDrawColor(renderer, cuby_color.r, cuby_color.g, cuby_color.b, cuby_color.a);
            SDL_RenderFillRect(renderer, &cuby_rect);
        }