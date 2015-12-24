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
	static AudioManager* s_Audiomanager; //for �̱���
public:
	virtual ~AudioManager();
	static AudioManager* getAudioManager()	//for �̱���
		{
				if(!s_Audiomanager)
				{
					s_Audiomanager = new AudioManager();
				}
				return s_Audiomanager;
		}
	void playEffectAudio(int id);	//ȿ���� ��� �Լ�
	void playSound(int id);			//����� ��� �Լ�
	void pauseSound();				//����� �Ͻ����� �Լ�
	void stopSound();				//����� ���� �Լ�
	void resumeSound();				//����� �Ͻ����� ���� �Լ�
	bool isPlaySound();
};

} /* namespace edu_kmu_audio_solmiengine */
#endif /* AudioManager_H_ */
