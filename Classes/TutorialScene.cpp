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
 * �� �ʱ�ȭ �Լ�
 */
bool TutorialScene::init()
{
    //////////////////////////////
   // �θ� Ŭ���� �ʱ�ȭ
    if ( !CCLayer::init() )
    {
        return false;
    }
    ////////////////////
   // Ʃ�丮�� ��������Ʈ ��ü ����, ��ǥ���� �� ���� �߰�
    _background = CCSprite::create("tutorial/images/tutorial.png");
    _background->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TUTORIAL).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_TUTORIAL).y));
    this->addChild(_background, 0);

    ////////////////////
   // �ڷ� ���� �޴� ������ ��ü ����.
    _menuBack = CCMenuItemImage::create("tutorial/images/backBtn1.png","tutorial/images/backBtn2.png",
    		this, menu_selector(TutorialScene::menuReturnMainMenuCallback));

    ////////////////////
   // �޴� ��ü�� �ڷ� ���� �޴� �������� ���� ��Ű�� ���� �߰�
    _menu = CCMenu::create(_menuBack,  NULL);
    _menu->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACK_BOUTTON).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_BACK_BOUTTON).y));
    this->addChild(_menu,1);

    return true;
}

/*
 * �ڷ� ���� �޴� ���ý� ȣ�� �Ǵ� �ݹ� �Լ�.
 */
void TutorialScene::menuReturnMainMenuCallback(CCObject* pSender){
	////////////////////
   // �޴� ȭ�� ����� ��� �� �޴� ȭ������ ���ư�.
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

