#pragma once
#include <Engine\CScript.h>

class CVideoPlayer : public CScript
{
private:
	Ptr<CVideo> m_pVideo;

public:
	void Play();

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void UpdateData() override;

public:
	virtual void SaveToFile(ofstream& fout);
	virtual void LoadFromFile(ifstream& fin);

public:
	CLONE(CVideoPlayer);
	CVideoPlayer();
	~CVideoPlayer();
};
