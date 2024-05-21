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

void RTViewPort::SetTargetObject(CGameObject* _target)
{
    if (!_target->Transform()) {
        m_pTarget = nullptr;
        return;
    }
    m_pTarget = _target;
}

#include <Engine/CCamera.h>

void RTViewPort::SetTargetCamera(CCamera* _camera)
{
    m_pCamera = _camera->GetOwner();
}

void RTViewPort::SetCamera(CCamera* _camera)
{
    RTViewPort* pViewport = (RTViewPort*)CImGuiMgr::GetInst()->FindUI("##Viewport");
    if (!pViewport) return;
    pViewport->SetTargetCamera(_camera);
}

void RTViewPort::tick()
{
}

#include "CLevelSaveLoad.h"
#include <Engine\CLevelMgr.h>
#include <Engine/CKeyMgr.h>

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
        if (KEY_TAP(W) && KEY_NONE(RBTN))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (KEY_TAP(E) && KEY_NONE(RBTN))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (KEY_TAP(R) && KEY_NONE(RBTN)) // r Key
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;

        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
        {
            ImGui::SameLine();
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        if (KEY_TAP(KEY::S) && KEY_NONE(RBTN))
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
    }

    ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = ImGui::GetWindowSize().x;
    float viewManipulateTop = ImGui::GetWindowPos().y;
    static ImGuiWindowFlags gizmoWindowFlags = 0;

    auto size = ImGui::GetWindowSize();
    ImGuizmo::SetRect(0, 0, size.x, size.y);

    ImGuizmo::SetDrawlist();
    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
    //ImGuizmo::ViewManipulate(cameraView, _distance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
}

void RTViewPort::render_update()
{
	CRenderMgr::GetInst()->CopyRTTex(m_ViewPortTexture);

    auto viewportPos = ImGui::GetWindowPos();
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

	// IMGUIZMO
    ImGuiIO& io = ImGui::GetIO();
    
    ImGuizmo::SetDrawlist();

    ImGui::SetCursorPos(ImVec2(10, 30));
    auto cameraViewMat = g_Transform.matView;
    auto cameraProjMat = g_Transform.matProj;
    if (m_pTarget == nullptr) return;
    auto objmat = m_pTarget->Transform()->GetWorldMat();
    Vec3 objPos = m_pTarget->Transform()->GetWorldPos();
    Vec3 cameraPos = m_pCamera->Transform()->GetWorldPos();

    auto projType = m_pCamera->Camera()->GetProjType();
    projType == PROJ_TYPE::ORTHOGRAPHIC ? ImGuizmo::SetOrthographic(true) : ImGuizmo::SetOrthographic(false);

    float distance = Vec3::Distance(objPos, cameraPos);

    float cameraView[16] = {};
    float cameraProjection[16] = {};
    float objectMatrix[16] = {};

    RoRMath::MatrixToFloat16(cameraView, cameraViewMat);
    RoRMath::MatrixToFloat16(cameraProjection, cameraProjMat);
    RoRMath::MatrixToFloat16(objectMatrix, objmat);

    EditTransform(cameraView, cameraProjection, objectMatrix, true, distance);

    RoRMath::Float16ToMatrix(cameraViewMat, cameraView);
    RoRMath::Float16ToMatrix(cameraProjMat, cameraProjection);
    RoRMath::Float16ToMatrix(objmat, objectMatrix);

    m_pTarget->Transform()->SetWorldMat(objmat);
}
