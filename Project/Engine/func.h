#pragma once

#include <vector>
class CGameObject;
class CAsset;
#include <Magic_Enum/magic_enum_all.hpp>

string ToString(const wstring& _str);
wstring ToWString(const string& _str);
string ToString(const std::string_view& _sv);
wstring ToWString(const std::string_view& _sv);

namespace RoRMath
{
	float Lerp(float A, float B, float Alpha);
	Vec2  Lerp(Vec2 A, Vec2 B, float Alpha);
	Vec3  Lerp(Vec3 A, Vec3 B, float Alpha);
	Vec4  Lerp(Vec4 A, Vec4 B, float Alpha);

	int	  ClampInt(int _input, int _min, int _max = INT_MAX);
	float ClampFloat(float _input, float _min);
	float ClampFloat(float _input, float _min, float _max);

	void MatrixToFloat16(float _dest[16], const Matrix& _src);
	void Float16ToMatrix(Matrix& _dest, const float _src[16]);
}

class CLevel;
namespace GamePlayStatic
{
	void SpawnGameObject(CGameObject* _Target, int _LayerIdx);
	void DestroyGameObject(CGameObject* _Target);

	void AddAsset(CAsset* _Asset);

	void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
		
	void DrawDebugCircle(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap = true);
	void Play2DBGM(const wstring& _SoundPath, float _Volume);

	void DrawDebugCube(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugCube(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void DrawDebugCone(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	
	class COLOR
	{
		// 유지보수 1/4 .Enum추가
		enum class DEFAULT_COLOR
		{
			WHITE,
			BLACK,
			RED,
			GREEN,
			BLUE,
			YELLOW,
			MAZENTA,
			END,
		};

		// 유지보수 2/4 . 컬러변수 추가
		static const Vec4 WHITE;
		static const Vec4 BLACK;
		static const Vec4 RED;
		static const Vec4 GREEN;
		static const Vec4 BLUE;
		static const Vec4 YELLOW;
		static const Vec4 MAZENTA;

	public:
		// 유지보수 3/4 . 컬러변수 맵핑
		static vector<std::pair<string, Vec4>> GetColors()
		{
			return
			{
				{ToString(magic_enum::enum_name(DEFAULT_COLOR::WHITE))	   , WHITE    }
				, {ToString(magic_enum::enum_name(DEFAULT_COLOR::BLACK))   , BLACK    }
				, {ToString(magic_enum::enum_name(DEFAULT_COLOR::RED))	   , RED      }
				, {ToString(magic_enum::enum_name(DEFAULT_COLOR::GREEN))   , GREEN    }
				, {ToString(magic_enum::enum_name(DEFAULT_COLOR::YELLOW))  , YELLOW   }
				, {ToString(magic_enum::enum_name(DEFAULT_COLOR::MAZENTA)) , MAZENTA  }
			};
		}
	};

	void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelStartState);
}

namespace Utils
{
	/// <summary>
	/// 경로의 모든 파일의 확장자를 포함한 파일 이름을 추출해줍니다. 하위 폴더도 포함됩니다.
	/// </summary>
	void LoadAllFileNames(const wstring& _path, vector<string>& vec);

	/// <summary>
	/// 경로의 모든 파일의 경로를 추출해줍니다. 하위폴더도 포함됩니다.
	/// </summary>
	void LoadAllFilePaths(const wstring& _path, vector<string>& vec);
	/// <summary>
	/// 경로를 갖고 있는 파일들을 경로를 제외하고 추출해줍니다. 해당 경로를 갖고있지 않는다면 제외시킵니다.
	/// </summary>
	void SlicePath(const wstring& _path, vector<string>& vec);

	/// <summary>
	/// 원하는 문자열중 하나가 나올 때 까지 파일을 끝까지 읽고 성공시 읽은 문자열을 반환합니다. 리딩 실패시 메시지를 띄웁니다.
	/// </summary>
	string GetLineUntilString(ifstream& fin, const std::initializer_list<string>& strings);

	/// <summary>
	/// 원하는 문자열중 하나가 나올 때 까지 파일을 끝까지 읽고 성공시 읽은 문자열을 반환합니다. 리딩 실패시 메시지를 띄웁니다.
	/// </summary>
	string GetLineUntilString(ifstream& fin, const string& strings);

	/// <summary>
	/// 원하는 문자열중 하나가 나올 때 까지 파일을 끝까지 읽고 성공시 읽은 문자열을 반환합니다. 리딩 실패시 메시지를 띄웁니다.
	/// </summary>
	string GetLineUntilString(ifstream& fin, const std::initializer_list<const char*> strings);
}

string ToString(const wstring& _str);
wstring ToWString(const string& _str);
string ToString(const std::string_view& _sv);
wstring ToWString(const std::string_view& _sv);

void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _FILE);


Vec3 DecomposeRotMat(const Matrix& _matRot);

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat);

template<typename T>
class Ptr;

#include "CAssetMgr.h"

template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	bool bAssetExist = false;
	_Asset == nullptr ? bAssetExist = false : bAssetExist = true;
	
	fwrite(&bAssetExist, sizeof(bool), 1, _File);

	if (bAssetExist)
	{
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

#define TagAssetExist "[AssetExist]"
#define TagKey "[Key]"
#define TagPath "[Path]"

template<typename T>
void SaveAssetRef(Ptr<T> _Asset, ofstream& fout)
{
	fout << TagAssetExist << endl;
	bool bAssetExist = false;
	_Asset == nullptr ? bAssetExist = false : bAssetExist = true;

	fout << bAssetExist << endl;

	if (bAssetExist)
	{
		fout << TagKey << endl;
		fout << ToString(_Asset->GetKey()) << endl;
		fout << TagPath << endl;
		fout << ToString(_Asset->GetRelativePath()) << endl;
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{	
	bool bAssetExist = false;
	fread(&bAssetExist, sizeof(bool), 1, _File);

	if (bAssetExist)
	{
		wstring strKey, strRelativePath;

		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		_Asset = CAssetMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, ifstream& fin)
{
	bool exist;
	Utils::GetLineUntilString(fin, TagAssetExist);
	fin >> exist;

	if (exist) 
	{
		string key, path;

		Utils::GetLineUntilString(fin, TagKey);
		getline(fin, key);

		Utils::GetLineUntilString(fin, TagPath);
		getline(fin, path);

		_Asset = CAssetMgr::GetInst()->Load<T>(key, path);
	}
}

template<typename T, UINT SIZE>
void Delete_Array(T* (&Arr)[SIZE])
{
	for (int i = i = 0; i < SIZE; ++i)
	{
		if (nullptr != Arr[i])
		{
			delete Arr[i];
			Arr[i] = nullptr;
		}
	}
}


template<typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}

template<typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		delete pair.second;
	}
	_map.clear();
}

