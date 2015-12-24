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
	CCSprite* _background; //튜토리얼 스프라이트
	CCMenuItemImage* _menuBack; //뒤로가기 버튼 메뉴아이템
	CCMenu* _menu;				// 뒤로가기 메뉴
	void menuReturnMainMenuCallback(CCObject* pSender); //뒤로가기 메뉴 선택 시 호출되는 콜백 함수

};

#endif /* TUTORIALSCENE_H_ */
