#include "pch.h"

#define TagLightType "[LightType]"
#define TagLightColor "[LightColor]"
#define TagSpecular "[Specular]"
#define TagAmbient "[Ambient]"
#define TagWorldPos "[WorldPos]"
#define TagWorldDir "[WorldDir]"
#define TagRadius "[Radius]"
#define TagAngle "[Angle]"
#define TagIsToon "[IsToon]"
#define TagToonRange "[ToonRange]"

ofstream& operator<<(ofstream& fout, const tLightInfo& info)
{
	fout << TagLightType << endl;
	auto str = magic_enum::enum_name((LIGHT_TYPE)info.LightType);
	fout << ToString(str) << endl;

	fout << TagLightColor << endl;
	fout << info.vColor << endl;

	fout << TagSpecular << endl;
	fout << info.vSpecular << endl;

	fout << TagAmbient << endl;
	fout << info.vAmbient << endl;

	fout << TagWorldPos << endl;
	fout << info.vWorldPos << endl;

	fout << TagWorldDir << endl;
	fout << info.vWorldDir << endl;

	fout << TagRadius << endl;
	fout << info.fRadius << endl;

	fout << TagAngle << endl;
	fout << info.fAngle;

	fout << TagIsToon << endl;
	fout << info.ToonShading;

	fout << TagToonRange << endl;
	fout << info.vToonShadeRange;

	return fout;
}

ifstream& operator>>(ifstream& fin, tLightInfo& info)
{
	string str;
	Utils::GetLineUntilString(fin, TagLightType);
	getline(fin, str);
	auto type = magic_enum::enum_cast<LIGHT_TYPE>(str);
	if (!type.has_value())
	{
		MessageBox(nullptr, L"없는 Enum 타입입니다.", L"Light 3D Load", 0);
		assert(0);
	}
	info.LightType = (int)type.value();

	Utils::GetLineUntilString(fin, TagLightColor);
	fin >> info.vColor;

	Utils::GetLineUntilString(fin, TagSpecular);
	fin >> info.vSpecular;

	Utils::GetLineUntilString(fin, TagAmbient);
	fin >> info.vAmbient;

	Utils::GetLineUntilString(fin, TagWorldPos);
	fin >> info.vWorldPos;

	Utils::GetLineUntilString(fin, TagWorldDir);
	fin >> info.vWorldDir;

	Utils::GetLineUntilString(fin, TagRadius);
	fin >> info.fRadius;

	Utils::GetLineUntilString(fin, TagAngle);
	fin >> info.fAngle;

	Utils::GetLineUntilString(fin, TagIsToon);
	fin >> info.ToonShading;

	Utils::GetLineUntilString(fin, TagToonRange);
	fin >> info.vToonShadeRange;

	return fin;
}

#define TagModuleName(moduleNum) "[" + ToString(magic_enum::enum_name(PARTICLE_MODULE::moduleNum)) + "]"
#define TagSpawnType "[SpawnType]"
#define TagBurstInterval "[BurstInterval]"
#define TagSpawnColor "[SpawnColor]"
#define TagSpawnMinScale "[SpawnMinScale]"
#define TagSpawnMaxScale "[SpawnMaxScale]"
#define TagMinLife "[MinLife]"
#define TagMaxLife "[MaxLife]"
#define TagMinMass "[MinMass]"
#define TagMaxMass "[MaxMass]"
#define TagSpawnRate "[SpawnRate]"
#define TagSpaceType "[SpaceType] (0 : LocalSpace, 1 : WorldSpace)"
#define TagSpawnShape "[SpawnShape] (0 : Sphere, 1 : Box)"
#define TagRadius "[Radius]"
#define TagSpawnBoxScale "[SpawnBoxScale]"

#define TagAddVelocityType "[AddVelocityType] (0 : From Center, 1: To Center, 2: Fix Direction)"
#define TagMinSpeed "[MinSpeed]"
#define TagMaxSpeed "[MaxSpeed]"
#define TagFixedAngle "[FixedAngle]"
#define TagFixedDirection "[FixedDirection]"

#define TagScaleRatio "[ScaleRatio]"

#define TagNoiseForceScale "[NoiseForceScale]"
#define TagNoiseForceTerm "[NoiseForceTerm]"

#define TagDragTime "[DragTime]"

#define TagVelocityAlignment "[VelocityAlignment]"
#define TagAlphaBasedLife "[[AlphaBasedLife] (0 : off, 1 : NomrlizedAge, 2: Age)]"
#define TagAlphaMaxAge "[AlphaMaxAge]"

ofstream& operator<<(ofstream& fout, const tParticleModule& module)
{
	// Sapwn 모듈
	fout << TagModuleName(SPAWN) << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] << endl;

	fout << TagSpawnType << endl;
	fout << module.SpawnType << endl;

	fout << TagBurstInterval << endl;
	fout << module.BurstInterval << endl;

	fout << TagSpawnColor << endl;
	fout << module.vSpawnColor << endl;

	fout << TagSpawnMinScale << endl;
	fout << module.vSpawnMinScale << endl;
	fout << TagSpawnMaxScale << endl;
	fout << module.vSpawnMaxScale << endl;

	fout << TagMinLife << endl;
	fout << module.MinLife << endl;
	fout << TagMaxLife << endl;
	fout << module.MaxLife << endl;

	fout << TagMinMass << endl;
	fout << module.MinMass << endl;
	fout << TagMaxMass << endl;
	fout << module.MaxMass << endl;

	fout << TagSpawnRate << endl;
	fout << module.SpawnRate << endl;

	fout << TagSpaceType << endl;
	fout << module.SpaceType << endl;

	fout << TagSpawnShape << endl;
	fout << module.SpawnShape << endl;

	fout << TagRadius << endl;
	fout << module.Radius << endl;

	fout << TagSpawnBoxScale << endl;
	fout << module.vSpawnBoxScale << endl;

	// Add Velocity
	fout << TagModuleName(ADD_VELOCITY) << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] << endl;

	fout << TagAddVelocityType << endl;
	fout << module.AddVelocityType << endl;

	fout << TagMinSpeed << endl;
	fout << module.MinSpeed << endl;
	fout << TagMaxSpeed << endl;
	fout << module.MaxSpeed << endl;

	fout << TagFixedAngle << endl;
	fout << module.FixedAngle << endl;

	fout << TagFixedDirection << endl;
	fout << module.FixedDirection << endl;

	// Scale
	fout << TagModuleName(SCALE) << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] << endl;

	fout << TagScaleRatio << endl;
	fout << module.vScaleRatio << endl;

	// Noise Force
	fout << TagModuleName(NOISE_FORCE) << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] << endl;

	fout << TagNoiseForceScale << endl;
	fout << module.NoiseForceScale << endl;

	fout << TagNoiseForceTerm << endl;
	fout << module.NoiseForceTerm << endl;

	// Drag
	fout << TagModuleName(DRAG) << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] << endl;

	fout << TagDragTime << endl;
	fout << module.DragTime << endl;

	// Render
	fout << TagModuleName(RENDER) << endl;
	fout << module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] << endl;

	fout << TagVelocityAlignment << endl;
	fout << module.VelocityAlignment << endl;
	fout << TagAlphaBasedLife << endl;
	fout << module.AlphaBasedLife << endl;
	fout << TagAlphaMaxAge << endl;
	fout << module.AlphaMaxAge << endl;

	return fout;
}

ifstream& operator>>(ifstream& fin, tParticleModule& module)
{
	// 스폰 모듈
	Utils::GetLineUntilString(fin, TagModuleName(SPAWN));
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN];

	Utils::GetLineUntilString(fin, TagSpawnType);
	fin >> module.SpawnType;

	Utils::GetLineUntilString(fin, TagBurstInterval);
	fin >> module.BurstInterval;

	Utils::GetLineUntilString(fin, TagSpawnColor);
	fin >> module.vSpawnColor;

	Utils::GetLineUntilString(fin, TagSpawnMinScale);
	fin >> module.vSpawnMinScale;

	Utils::GetLineUntilString(fin, TagSpawnMaxScale);
	fin >> module.vSpawnMaxScale;

	Utils::GetLineUntilString(fin, TagMinLife);
	fin >> module.MinLife;

	Utils::GetLineUntilString(fin, TagMaxLife);
	fin >> module.MaxLife;

	Utils::GetLineUntilString(fin, TagMinMass);
	fin >> module.MinMass;

	Utils::GetLineUntilString(fin, TagMaxMass);
	fin >> module.MaxMass;

	Utils::GetLineUntilString(fin, TagSpawnRate);
	fin >> module.SpawnRate;

	Utils::GetLineUntilString(fin, TagSpaceType);
	fin >> module.SpaceType;

	Utils::GetLineUntilString(fin, TagSpawnShape);
	fin >> module.SpawnShape;

	Utils::GetLineUntilString(fin, TagRadius);
	fin >> module.Radius;

	Utils::GetLineUntilString(fin, TagSpawnBoxScale);
	fin >> module.vSpawnBoxScale;

	// Add Velocity
	Utils::GetLineUntilString(fin, TagModuleName(ADD_VELOCITY));
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY];

	Utils::GetLineUntilString(fin, TagAddVelocityType);
	fin >> module.AddVelocityType;

	Utils::GetLineUntilString(fin, TagMinSpeed);
	fin >> module.MinSpeed;

	Utils::GetLineUntilString(fin, TagMaxSpeed);
	fin >> module.MaxSpeed;

	Utils::GetLineUntilString(fin, TagFixedAngle);
	fin >> module.FixedAngle;

	Utils::GetLineUntilString(fin, TagFixedDirection);
	fin >> module.FixedDirection;

	// Scale Module
	Utils::GetLineUntilString(fin, TagModuleName(SCALE));
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE];

	Utils::GetLineUntilString(fin, TagScaleRatio);
	fin >> module.vScaleRatio;

	// Noise Force
	Utils::GetLineUntilString(fin, TagModuleName(NOISE_FORCE));
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE];

	Utils::GetLineUntilString(fin, TagNoiseForceScale);
	fin >> module.NoiseForceScale;

	Utils::GetLineUntilString(fin, TagNoiseForceTerm);
	fin >> module.NoiseForceTerm;

	// Drag
	Utils::GetLineUntilString(fin, TagModuleName(DRAG));
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG];

	Utils::GetLineUntilString(fin, TagDragTime);
	fin >> module.DragTime;

	// Render
	Utils::GetLineUntilString(fin, TagModuleName(RENDER));
	fin >> module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER];

	Utils::GetLineUntilString(fin, TagVelocityAlignment);
	fin >> module.VelocityAlignment;

	Utils::GetLineUntilString(fin, TagAlphaBasedLife);
	fin >> module.AlphaBasedLife;

	Utils::GetLineUntilString(fin, TagAlphaMaxAge);
	fin >> module.AlphaMaxAge;

	return fin;
}

#define TagIntArr "[IntArr]"
#define TagFloatArr "[FloatArr]"
#define TagVec2Arr "[Vec2Arr]"
#define TagVec4Arr "[Vec4Arr]"
#define TagMatrixArr "[MatrixArr]"

ofstream& operator<<(ofstream& fout, const tMtrlConst& _mtrlConst)
{
	fout << TagIntArr << endl;
	for (auto& num : _mtrlConst.iArr)
		fout << num << " ";
	fout << endl;

	fout << TagFloatArr << endl;
	for (auto& num : _mtrlConst.fArr)
		fout << num << " ";
	fout << endl;

	fout << TagVec2Arr << endl;
	for (auto& vec : _mtrlConst.v2Arr)
		fout << vec << endl;

	fout << TagVec4Arr << endl;
	for (auto& vec : _mtrlConst.v4Arr)
		fout << vec << endl;

	fout << TagMatrixArr << endl;
	for (const Matrix& mat : _mtrlConst.matArr)
		fout << mat << endl;

	return fout;
}

ifstream& operator>>(ifstream& fin, tMtrlConst& _mtrlConst)
{
	Utils::GetLineUntilString(fin, TagIntArr);
	for (auto& num : _mtrlConst.iArr)
		fin >> num;

	Utils::GetLineUntilString(fin, TagFloatArr);
	for (auto& num : _mtrlConst.fArr)
		fin >> num;

	Utils::GetLineUntilString(fin, TagVec2Arr);
	for (auto& vec : _mtrlConst.v2Arr)
		fin >> vec;

	Utils::GetLineUntilString(fin, TagVec4Arr);
	for (auto& vec : _mtrlConst.v4Arr)
		fin >> vec;

	Utils::GetLineUntilString(fin, TagMatrixArr);
	for (Matrix& mat : _mtrlConst.matArr)
		fin >> mat;

	return fin;
}
