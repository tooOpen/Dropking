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
	static int s_count;		// �޴� ���� ���ö� �ѹ��� ������ ����ǰ� �ϱ����� ����
	CREATE_FUNC(MainMenuScene);
	JniMethodInfo t;		//JNI �޼ҵ� ���� ����
	CCSprite* _background;  //��� ��������Ʈ
	CCSprite* _table;		//��� ��������Ʈ 2
	CCSprite* _title;		//Ÿ��Ʋ ��������Ʈ

	CCMenuItemImage* _menuTutorial; 		//Ʃ�丮�� �޴�������
	CCMenuItemImage* _menuGamePlay; 		//���ӽ��� �޴�������
	CCMenuItemImage* _menuGamePlayCouvered; //�� Ȱ��ȭ �� ���ӽ��� �޴�������

	CCMenu* _tutorialMenu;	//Ʃ�丮�� �޴�
	CCMenu* _startMenu;		//���ӽ��� �޴�
	CCMenu* _startMenuCouvered;	//�� Ȱ��ȭ �� ���ӽ��� �޴�

	virtual void keyBackClicked();					//�ȵ���̵� �� ��ư �� ����
	void menuTutorialCallBack(CCObject* pSender);	//Ʃ�丮�� �޴� ���� �� ȣ��Ǵ� �ݹ� �Լ�
	void menuGamePlayCallBack(CCObject* pSender);	//���ӽ��� �޴� ���� �� ȣ��Ǵ� �ݹ� �Լ�
	void checkConnect(float f);						//������ ������ �Ϸ� �Ǿ����� Ȯ���ϴ� ������ �Լ�
};

#endif /* MAINMENUSCENE_H_ */
