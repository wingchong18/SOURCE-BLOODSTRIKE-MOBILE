#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include <map>
#include <string>
#include <chrono>

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator/(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }

// --- FONTS ---
namespace font {
    inline ImFont* lexend_bold;
    inline ImFont* lexend_regular;
    inline ImFont* icomoon;
    inline ImFont* icomoon_widget;
    inline ImFont* icomoon_widget2;
    inline ImFont* lexend_general_bold;
    inline ImFont* icomoon_tabs;
}

// --- COLORS & STYLE ---
namespace c {
    namespace bg {
        inline ImVec2 size = ImVec2(950, 600);
        inline ImVec4 background = ImVec4(20 / 255.f, 20 / 255.f, 22 / 255.f, 1.f);
        inline ImVec4 border = ImVec4(17 / 255.f, 17 / 255.f, 17 / 255.f, 1.f);
        inline float rounding = 12.0f;
    }
    inline ImVec4 accent_color = ImVec4(255 / 255.f, 40 / 255.f, 40 / 255.f, 1.f); // Red
    namespace widget {
        inline ImVec4 background = ImVec4(28 / 255.f, 28 / 255.f, 35 / 255.f, 1.f);
        inline float rounding = 8.0f;
    }
    namespace text {
        inline ImVec4 text = ImVec4(150 / 255.f, 150 / 255.f, 150 / 255.f, 1.f);
        inline ImVec4 text_active = ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f);
        inline ImVec4 text_hov = ImVec4(220 / 255.f, 220 / 255.f, 220 / 255.f, 1.f);
    }
}

// --- HELPERS ---
inline ImU32 GetColorU35(ImVec4 col, float alpha = 1.0f) {
    ImVec4 v = col;
    v.w *= alpha;
    return ImGui::GetColorU32(v);
}

// --- CUSTOM WIDGETS ---
namespace e_elements {
    struct tab_state { float bg_opticaly = 0.f; ImVec4 image = ImVec4(150/255.f, 150/255.f, 150/255.f, 1.f); };

    inline bool Tabis(bool selected, ImGuiID id, const char* icon, const ImVec2& size_arg) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiContext& g = *GImGui;
        const ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImGui::CalcItemSize(size_arg, 0, 0);
        const ImRect bb(pos, pos + size);
        ImGui::ItemSize(size, 0.f);
        if (!ImGui::ItemAdd(bb, id)) return false;
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
        static std::map<ImGuiID, tab_state> anim;
        auto& state = anim[id];
        state.bg_opticaly = ImLerp(state.bg_opticaly, selected ? 1.f : (hovered ? 0.3f : 0.f), g.IO.DeltaTime * 12.f);
        state.image = ImLerp(state.image, selected || hovered ? c::accent_color : c::text::text, g.IO.DeltaTime * 12.f);

        if (selected) {
            window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU35(c::accent_color, 0.15f), 10.f);
            window->DrawList->AddRectFilled(bb.Min, bb.Min + ImVec2(4, size.y), GetColorU35(c::accent_color), 10.f);
        } else if (hovered) {
            window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU35(c::widget::background, 0.3f), 10.f);
        }

        ImGui::PushFont(font::icomoon_tabs);
        window->DrawList->AddText(font::icomoon_tabs, ImGui::GetFontSize(), bb.Min + (size - ImGui::CalcTextSize(icon))/2, GetColorU35(state.image), icon);
        ImGui::PopFont();
        return pressed;
    }

    inline bool Checkbox(const char* label, bool* v) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiContext& g = *GImGui;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        float w = ImGui::GetContentRegionAvail().x;
        float y_size = 50.0f;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, pos + ImVec2(w, y_size));
        ImGui::ItemSize(total_bb, 0.f);
        if (!ImGui::ItemAdd(total_bb, id)) return false;
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed) { *v = !(*v); ImGui::MarkItemEdited(id); }

        static std::map<ImGuiID, float> anim;
        anim[id] = ImLerp(anim[id], *v ? 1.0f : 0.0f, g.IO.DeltaTime * 12.f);

        ImVec2 check_pos = pos + ImVec2(w - 55, (y_size - 24) / 2);

        // Background track
        window->DrawList->AddRectFilled(check_pos, check_pos + ImVec2(46, 24), GetColorU35(ImVec4(45/255.f, 45/255.f, 50/255.f, 1.f)), 12.f);
        // Active track
        if (anim[id] > 0.01f)
            window->DrawList->AddRectFilled(check_pos, check_pos + ImVec2(46, 24), GetColorU35(c::accent_color, anim[id]), 12.f);

        // Circle
        window->DrawList->AddCircleFilled(check_pos + ImVec2(12 + (22.0f * anim[id]), 12), 9.0f, IM_COL32(255, 255, 255, 255), 30);

        window->DrawList->AddText(pos + ImVec2(5, (y_size - label_size.y) / 2), GetColorU35(*v ? c::text::text_active : c::text::text), label);
        return pressed;
    }

    inline bool RadioButton(const char* label, bool active) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiContext& g = *GImGui;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect bb(pos, pos + ImVec2(label_size.x + 50, 40));
        ImGui::ItemSize(bb, 0.f);
        if (!ImGui::ItemAdd(bb, id)) return false;
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

        static std::map<ImGuiID, float> anim;
        anim[id] = ImLerp(anim[id], active ? 1.0f : 0.0f, g.IO.DeltaTime * 12.f);

        window->DrawList->AddCircle(pos + ImVec2(15, 20), 10.f, GetColorU35(c::text::text, 0.5f), 30, 2.f);
        if (anim[id] > 0.01f)
            window->DrawList->AddCircleFilled(pos + ImVec2(15, 20), 6.5f * anim[id], GetColorU35(c::accent_color, anim[id]), 30);

        window->DrawList->AddText(pos + ImVec2(40, 20 - label_size.y/2), GetColorU35(active ? c::text::text_active : c::text::text), label);
        return pressed;
    }

    inline bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f") {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiContext& g = *GImGui;
        const ImGuiID id = window->GetID(label);
        float w = ImGui::GetContentRegionAvail().x;
        float y_size = 65.0f;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, pos + ImVec2(w, y_size));
        ImGui::ItemSize(total_bb, 0.f);
        if (!ImGui::ItemAdd(total_bb, id)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
        if (held) {
            *v = ImClamp((g.IO.MousePos.x - (pos.x + 10)) / (w - 20) * (v_max - v_min) + v_min, v_min, v_max);
            ImGui::MarkItemEdited(id);
        }

        float grab_t = (*v - v_min) / (v_max - v_min);
        float grab_pos = ImLerp(pos.x + 10, pos.x + w - 10, grab_t);

        window->DrawList->AddText(pos + ImVec2(5, 5), GetColorU35(c::text::text), label);
        window->DrawList->AddRectFilled(pos + ImVec2(10, 45), pos + ImVec2(w - 10, 49), GetColorU35(c::widget::background), 2.f);
        window->DrawList->AddRectFilled(pos + ImVec2(10, 45), ImVec2(grab_pos, pos.y + 49), GetColorU35(c::accent_color), 2.f);
        window->DrawList->AddCircleFilled(ImVec2(grab_pos, pos.y + 47), 7.f, GetColorU35(ImVec4(1,1,1,1)), 20);

        char buf[32]; sprintf(buf, format, *v);
        window->DrawList->AddText(pos + ImVec2(w - ImGui::CalcTextSize(buf).x - 5, 5), GetColorU35(c::text::text_active), buf);
        return held;
    }

    inline bool SliderInt(const char* label, int* v, int v_min, int v_max) {
        float val_f = (float)*v;
        bool changed = SliderFloat(label, &val_f, (float)v_min, (float)v_max, "%.0f");
        if (changed) *v = (int)val_f;
        return changed;
    }

    inline void BeginChilds(const char* name, ImVec2 size) {
        ImGui::BeginChild(name, size, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImVec2 pos = window->Pos;
        window->DrawList->AddRectFilled(pos, pos + size, GetColorU35(c::widget::background, 0.3f), 10.f);
        ImGui::Indent(15.f); ImGui::Spacing();
        ImGui::TextColored(c::accent_color, name);
        window->DrawList->AddLine(pos + ImVec2(15, 45), pos + ImVec2(size.x - 15, 45), GetColorU35(ImVec4(1,1,1,0.05f)));
        ImGui::Spacing(); ImGui::Spacing();
    }
    inline void EndChilds() { ImGui::Unindent(15.f); ImGui::EndChild(); }
}
