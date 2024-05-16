#include "pch.h"
#include "RTViewPort.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CDevice.h>
#include "CImGuiMgr.h"

RTViewPort::RTViewPort()
	: UI("Viewport", "##Viewport")
{
	
	m_ViewPortTexture = CAssetMgr::GetInst()->CreateTexture(L"CopyRTtex",
			CDevice::GetInst()->GetRenderResolution().x,
			CDevice::GetInst()->GetRenderResolution().y,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE);
	
}

RTViewPort::~RTViewPort()
{
}

void RTViewPort::tick()
{
}

#include "CLevelSaveLoad.h"
#include <Engine\CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include "ImGuizmo.h"

void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition, float _distance)
{
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
    static bool useSnap = false;
    static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    static bool boundSizingSnap = false;
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
    if (editTransformDecomposition)
    {
        if (ImGui::IsKeyPressed(ImGuiKey_T))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(ImGuiKey_E))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;

        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_S))
            useSnap = !useSnap;
        ImGui::Checkbox("##UseSnap", &useSnap);
        ImGui::SameLine();

        switch (mCurrentGizmoOperation)
        {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
        }
        ImGui::Checkbox("Bound Sizing", &boundSizing);
        if (boundSizing)
        {
            ImGui::PushID(3);
            ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
            ImGui::SameLine();
            ImGui::InputFloat3("Snap", boundsSnap);
            ImGui::PopID();
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = ImGui::GetWindowSize().x;
    float viewManipulateTop = ImGui::GetWindowPos().y;
    static ImGuiWindowFlags gizmoWindowFlags = 0;
    static bool useWindow = false;
    if (useWindow)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
        ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
        viewManipulateTop = ImGui::GetWindowPos().y;
        //ImGuiWindow* window = ImGui::GetCurrentWindow();
        //gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
    }
    else
    {
        //ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        auto size = ImGui::GetWindowSize();
        ImGuizmo::SetRect(0, 0, size.x, size.y);

    }
    ImGuizmo::Enable(true);

    ImGuizmo::SetDrawlist();
    //ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
    //ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

    ImGuizmo::ViewManipulate(cameraView, _distance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
    auto pos = ImGuizmo::IsOver();
    //ImGuizmo::
    if (pos) {
        int a = 0;
    }
    if (useWindow)
    {
        ImGui::End();
        ImGui::PopStyleColor(1);
    }
}

void RTViewPort::render_update()
{
	CRenderMgr::GetInst()->CopyRTTex(m_ViewPortTexture);

	ImGui::Dummy(ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 40));

	// 레벨 파일 드랍 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			if (data == 0) return;
			string& str = *(string*)data;
			auto extension = path(str).extension().string();
			if (extension == ".lv") {
				auto pLevel = CLevelSaveLoad::LoadLevel(str);
				CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);
			}
		}

		ImGui::EndDragDropTarget();
	}

    ImGui::GetWindowDrawList()->AddImage(
        m_ViewPortTexture->GetSRV().Get(),
        ImVec2(0, 0),
        ImGui::GetWindowPos() + ImGui::GetWindowSize(),
        ImVec2(0, 0), ImVec2(1, 1));

    CImGuiMgr::GetInst()->GetbViewportFocused() = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);

	//
	// 
	// 
	// 
	// 
	// 
	// 
	//
    ImGuiIO& io = ImGui::GetIO();
    
    ImGuizmo::SetOrthographic(false);
    //ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist();

    ImGui::SetNextWindowPos(ImVec2(1024, 100), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiCond_Appearing);

    // create a window and insert the inspector
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(320, 340), ImGuiCond_Appearing);
    ImGui::Begin("Editor");


    ImGui::Text("Camera");
    static int gizmoCount = 1;
    ImGui::SliderInt("Gizmo count", &gizmoCount, 1, 4);


    ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
    ImGuizmo::Enable(true);
    if (ImGuizmo::IsUsing())
    {
        ImGui::Text("Using gizmo");
    }
    else
    {
        ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
        ImGui::SameLine();
        ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
        ImGui::SameLine();
        ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
        ImGui::SameLine();
        ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
    }
    ImGui::Separator();

    ImGui::End();
    auto cameraview = g_Transform.matView;
    auto cameraproj = g_Transform.matProj;
    auto player = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
    auto objmat = player->Transform()->GetWorldMat();
    Vec3 playerPos = player->Transform()->GetWorldPos();
    Vec3 cameraPos = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"MainCamera")->Transform()->GetWorldPos();
    float distance = Vec3::Distance(playerPos, cameraPos);

    float cameraView[16] = {
        cameraview._11, cameraview._12, cameraview._13, cameraview._14,
        cameraview._21, cameraview._22, cameraview._23, cameraview._24,
        cameraview._31, cameraview._32, cameraview._33, cameraview._34,
        cameraview._41, cameraview._42, cameraview._43, cameraview._44,
    };
    float cameraProjection[16] = {
        cameraproj._11, cameraproj._12, cameraproj._13, cameraproj._14,
        cameraproj._21, cameraproj._22, cameraproj._23, cameraproj._24,
        cameraproj._31, cameraproj._32, cameraproj._33, cameraproj._34,
        cameraproj._41, cameraproj._42, cameraproj._43, cameraproj._44,
    };
    float objectMatrix[16] = {
        objmat._11, objmat._12, objmat._13, objmat._14,
        objmat._21, objmat._22, objmat._23, objmat._24,
        objmat._31, objmat._32, objmat._33, objmat._34,
        objmat._41, objmat._42, objmat._43, objmat._44,
    };
    static int lastUsing = 1;
    for (int matId = 0; matId < gizmoCount; matId++)
    {
        ImGuizmo::SetID(matId);

        EditTransform(cameraView, cameraProjection, objectMatrix, lastUsing == matId, distance);
        if (ImGuizmo::IsUsing())
        {
            lastUsing = matId;
        }
    }
    objmat. _11 = objectMatrix[0];
    objmat._12 = objectMatrix[1];
    objmat._13 = objectMatrix[2];
    objmat._14 = objectMatrix[3];

    objmat._21 = objectMatrix[4];
    objmat._22 = objectMatrix[5];
    objmat._23 = objectMatrix[6];
    objmat._24 = objectMatrix[7];

    objmat._31 = objectMatrix[8];
    objmat._32 = objectMatrix[9];
    objmat._33 = objectMatrix[10];
    objmat._34 = objectMatrix[11];

    objmat._41 = objectMatrix[12];
    objmat._42 = objectMatrix[13];
    objmat._43 = objectMatrix[14];
    objmat._44 = objectMatrix[15];
    Vec3 scale;
    Quaternion quat;
    Vec3 pos;
    objmat.Decompose(scale, quat, pos);
    player->Transform()->SetRelativePos(pos);
    player->Transform()->SetRelativeScale(scale);
    //player->Transform()->SetDir(Vec3(quat.x / quat.w, quat.y/quat.w, quat.z/quat.w));s
    //player->Transform()->SetRelativeRotation(Vec3(quat.x, quat.y, quat.z));
    //player->Transform()->SetRelativePos(Vec3(objectMatrix[12], objectMatrix[13], objectMatrix[14]));
}
