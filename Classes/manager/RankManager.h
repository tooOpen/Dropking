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
	std::string dbfileName;			//db 파일 이름
	static RankManager* rankManager;//for 싱글톤
	int dataCount;					//랭크에 몇명이 있는지 저장하는 변수
	RankManager();
public:
	static RankManager* getRankManager()//for 싱글톤
	{
			if(!rankManager)
			{
				rankManager = new RankManager();
			}
			return rankManager;
	}
	virtual ~RankManager();

	 void createDatabase();
	    void insertData(char* name,int stack ,int score); //테이블에 데이터 넣는 함수
	    void selectData(GameResultScene::RankData* rdata);//정렬된 데이터 반환하는 함수
	    void deleteData(int minimum);					  //N이하의 점수를 가진 정보들은 모두 삭제하는 함수
	    int getDataRowNum();							  //랭크에 몇명이 있는지 반환하는 함수

};

} /* namespace com_solmi_happplus_RankManager */
#endif /* RANKMANAGER_H_ */
