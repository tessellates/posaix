#include "RotationDisplay.hpp"


void RotationDisplay::init() 
{
    if (isInit)
        return;
    cubeScene.init();
    cubeScene.update();
    isInit = true;
}

void RotationDisplay::render() 
{
    init();
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos({650, 50}, ImGuiCond_FirstUseEver);

    ImGui::Begin("Wrap", nullptr, ImGuiWindowFlags_NoResize);
    
    if (ImGui::Button("Reset")) 
    {
        cubeScene.cube.data.angleX = 0;
        cubeScene.cube.data.angleY = 0;
        cubeScene.cube.data.rotate();
        cubeScene.cube.updateEvent();
    }

    ImGui::SameLine();
    if (ImGui::Checkbox("Culling", &cubeScene.cube.data.culling))
        cubeScene.cube.updateEvent();

    ImGui::SameLine();
    ImGui::SliderFloat("##Slider zoom", &zoom, 0, 0.5, "");

    // Optionally show the state of the boolean
    ImGui::Text("Button has been clicked: %s", cubeScene.cube.data.culling ? "Yes" : "No");


    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    ImVec2 csize = {512, 512}; 

    // Calculate the position to start the child window so it's centered
    ImVec2 childWindowPos = ImVec2(
        ImGui::GetCursorPosX() + (contentSize.x - csize.x) * 0.5f,
        ImGui::GetCursorPosY() + (contentSize.y - csize.y) * 0.5f
    );

    // Set the next window position to center the child
    ImGui::SetNextWindowPos(ImGui::GetWindowPos() + childWindowPos);

    ImGuiStyle originalStyle = ImGui::GetStyle();
    ImGui::GetStyle().WindowPadding = ImVec2(0.0f, 0.0f);
    ImGui::BeginChild("WrapDisplay", csize, ImGuiChildFlags_Border, (ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize));
    //ImGui::Image(static_cast<ImTextureID>(cubeScene.scene), {256,256});
    // Define the UV coordinates for the region you want to zoom into
    ImVec2 uv0 = ImVec2(zoom, zoom); // Top-left corner of the zoom region
    ImVec2 uv1 = ImVec2(1-zoom, 1-zoom); // Bottom-right corner of the zoom region

    // Display the image, specifying the UV coordinates
    ImGui::Image(static_cast<ImTextureID>(cubeScene.scene), csize, uv0, uv1);

    ImGui::EndChild();
    ImGui::GetStyle() = originalStyle;

    ImGui::SetCursorPos({size.x - 27, childWindowPos.y});
    // Add a vertical slider to control the texture scale
    if (ImGui::VSliderFloat("##X-Rotation", ImVec2(18, csize.y), &cubeScene.cube.data.angleX, 180, -180, ""))
    {
        cubeScene.cube.data.rotate();
        cubeScene.cube.updateEvent();
    }

    ImGui::SameLine();

    ImGui::SetCursorPos({childWindowPos.x, size.y-27});
    // Set the slider width to 200 pixels
    ImGui::PushItemWidth(csize.x);
    if (ImGui::SliderFloat("##Slider Label", &cubeScene.cube.data.angleY, 180, -180, ""))
    {
        cubeScene.cube.data.rotate();
        cubeScene.cube.updateEvent();
    } // Adjusts `value` from 0 to 100
    ImGui::PopItemWidth(); // Reset item width to default


    ImGui::End();    
}
