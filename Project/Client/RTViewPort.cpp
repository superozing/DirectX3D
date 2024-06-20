#include "pch.h"
#include "RTViewPort.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CDevice.h>
#include "CImGuiMgr.h"

#include "CLevelSaveLoad.h"
#include <Engine\CLevelMgr.h>
#include <Engine/CKeyMgr.h>

#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>

#include "ImGuizmo.h"

#include "imgui_internal.h"
#include <Engine\CLogMgr.h>

RTViewPort::RTViewPort()
	: UI("Viewport", "##Viewport")
    , m_pTarget(nullptr)
    , m_pCamera(nullptr)
    , m_ViewportPos(0.f, 0.f)
    , m_MouseCoord(0.f, 0.f)
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

void RTViewPort::render_update()
{
    CRenderMgr::GetInst()->CopyRTTex(m_ViewPortTexture);

    m_fTapHeight = ImGui::GetFrameHeightWithSpacing();
    m_ViewportSize.x = (float)ImGui::GetWindowSize().x;
    m_ViewportSize.y = (float)ImGui::GetWindowSize().y - m_fTapHeight;

    m_ViewportPos = Vec2((float)ImGui::GetWindowPos().x, (float)ImGui::GetWindowPos().y);
    m_MouseCoord = Vec2((float)ImGui::GetIO().MousePos.x, (float)ImGui::GetIO().MousePos.y);

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
				wstring levelname = path(str).stem().wstring();
				pLevel->SetName(levelname);
				GamePlayStatic::ChangeLevel(pLevel, LEVEL_STATE::STOP);
			}
		}

        ImGui::EndDragDropTarget();
    }

    ImGui::GetWindowDrawList()->AddImage(
        m_ViewPortTexture->GetSRV().Get(),
        ImGui::GetWindowPos(),
        ImGui::GetWindowPos() + ImGui::GetWindowSize(),
        ImVec2(0, 0), ImVec2(1, 1));

    CImGuiMgr::GetInst()->SetViewportFocused(ImGui::IsWindowFocused(ImGuiFocusedFlags_None));

    // IMGUIZMO
    Gizmo();

    if (KEY_TAP(KEY::V)) {
        MoveCameraToObject();
    }
}

void RTViewPort::enter()
{
    SetCamera(CRenderMgr::GetInst()->GetEditorCam());
    SetTargetObject(nullptr);
}

void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition, float _distance);

void RTViewPort::Gizmo()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGuizmo::SetDrawlist();

    ImGui::SetCursorPos(ImVec2(10, 30));
    auto cameraViewMat = g_Transform.matView;
    auto cameraProjMat = g_Transform.matProj;
    if (m_pTarget == nullptr || !m_pTarget->Transform()) return;
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


void RTViewPort::SetTargetObject(CGameObject* _target)
{
    if (!_target || !_target->Transform()|| _target->Camera()) {
        m_pTarget = nullptr;
        return;
    }
    m_pTarget = _target;
}

void RTViewPort::SetTargetCamera(CCamera* _camera)
{
    if(_camera != nullptr)
    m_pCamera = _camera->GetOwner();
}

void RTViewPort::SetCamera(CCamera* _camera)
{
    RTViewPort* pViewport = (RTViewPort*)CImGuiMgr::GetInst()->FindUI("##Viewport");
    if (!pViewport) return;
    pViewport->SetTargetCamera(_camera);
}

void RTViewPort::MoveCameraToObject()
{
    if (!m_pTarget || !m_pCamera) return;

    Vec3 vPos = m_pTarget->Transform()->GetRelativePos();
    float distance = 500.f;

    Vec3 vDir = m_pCamera->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.Normalize();
    Vec3 vNewPos = vPos - (vDir)* distance;
    m_pCamera->Transform()->Lerp(vNewPos, false, Vec3(), false, Vec3(), 0.1f);
}



Vec2 RTViewPort::ConvertCoord()
{
    RTViewPort* Viewport = dynamic_cast<RTViewPort*>(CImGuiMgr::GetInst()->FindUI("##Viewport"));
    
     Vec2 OriginResolution = CDevice::GetInst()->GetRenderResolution();
     Vec2 Mousepos = Viewport->GetMouseCoord();
     float fTapHeight = Viewport->GetTapHeight();

     Mousepos.x = Mousepos.x - Viewport->GetViewPortPos().x;
     Mousepos.y = Mousepos.y - Viewport->GetViewPortPos().y - fTapHeight;

     float OriginAspect = OriginResolution.x / OriginResolution.y;

     // 각 축에 대한 변환 비율 계산
     float xScale;
     float yScale;
     
     if (OriginResolution.x > Viewport->GetViewPortSize().x)
         xScale = OriginResolution.x / Viewport->GetViewPortSize().x;
     else
         xScale = Viewport->GetViewPortSize().x / OriginResolution.x;

     if(OriginResolution.y >= Viewport->GetViewPortSize().y)
         yScale =  OriginResolution.y / Viewport->GetViewPortSize().y;
     else
         yScale =  Viewport->GetViewPortSize().y / OriginResolution.y;

     Mousepos.x *= xScale;
     Mousepos.y *= yScale;

     Mousepos.x = floor(Mousepos.x);
     Mousepos.y = floor(Mousepos.y);

     return Mousepos;
 }
