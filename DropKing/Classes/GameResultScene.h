/*
 * GameResultScene.h
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */

#ifndef GAMERESULTSCENE_H_
#define GAMERESULTSCENE_H_

#include "cocos2d.h"
#include "manager/AudioManager.h"
#include "platform/android/jni/JniHelper.h"
using namespace edu_kmu_audio_solmiengine;
using namespace cocos2d;

class GameResultScene :public CCLayer {
public:
	GameResultScene();
	virtual ~GameResultScene();
	//////////////////////////////
   // 부모 클래스 사용자의 이름과 쌓은 블럭 수, 점수를 가지는 구조체
	struct RankData
	{
		RankData()
		{
			name = new char[64]();
			stack = 0;
			score = 0;
		};
		char* name; //이름
		int stack;  //쌓은 블럭 수
		int score;  //점수
	};

	struct RankData _rankData[5];	//1~5위 까지 정보
	virtual bool init();
	static CCScene* scene();
	std::string _playerName; 		//사용자 이름
	int _accumulatedBox;			//쌓은 블럭 수
	int _finalScore;				//최종 점수
	JniMethodInfo t;
	jobject jobj;
	CREATE_FUNC(GameResultScene);
	CCSprite* _background;			//배경 스프라이트
	CCSprite* _resultWindow;		//결과 창 스프라이트

	//////////////////////////////
   // 현재 사용자의 이름, 블럭 수, 최종 점수 출력 레이블
	CCLabelTTF* _finalScoreLabel;
	CCLabelTTF* _accumulatedBoxLabel;
	CCLabelTTF* _nameLabel;

	//////////////////////////////
   // 1~5등까지 이름 출력 레이블
	CCLabelTTF* _firstName;
	CCLabelTTF* _secondName;
	CCLabelTTF* _thirdName;
	CCLabelTTF* _fourthName;
	CCLabelTTF* _fifthName;

	//////////////////////////////
   // 1~5등까지 쌓은 블럭 수 출력 레이블
	CCLabelTTF* _firstBlocks;
	CCLabelTTF* _secondBlocks;
	CCLabelTTF* _thirdBlocks;
	CCLabelTTF* _fourthBlocks;
	CCLabelTTF* _fifthBlocks;

	//////////////////////////////
   // 1~5등까지 점수 출력 레이블
	CCLabelTTF* _firstScore;
	CCLabelTTF* _secondScore;
	CCLabelTTF* _thirdScore;
	CCLabelTTF* _fourthScore;
	CCLabelTTF* _fifthScore;

	//////////////////////////////
   // 게임 재시작, 메인메뉴로 돌아가기 메뉴 아이템과 메뉴, 콜백
	CCMenuItemImage* _menuReturnMainMenu;
	CCMenuItemImage* _menuReturnGamePlay;
	CCMenu* _resultMenu;
	void menuRetrunMainMenuCallBack(CCObject* pSender);
	void menuRetrunGamePlayCallBack(CCObject* pSender);

};

#endif /* GAMERESULTSCENE_H_ */
