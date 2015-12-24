/*
 * GamePlayScene.h
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */

#ifndef GAMEPLAYSCENE_H_
#define GAMEPLAYSCENE_H_


#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "ContactListener.h"
#include "manager/AudioManager.h"
#include "manager/CoordinationManager.h"
#include "platform/android/jni/JniHelper.h"
#include "DataPossessor.h"


using namespace edu_kmu_coordination_solmiengine;
using namespace edu_kmu_audio_solmiengine;
USING_NS_CC;

#define PTM_RATIO 32 //Box2D의 기본단위는 1m 이것을 32픽셀로 환산하기 위한 상수

////////////////////
// 게임 중 상태 변수
enum stageType{
	STAGE_READY, STAGE_PLAY, STAGE_END, STAGE_DROP,STAGE_MAXIMUN,
};

class GamePlayScene :public CCLayer {
public:
	//GamePlayScene();
	virtual ~GamePlayScene();

	virtual bool init();
	static CCScene* scene();

	CREATE_FUNC(GamePlayScene);

	ContactListener* _listener;

	CCSprite* _gameResult; //gameover글자 스프라이트
	CCSprite* _background; //배경 스프라이트
	CCSprite* _background2;//스크롤을 위한 배경 스프라이트
	CCSprite* _handMotion; //크레인 스프라이트
	CCSprite* _block;	   //블럭 스프라이트
	CCSprite* _timeGauge;  //남은시간을 보여주는 게이지 스프라이트
	CCSprite* _timeFrame;  //게이지 프레임 스프라이트
	CCSprite* _scoreSprite;//Score 스프라이트
	CCSprite* _accuracySprite;//정확도에 따라 bad good perfect 스프라이트
	CCSprite* _table;		 //곰돌이 배경

	b2World* _world;			//Box2D 월드
	CCTexture2D* _blockTexture;	//body에 입힐 블럭 텍스쳐
	CCSprite* _pscoreNumber[5]; //5자리 점수 스프라이트

	float _TotalBlockHeight;	//총 블럭 높이(카메라 이동,스크롤에 사용)
	int _blockIndex;			//각 블럭의 인덱스
	float _firstBlockY;			//바로 아래의 블럭
	float _lastBlockX;			//방금 떨어트린 블럭 x 중심 좌표
	float _lastBlockY;			//방금 떨어트린 블럭 y 중심 좌표
	int _stage;					//게임 상태 저장 변수
	float _handSpeed;			//크레인의 속도 조절 변수
	int _scoreNumber;			//점수
	int _raiseHeight;			//화면 스크롤을 위한 높이 변수
	int _raiseWindow;			//카메라 이동을 위한 높이 변수
	int _raiseCheck;			//배경 2개를 번갈아가며 바꾸기 위한 상태변수

	bool _isDrop;				//블럭이 떨어지는 중인지 아닌지 상태변수
	float _resultCheckTime;		//반동을 고려하여 움직임이 없어진 후에 결과를 판별하기 위한 시간
	float _gameOverCounter;		//게임오버 스프라이트가 화면에 출력되는 시간
	int _time;					//남은 게임시간


	void setBox2dWorld();				//Box2D월드 세팅 함수
	void moveHand(float f);				//크레인이 왔다갓다 하는 스케쥴함수
	void tick(float f);					//게임 전반의 tick함수
	void addBlock(CCPoint location);	//월드에 블럭을 추가하는 함수
	void timeCounter(float dt);			//게임시간이 흐르는 함수
	virtual void keyBackClicked();		//안드로이드 백 버튼 재 정의
	void countScore(float locationX);	//블럭의 x축을 이용한 정확도 판별 함수
	void stageEnd();					//게임오버시 처리 함수
	void moveCamera(float dt);			//카메라 이동함수
	void countGameOverTime(float dt);	//게임오버 카운터 스케쥴
	void shakeMachine();				//기기를 흔드는지 판별하는 함수
};

#endif /* GAMEPLAYSCENE_H_ */
