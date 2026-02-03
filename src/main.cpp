#include <iostream>

#include "include.h"

#include "ExternMakers/sam.h"

#include "Smf/smf.h"
#include "img/img.h"
#include "glsl/glsl.h"
#include "control/control.h"
#include "other/BetherCam.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

smq::Engine eng(smq::Window({ {1280,720}, "SAM" }));

int main() {

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ImGui::SetCurrentContext(eng.GetImGuiContext());

    smq::Scene* scene = new smq::Scene();

    initSam();

    GetMaterial(s_shadow)->AddTexture(GetTexture(t_blocks));

    smq::Renderer* renderer = new smq::DefultRenderer();
    smq::RenderTexture* render = new smq::RenderTexture({ 1280,720 }, renderer);
    scene->renderTexture = render;
    scene->cameras.emplace_back(render, smq::RenderType_Perspective);

    scene->rootObject = new smq::Object();
    scene->rootObject->AddComponent(new Smf());
    scene->rootObject->AddComponent(new Img());
    scene->rootObject->AddComponent(new Glsl());
    scene->rootObject->AddComponent(new Control());
    scene->rootObject->AddComponent(new BetherCam(&scene->cameras[0]));

    smq::Object* smf_Previev = new smq::Object();
    smf_Previev->AddComponent(new smq::comp::Position3D());
    smf_Previev->AddComponent(new smq::comp::ModelMaterial(nullptr, GetMaterial(s_shadow)));
    scene->rootObject->AddChild(smf_Previev);

    scene->lights.reserve(1);

    smq::Light r;
    r.position = { 10.0f, 10.0f, -10.0f, 100.0f };
    r.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    scene->lights.push_back(r);

    GetMaterial(s_shadow)->UpdateUniform(s_shadow_fs_u_lightCount, (int)scene->lights.size());
    eng.SetPostProcesingMaterial(GetMaterial(s_post));

    eng.SetScene(scene);

    eng.StartRuntime();

	delete renderer;
	delete render;
}

