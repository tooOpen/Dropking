/*
 * GamePlayScene.cpp
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */
#include "GamePlayScene.h"
#include "MainMenuScene.h"
#include "GameResultScene.h"

#define LIMITE_TIME 120				//총 게임플레이 가능 시간
#define BLOCK_HEIGHT 90				//블럭 한칸의 높이

const int velocityIterations = 8;	//바디들을 정상적으로 이동시키는 데 필요한 충돌을 반복적으로 계산
const int positionIterations = 3;	//조인트 분리와 겹침 현상을 줄이기 위해 바디의 위치를 반복적으로 적용

int accumulateBox;					//총 쌓은 블럭 수(extern)
int finalScore;						//최종 점수(extern)

class DataPossessor;
class GameResultScene;
DataPossessor dataPossessor;		//기기로부터 얻은 값을 저장한 객체

USING_NS_CC;

CCScene* GamePlayScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    GamePlayScene *layer = GamePlayScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GamePlayScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    _raiseWindow = 0;
    _gameOverCounter = 0;
    _isDrop = false;

    AudioManager::getAudioManager()->playSound(SOUND_PALYAUDIO);
    _time = LIMITE_TIME;
    _scoreNumber = 0;
    _handSpeed = 1.5f;
    _resultCheckTime = 0.0f;
    _blockIndex = -1;
    _stage = STAGE_PLAY;
    _TotalBlockHeight = 0;
    _raiseHeight = 0;
    _raiseCheck = 0;

    _listener = new ContactListener();
    this->setKeypadEnabled(true);

    //////////////////////////////
   // 배경 스프라이트 객체 생성
    _background = CCSprite::create("common/images/background.png");
    _background->setPosition(ccp(  CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).y));
    this->addChild(_background,0);
    _background2 = CCSprite::create("common/images/background.png");
    _background2->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND).y+ _background->getContentSize().height));
    this->addChild(_background2,0);

    _table = CCSprite::create("common/images/table.png");
    _table->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND_TABLE).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACKGROUND_TABLE).y));
    this->addChild(_table);

    //////////////////////////////
   // 블럭 & 크레인 스프라이트 객체 생성
    _blockTexture = CCTextureCache::sharedTextureCache()->addImage("gameplay/images/block01.png");

    _block = CCSprite::createWithTexture(_blockTexture);
    _block->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BLOCK).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BLOCK).y));

    _handMotion = CCSprite::create("gameplay/images/handHoldBlock.png");
    _handMotion->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_HAND).x,
        		CoordinationManager::getCoordinationManager()->getPos(IMAGE_HAND).y));

    this->addChild(_handMotion,2);
    this->addChild(_block,3);

    //////////////////////////////
   // 타임 게이지 프레임 & 게이지 스프라이트 객체 생성
	_timeFrame = CCSprite::create("gameplay/images/timeFrame.png");
	_timeFrame->setPosition(
			ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMEBAR).x,
					CoordinationManager::getCoordinationManager()->getPos(IMAGE_TIMEBAR).y));
	this->addChild(_timeFrame, 5);
	_timeGauge = CCSprite::create("gameplay/images/timeGaugeG.png");
	_timeGauge->setPosition(_timeFrame->getPosition());
	this->addChild(_timeGauge, 4);

    //////////////////////////////
   // 크레인 움직임 스케쥴
    this->schedule(schedule_selector(GamePlayScene::moveHand));

    //////////////////////////////
   // 점수 스프라이트 객체생성과 점수 숫자 렉트 초기화
    _scoreSprite = CCSprite::create("gameplay/images/score.png");
    _scoreSprite->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_SCORE).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_SCORE).y));
    this->addChild(_scoreSprite, 4);

    CCTexture2D* scoreNumberTexture = CCTextureCache::sharedTextureCache()->addImage("gameplay/images/scoreNum.png");
    for(int i = 0 ; i<5 ; i++){
    	_pscoreNumber[i] = CCSprite::createWithTexture(scoreNumberTexture,CCRectMake(42*9,0,42,47));
    	_pscoreNumber[i]->setPosition(ccp(_scoreSprite->getContentSize().width + 25 + 42*i,
    			_scoreSprite->getContentSize().height/2));
    	_scoreSprite->addChild(_pscoreNumber[i]);
    }


    //////////////////////////////
   // 정확도 (bad, good, perfect)스프라이트 객체생성
    _accuracySprite = CCSprite::create("gameplay/images/accuracyGood.png");
    _accuracySprite->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_ACCURACY).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_ACCURACY).y));
    this->addChild(_accuracySprite,-1);

    //////////////////////////////
   // gameover스프라이트 객체 생성
    _gameResult = CCSprite::create("gameplay/images/gameOver.png");
    _gameResult->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_GAMEOVER).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_GAMEOVER).y));
    this->addChild(_gameResult, -1);
    //---------------

    setBox2dWorld(); //Box2D 월드 초기화

    //////////////////////////////
   // 게임 tick 스케쥴
    this->schedule(schedule_selector(GamePlayScene::tick));
    this->schedule(schedule_selector(GamePlayScene::timeCounter),1.0f);

    return true;
}
/*
 *  게임 tick 함수
 */
void GamePlayScene::tick(float f){

	_world->Step(f, velocityIterations, positionIterations); //월드 시뮬레이션 시작

	  //////////////////////////////
	 // 만들어진 객체만큼 루프를 돌리면서 스프라이트 제어
	for(b2Body *b = _world->GetBodyList() ; b ; b = b->GetNext()){
		if(b->GetUserData() != NULL){
			CCSprite *spriteData = (CCSprite*)b->GetUserData();
			spriteData->setPosition(ccp(b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO));
			spriteData->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
	 //////////////////////////////
	// 게임상태가 플레이 중일 시
	if(_stage == STAGE_PLAY){
		_resultCheckTime += f;
		  //////////////////////////////
		 // 블럭 한칸 만큼 카메라가 이동 하였을 시 이동 정지
		if(BLOCK_HEIGHT == _raiseWindow){
			this->unschedule(schedule_selector(GamePlayScene::moveCamera));
		}
		//////////////////////////////
	   // 블럭이 떨어지는 중이 아니라면 기기를 흔들었는지 계속 판별
		if(!_isDrop){
			shakeMachine();
		}
		//////////////////////////////
	   // 게임 오버 확인
		if(_lastBlockY <= _firstBlockY + BLOCK_HEIGHT/2 &&_resultCheckTime >2.0 && _blockIndex > 0){ //전블럭 위 끝보다 방금블럭 가운데가 밑에있을때
			_stage = STAGE_END;
		}
		if(_time == 0){//시간다됬을때
			_stage = STAGE_END;
		}
		if(_blockIndex >0){//1개이상 쌓엿는데 총 높이 보다 낮을때
			b2Body *b = _world->GetBodyList();

			if(b->GetUserData() != NULL){
				CCSprite *spriteData = (CCSprite *)b->GetUserData();

				if(spriteData->getPositionY() + BLOCK_HEIGHT < _TotalBlockHeight){
					_stage = STAGE_END;
				}
			}
		}
		//collision check
		if(_listener->_contactStage == CONTACT_BEGIN){
			b2Body *b = _world->GetBodyList();
			if(b->GetUserData() != NULL)
			{
				CCSprite *spriteData = (CCSprite *)b->GetUserData();
				if(_blockIndex == 0)
				{
					_firstBlockY = spriteData->getPositionY();
				}else{

				}
				_lastBlockY = spriteData->getPositionY();
				b=b->GetNext();

				 AudioManager::getAudioManager()->playEffectAudio(SOUND_BLOCKDROP);
				_accuracySprite->setZOrder(-1);
				_listener->_contactStage = CONTACT_NULL;
			}
		}
		//////////////////////////////
	   // 크레인에 블럭 재생성
		if(_resultCheckTime > 2.5 && _isDrop){
			_handMotion->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/handHoldBlock.png"));
			int blockColor = (_blockIndex+1)%4;
				CCSprite* blockSprite;
				switch(blockColor){
				case 0:
					_block->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/block01.png"));
					break;
				case 1:
					_block->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/block02.png"));
					break;
				case 2:
					_block->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/block03.png"));
					break;
				case 3:
					_block->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/block04.png"));
					break;
			}
			_block->setZOrder(3);
			_isDrop = false;
		}
	}else if(_stage == STAGE_END){
		if(BLOCK_HEIGHT == _raiseWindow){
			this->unschedule(schedule_selector(GamePlayScene::moveCamera));
		}
		_accuracySprite->setZOrder(-1);
		stageEnd();
		if(_gameOverCounter == 0){
			this->schedule(schedule_selector(GamePlayScene::countGameOverTime));
		}
	}
}

void GamePlayScene::countGameOverTime(float dt){
	_gameOverCounter+= dt;
}
 //////////////////////////////
// 게임 오버 처리
void GamePlayScene::stageEnd(){

	if(_gameOverCounter == 0){ //3초 동안 gameover스프라이트 보여주기
		 AudioManager::getAudioManager()->stopSound();
		 AudioManager::getAudioManager()->playEffectAudio(SOUND_GAMEOVER);
		_gameResult->setZOrder(6);
	}else if(_gameOverCounter > 3){ //3초 후 결과씬으로 전환
		GameResultScene* resultScene = new GameResultScene();
		accumulateBox = (_blockIndex+1);
		finalScore = _scoreNumber;

		CCScene* scene = resultScene->scene();
		CCDirector::sharedDirector()->popScene();
		CCDirector::sharedDirector()->pushScene(scene);
	}
}
 //////////////////////////////
// 블럭의 x좌표를 통한 정확도 판별 함수
void GamePlayScene::countScore(float locationX){
	if(_blockIndex == 0){
		_scoreNumber += 100;
	}
	else if(_blockIndex > 0){
		int checkAccuracy = (int)_lastBlockX - (int)locationX;

		if(abs(checkAccuracy)>=0 && abs(checkAccuracy) < 5){
			_accuracySprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/accuracyPerfect.png"));
			_accuracySprite->setZOrder(4);
			_scoreNumber += 200;
		}else if(abs(checkAccuracy)>=5 && abs(checkAccuracy) < 15){
			_accuracySprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/accuracyGood.png"));
			_accuracySprite->setZOrder(4);
			_scoreNumber += 100;
		}else if(abs(checkAccuracy)>=15 && abs(checkAccuracy)<80){
			_accuracySprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/accuracyBad.png"));
			_accuracySprite->setZOrder(4);
			_scoreNumber += 80;
		}else if(abs(checkAccuracy)>=80){

		}
	}
	//////////////////////////////
   // 점수를 _pscoreNumber로 나타내기 위해 각 자리수의 값을 구하여 렉트로 이미지를 자른다
	int cipher = 10000;
	int score = _scoreNumber;
	for(int i = 0; i <4 ; i++){
		int scoreNum = score / cipher;
		if(scoreNum == 0)//그자리가 없으면
			scoreNum = 10; //0부분을 그린다
		_pscoreNumber[i]->setTextureRect(CCRectMake(42*(scoreNum-1),0,42,47)); //숫자부분 렉트로 잘라오기

		score = score%cipher; //계산한 이전 자리수 부터
		cipher = cipher/10;   //이전 자리로
	}
}
 //////////////////////////////
// 기기가 흔들어 졌을 시 그 위치에 Box2D body를 생성한다
void GamePlayScene::addBlock(CCPoint location){
	_TotalBlockHeight += (BLOCK_HEIGHT);
	_isDrop = true;
	_resultCheckTime = 0.0f;
	_raiseWindow = 0;
	_blockIndex++;
	_block->setZOrder(-1); //스프라이트는 배경 뒤로 보내 안보이게 함
	int blockColor = _blockIndex%4;
	CCSprite* blockSprite;
	switch(blockColor){
	case 0:
		blockSprite = CCSprite::create("gameplay/images/block01.png");
		break;
	case 1:
		blockSprite = CCSprite::create("gameplay/images/block02.png");
		break;
	case 2:
		blockSprite = CCSprite::create("gameplay/images/block03.png");
		break;
	case 3:
		blockSprite = CCSprite::create("gameplay/images/block04.png");
		break;
	}

	blockSprite->setPosition(ccp(location.x, location.y));

	this->addChild(blockSprite,3);


	//////////////////////////////
	//Box2D body 설정 후 월드에 추가
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(location.x/PTM_RATIO, location.y/PTM_RATIO);
	bodyDef.userData = blockSprite;

	b2Body* body = _world->CreateBody(&bodyDef);

	b2PolygonShape box;
	box.SetAsBox((blockSprite->getContentSize().width/2)/PTM_RATIO,(BLOCK_HEIGHT/2)/32.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = 10.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;
	body->CreateFixture(&fixtureDef);

	countScore(location.x); //body의 x축으로 정확도 판별

	_lastBlockX = location.x;
	if(_blockIndex > 6){		//블럭이 6개 이상이면 카메라 위로 이동
		this->schedule(schedule_selector(GamePlayScene::moveCamera));
	}
	if((_blockIndex+1)%4 == 0){ //4개씩 마다 크레인 속도 up
		if(_handSpeed > 0){
			_handSpeed += 0.5f;
		}else if(_handSpeed < 0){
			_handSpeed -= 0.5f;
		}
	}

}
 //////////////////////////////
// glulookat을 이용한 카메라 이동
void GamePlayScene::moveCamera(float dt){

	float x = 0, y = 0, z = 0;
	//////////////////////////////
   // 카메라의 위치를 1씩 위로 이동
	this->getCamera()->getCenterXYZ(&x, &y, &z);
	this->getCamera()->setCenterXYZ(x, y+1, z);

	//////////////////////////////
   // 카메라가 보는곳을 1씩 위로 이동
	this->getCamera()->getEyeXYZ(&x, &y, &z);
	this->getCamera()->setEyeXYZ(x, y+1, z);

	//////////////////////////////
   // 다른 스프라이트들 카메라에 맞추어 이동
	_handMotion->setPosition(ccp(_handMotion->getPosition().x, _handMotion->getPosition().y+1));
	_scoreSprite->setPosition(ccp(_scoreSprite->getPosition().x, _scoreSprite->getPosition().y+1));
	_timeFrame->setPosition(ccp(_timeFrame->getPosition().x, _timeFrame->getPosition().y+1));
	_timeGauge->setPosition(ccp(_timeGauge->getPosition().x, _timeGauge->getPosition().y+1));
	_block->setPosition(ccp(_block->getPosition().x, _block->getPosition().y+1));
	_accuracySprite->setPosition(ccp(_accuracySprite->getPosition().x, _accuracySprite->getPosition().y+1));
	_gameResult->setPosition(ccp(_gameResult->getPosition().x, _gameResult->getPosition().y+1));

	_raiseWindow++;
	_raiseHeight++;
	//////////////////////////////
   // 배경 화면 보다 카메라가 위로 갈 시 다른 배경을 위에 이어줌
	if((_raiseHeight)%1280 == 0 && _raiseHeight != 0){
		if(_raiseCheck == 0){
			_background->setPositionY(_background->getPositionY() + _background->getContentSize().height*2);
			_raiseCheck = 1;
		}else if(_raiseCheck == 1){
			_background2->setPositionY(_background2->getPositionY() + _background->getContentSize().height*2);
			_raiseCheck = 0;
		}
	}
}
 //////////////////////////////
// Box2D월드 세팅 함수
void GamePlayScene::setBox2dWorld(){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	b2Vec2 gravity = b2Vec2(0.0f, -20.0f);
	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);
	b2BodyDef groundBodydef;
	groundBodydef.position.Set(0,0);

	b2Body* groundBody = _world->CreateBody(&groundBodydef);

	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	groundEdge.Set(b2Vec2(0,0), b2Vec2(size.width/PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(360/PTM_RATIO, 70/PTM_RATIO);

	b2Body* body = _world->CreateBody(&bodyDef);

	b2PolygonShape box;
	box.SetAsBox(360/PTM_RATIO,70/PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = 20.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture(&fixtureDef);

	_world->SetContactListener((b2ContactListener*)_listener);

	_TotalBlockHeight = 100 - BLOCK_HEIGHT;

}
 //////////////////////////////
// 게임시간이 흐르는 함수
void GamePlayScene::timeCounter(float dt){
	_time--;
	float subPosition = _timeGauge->getContentSize().width/LIMITE_TIME;
	if(_time == LIMITE_TIME/3*2){
		_timeGauge->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/timeGaugeY.png"));
	}else if(_time == LIMITE_TIME/3){
		_timeGauge->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/timeGaugeR.png"));
	}else if(time<0){

	}
	_timeGauge->setPosition(ccp(_timeGauge->getPosition().x - subPosition , _timeGauge->getPosition().y));

}

void GamePlayScene::moveHand(float f){
	if(_stage == STAGE_PLAY){
		_handMotion->setPosition(ccp(_handMotion->getPosition().x + _handSpeed,_handMotion->getPosition().y ));
		_block->setPosition(ccp(_block->getPosition().x + _handSpeed,_block->getPosition().y ));
		if(_handMotion->getPosition().x < _handMotion->getTexture()->getPixelsWide()/2 ||
				_handMotion->getPosition().x > 720 -_handMotion->getTexture()->getPixelsWide()/2 ){
			_handSpeed *= -1;
		}
	}

}

 //////////////////////////////
// 기기가 흔들었는지 판별하는 함수
void GamePlayScene::shakeMachine(){
	if(dataPossessor.getAccx() > 1000){ //x축의 가속도가 1000이상일때는 기기를 흔들었다고 판별
		CCPoint location;
		location.x = _block->getPosition().x;
		location.y = _block->getPosition().y;
		_handMotion->setTexture(CCTextureCache::sharedTextureCache()->addImage("gameplay/images/handPutBlock.png"));
		addBlock(location);
	}
}

void GamePlayScene::keyBackClicked(){

	CCDirector::sharedDirector()->end();
}

GamePlayScene::~GamePlayScene() {
	// TODO Auto-generated destructor stub
	 AudioManager::getAudioManager()->stopSound();
	this->cleanup();
	this->removeAllChildren();
}

 //////////////////////////////
// native call을 통해 기기로부터 값이 들어있는 객체를 받아 dataPossessor 옮김
#ifdef __cplusplus
extern "C" {
#endif
void Java_com_solmi_refitgames_dropking_DropKing_getValue(JNIEnv* env, jobject thisObj,
		jobject valueObj) {
	int message = 0;
	jfieldID fid;
	jmethodID mid;
	jclass cls = env->GetObjectClass(valueObj);
	mid = env->GetMethodID(cls, "getDataType", "()I");
	jint jmessage = env->CallIntMethod(valueObj, mid);
	message = (int) jmessage;
	jint accx, accy ,accz ,ecg, stess ,hr;
	switch(message)
	{
	case 1:	//가속도값을 받는 경우
		mid = env->GetMethodID(cls, "getACCDataX", "()I");
		accx = env->CallIntMethod(valueObj, mid);
		mid = env->GetMethodID(cls, "getACCDataY", "()I");
		accy = env->CallIntMethod(valueObj, mid);
		mid = env->GetMethodID(cls, "getACCDataZ", "()I");
		accz = env->CallIntMethod(valueObj, mid);
		dataPossessor.setAccData((int)accx,(int)accy,(int)accz);
		break;
	case 2:	//심전도값을 받는 경우
		mid = env->GetMethodID(cls, "getECGData", "()I");
		ecg = env->CallIntMethod(valueObj, mid);
		dataPossessor.setECG((int)ecg);

		break;
	case 4:	//스트레스값을 받는 경우
		mid = env->GetMethodID(cls, "getStressData", "()I");
		stess = env->CallIntMethod(valueObj, mid);
		dataPossessor.setStress((int) stess);

		break;
	case 8:	//심박값을 받는 경우
		mid = env->GetMethodID(cls, "getHRData", "()I");
		hr = env->CallIntMethod(valueObj, mid);
		dataPossessor.setHR((int) hr);
		break;
	}
}

#ifdef __cplusplus
}
#endif

