#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text
{
private:
    static TTF_Font* font;
    static SDL_Color background; // color of the debugger's background
    
    SDL_Renderer* renderer; // Used to create the texture of the text and drawing it

    std::string text = "";
    SDL_Texture* texture = nullptr;
    SDL_Color color = { 255 };
    SDL_Point position;

public:

    static void destroyFont();
    static void setBackground(Uint8, Uint8, Uint8, Uint8 a = 255);
    
    Text(const std::string&, const SDL_Point&, SDL_Renderer*);
    ~Text();

    void draw();

    void setColor(Uint8, Uint8, Uint8, Uint8 a = 255);

    std::string getText();
    void setText(const std::string&);
};