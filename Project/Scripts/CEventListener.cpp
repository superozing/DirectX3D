#include "pch.h"
#include "CEventListener.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLogMgr.h>

static string strNotation = "This Is EvenetListen Script";

CEventListener::CEventListener()
	: CScript((UINT)SCRIPT_TYPE::EVENTLISTENER)
	, m_bDrawing(true)
{
	AppendScriptParam("Draw", SCRIPT_PARAM::BOOL, &m_bDrawing);
	AppendScriptParam("", SCRIPT_PARAM::STRING, &strNotation);

	m_vecAllScriptNames.resize((UINT)SCRIPT_TYPE::END);
	for (size_t i = 0; i < (UINT)SCRIPT_TYPE::END; i++)
	{
		m_vecAllScriptNames[i] = ToString(magic_enum::enum_name((SCRIPT_TYPE)i));
	}

	AppendScriptVector("Scripts Lists", &m_vecAllScriptNames, &m_iAllNamesIdx, true, "This is All Script List for Add");
	SameLine();
	AppendMemberFunction("Add Target", SCRIPT_PARAM::FUNC_MEMBER, "Add Target",
						 std::bind(&CEventListener::AddTargetEditor, this));

	AppendScriptVector("Target Scripts", &m_vecScriptNames, &m_iNamesIdx, true, "This is Current Has Script List");
	SameLine();
	AppendMemberFunction("Sub Target", SCRIPT_PARAM::FUNC_MEMBER, "Sub Target",
						 std::bind(&CEventListener::SubTargetEditor, this));
}

CEventListener::~CEventListener()
{
}

void CEventListener::AddTarget(SCRIPT_TYPE _type)
{
	auto iter = find(m_vecTargets.begin(), m_vecTargets.end(), _type);
	if (iter == m_vecTargets.end())
	{
		m_vecTargets.push_back(_type);
		m_vecScriptNames.push_back(ToString(magic_enum::enum_name((SCRIPT_TYPE)_type)));
	}
}

void CEventListener::SubTarget(SCRIPT_TYPE _type)
{
	auto iter = find(m_vecTargets.begin(), m_vecTargets.end(), _type);
	if (iter != m_vecTargets.end())
		m_vecTargets.erase(iter);

	auto name	 = ToString(magic_enum::enum_name(_type));
	auto strIter = find(m_vecScriptNames.begin(), m_vecScriptNames.end(), name);
	if (strIter != m_vecScriptNames.end())
		m_vecScriptNames.erase(strIter);
}

void CEventListener::AddTargetEditor()
{
	AddTarget((SCRIPT_TYPE)m_iAllNamesIdx);
}

void CEventListener::SubTargetEditor()
{
	m_vecTargets.erase(m_vecTargets.begin() + m_iNamesIdx);
	m_vecScriptNames.erase(m_vecScriptNames.begin() + m_iNamesIdx);
}

void CEventListener::tick()
{
	if (m_bDrawing)
	{
		if (HasTargets())
			GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(0.f, 1.f, 1.f), false);
		else
			GamePlayStatic::DrawDebugCube(Transform()->GetWorldMat(), Vec3(1.f, 1.f, 0.f), false);
	}

	Test();
}

void CEventListener::begin()
{
	if (!ComponentCheck())
		return;

	if (MeshRender())
		GetOwner()->DeleteComponent(COMPONENT_TYPE::MESHRENDER);
}

void CEventListener::BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	for (size_t i = 0; i < m_vecTargets.size(); i++)
	{
		auto scripts = _OtherObj->GetScripts();
		for (size_t j = 0; j < scripts.size(); j++)
		{
			if (scripts[j]->GetScriptType() == (UINT)m_vecTargets[i])
			{
				m_iInternalTargetCnt++;
				return;
			}
		}
	}
}

void CEventListener::EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider)
{
	for (size_t i = 0; i < m_vecTargets.size(); i++)
	{
		auto scripts = _OtherObj->GetScripts();
		for (size_t j = 0; j < scripts.size(); j++)
		{
			if (scripts[j]->GetScriptType() == (UINT)m_vecTargets[i])
			{
				m_iInternalTargetCnt--;
				return;
			}
		}
	}
}

#define TagTargets "[TargetScripts]"
void CEventListener::SaveToFile(ofstream& fout)
{
	fout << TagTargets << endl;
	fout << m_vecTargets.size() << endl;
	for (size_t i = 0; i < m_vecTargets.size(); i++)
	{
		fout << m_vecScriptNames[i] << endl;
	}
}

void CEventListener::LoadFromFile(ifstream& fin)
{
	Utils::GetLineUntilString(fin, TagTargets);
	string str;
	getline(fin, str);
	int size = stoi(str);

	for (size_t i = 0; i < size; i++)
	{
		getline(fin, str);
		auto type = magic_enum::enum_cast<SCRIPT_TYPE>(str);
		AddTarget(type.value());
	}
}

bool CEventListener::ComponentCheck()
{
	wstring msg;
	msg += GetOwner()->GetName() + L"의 ";
	bool err = false;
	if (!Transform())
	{
		msg += L"Transform ";
		err = true;
	}
	if (!PhysX())
	{
		msg += L"PhysX ";
		err = true;
	}
	if (err)
	{
		msg += L"컴포넌트가 없습니다.";
		MessageBox(nullptr, msg.c_str(), L"EventDetector 오류", 0);

		return false;
	}
	return true;
}

void CEventListener::Test()
{
	if (KEY_TAP(_1))
	{
		static int addtype = 0;
		AddTarget((SCRIPT_TYPE)addtype++);
	}

	if (KEY_TAP(_2))
	{
		static int subtype = 0;
		SubTarget((SCRIPT_TYPE)subtype++);
	}
}
