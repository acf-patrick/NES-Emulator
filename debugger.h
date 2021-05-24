#pragma once

#include "cpu6502.h"
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text;

// Supposed to be only used for debugging
class SliderBox
{
    SDL_Rect drag;                                  // attach to the texts, it will help us to drag them together (up or down)
    SDL_Rect viewport;                              // the box
    std::map<std::string, Text*> texts;   // the texts inside
    
    // boundaries of the text list
    Text *first = nullptr;
    Text *last;

    SliderBox(const SDL_Rect&);                     // the box as parameter
    ~SliderBox();

    void slide(int);
    void draw();

    // get text object according to the tag
    Text*& operator[](const std::string&);

    // add text
    Text*  push(const std::string&, const std::string&, const SDL_Point&);

friend class Debugger;
};

class Debugger;

// Supposed to be only used for debugging
class Text
{
    static std::vector<Text*> instances;
    static TTF_Font* font;
    static SDL_Color background;    // color of the debugger's background
    
    SDL_Rect& viewport;              // box containing the text

    std::string text = "";
    SDL_Texture* texture = nullptr;
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Point position;
    SDL_Point size;                 // texture size

// update texture according to the datas
    void update();

    static void destroyFont();

    static SDL_Color getBackground();
    static void setBackground(Uint8, Uint8, Uint8, Uint8 a = 255);

public:    
    Text(const std::string&, const SDL_Point&, SDL_Rect&);
    ~Text();

    void draw();
    void draw(const SDL_Rect&); // draw inside what?

    void setColor(Uint8, Uint8, Uint8, Uint8 a = 255);

    std::string getText();
    void setText(const std::string&);

    int getX();
    int getY();
    int getDown();
    int getRight();

friend class SliderBox;
friend class Debugger;
};

// Append a Word
Text& operator<<(Text&, Word);
// Append a string
Text& operator<<(Text&, const std::string&);

class Debugger
{
private:
    std::vector<SDL_Rect> viewport;
    std::map<std::string, Text*> texts;
    std::map<std::string, SliderBox*> sliders;
    Cpu6502& cpu;

    static SDL_Window* window; // show the debugger on a separated window
    static SDL_Renderer* renderer;

    void update();

public:

    Debugger(Cpu6502*);
    ~Debugger();

    void handle(SDL_Event&);
    void draw();

friend class Text;
friend class SliderBox;
};