#pragma once

#include "App.h"
#include "UINode.h"
#include <functional>
#include <optional>
#include <sstream>
#include <iostream>

class FloatInput : public UINode
{
public:
    FloatInput(float* number) :
        FloatInput(*number, number, std::nullopt) {}

    FloatInput(float initialValue, std::function<void(float)> onUpdate) :
        FloatInput(initialValue, nullptr, onUpdate) {}

    ~FloatInput()
    {
        UINode::~UINode();
    }

    float value;
    std::string textValue;
    float* number;
    std::optional<std::function<void(float)>> onUpdate;

    bool onTextInput(const SDL_Event& e) override
    {
        if (!((e.text.text[0] >= '0' && e.text.text[0] <= '9') || (e.text.text[0] == '.')))
        {
            return true;
        }
        textValue = textValue.substr(0, cursor) + e.text.text + textValue.substr(cursor, textValue.length());
        cursor++;
        save();

        return true;
    }

    bool onKeyDown(const SDL_Event& e) override {
        if (!isActive())
        {
            return true;
        }

        switch (e.key.keysym.scancode)
        {
        case SDL_Scancode::SDL_SCANCODE_LEFT:
        {
            if (cursor > 0)
            {
                cursor--;
            }
            break;
        }
        case SDL_Scancode::SDL_SCANCODE_RIGHT:
        {
            if (cursor < textValue.length())
            {
                cursor++;
            }
            break;
        }
        case SDL_Scancode::SDL_SCANCODE_BACKSPACE:
        {
            if (textValue.length() > 0 && cursor > 0)
            {
                textValue = textValue.substr(0, cursor - 1) + textValue.substr(cursor, textValue.length());
                cursor--;
            }
            save();
            break;
        }
        }
    }

    void save()
    {
        try
        {
            value = std::stof(textValue);
        }
        catch (std::invalid_argument e)
        {
            std::cerr << "Invalid float value: " << textValue << std::endl;
            value = 0;
        }
        if(onUpdate.has_value())
        {
            onUpdate.value()(value);
        }
        else if (number != nullptr)
        {
            *number = value;
        };
    }

    void draw(const SDL_Rect& container) override
    {
        UINode::draw(container);
        SDL_Point offset = getDrawOffset(container);

        SDL_Rect clipRect = {
                .x = container.x + 140,
                .y = container.y + 5,
                .w = 84,
                .h = 32
        };
        SDL_RenderSetClipRect(APP->renderer, &clipRect);
        APP->drawTextBox(
            {
                .x = container.x + 140,
                .y = container.y + 5,
                .w = 84,
                .h = 32
            },
            textValue.c_str(),
            cursor,
            isActive(),
            false
        );
        SDL_RenderSetClipRect(APP->renderer, nullptr);
    }

    bool onMouseDown(const SDL_Event& event) override
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            APP->setActive(this);
        }
        return false;
    }

private:
    FloatInput(float initialValue, float* number, std::optional<std::function<void(float)>> onUpdate) :
        UINode({
            4, 0,
            84, 32,
            "Resources/UI/FloatInput",
            CGA_TRANSPARENT,
            StyleDirection::TOP,
            StyleDirection::RIGHT
        }),
        value(initialValue),
        number(number),
        onUpdate(onUpdate),
        cursor(0),
        textValue()
    {
        std::stringstream ss;
        ss << value;
        textValue = ss.str();
    }

    int cursor = 0;
};

