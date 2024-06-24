#pragma once
class CCameraShake
{
private:
	CGameObject* m_pTargetCamera;

	Vec3 m_vOriginPos;
	Vec3 m_vOriginRot;

	bool  m_bRelease;
	float m_fReleaseTimer;
	float m_fReleaseDuration;

	bool  m_bShake;
	float m_fShakeTimer;
	float m_fShakeDuration;
	Vec3  m_vShakePosIntensity;
	Vec3  m_vShakeRotationIntensity;

	float m_fShakeFrequencyTimer;
	float m_fShakeFrequency;
	Vec3  m_vStartPos;
	Vec3  m_vTargetPos;
	Vec3  m_vStartRotation;
	Vec3  m_vTargetRotation;

private:
	void Releasing();
	void Shaking();

public:
	void RegistCamera(CGameObject* _target) { m_pTargetCamera = _target; }
	void Shake();
	void SetShakeAttribute(CGameObject* _target, float _duration, Vec3 _posScale, Vec3 _rotScale, float _frequency,
						   float _releaseTime);

public:
	void finaltick();

public:
	CCameraShake(CGameObject* _target);
	CCameraShake(CGameObject* _target, float _duration, Vec3 _posScale, Vec3 _rotScale, float _frequency,
				 float _releaseTime);
	~CCameraShake();

	friend ofstream& operator<<(ofstream& fout, const CCameraShake& _shake);
	friend ifstream& operator>>(ifstream& fin, CCameraShake& _shake);

	friend class CameraUI;
};
