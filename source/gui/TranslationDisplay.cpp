
#include "TranslationDisplay.hpp"
#include "Utilities.hpp"


void TranslationDisplay::init()
{
    if (isInit)
        return;

    initTRD(&tdr);
    loadTRD(&tdr, create_texture_from_file("assets/test_square.png"));
    offset = &tdr.uData;
    updateTRD(&tdr);
    isInit = true;
}

void TranslationDisplay::render()
{
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Texture", nullptr, flags);
    execute();
    ImGui::End();
}

void TranslationDisplay::execute()
{
    init();

    // Invisible button to capture mouse inputs over the whole window.
    // This is a common trick to make an ImGui item cover an area for input detection.
    ImGui::InvisibleButton("canvas", size);
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            if (!isDragging)
            {
                // Start drag
                isDragging = true;
                dragStartPos = ImGui::GetMousePos() - offset->data.translation/2;
            }
            else
            {
                // While dragging
                ImVec2 mousePos = ImGui::GetMousePos();
                offset->data.translation = (mousePos - dragStartPos)*2;
                offset->updateEvent();
            }
        }
        else
        {
            isDragging = false;
        }
    }
    else
    {
        isDragging = false;
    }
    ImGui::SetCursorPos({0,0});

    ImGui::Image(static_cast<ImTextureID>(tdr.scene), size);
}

void TranslationDisplay::reset()
{
    offset->data.translation = {0,0};
    offset->data.scale = 1;
    offset->updateEvent();
}
