#include "pch.h"

#include "CTaskMgr.h"
#include "CRenderMgr.h"
#include "CLevel.h"

void GamePlayStatic::SpawnGameObject(CGameObject* _Target, int _LayerIdx)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_1 = (DWORD_PTR)_LayerIdx;
	task.Param_2 = (DWORD_PTR)_Target;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _Target)
{
	tTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_1 = (DWORD_PTR)_Target;	
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::AddAsset(CAsset* _Asset)
{
	tTask task = {};
	task.Type = TASK_TYPE::ADD_ASSET;
	task.Param_1 = (DWORD_PTR)_Asset;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::RECT;
	info.matWorld = _WorldMat;
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::RECT;

	info.vWorldPos = _vWorldPos;	
	info.vWorldScale = _vWorldScale;
	info.vWorldRot = _vWorldRot;

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
					* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
					* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);
		
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCube(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CUBE;
	info.matWorld = _WorldMat;
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCube(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CUBE;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = _vWorldScale;
	info.vWorldRot = _vWorldRot;

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::SPHERE;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius * 2.f, _fRadius * 2.f, _fRadius * 2.f);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCone(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CONE;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = _vWorldScale;
	info.vWorldRot = _vWorldRot;

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

#include "CLevel.h"
void GamePlayStatic::ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelStartState)
{
	tTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param_1 = (DWORD_PTR)_NextLevel;
	task.Param_2 = (DWORD_PTR)_NextLevelStartState;

	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CIRCLE;

	info.matWorld = _WorldMat;
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CIRCLE;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius * 2.f, _fRadius * 2.f, 1.f);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CROSS;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fScale, _fScale, _fScale);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap)
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		pSound->Play(_Loop, _Volume, _Overlap);
	}
}

void GamePlayStatic::Play2DBGM(const wstring& _SoundPath, float _Volume)
{
	static Ptr<CSound> CurBGM = nullptr;

	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		if (nullptr != CurBGM)
		{
			CurBGM->Stop();
		}

		pSound->Play(0, _Volume);
		CurBGM = pSound;
	}
}


void Vector3::ToDegree()
{
	x = (x / XM_PI) * 180.f;
	y = (y / XM_PI) * 180.f;
	z = (z / XM_PI) * 180.f;
}

void Vector3::ToRadian()
{
	x = (x / 180.f) * XM_PI;
	y = (y / 180.f) * XM_PI;
	z = (z / 180.f) * XM_PI;
}

string ToString(const wstring& _str)
{
	return string(_str.begin(), _str.end());
}

wstring ToWString(const string& _str)
{
	return wstring(_str.begin(), _str.end());
}

string ToString(const std::string_view& _sv)
{
	return string(_sv.begin(), _sv.end());
}

wstring ToWString(const std::string_view& _sv)
{
	return wstring(_sv.begin(), _sv.end());
}


void SaveWString(const wstring& _str, FILE* _File)
{	
	size_t len = _str.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWString(wstring& _str, FILE* _FILE)
{
	size_t len = 0;
	wchar_t szBuff[256] = {};
	fread(&len, sizeof(size_t), 1, _FILE);
	fread(szBuff, sizeof(wchar_t), len, _FILE);
	_str = szBuff;
}

#include "CPathMgr.h"
#include <filesystem>
#include "func.h"
void Utils::LoadAllFileNames(const wstring& _path, vector<string>& vec)
{
	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(_path)) {
		if (entry.is_directory()) {
			LoadAllFileNames(entry.path().wstring(), vec);
		}
		else {
			string str = entry.path().filename().string();
			vec.push_back(str);
		}
	}
}

void Utils::LoadAllFilePaths(const wstring& _path, vector<string>& vec)
{
	namespace fs = std::filesystem;
	for (const fs::directory_entry& entry : fs::directory_iterator(_path)) {
		if (entry.is_directory()) {
			LoadAllFilePaths(entry.path().wstring(), vec);
		}
		else {
			string str = entry.path().string();
			vec.push_back(str);
		}
	}
}

void Utils::SlicePath(const wstring& _path, vector<string>& vec)
{
	vector<string> paths;
	for (const auto& str : vec) {
		if (str.find(ToString(_path)) == string::npos) continue;
		paths.push_back(str.substr(_path.length()));
	}
	vec.swap(paths);
}

string Utils::GetLineUntilString(ifstream& fin, const std::initializer_list<string>& strings)
{
	string s;

	while (getline(fin, s)) {

		for (const string& str : strings) {
			if (s == str) {
				return str;
			}
		}
	}

	wstring wstr;
	for (const string& str : strings) {
		wstr += ToWString(str) + L", ";
	}
	wstr.pop_back();
	wstr.pop_back();
	MessageBox(nullptr, wstr.c_str(), L"파일 읽기 실패", 0);
	return string();
}

string Utils::GetLineUntilString(ifstream& fin, const string& str)
{
	string s;

	while (getline(fin, s)) {

		if (s == str) {
			return str;
		}
	}

	wstring wstr = ToWString(str);
	MessageBox(nullptr, wstr.c_str(), L"파일 읽기 실패", 0);
	return string();
}

string Utils::GetLineUntilString(ifstream& fin, const std::initializer_list<const char*> strings)
{
	string s;

	while (getline(fin, s)) {

		for (const char* str : strings) {
			if (s == str) {
				return str;
			}
		}
	}

	wstring wstr;
	for (const string& str : strings) {
		wstr += ToWString(str) + L", ";
	}
	wstr.pop_back();
	wstr.pop_back();
	MessageBox(nullptr, wstr.c_str(), L"파일 읽기 실패", 0);
	return string();
}

// 유지보수 4/4 . 컬러변수 값할당
const Vec4 GamePlayStatic::COLOR::WHITE = Vec4(1.f, 1.f, 1.f, 1.f);
const Vec4 GamePlayStatic::COLOR::BLACK = Vec4(0.f, 0.f, 0.f, 1.f);
const Vec4 GamePlayStatic::COLOR::RED = Vec4(1.f, 0.f, 0.f, 1.f);
const Vec4 GamePlayStatic::COLOR::GREEN = Vec4(0.f, 1.f, 0.f, 1.f);
const Vec4 GamePlayStatic::COLOR::BLUE = Vec4(0.f, 0.f, 1.f, 1.f);
const Vec4 GamePlayStatic::COLOR::YELLOW = Vec4(1.f, 1.f, 0.f, 1.f);
const Vec4 GamePlayStatic::COLOR::MAZENTA = Vec4(1.f, 0.f, 1.f, 1.f);

float RoRMath::Lerp(float A, float B, float Alpha)
{
	return A * (1 - Alpha) + B * Alpha;
}

Vec2 RoRMath::Lerp(Vec2 A, Vec2 B, float Alpha)
{
	float x = RoRMath::Lerp(A.x, B.x, Alpha);
	float y = RoRMath::Lerp(A.y, B.y, Alpha);

	return Vec2(x, y);
}

Vec3 RoRMath::Lerp(Vec3 A, Vec3 B, float Alpha)
{
	float x = RoRMath::Lerp(A.x, B.x, Alpha);
	float y = RoRMath::Lerp(A.y, B.y, Alpha);
	float z = RoRMath::Lerp(A.z, B.z, Alpha);
	
	return Vec3(x, y, z);
}

Vec4 RoRMath::Lerp(Vec4 A, Vec4 B, float Alpha)
{
	float x = RoRMath::Lerp(A.x, B.x, Alpha);
	float y = RoRMath::Lerp(A.y, B.y, Alpha);
	float z = RoRMath::Lerp(A.z, B.z, Alpha);
	float w = RoRMath::Lerp(A.w, B.w, Alpha);

	return Vec4(x, y, z, w);
}

int RoRMath::ClampInt(int _input, int _min, int _max)
{
	if (_min > _input)
		return _min;

	if (_max < _input)
		return _max;

	return _input;
}

float RoRMath::ClampFloat(float _input, float _min)
{
	if (_min > _input)
		return _min;

	return _input;
}

float RoRMath::ClampFloat(float _input, float _min, float _max)
{
	if (_min > _input)
		return _min;

	if (_max < _input)
		return _max;

	return _input;
}

void RoRMath::MatrixToFloat16(float _dest[16], const Matrix& _src)
{
	_dest[0] = _src._11;
	_dest[1] = _src._12;
	_dest[2] = _src._13;
	_dest[3] = _src._14;

	_dest[4] = _src._21;
	_dest[5] = _src._22;
	_dest[6] = _src._23;
	_dest[7] = _src._24;

	_dest[8] = _src._31;
	_dest[9] = _src._32;
	_dest[10] = _src._33;
	_dest[11] = _src._34;

	_dest[12] = _src._41;
	_dest[13] = _src._42;
	_dest[14] = _src._43;
	_dest[15] = _src._44;
}

void RoRMath::Float16ToMatrix(Matrix& _dest, const float _src[16])
{
	_dest._11 = _src[0];
	_dest._12 = _src[1];
	_dest._13 = _src[2];
	_dest._14 = _src[3];

	_dest._21 = _src[4];
	_dest._22 = _src[5];
	_dest._23 = _src[6];
	_dest._24 = _src[7];

	_dest._31 = _src[8];
	_dest._32 = _src[9];
	_dest._33 = _src[10];
	_dest._34 = _src[11];

	_dest._41 = _src[12];
	_dest._42 = _src[13];
	_dest._43 = _src[14];
	_dest._44 = _src[15];
}


bool closeEnough(const float& a, const float& b
	, const float& epsilon = std::numeric_limits<float>::epsilon())
{
	return (epsilon > std::abs(a - b));
}

Vec3 DecomposeRotMat(const Matrix& _matRot)
{
	// _mat 을 분해 후 다시 행렬 만들기	
	Vec4 vMat[4];

	vMat[0] = Vec4(_matRot._11, _matRot._12, _matRot._13, _matRot._14);
	vMat[1] = Vec4(_matRot._21, _matRot._22, _matRot._23, _matRot._24);
	vMat[2] = Vec4(_matRot._31, _matRot._32, _matRot._33, _matRot._34);
	vMat[3] = Vec4(_matRot._41, _matRot._42, _matRot._43, _matRot._44);

	/*XMStoreFloat4(&vMat[0], _matRot._11);
	XMStoreFloat4(&vMat[1], _matRot.r[1]);
	XMStoreFloat4(&vMat[2], _matRot.r[2]);
	XMStoreFloat4(&vMat[3], _matRot.r[3]);*/

	Vec3 vNewRot;
	if (closeEnough(vMat[0].z, -1.0f)) {
		float x = 0; //gimbal lock, value of x doesn't matter
		float y = XM_PI / 2;
		float z = x + atan2f(vMat[1].x, vMat[2].x);
		vNewRot = Vec3{ x, y, z };
	}
	else if (closeEnough(vMat[0].z, 1.0f)) {
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2f(-vMat[1].x, -vMat[2].x);
		vNewRot = Vec3{ x, y, z };
	}
	else { //two solutions exist
		float y1 = -asinf(vMat[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(vMat[1].z / cosf(y1), vMat[2].z / cosf(y1));
		float x2 = atan2f(vMat[1].z / cosf(y2), vMat[2].z / cosf(y2));

		float z1 = atan2f(vMat[0].y / cosf(y1), vMat[0].x / cosf(y1));
		float z2 = atan2f(vMat[0].y / cosf(y2), vMat[0].x / cosf(y2));

		//choose one solution to return
		//for example the "shortest" rotation
		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			vNewRot = Vec3{ x1, y1, z1 };
		}
		else {
			vNewRot = Vec3{ x2, y2, z2 };
		}
	}
	return vNewRot;
}

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat)
{
	Matrix mat;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat.m[i][j] = (float)_mat.Get(i, j);
		}
	}
	return mat;
}
