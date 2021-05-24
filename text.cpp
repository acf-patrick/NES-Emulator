#include "nes.h"
#include "defs.h"
#include "debugger.h"
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

Text::Text(const std::string& t, const SDL_Point& p, SDL_Rect& v) : 
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
    for (auto& c : text)
        if ((int)c >= (int)'a' and (int)c <= (int)'z')
            c -= 32;
    SDL_Surface* tmp = TTF_RenderText_Shaded(font, text.c_str(), color, background);
    if (!tmp)
    {
        std::cerr << "Unable to write text : " << text << std::endl;
        exit(1);
    }
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(Debugger::renderer, tmp);
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

void Text::draw(const SDL_Rect& box)
{
    SDL_Rect src,
    dst = {  // use the texture size.
        position.x + viewport.x, position.y + viewport.y,
        size.x, size.y
    };

 // show only what's inside the viewport, in this case, the src part of the texture
    SDL_IntersectRect(&box, &dst, &src);

    src.x = src.y = 0;

    dst.w = src.w;          // We don't want the texture to fill the whole screen
    dst.h = src.h;

    SDL_RenderCopy(Debugger::renderer, texture, &src, &dst);
}

void Text::draw()
{
    draw(viewport);
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