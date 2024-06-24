#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _OriginAnimator)
	: CComponent(_OriginAnimator)
	, m_CurAnim(nullptr)
	, m_bRepeat(_OriginAnimator.m_bRepeat)
{
	map<wstring, CAnim*>::const_iterator iter = _OriginAnimator.m_mapAnim.begin();
	for (; iter != _OriginAnimator.m_mapAnim.end(); ++iter)
	{
		CAnim* pCloneAnim = iter->second->Clone();

		pCloneAnim->m_Animator = this;
		m_mapAnim.insert(make_pair(iter->first, pCloneAnim));
	}

	if (nullptr != _OriginAnimator.m_CurAnim)
	{
		m_CurAnim = FindAnim(_OriginAnimator.m_CurAnim->GetName());
	}
}

CAnimator2D::~CAnimator2D()
{
	Delete_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr == m_CurAnim)
		return;

	if (m_CurAnim->IsFinish() && m_bRepeat)
	{
		m_CurAnim->Reset();
	}

	m_CurAnim->finaltick();
}

void CAnimator2D::UpdateData()
{
	if (nullptr == m_CurAnim)
		return;

	m_CurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
	CAnim::Clear();
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, Vec2 _LeftTop, Vec2 _vSliceSize,
						 Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _FPS)
{
	CAnim* pAnim = FindAnim(_strKey);
	assert(!pAnim);

	pAnim = new CAnim;
	pAnim->Create(this, _AltasTex, _LeftTop, _vSliceSize, _OffsetSize, _Background, _FrmCount, _FPS);
	auto name = std::filesystem::path(_strKey).filename();
	pAnim->SetName(name);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

CAnim* CAnimator2D::FindAnim(const wstring& _strKey)
{
	map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _bRepeat)
{
	CAnim* pAnim = FindAnim(_strAnimName);
	if (nullptr == pAnim)
		return;

	m_bRepeat = _bRepeat;

	m_CurAnim = pAnim;
	m_CurAnim->Reset();
}

void CAnimator2D::SaveToFile(FILE* _File)
{
	// 애니메이션 개수 저장
	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _File);

	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToFile(_File);
	}

	// 플레이 중이던 애니메이션의 키를 저장한다.
	wstring PlayAnimName;

	if (nullptr != m_CurAnim)
	{
		PlayAnimName = m_CurAnim->GetName();
	}

	SaveWString(PlayAnimName, _File);
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);
}

#define TagAnimCount "[AnimCount]"
#define TagAnimNames "[AnimNames]"
#define TagPlayingAnimName "[PlayingAnimName]"
#define TagRepeat "[IsRepeat]"

void CAnimator2D::SaveToFile(ofstream& fout)
{
	// 애니메이션 개수 저장
	size_t AnimCount = m_mapAnim.size();
	fout << TagAnimCount << endl;
	fout << AnimCount << endl;
	if (AnimCount == 0)
		return;

	fout << TagAnimNames << endl;
	for (const auto& pair : m_mapAnim)
	{
		fout << ToString(pair.first) << endl;
	}

	// 플레이 중이던 애니메이션의 키를 저장한다.
	wstring PlayAnimName;
	fout << TagPlayingAnimName << endl;

	if (nullptr != m_CurAnim)
	{
		PlayAnimName = m_CurAnim->GetName();
	}

	fout << ToString(PlayAnimName) << endl;

	fout << TagRepeat << endl;
	fout << m_bRepeat << endl;
}

void CAnimator2D::LoadFromFile(FILE* _File)
{
	// 애니메이션 개수 로드
	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < AnimCount; ++i)
	{
		CAnim* pAnim = new CAnim;
		pAnim->LoadFromFile(_File);

		pAnim->m_Animator = this;
		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
	}

	// 플레이 중이던 애니메이션의 키를 불러온다
	wstring PlayAnimName;
	LoadWString(PlayAnimName, _File);

	if (!PlayAnimName.empty())
	{
		m_CurAnim = FindAnim(PlayAnimName);
	}

	fread(&m_bRepeat, sizeof(bool), 1, _File);
}

void CAnimator2D::LoadFromFile(ifstream& fin)
{
	size_t animcount;
	Utils::GetLineUntilString(fin, TagAnimCount);
	fin >> animcount;

	if (animcount == 0)
		return;

	Utils::GetLineUntilString(fin, TagAnimNames);

	wstring path = CPathMgr::GetContentPath();
	path += L"anim\\";

	vector<string> files;
	Utils::LoadAllFilePaths(path, files);

	for (size_t i = 0; i < animcount; i++)
	{
		string animName;
		getline(fin, animName);

		bool isFind = false;
		for (int i = 0; i < files.size(); i++)
		{
			if (files[i].find(animName) != string::npos)
			{
				isFind = true;

				ifstream finAnim(files[i]);

				CAnim* pAnim = new CAnim;
				pAnim->LoadFromFile(finAnim);

				pAnim->m_Animator = this;
				m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
				break;
			}
		}

		if (!isFind)
		{
			MessageBox(nullptr, L"없는 애니메이션입니다.", L"애니메이션 로드 실패", 0);
		}
	}

	string str;
	Utils::GetLineUntilString(fin, TagPlayingAnimName);
	getline(fin, str);

	bool repeat;
	Utils::GetLineUntilString(fin, TagRepeat);
	fin >> repeat;

	Play(ToWString(str), repeat);
}

void CAnimator2D::SaveAllAnim(const wstring& path)
{
	for (auto iter : m_mapAnim)
	{
		string animPath = ToString(path + iter.second->GetName());
		animPath += ExtensionAnim;
		ofstream fout(animPath);
		iter.second->SaveToFile(fout);
	}
}
