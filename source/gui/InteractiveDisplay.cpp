#include "InteractiveDisplay.hpp"


void InteractiveDisplay::init() {
    
}

void InteractiveDisplay::render() 
{
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos({25, 75}, ImGuiCond_FirstUseEver);

    ImGui::Begin("Interactive", nullptr, ImGuiWindowFlags_NoResize);
    
    if (ImGui::Button("Reset")) 
    {
        tdisplay.reset();
    }

    ImVec2 contentSize = ImGui::GetContentRegionAvail();

    // Calculate the position to start the child window so it's centered
    ImVec2 childWindowPos = ImVec2(
        ImGui::GetCursorPosX() + (contentSize.x - tdisplay.size.x) * 0.5f,
        ImGui::GetCursorPosY() + (contentSize.y - tdisplay.size.y) * 0.5f
    );

    // Set the next window position to center the child
    ImGui::SetNextWindowPos(ImGui::GetWindowPos() + childWindowPos);

    ImGuiStyle originalStyle = ImGui::GetStyle();
    ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
    ImGui::BeginChild("TranslationDisplayChild", tdisplay.size, ImGuiChildFlags_Border, tdisplay.flags);
    tdisplay.execute();
    ImGui::EndChild();
    ImGui::GetStyle() = originalStyle;

    ImGui::SetCursorPos({size.x - 27, childWindowPos.y});
    // Add a vertical slider to control the texture scale
    if (ImGui::VSliderFloat("##TextureScale", ImVec2(18, tdisplay.size.y), &tdisplay.offset->data.scale, 0.1f, 3.0f, ""))
    {
        tdisplay.offset->updateEvent();
    }
    ImGui::SameLine();


    ImGui::End();
}   

