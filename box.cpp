#include "debugger.h"

Text*& Box::operator[](const std::string& tag)
{
    return texts[tag];
}

Text* Box::push(const std::string& tag, const std::string& content, const SDL_Point& position)
{
    last = texts[tag] = new Text(content, position, drag);
    if (!first)
        first = last;
    return texts[tag];
}

Box::Box(const SDL_Rect& rect)
{
    drag = rect;
    viewport = rect;
}

Box::~Box()
{
    delete label;
    for (auto& pair : texts)
        delete pair.second;
    texts.clear();
}

void Box::slide(int diff)
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

void Box::draw()
{
    auto renderer = Debugger::renderer;

    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
    SDL_RenderFillRect(renderer, &viewport);

    if (label)
    {
        SDL_Point mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y); 
        if (SDL_PointInRect(&mouse, &viewport))
        {
            const int offset = 3;
            SDL_Rect ext = {
                viewport.x - offset, label->position.y - offset,
                viewport.w + 2*offset, label->size.y + viewport.h + 2*offset
            };
            SDL_SetRenderDrawColor(renderer, 22, 54, 99, 255);
            SDL_RenderDrawRect(renderer, &ext);
        }
        label->draw();
    }

    // draw only if inside the viewport
    for (auto& pair : texts)
    {
        auto& text = pair.second;
        auto& size = text->size;
        SDL_Rect box = { drag.x+text->position.x, drag.y+text->position.y, size.x, size.y };
        if (SDL_HasIntersection(&viewport, &box))   // save GPU usage and otherwise we will render all of the 256 texts
            text->draw(viewport);
    }

    SDL_SetRenderDrawColor(renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
    SDL_RenderDrawRect(renderer, &viewport);
}

void Box::setLabel(const std::string& l)
{
    label = new Text(l, {0, viewport.y});
    // center it on top of the box
    label->position.x = viewport.x + 0.5*(viewport.w - label->size.x);
    
    // push the box down
    viewport.y += label->size.y;
    drag.y += label->size.y;

}