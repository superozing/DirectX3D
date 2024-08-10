#include "pch.h"
#include "CVideoPlayer.h"

CVideoPlayer::CVideoPlayer()
	: CScript((UINT)SCRIPT_TYPE::VIDEOPLAYER)
{
	AppendScriptAsset("Video", m_pVideo.GetAdressOf(), ASSET_TYPE::VIDEO);
	AppendMemberFunction("Play", SCRIPT_PARAM::FUNC_MEMBER, "", std::bind(&CVideoPlayer::Play, this));
}

CVideoPlayer::~CVideoPlayer()
{
}

void CVideoPlayer::Play()
{
	m_pVideo->Play();
}

void CVideoPlayer::begin()
{
}

void CVideoPlayer::tick()
{
	if (!m_pVideo.Get())
		return;
}

void CVideoPlayer::UpdateData()
{
	if (!m_pVideo.Get())
		return;

	m_pVideo->UpdateData();
}

#define TagVideo "[Video]"
void CVideoPlayer::SaveToFile(ofstream& fout)
{
	fout << TagVideo << endl;
	SaveAssetRef(m_pVideo, fout);
}

void CVideoPlayer::LoadFromFile(ifstream& fin)
{
	// Utils::GetLineUntilString(fin, TagVideo);
	// LoadAssetRef(m_pVideo, fin);
}
