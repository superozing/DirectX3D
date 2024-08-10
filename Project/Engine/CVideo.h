#pragma once
#include "CAsset.h"

class CVideo : public CAsset
{
public:
	static IMFAttributes* g_pMFAttributes; // Media Foundation 속성 객체

private:
	IMFSourceReader*		  m_pReader;			 // Media Foundation 소스 리더
	ID3D11Texture2D*		  m_pTexture;			 // DirectX 비디오 텍스처
	ID3D11ShaderResourceView* m_pShaderResourceView; // 셰이더에서 사용할 텍스처 뷰

	DWORD	 m_streamIndex;		// 선택된 비디오 스트림 인덱스
	LONGLONG m_duration;		// 비디오 총 길이 (100나노초 단위)
	UINT	 m_width, m_height; // 비디오 프레임 크기
	UINT	 m_frameRate;		// 비디오 프레임 레이트

	LONGLONG m_currentTime; // 현재 재생 시간
	bool	 m_isPlaying;	// 재생 중인지 여부
	bool	 m_isLooping;	// 반복 재생 여부

	IMFSample*		m_pCurrentSample; // 현재 디코딩된 프레임 샘플
	IMFMediaBuffer* m_pMediaBuffer;	  // 현재 프레임의 미디어 버퍼
	BYTE*			m_pBufferData;	  // 프레임 데이터 포인터
	DWORD			m_bufferLength;	  // 버퍼 길이

	IMFAudioStreamVolume* m_pAudioVolume; // 오디오 볼륨 제어용

	std::mutex m_mutex; // 멀티스레딩 동기화를 위한 뮤텍스

	std::queue<IMFSample*> m_sampleQueue;	 // 디코딩된 프레임 큐
	std::thread			   m_decodingThread; // 백그라운드 디코딩 스레드

public:
	// 비디오 파일 로드
	int Load(const wstring& _strFilePath) override;

	// 재생 제어
	int	 Play();
	void Stop();
	void Pause();
	void Resume();

	// 비디오 텍스처 업데이트 (매 프레임 호출)
	void UpdateData();

	// 볼륨 및 루핑 제어
	void SetVolume(float volume);
	void SetLooping(bool isLooping);

	// 재생 정보 조회
	float	 GetProgress();
	LONGLONG GetDuration();

public:
	CLONE_DISABLE(CVideo);
	// 생성자 및 소멸자
	CVideo();
	~CVideo();

private:
	// 디코딩 스레드 함수
	void DecodeThreadFunction();

	// 다음 프레임 읽기
	HRESULT GetNextFrame(IMFSample** ppSample);

	// 내부 헬퍼 함수
	// void RemoveFromSampleQueue(IMFSample* pSample); // 디코딩 스레드에서 사용하는 샘플 큐에서 특정 샘플을
	// 제거합니다.
	// void ReleaseAllSamples(); // 모든 샘플을 릴리스하여 메모리 누수를 방지합니다.

	friend class CVideoPlayer;
};