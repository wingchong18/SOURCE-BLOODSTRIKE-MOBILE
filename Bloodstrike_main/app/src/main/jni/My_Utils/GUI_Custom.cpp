#include "GUI_Custom.h"

bool ImGui::RadioButton(const char* label, float* v, float v_button)
{
    const bool pressed = RadioButton(label, *v == v_button);
    if (pressed)
        *v = v_button;
    return pressed;
}
