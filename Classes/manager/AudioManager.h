/*
 * AudioManager.h
 *
 *  Created on: 2014. 2. 7.
 *      Author: bsh
 */

#ifndef AudioManager_H_
#define AudioManager_H_

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

namespace edu_kmu_audio_solmiengine {

class AudioManager {
private:
	AudioManager();
	static AudioManager* s_Audiomanager; //for 싱글톤
public:
	virtual ~AudioManager();
	static AudioManager* getAudioManager()	//for 싱글톤
		{
				if(!s_Audiomanager)
				{
					s_Audiomanager = new AudioManager();
				}
				return s_Audiomanager;
		}
	void playEffectAudio(int id);	//효과음 재생 함수
	void playSound(int id);			//배경은 재생 함수
	void pauseSound();				//배경음 일시정지 함수
	void stopSound();				//배경음 정지 함수
	void resumeSound();				//배경음 일시정지 해제 함수
	bool isPlaySound();
};

} /* namespace edu_kmu_audio_solmiengine */
#endif /* AudioManager_H_ */
