#include "PXGUI.hpp"
#include "PXApplication.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

void PXGUI::render()
{
    if (!isInit)
    {
        isInit = true;
        rdisplay.isInit = true;
        idisplay.tdisplay.init();
        rdisplay.cubeScene.init(&idisplay.tdisplay.tdr);
        rdisplay.cubeScene.update();
    }
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();  
    ImGui::NewFrame();
    idisplay.render();
    rdisplay.render();
    //ImGui::Image(idisplay.tdisplay.tdr.scene2, {512,512});
    ImGui::Render();
    SDL_RenderSetScale(PXApplication::renderer, ImGui::GetIO().DisplayFramebufferScale.x, ImGui::GetIO().DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}