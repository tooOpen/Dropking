/*
 * RankManager.h
 *
 *  Created on: 2014. 2. 12.
 *      Author: bsh
 */

#ifndef RANKMANAGER_H_
#define RANKMANAGER_H_
#include "sqlite3/include/sqlite3.h"
#include "../GameResultScene.h"
#include "cocos2d.h"
#include <string>

namespace com_solmi_happplus_RankManager {

class RankManager {
private:
	std::string dbfileName;			//db ���� �̸�
	static RankManager* rankManager;//for �̱���
	int dataCount;					//��ũ�� ����� �ִ��� �����ϴ� ����
	RankManager();
public:
	static RankManager* getRankManager()//for �̱���
	{
			if(!rankManager)
			{
				rankManager = new RankManager();
			}
			return rankManager;
	}
	virtual ~RankManager();

	 void createDatabase();
	    void insertData(char* name,int stack ,int score); //���̺� ������ �ִ� �Լ�
	    void selectData(GameResultScene::RankData* rdata);//���ĵ� ������ ��ȯ�ϴ� �Լ�
	    void deleteData(int minimum);					  //N������ ������ ���� �������� ��� �����ϴ� �Լ�
	    int getDataRowNum();							  //��ũ�� ����� �ִ��� ��ȯ�ϴ� �Լ�

};

} /* namespace com_solmi_happplus_RankManager */
#endif /* RANKMANAGER_H_ */
