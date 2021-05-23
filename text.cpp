#include "nes.h"
#include "defs.h"
#include "text.h"
#include <sstream>
#include <iostream>

TTF_Font* Text::font = nullptr;
SDL_Color Text::background = { 15, 16, 17, 255 };
std::vector<Text*> Text::instances;

void Text::destroyFont()
{
    TTF_CloseFont(font);
    font = nullptr;
}

SDL_Color Text::getBackground()
{ return background; }
void Text::setBackground(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    background.r = r;
    background.g = g;
    background.b = b;
    background.a = a;

    // inform all of the texts that the bg color has changed
    for (auto& text : instances)
        text->update();
}

Text::Text(const std::string& t, const SDL_Point& p, const SDL_Rect& v) : 
    viewport(v), position(p)
{
    if (!font)
        font = TTF_OpenFont("Ubuntu-M.ttf", 15);
    setText(t);
    instances.push_back(this);
}

Text::~Text()
{
    SDL_DestroyTexture(texture);
}

void Text::update()
{
    SDL_Surface* tmp = TTF_RenderText_Shaded(font, text.c_str(), color, background);
    if (!tmp)
    {
        std::cerr << "Unable to write text : " << text << std::endl;
        exit(1);
    }
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(Nes::renderer, tmp);
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    SDL_FreeSurface(tmp);
}

void Text::setText(const std::string& txt)
{
    text = txt;

    update();
}

std::string Text::getText()
{ return text; }

void Text::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    update();
}

void Text::draw()
{
    SDL_Rect src, 
    dst = { 
        viewport.x + position.x, viewport.y + position.y,
        0, 0
    }; // enter the text inside its respective box

    src.x = src.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

// use the texture size. We don't want the texture to fill the whole screen
    dst.w = size.x;
    dst.h = size.y;

    SDL_RenderCopy(Nes::renderer, texture, &src, &dst);
}

int Text::getX()
{
    return position.x;
}
int Text::getY()
{
    return position.y;
}
int Text::getDown()
{
    return position.y + size.y;
}
int Text::getRight()
{
    return position.x + size.x;
}

Text& operator<< (Text& text, const std::string& s)
{
    text.setText(text.getText()+s);
    return text;
}
Text& operator<< (Text& text, Word n)
{
    std::stringstream ss;
    ss << std::hex << n;
    text << ss.str();
    return text;
}