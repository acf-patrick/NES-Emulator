#include "debugger.h"

Text*& SliderBox::operator[](const std::string& tag)
{
    return texts[tag];
}

Text* SliderBox::push(const std::string& tag, const std::string& content, const SDL_Point& position)
{
    last = texts[tag] = new Text(content, position, drag);
    if (!first)
        first = last;
    return texts[tag];
}

SliderBox::SliderBox(const SDL_Rect& rect)
{
    drag = rect;
    viewport = rect;
}

SliderBox::~SliderBox()
{
    for (auto& pair : texts)
        delete pair.second;
    texts.clear();
}

void SliderBox::slide(int diff)
{
    if (diff < 0)
    {
        if (last->getDown() + drag.y >= viewport.y + viewport.h)
            drag.y += diff;
    }
    else if (diff > 0)
    {
        if (first->getY() + drag.y <= viewport.y)
            drag.y += diff;
    }
}

void SliderBox::draw()
{
    auto renderer = Debugger::renderer;

    // draw only if inside the viewport
    for (auto& pair : texts)
    {
        auto& text = pair.second;
        auto& size = text->size;
        SDL_Rect box = { drag.x+text->position.x, drag.y+text->position.y, size.x, size.y };
        if (SDL_HasIntersection(&viewport, &box))   // save GPU usage and otherwise we will render all of the 256 texts
            text->draw(viewport);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &viewport);
}