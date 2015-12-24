/*
 * TutorialScene.cpp
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */

#include "C:\cocos2d-x-2.2.3\cocos2d-x-2.2.3\projects\DropKing\Classes/TutorialScene.h"
#include "TutorialScene.h"
#include "MainMenuScene.h"
#include "manager/CoordinationManager.h"

using namespace edu_kmu_coordination_solmiengine;
USING_NS_CC;

CCScene* TutorialScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    TutorialScene *layer = TutorialScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
/*
 * 씬 초기화 함수
 */
bool TutorialScene::init()
{
    //////////////////////////////
   // 부모 클래스 초기화
    if ( !CCLayer::init() )
    {
        return false;
    }
    ////////////////////
   // 튜토리얼 스프라이트 객체 생성, 좌표지정 후 씬에 추가
    _background = CCSprite::create("tutorial/images/tutorial.png");
    _background->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TUTORIAL).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_TUTORIAL).y));
    this->addChild(_background, 0);

    ////////////////////
   // 뒤로 가기 메뉴 아이템 객체 생성.
    _menuBack = CCMenuItemImage::create("tutorial/images/backBtn1.png","tutorial/images/backBtn2.png",
    		this, menu_selector(TutorialScene::menuReturnMainMenuCallback));

    ////////////////////
   // 메뉴 객체에 뒤로 가기 메뉴 아이템을 포함 시키고 씬에 추가
    _menu = CCMenu::create(_menuBack,  NULL);
    _menu->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACK_BOUTTON).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACK_BOUTTON).y));
    this->addChild(_menu,1);

    return true;
}

/*
 * 뒤로 가기 메뉴 선택시 호출 되는 콜백 함수.
 */
void TutorialScene::menuReturnMainMenuCallback(CCObject* pSender){
	////////////////////
   // 메뉴 화면 배경음 재생 후 메뉴 화면으로 돌아감.
	 AudioManager::getAudioManager()->playEffectAudio(SOUND_CLICK);
	 AudioManager::getAudioManager()->playSound(SOUND_MENUAUDIO);
	CCDirector::sharedDirector()->popScene();
}
TutorialScene::TutorialScene() {
	// TODO Auto-generated constructor stub

}

TutorialScene::~TutorialScene() {
	// TODO Auto-generated destructor stub
}

