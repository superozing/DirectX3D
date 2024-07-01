#pragma once

#include <stdlib.h>
#include <Windows.h>
#include <typeinfo>

#include <string>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>

using std::list;
using std::make_pair;
using std::map;
using std::unordered_map;
using std::vector;

using std::string;
using std::wstring;

#include <memory>
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

#include <wrl.h>
using namespace Microsoft::WRL;

#include <filesystem>
using namespace std::filesystem;

#include <fstream>
using std::endl;
using std::ifstream;
using std::ofstream;

// Directx 11 헤더 및 라이브러리 참조
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// DirectxTex
// vcpkg install directxtex[openexr]:x64-windows
#include <DirectXTex.h>

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// Fbx Loader
#include <FBXLoader/fbxsdk.h>
#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif

// 엔진 헤더 참조
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"

// Magic Enum 헤더 참조
#include <Magic_Enum/magic_enum_all.hpp>

// String Table(Gen)
#include "GenStrings.h"

// String Table(User)
#include "UserStrings.h"

#include "physx/PxPhysicsAPI.h"
using namespace physx;