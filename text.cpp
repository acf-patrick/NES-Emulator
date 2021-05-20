#include "text.h"
#include <iostream>

TTF_Font* Text::font = nullptr;
SDL_Color Text::background = { 0, 0, 0, 255 };

void Text::destroyFont()
{
    TTF_CloseFont(font);
    font = nullptr;
}
void Text::setBackground(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    background.r = r;
    background.g = g;
    background.b = b;
    background.a = a;
}

Text::Text(const std::string& t, const SDL_Point& p, SDL_Renderer* r) : 
    renderer(r), position(p)
{
    if (!font)
        font = TTF_OpenFont("Ubunt-M.ttf", 25);
    setText(t);
}

Text::~Text()
{
    SDL_DestroyTexture(texture);
}

void Text::setText(const std::string& txt)
{
    text = txt;
    SDL_Surface* tmp = TTF_RenderText_Shaded(font, text.c_str(), color, background);
    if (!tmp)
    {
        std::cerr << "Unable to write text : " << txt << std::endl;
        exit(1);
    }
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
}
std::string Text::getText()
{ return text; }

void Text::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    // update texture
    setText(text);
}

void Text::draw()
{
    SDL_Rect src = { 0 }, dst = { position.x, position.y };
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
    dst.w = src.w;
    dst.h = src.h;
    SDL_RenderCopy(renderer, texture, &src, &dst);
}