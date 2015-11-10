/*
 * MainMenuScene.cpp
 *
 *  Created on: 2014. 1. 27.
 *       Author: bsh
 */


#include "MainMenuScene.h"
#include "TutorialScene.h"
#include "GamePlayScene.h"
#include "GameResultScene.h"
#include "manager/CoordinationManager.h"

using namespace edu_kmu_coordination_solmiengine;
USING_NS_CC;

int MainMenuScene::s_count = 0;

CCScene* MainMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MainMenuScene *layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

/*
 * �� �ʱ�ȭ �Լ�
 */
bool MainMenuScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    this->setKeypadEnabled(true);

    //////////////////////////////
   // �ѹ��� ����� ���� �Լ��� JNI�� �̿��Ͽ� native call
    if(s_count<1)
        {
        if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/dropking/DropKing","connectDevice","()V"))
       				    {
       					  t.env->CallStaticVoidMethod(t.classID, t.methodID);
       					  t.env->DeleteLocalRef(t.classID);
       				    }
        s_count++; //ī��Ʈ ����
        }

    ////////////////////
   // Ÿ��Ʋ ��������Ʈ ��ü ����, ��ǥ���� �� ���� �߰�
    _title = CCSprite::create("mainmenu/images/title.png");
    _title->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_TITLE).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_TITLE).y));
    this->addChild(_title,3);

    ////////////////////
   // ��� ��������Ʈ ��ü ����, ��ǥ���� �� ���� �߰�
    _background = CCSprite::create("mainmenu/images/background.jpg");
    _background->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_MAIN_BACKGROUND).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_MAIN_BACKGROUND).y) );
    this->addChild(_background,0);

    ////////////////////
   // ���ӽ���, ��Ȱ�� ���ӽ���, Ʃ�丮�� �޴� ������ ��ü ����
    _menuGamePlay = CCMenuItemImage::create("mainmenu/images/startBtn2.png","mainmenu/images/startBtn3.png",
    		this,menu_selector(MainMenuScene::menuGamePlayCallBack) );
    _menuTutorial = CCMenuItemImage::create("mainmenu/images/tutorialBtn2.png",
    		"mainmenu/images/tutorialBtn3.png",this, menu_selector(MainMenuScene::menuTutorialCallBack));
    _menuGamePlayCouvered = CCMenuItemImage::create("mainmenu/images/startBtn1.png","mainmenu/images/startBtn1.png",this, NULL);

    ////////////////////
   // �� �޴� �������� ���� ��Ų �޴� ��ü ���� �� ���� �߰�
    _startMenu = CCMenu::create(_menuGamePlay,NULL);
    _startMenu->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_START_BUTTON).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_START_BUTTON).y));
    _tutorialMenu = CCMenu::create(_menuTutorial,NULL);
    _tutorialMenu->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_TUTORIAL_BUTTON).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_TUTORIAL_BUTTON).y));
    _startMenuCouvered = CCMenu::create(_menuGamePlayCouvered,NULL);
    _startMenuCouvered->setPosition(ccp(CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_START_BUTTON).x,
    		CoordinationManager::getCoordinationManager()->getPos(IMAGE_VERTICALLY_START_BUTTON).y));
    this->addChild(_startMenu,1);
    this->addChild(_tutorialMenu,1);
    this->addChild(_startMenuCouvered, 2);

    AudioManager::getAudioManager()->playSound(SOUND_MENUAUDIO);
    ////////////////////
   //���� �ʱ�ȭ �ǰ� 4�� �ں��� 1�ʾ� 100�� ������ �Ϸ� �Ǿ����� Ȯ���ϴ� ������
    this->schedule(schedule_selector(MainMenuScene::checkConnect),1.0f,100,4.0f);

    return true;
}
/*
 * ������ ������ Ȯ���ϴ� �Լ�
 */
void MainMenuScene::checkConnect(float f)
{
	jboolean connectedjni = false;
	bool connect = false;
	////////////////////
   // getConnected java �Լ��� native call�Ͽ� boolean������ ������ �Ǿ������� Ȯ����.
	  if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/dropking/DropKing","getConnected","()Z"))
	      				    {
		  	  	  	  	  	  connectedjni = (jboolean)t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	      					  connect = (bool)connectedjni;
	      					  t.env->DeleteLocalRef(t.classID);
	      				    }
	  if(connect){								//if(������ �Ǿ��ٸ�)
		this->removeChild(_startMenuCouvered);	//�� Ȱ�� �޴��� ������ ����
	  }
}
/*
 * �ȵ���̵� ���ư �Լ� ������
 */
void MainMenuScene::keyBackClicked(){
	CCDirector::sharedDirector()->end(); //���� ����
}

/*
 * Ʃ�丮�� �޴� ���ý� ȣ�� �Ǵ� �ݹ� �Լ�.
 */
void MainMenuScene::menuTutorialCallBack(CCObject* pSender){
	 AudioManager::getAudioManager()->playEffectAudio(SOUND_CLICK);
	 AudioManager::getAudioManager()->stopSound();
	CCScene* scene = TutorialScene::scene();
	CCDirector::sharedDirector()->pushScene(scene);

}

/*
 * ���� ���� �޴� ���ý� ȣ�� �Ǵ� �ݹ� �Լ�.
 */
void MainMenuScene::menuGamePlayCallBack(CCObject* pSender){
	 AudioManager::getAudioManager()->playEffectAudio(SOUND_CLICK);
	 AudioManager::getAudioManager()->stopSound();
	CCScene* scene = GamePlayScene::scene();
	CCDirector::sharedDirector()->pushScene(scene);

}

MainMenuScene::MainMenuScene() {
	// TODO Auto-generated constructor stub

}
/*
 * �Ҹ��� ȣ�� �� java�� disConnect�Լ��� ȣ���Ͽ� ���� ���� ����
 */
MainMenuScene::~MainMenuScene() {
	// TODO Auto-generated destructor stub
	if(JniHelper::getStaticMethodInfo(t, "com/solmi/refitgames/dropking/DropKing","disConnect","()V"))
								    {
									  t.env->CallStaticVoidMethod(t.classID, t.methodID);
									  t.env->DeleteLocalRef(t.classID);
								    }
	 AudioManager::getAudioManager()->stopSound();
}

