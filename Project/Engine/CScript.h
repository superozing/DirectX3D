#pragma once
#include "CComponent.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
// #include "CResMgr.h"

#include "CGameObject.h"
#include "components.h"

#include <variant>

typedef void (*StaticFuncPtr)();
struct tScriptParam
{
	SCRIPT_PARAM		  Type;
	void*				  pData;
	float				  fmin;
	float				  fMax;
	bool				  View;
	string				  Tooltip;
	bool				  b_Precision;
	StaticFuncPtr		  StaticFunc;
	std::function<void()> MemberFunc;
	COMPONENT_TYPE		  CompType;
	UINT				  ScriptType;
	ASSET_TYPE			  AssetType;
	int*				  idx;

	// public:
	//     tScriptParam(SCRIPT_PARAM type, void* data, float _min, float _max, bool view, const string& tooltip,
	//     FUNC_PARAM funcType, const std::vector<std::variant<int, float, Vec2, Vec3, Vec4>>& args)
	//         : Type(type), pData(data), fmin(_min), fMax(_max), View(view), Tooltip(tooltip), CallbackArgs(args) {}
};

class CScript : public CComponent
{
private:
	const UINT								m_iScriptType;
	vector<std::pair<string, tScriptParam>> m_vScriptParam;

public:
	UINT		GetScriptType() const { return m_iScriptType; }
	const auto& GetScriptParam() { return m_vScriptParam; }

protected:
	void Instantiate(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _LayerIdx);

	/// @brief 파람 UI에 기본적인 스크립트 파람 타입들을 제공하는 함수입니다. 오브젝트 타입을 사용하려면
	/// AppendScriptObject를 사용해야 합니다.
	/// @param _Key Desc으로 들어가는 string
	/// @param _Param Param 타입
	/// @param _Data 넘기고 싶은 데이터 주소
	/// @param _min 최솟값
	/// @param _Max 최댓값
	/// @param _View 변경할 지 말지 여부
	/// @param _Tooltip 호버했을 때 뜨는 스트링 창
	void AppendScriptParam(const string& _Key, SCRIPT_PARAM _Param, void* _Data, float _min = 0.f, float _Max = 0.f,
						   bool _View = false, const string& _Tooltip = {}, bool _Precision = false)
	{
		m_vScriptParam.push_back({_Key, tScriptParam{_Param, _Data, _min, _Max, _View, _Tooltip, _Precision}});
	}

	/// @brief 파람 UI에 오브젝트를 추가하는 함수, 컴포넌트 타입이나 스크립트 타입으로 받고싶은 오브젝트를 필터링
	/// 합니다.
	/// @param _CType 제한하고 싶은 컴포넌트 타입(해당 컴포넌트가 있는 obj들만 들어옴)
	/// @param _SType 제한하고 싶은 스크립트 타입(해당 스크립트가 있는 obj들만 들어옴)
	void AppendScriptObject(const string& _Key, void* _Data, COMPONENT_TYPE _CType = COMPONENT_TYPE::END,
							UINT _SType = -1, bool _View = false, const string& _Tooltip = {})
	{
		m_vScriptParam.push_back({_Key, tScriptParam{SCRIPT_PARAM::OBJECT, _Data, 0, 0, _View, _Tooltip, false, nullptr,
													 nullptr, _CType, _SType}});
	}

	void AppendScriptAsset(const string& _Key, void* _Data, ASSET_TYPE _Type = ASSET_TYPE::END, bool _View = false,
						   const string& _Tooltip = {})
	{
		m_vScriptParam.push_back({_Key, tScriptParam{SCRIPT_PARAM::ASSET, _Data, 0, 0, _View, _Tooltip, false, nullptr,
													 nullptr, COMPONENT_TYPE::END, (UINT)-1, _Type}});
	}

	void AppendScriptVector(const string& _Key, void* _Data, int* _idx, bool _View = false, const string& _Tooltip = {})
	{
		m_vScriptParam.push_back({_Key, tScriptParam{SCRIPT_PARAM::VECTOR, _Data, 0, 0, _View, _Tooltip, false, nullptr,
													 nullptr, COMPONENT_TYPE::END, (UINT)-1, ASSET_TYPE::END, _idx}});
	}

	void AppendStaticFunction(const string& _Key, SCRIPT_PARAM _Param, string _Desc, StaticFuncPtr _StaticFuncPtr)
	{
		// 새로운 스크립트 파라미터 생성 후 맵에 추가
		m_vScriptParam.push_back({_Key, tScriptParam{_Param, nullptr, 0.f, 0.f, false, _Desc, false, _StaticFuncPtr}});
	}

	void AppendMemberFunction(const string& _Key, SCRIPT_PARAM _Param, string _Desc, std::function<void()> _MemberFunc)
	{
		// 새로운 스크립트 파라미터 생성 후 맵에 추가
		m_vScriptParam.push_back(
			{_Key, tScriptParam{_Param, nullptr, 0.f, 0.f, false, _Desc, false, nullptr, _MemberFunc}});
	}

	void AppendSeperateLine() { m_vScriptParam.push_back({"", tScriptParam{SCRIPT_PARAM::LINE}}); }
	void SameLine() { m_vScriptParam.push_back({"", tScriptParam{SCRIPT_PARAM::SAMELINE}}); }

public:
	virtual void finaltick() final {}
	virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
	virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
	virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}

	virtual void BeginOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) {}
	virtual void Overlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) {}
	virtual void EndOverlap(CPhysX* _Collider, CGameObject* _OtherObj, CPhysX* _OtherCollider) {}

	virtual void SaveToFile(FILE* _File){};
	virtual void SaveToFile(ofstream& fout){};
	virtual void LoadFromFile(FILE* _File){};
	virtual void LoadFromFile(ifstream& fin){};

	virtual void UpdateData(){};
	virtual void Clear(){};

	CScript* Clone() = 0;

public:
	CScript(UINT _ScriptType);
	~CScript();
};
typedef void (CScript::*Delegate_S)();
typedef bool (CScript::*Delegate_BS)();