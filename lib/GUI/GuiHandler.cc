//
// Created by lucas on 25/10/2022.
//

#include <cmath>
#include "GuiHandler.h"

namespace GUI {
    void GUIHandler::Initialise(GLFWwindow* window) {
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		 // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		 // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		 // Enable Multi-Viewport / Platform Windows

        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        ImFontConfig fontConfig;
        io.Fonts->AddFontDefault(&fontConfig);
    }

    void GUIHandler::Cleanup(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GUIHandler::BeginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void GUIHandler::EndFrame() {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }


    }
    void GUIHandler::Demo() {
        ImGui::ShowDemoWindow();
    }

    void GUIHandler::EntityUI(Managers::EntityManager& entityManager, Managers::ComponentManager& componentManager) {
        ImGui::ShowDemoWindow();
        ImGuiWindowFlags window_flags = 0;
        bool p_open = true;
        ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin("Entities", &p_open, window_flags))
        {
            ImGui::End();
            return;
        }
        ImGui::SetWindowFontScale(1.2);
        ImGui::TextColored(ImVec4(1,1,0,1), "Entity List");
        ImGui::BeginChild("Scrolling");
        auto entities = entityManager.entities;

        for(const auto& e_pair : entities) {
            auto label = e_pair.first;
            if(!e_pair.second.empty())
                label = e_pair.second;
            if(ImGui::Button(label.c_str())){
                inspectorEntity = e_pair.first;
            }



        }
        ImGui::EndChild();
        ImGui::End();
    }

    void GUIHandler::InspectorUI(Managers::EntityManager &entityManager, Managers::ComponentManager &componentManager) {
        ImGuiWindowFlags window_flags = 0;


        bool p_open = true;

        if (!ImGui::Begin("Inspector", &p_open, window_flags) or inspectorEntity.empty())
        {
            ImGui::End();
            return;
        }

        ImGui::SetWindowFontScale(1.2);

        std::string name;

        for(auto const& entity : entityManager.entities){
            if(entity.first == inspectorEntity){
                name = entity.second;
                break;
            }
        }


        ImGui::Text("Name: '%s'", name.c_str());
        ImGui::Text("UUID: %s", inspectorEntity.c_str());

        Components::Shader* shader = nullptr;

        //Get Component List
        auto &comp_list = componentManager.entityComponents[inspectorEntity];
        for(auto const& comp : comp_list){
            auto ptr_comp = componentManager.components[comp];

            std::string type = ptr_comp->GetTypeInfo().Name();


            if(type == "Components::Transform"){
                ImGui::DragFloat3("Position", &((Components::Transform*)ptr_comp)->position[0],0.1f);
            }
            else if(type == "Components::Shader"){
                shader = (Components::Shader*)ptr_comp;
                int glow = 1;
                shader->SetUniform(&glow, "inspectorSelect", Components::Shader::UniformType::INT1);
            }
        }

        if(shader) {
            for (auto const &comp: componentManager.components) {
                std::string type = comp->GetTypeInfo().Name();
                if (type == "Components::Shader") {
                    auto c_shader = (Components::Shader *) comp;
                    int glow = 0;
                    if(shader != c_shader)
                        c_shader->SetUniform(&glow, "inspectorSelect", Components::Shader::UniformType::INT1);

                }
            }
        }



        if(name != "Camera") {
            if (ImGui::Button("Delete")) {
                entityManager.RemoveEntity(inspectorEntity);
                componentManager.RemoveEntity(inspectorEntity);
                inspectorEntity = "";
            }
        }





        ImGui::End();
    }

    void GUIHandler::LuaConsoleUI(lua_State *L) {
        ImGuiWindowFlags window_flags = 0;


        bool p_open = true;

        if (!ImGui::Begin("Console", &p_open, window_flags))
        {
            ImGui::End();
            return;
        }
        //First, get the last log entry.
        auto& lastEntries = Debug::Logging::logEntries;


        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
            if (ImGui::BeginTabItem("Main Console")) {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
                for (auto &entry: lastEntries) {
                    ImGui::TextWrapped("%s", entry.msg.c_str());
                }
                ImGui::PopStyleColor();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Lua Console"))
            {

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}
