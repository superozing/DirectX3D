#include "pch.h"
#include "AssetUI.h"

AssetUI::AssetUI(const string& _strName, const string& _ID, ASSET_TYPE _Type)
	: UI(_strName, _ID)
	, m_Type(_Type)
{
	Deactivate();

}

AssetUI::~AssetUI()
{
}


void AssetUI::render_update()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));

	ImGui::Button(ASSET_TYPE_STRING[(UINT)m_Type]);

	ImGui::PopStyleColor(3);

	ImGui::PopID();

	if (ImGui::Button("Save Edit"))
	{

	} 
	
	ImGui::SameLine();

	if (ImGui::Button("Create Instance"))
	{
		CreateAssetInstance(m_Asset);
	}

	ImGui::Separator();
}


void AssetUI::SetAsset(Ptr<CAsset> _Asset)
{
	m_Asset = _Asset;

	// 입력된 Asset 이 nullptr 이면 해당 AssetUI 를 비활성화 한다.
	if (nullptr == m_Asset)
	{
		Deactivate();
	}

	else
	{
		// 정상적인 에셋 주소가 들어온 경우, 이 에셋UI 의 타입과 실제 에셋의 타입이 일치하면
		// 해당 에셋UI 를 활성화 시키고 아니면 비활성화 시킨다.
		if (_Asset->GetType() == m_Type)
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}


void AssetUI::SetAssetKey(Ptr<CAsset> _Asset, const wstring& _Key)
{
	_Asset->SetKey(_Key);
}

void AssetUI::CreateAssetInstance(Ptr<CAsset> _Asset)
{
	wstring strKey = _Asset->GetKey();

	filesystem::path pathObj(strKey);

	wstring ParentPath = pathObj.parent_path().wstring();
	wstring AssetName = pathObj.stem().wstring();
	wstring AssetType = pathObj.extension().wstring();

	wchar_t szPath[255] = {};
	wstring FilePath = CPathMgr::GetContentPath();
	int num = 0;
	while (true)
	{
		swprintf_s(szPath, (ParentPath + L"\\" + AssetName + L"_Inst_%d" + AssetType).c_str(), num);
		if (!exists(FilePath + szPath))
			break;

		++num;
	}

	ASSET_TYPE type = _Asset->GetType();
	CAsset* pNewAsset = nullptr;
	switch (type)
	{
	case ASSET_TYPE::MATERIAL:
		_Asset->Save(szPath);
		pNewAsset = new CMaterial;
		pNewAsset->Load(szPath);
		GamePlayStatic::AddAsset(pNewAsset);
		break;
	case ASSET_TYPE::GRAPHICS_SHADER:
		_Asset->Save(szPath);
		pNewAsset = new CGraphicsShader;
		pNewAsset->Load(szPath);
		GamePlayStatic::AddAsset(pNewAsset);
		break;
	default:
		MessageBox(nullptr, L"지원하지 않는 기능입니다", L"지원하지 않는 기능입니다", MB_OK);
		break;
	}
}

void AssetUI::ChangeAssetName(const string& _OriginRelativePath, const string& _NewRelativePath)
{
	wstring strPath = CPathMgr::GetContentPath();
	strPath += ToWString(_OriginRelativePath);

	wstring NstrPath = CPathMgr::GetContentPath();
	NstrPath += ToWString(_NewRelativePath);

	if (exists(strPath))
	{
		filesystem::rename(strPath, NstrPath);
	}
}