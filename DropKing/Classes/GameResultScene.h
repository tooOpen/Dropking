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
   // �θ� Ŭ���� ������� �̸��� ���� �� ��, ������ ������ ����ü
	struct RankData
	{
		RankData()
		{
			name = new char[64]();
			stack = 0;
			score = 0;
		};
		char* name; //�̸�
		int stack;  //���� �� ��
		int score;  //����
	};

	struct RankData _rankData[5];	//1~5�� ���� ����
	virtual bool init();
	static CCScene* scene();
	std::string _playerName; 		//����� �̸�
	int _accumulatedBox;			//���� �� ��
	int _finalScore;				//���� ����
	JniMethodInfo t;
	jobject jobj;
	CREATE_FUNC(GameResultScene);
	CCSprite* _background;			//��� ��������Ʈ
	CCSprite* _resultWindow;		//��� â ��������Ʈ

	//////////////////////////////
   // ���� ������� �̸�, �� ��, ���� ���� ��� ���̺�
	CCLabelTTF* _finalScoreLabel;
	CCLabelTTF* _accumulatedBoxLabel;
	CCLabelTTF* _nameLabel;

	//////////////////////////////
   // 1~5����� �̸� ��� ���̺�
	CCLabelTTF* _firstName;
	CCLabelTTF* _secondName;
	CCLabelTTF* _thirdName;
	CCLabelTTF* _fourthName;
	CCLabelTTF* _fifthName;

	//////////////////////////////
   // 1~5����� ���� �� �� ��� ���̺�
	CCLabelTTF* _firstBlocks;
	CCLabelTTF* _secondBlocks;
	CCLabelTTF* _thirdBlocks;
	CCLabelTTF* _fourthBlocks;
	CCLabelTTF* _fifthBlocks;

	//////////////////////////////
   // 1~5����� ���� ��� ���̺�
	CCLabelTTF* _firstScore;
	CCLabelTTF* _secondScore;
	CCLabelTTF* _thirdScore;
	CCLabelTTF* _fourthScore;
	CCLabelTTF* _fifthScore;

	//////////////////////////////
   // ���� �����, ���θ޴��� ���ư��� �޴� �����۰� �޴�, �ݹ�
	CCMenuItemImage* _menuReturnMainMenu;
	CCMenuItemImage* _menuReturnGamePlay;
	CCMenu* _resultMenu;
	void menuRetrunMainMenuCallBack(CCObject* pSender);
	void menuRetrunGamePlayCallBack(CCObject* pSender);

};

#endif /* GAMERESULTSCENE_H_ */
