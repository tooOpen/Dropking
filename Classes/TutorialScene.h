/*
 * TutorialScene.h
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */

#ifndef TUTORIALSCENE_H_
#define TUTORIALSCENE_H_

#include "cocos2d.h"
#include "manager/AudioManager.h"
using namespace edu_kmu_audio_solmiengine;
using namespace cocos2d;

class TutorialScene :public CCLayer {
public:
	TutorialScene();
	virtual ~TutorialScene();

	virtual bool init();
	static CCScene* scene();

	CREATE_FUNC(TutorialScene);
	CCSprite* _background; //Ʃ�丮�� ��������Ʈ
	CCMenuItemImage* _menuBack; //�ڷΰ��� ��ư �޴�������
	CCMenu* _menu;				// �ڷΰ��� �޴�
	void menuReturnMainMenuCallback(CCObject* pSender); //�ڷΰ��� �޴� ���� �� ȣ��Ǵ� �ݹ� �Լ�

};

#endif /* TUTORIALSCENE_H_ */
