#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Debugger;

// Supposed to be only used for debugging
class Text
{
private:
    static std::vector<Text*> instances;
    static TTF_Font* font;
    static SDL_Color background;    // color of the debugger's background
    
private:
    SDL_Rect viewport;              // box containing the text

    std::string text = "";
    SDL_Texture* texture = nullptr;
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Point position;
    SDL_Point size;                 // texture size

// update texture according to the datas
    void update();

public:

    static void destroyFont();

    static SDL_Color getBackground();
    static void setBackground(Uint8, Uint8, Uint8, Uint8 a = 255);
    
    Text(const std::string&, const SDL_Point&, const SDL_Rect&);
    ~Text();

    void draw();

    void setColor(Uint8, Uint8, Uint8, Uint8 a = 255);

    std::string getText();
    void setText(const std::string&);

    int getX();
    int getY();
    int getDown();
    int getRight();

friend class Debugger;
};

// set inner text of a Text object. Append
Text& operator<<(Text&, Word);
Text& operator<<(Text&, const std::string&);