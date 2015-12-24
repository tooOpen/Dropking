/*
 * MainMenuScene.h
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */

#ifndef MAINMENUSCENE_H_
#define MAINMENUSCENE_H_
#include "cocos2d.h"
#include "Manager/AudioManager.h"
#include "platform/android/jni/JniHelper.h"
using namespace edu_kmu_audio_solmiengine;
using namespace cocos2d;

class MainMenuScene :public CCLayer {
public:
	MainMenuScene();
	virtual ~MainMenuScene();

	virtual bool init();
	static CCScene* scene();
	static int s_count;		// 메뉴 씬에 들어올때 한번만 기기와의 연결되게 하기위한 변수
	CREATE_FUNC(MainMenuScene);
	JniMethodInfo t;		//JNI 메소드 정보 변수
	CCSprite* _background;  //배경 스프라이트
	CCSprite* _table;		//배경 스프라이트 2
	CCSprite* _title;		//타이틀 스프라이트

	CCMenuItemImage* _menuTutorial; 		//튜토리얼 메뉴아이템
	CCMenuItemImage* _menuGamePlay; 		//게임시작 메뉴아이템
	CCMenuItemImage* _menuGamePlayCouvered; //비 활성화 된 게임시작 메뉴아이템

	CCMenu* _tutorialMenu;	//튜토리얼 메뉴
	CCMenu* _startMenu;		//게임시작 메뉴
	CCMenu* _startMenuCouvered;	//비 활성화 된 게임시작 메뉴

	virtual void keyBackClicked();					//안드로이드 백 버튼 재 정의
	void menuTutorialCallBack(CCObject* pSender);	//튜토리얼 메뉴 선택 시 호출되는 콜백 함수
	void menuGamePlayCallBack(CCObject* pSender);	//게임시작 메뉴 선택 시 호출되는 콜백 함수
	void checkConnect(float f);						//기기와의 연결이 완료 되었는지 확인하는 스케쥴 함수
};

#endif /* MAINMENUSCENE_H_ */
