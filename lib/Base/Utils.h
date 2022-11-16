#pragma once

#include <cstdint>
#include <string>
#include "imgui.h"

namespace Base {
    struct Utils {

        static constexpr uint32_t hash(const char *data, size_t const size) noexcept {
            uint32_t hash = 5381;

            for (const char *c = data; c < data + size; ++c)
                hash = ((hash << 5) + hash) + (unsigned char) *c;

            return hash;

        }

        static bool stringReplace(std::string &str, const std::string &from, const std::string &to) {
            size_t start_pos = str.find(from);
            if (start_pos == std::string::npos)
                return false;
            str.replace(start_pos, from.length(), to);
            return true;
        }


        static int strpos(char *haystack, char *needle, int nth) {
            char *res = haystack;
            for (int i = 1; i <= nth; i++) {
                res = strstr(res, needle);
                if (!res)
                    return -1;
                else if (i != nth)
                    res++;
            }
            return res - haystack;
        }


        static void SetupImGuiStyle(bool bStyleDark_, float alpha_) {
            ImGuiStyle &style = ImGui::GetStyle();
            ImGuiIO &io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 20.0f, NULL);
            io.FontDefault = io.Fonts->Fonts[1];
            style.Alpha = 1.0f;
            style.FrameRounding = 3.0f;
            style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
            style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

            if (bStyleDark_) {
                for (int i = 0; i <= ImGuiCol_COUNT; i++) {
                    ImVec4 &col = style.Colors[i];
                    float H, S, V;
                    ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

                    if (S < 0.1f) {
                        V = 1.0f - V;
                    }
                    ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
                    if (col.w < 1.00f) {
                        col.w *= alpha_;
                    }
                }
            } else {
                for (int i = 0; i <= ImGuiCol_COUNT; i++) {
                    ImVec4 &col = style.Colors[i];
                    if (col.w < 1.00f) {
                        col.x *= alpha_;
                        col.y *= alpha_;
                        col.z *= alpha_;
                        col.w *= alpha_;
                    }
                }
            }

        };
    };



}