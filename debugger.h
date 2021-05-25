#pragma once

#include "cpu6502.h"
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include "SDL2/include/SDL2/SDL_ttf.h"

class Text;
class Debugger;

// Supposed to be only used for debugging
class Box
{
    SDL_Rect drag;                                  // attach to the texts, it will help us to drag them together (up or down)
    SDL_Rect viewport;                              // the box
    std::map<std::string, Text*> texts;             // the texts inside

    SDL_Color background = { 15, 16, 17, 255 },     // Drak Grey
                boxColor = { 255, 255, 255, 255 };  // Color of the bounding box
    
    // boundaries of the text list
    Text *first = nullptr;
    Text *last;

    // put on top of the box
    Text*  label = nullptr;

    Box(const SDL_Rect&);                            // the box as parameter
    ~Box();

    void setLabel(const std::string&);
    void slide(int);
    void draw();

    // get text object according to the tag
    Text*& operator[](const std::string&);

    // add text
    Text*  push(const std::string&, const std::string&, const SDL_Point&);

friend class Debugger;
};

// Supposed to be only used for debugging
class Text
{
protected:
    static SDL_Rect screen;
    static TTF_Font* font;
    
    SDL_Rect& viewport;              // box containing the text

    std::string text = "";
    SDL_Texture* texture = nullptr;
    SDL_Color color = { 37, 196, 246, 255 };
    SDL_Point position;
    SDL_Point size;                 // texture size

// update texture according to the datas
    void update();

    static void destroyFont();

    static SDL_Color getBackground();
    static void setBackground(Uint8, Uint8, Uint8, Uint8 a = 255);

public:    
    Text(const std::string&, const SDL_Point&, SDL_Rect&);
    Text(const std::string&, const SDL_Point&);
    ~Text();

    virtual void draw();                // draw inside the viewport
    void draw(const SDL_Rect&); // draw inside what?

    void setColor(Uint8, Uint8, Uint8, Uint8 a = 255);

    std::string getText();
    void setText(const std::string&);

    int getX();
    int getY();
    int getDown();
    int getRight();

friend class Box;
friend class Debugger;
};

// Append a Word
Text& operator<<(Text&, Word);
// Append a string
Text& operator<<(Text&, const std::string&);

// Supposed to be only used for debugging
class Button : private Text
{
    static std::map<std::string, SDL_Color> colors;
    std::string state = "idle";     // idle - hover - pressed
    SDL_Point padding = { 10, 5 };   // distance separating the text and the bounding box

    Button(const std::string&);
    void draw() override;
    bool contains(const SDL_Point&);

friend class Debugger;
};

class Debugger
{
private:
    std::map<std::string, Box*> div;    // divers block dans le debugger
    std::map<std::string, Button*> buttons;
    Cpu6502& cpu;

    static SDL_Window* window;          // show the debugger on a separated window
    static SDL_Renderer* renderer;

    void update();

public:

    Debugger(Cpu6502*);
    ~Debugger();

    void handle(SDL_Event&);
    void draw();

friend class Button;
friend class Text;
friend class Box;
};