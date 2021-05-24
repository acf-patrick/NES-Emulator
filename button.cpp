#include "debugger.h"

std::map<std::string, SDL_Color> Button::colors;

Button::Button(const std::string& label) :
    Text(label, {0, 0})
{}

bool Button::contains(const SDL_Point& point)
{
    SDL_Rect r = { position.x, position.y, size.x + 2*padding.x, size.y + 2*padding.y };
    return SDL_PointInRect(&point, &r);
}

void Button::draw()
{
    SDL_Rect bg = { position.x, position.y, size.x + 2*padding.x, size.y + 2*padding.y };
    auto& color = colors[state];
    SDL_SetRenderDrawColor(Debugger::renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(Debugger::renderer, &bg);

    // draw label
    auto tmp = position;
    position.x += padding.x;
    position.y += padding.y;
    Text::draw();
    position = tmp;
}