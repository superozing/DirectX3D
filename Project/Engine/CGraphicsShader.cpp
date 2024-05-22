﻿#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

CGraphicsShader::CGraphicsShader()
	: CShader(ASSET_TYPE::GRAPHICS_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RSType(RS_TYPE::CULL_BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BSType(BS_TYPE::DEFAULT)
	, m_Domain(SHADER_DOMAIN::DOMAIN_MASKED)
{
}

CGraphicsShader::~CGraphicsShader()
{
}


int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// 버텍스 쉐이더
	// HLSL 버텍스 쉐이더 함수 컴파일
	wstring strContentPath = CPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
		, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize(), nullptr
		, m_VS.GetAddressOf());

	// VS 생성에 성공했다면 멤버로 저장
	m_VSPath = ToString(_strRelativePath);
	m_VSFuncName = ToString(_strFuncName);

	// 정점 구조정보(Layout) 생성
	D3D11_INPUT_ELEMENT_DESC arrElement[6] = {};

	arrElement[0].InputSlot = 0;
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[0].SemanticName = "POSITION";
	arrElement[0].SemanticIndex = 0;
	arrElement[0].InstanceDataStepRate = 0;
	arrElement[0].AlignedByteOffset = 0;
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	arrElement[3].InputSlot = 0;
	arrElement[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[3].SemanticName = "TANGENT";
	arrElement[3].SemanticIndex = 0;
	arrElement[3].InstanceDataStepRate = 0;
	arrElement[3].AlignedByteOffset = 36;
	arrElement[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[4].InputSlot = 0;
	arrElement[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[4].SemanticName = "NORMAL";
	arrElement[4].SemanticIndex = 0;
	arrElement[4].InstanceDataStepRate = 0;
	arrElement[4].AlignedByteOffset = 48;
	arrElement[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[5].InputSlot = 0;
	arrElement[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[5].SemanticName = "BINORMAL";
	arrElement[5].SemanticIndex = 0;
	arrElement[5].InstanceDataStepRate = 0;
	arrElement[5].AlignedByteOffset = 60;
	arrElement[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;


	// Layout 생성
	DEVICE->CreateInputLayout(arrElement, 6
							, m_VSBlob->GetBufferPointer()
							, m_VSBlob->GetBufferSize()
							, m_Layout.GetAddressOf());

	return S_OK;
}


int CGraphicsShader::CreateGeometryShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strContentPath = CPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	// 픽셀 쉐이더 생성	
	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "gs_5_0", D3DCOMPILE_DEBUG, 0
		, m_GSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreateGeometryShader( m_GSBlob->GetBufferPointer()
								, m_GSBlob->GetBufferSize(), nullptr
								, m_GS.GetAddressOf());

	m_GSPath = ToString(_strRelativePath);
	m_GSFuncName = ToString(_strFuncName);

	return S_OK;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strContentPath = CPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	// 픽셀 쉐이더 생성
	// 픽셀 쉐이더 컴파일
	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer()
		, m_PSBlob->GetBufferSize(), nullptr
		, m_PS.GetAddressOf());

	m_PSPath = ToString(_strRelativePath);
	m_PSFuncName = ToString(_strFuncName);

	return S_OK;
}

int CGraphicsShader::UpdateData()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType).Get());
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType).Get(), 0);
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType).Get(), nullptr, 0xffffffff);

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

	return S_OK;
}

#define TagTopology "[Topology]"

#define TagRS_TYPE "[RS_TYPE]"
#define TagDS_TYPE "[DS_TYPE]"
#define TagBS_TYPE "[BS_TYPE]"
#define TagSHADER_DOMAIN "[SHADER_DOMAIN]"

#define TagScalarParam "[ScalarParam]"
#define TagTexParam "[TexParam]"

#define TagVSPath "[VSPath]"
#define TagHSPath "[HSPath]"
#define TagDSPath "[DSPath]"
#define TagGSPath "[GSPath]"
#define TagPSPath "[PSPath]"

#define TagVSFuncName "[VSFuncName]"
#define TagHSFuncName "[HSFuncName]"
#define TagDSFuncName "[DSFuncName]"
#define TagGSFuncName "[GSFuncName]"
#define TagPSFuncName "[PSFuncName]"

int CGraphicsShader::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	ofstream fout(strFilePath);
	if (!fout.is_open()) return E_FAIL;


	fout << TagTopology << endl;
	fout << ToString(magic_enum::enum_name(m_Topology)) << endl;
	

	fout << TagRS_TYPE << endl;
	fout << ToString(magic_enum::enum_name(m_RSType)) << endl;

	fout << TagDS_TYPE << endl;
	fout << ToString(magic_enum::enum_name(m_DSType)) << endl;

	fout << TagBS_TYPE << endl;
	fout << ToString(magic_enum::enum_name(m_BSType)) << endl;

	fout << TagSHADER_DOMAIN << endl;
	fout << ToString(magic_enum::enum_name(m_Domain)) << endl;


	fout << TagScalarParam << endl;
	for (UINT i = 0; i < (UINT)SCALAR_PARAM::END; ++i)
	{
		fout << ToString(magic_enum::enum_name(m_ScalarParam[i].Type)) << endl;
		fout << m_ScalarParam[i].Desc << endl;
		fout << m_ScalarParam[i].min << endl;
		fout << m_ScalarParam[i].Max << endl;
		fout << m_ScalarParam[i].View << endl;
		fout << m_ScalarParam[i].Tooltip << endl;
		fout << m_ScalarParam[i].IsUse << endl;
	}

	fout << TagTexParam << endl;
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		fout << ToString(magic_enum::enum_name(m_TexParam[i].Type)) << endl;
		fout << m_TexParam[i].Desc << endl;
		fout << m_TexParam[i].IsUse << endl;
	}


	fout << TagVSPath << endl;
	fout << m_VSPath << endl;
	
	fout << TagHSPath << endl;
	fout << m_HSPath << endl;
	
	fout << TagDSPath << endl;
	fout << m_DSPath << endl;

	fout << TagGSPath << endl;
	fout << m_GSPath << endl;

	fout << TagPSPath << endl;
	fout << m_PSPath << endl;



	fout << TagVSFuncName << endl;
	fout << m_VSFuncName << endl;

	fout << TagHSFuncName << endl;
	fout << m_HSFuncName << endl;

	fout << TagDSFuncName << endl;
	fout << m_DSFuncName << endl;

	fout << TagGSFuncName << endl;
	fout << m_GSFuncName << endl;

	fout << TagPSFuncName << endl;
	fout << m_PSFuncName << endl;



	return 0;
}

int CGraphicsShader::Load(const wstring& _strFilePath)
{
	ifstream fin(_strFilePath);
	if (!fin.is_open()) return E_FAIL;

	string strEnumBuf;

	Utils::GetLineUntilString(fin, TagTopology);
	fin >> strEnumBuf;
	{
		auto EnumVal = magic_enum::enum_cast<D3D11_PRIMITIVE_TOPOLOGY>(strEnumBuf);
		if (EnumVal.has_value())
			SetTopology(EnumVal.value());
	}

	Utils::GetLineUntilString(fin, TagRS_TYPE);
	fin >> strEnumBuf;
	{
		auto EnumVal = magic_enum::enum_cast<RS_TYPE>(strEnumBuf);
		if (EnumVal.has_value())
			SetRSType(EnumVal.value());
	}

	Utils::GetLineUntilString(fin, TagDS_TYPE);
	fin >> strEnumBuf;
	{
		auto EnumVal = magic_enum::enum_cast<DS_TYPE>(strEnumBuf);
		if (EnumVal.has_value())
			SetDSType(EnumVal.value());
	}

	Utils::GetLineUntilString(fin, TagBS_TYPE);
	fin >> strEnumBuf;
	{
		auto EnumVal = magic_enum::enum_cast<BS_TYPE>(strEnumBuf);
		if (EnumVal.has_value())
			SetBSType(EnumVal.value());
	}

	Utils::GetLineUntilString(fin, TagSHADER_DOMAIN);
	fin >> strEnumBuf;
	{
		auto EnumVal = magic_enum::enum_cast<SHADER_DOMAIN>(strEnumBuf);
		if (EnumVal.has_value())
			SetDomain(EnumVal.value());
	}


	Utils::GetLineUntilString(fin, TagScalarParam);
	for (UINT i = 0; i < (UINT)SCALAR_PARAM::END; ++i)
	{
		fin >> strEnumBuf;
		{
			auto EnumVal = magic_enum::enum_cast<SCALAR_PARAM>(strEnumBuf);
			if (EnumVal.has_value())
				SetTopology(EnumVal.value());
		}
		fin >> m_ScalarParam[i].Desc;
		fin >> m_ScalarParam[i].min;
		fin >> m_ScalarParam[i].Max;
		fin >> m_ScalarParam[i].View;
		fin >> m_ScalarParam[i].Tooltip;
		fin >> m_ScalarParam[i].IsUse;
	}

	Utils::GetLineUntilString(fin, TagTexParam);
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		fin >> strEnumBuf;
		{
			auto EnumVal = magic_enum::enum_cast<TEX_PARAM>(strEnumBuf);
			if (EnumVal.has_value())
				SetTopology(EnumVal.value());
		}
		fin >> m_TexParam[i].Desc;
		fin >> m_TexParam[i].IsUse;
	}


	Utils::GetLineUntilString(fin, TagVSPath);
	fin >> m_VSPath;

	Utils::GetLineUntilString(fin, TagHSPath);
	fin >> m_HSPath;

	Utils::GetLineUntilString(fin, TagDSPath);
	fin >> m_DSPath;

	Utils::GetLineUntilString(fin, TagGSPath);
	fin >> m_GSPath;

	Utils::GetLineUntilString(fin, TagPSPath);
	fin >> m_PSPath;



	Utils::GetLineUntilString(fin, TagVSFuncName);
	fin >> m_VSFuncName;

	Utils::GetLineUntilString(fin, TagHSFuncName);
	fin >> m_HSFuncName;

	Utils::GetLineUntilString(fin, TagDSFuncName);
	fin >> m_DSFuncName;

	Utils::GetLineUntilString(fin, TagGSFuncName);
	fin >> m_GSFuncName;

	Utils::GetLineUntilString(fin, TagPSFuncName);
	fin >> m_PSFuncName;

	return 0;
}

