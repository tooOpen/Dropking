/*
 * RankManager.cpp
 *
 *  Created on: 2014. 2. 12.
 *       Author: bsh
 */

#include "RankManager.h"
USING_NS_CC;
namespace com_solmi_happplus_RankManager {

RankManager::RankManager() {
	dbfileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath(); //사용가능한 패스 얻기
	dbfileName = dbfileName + "rankdb.sqlite"; //파일이름 더하기
	dataCount = 0;
	this->createDatabase();
}
/*
 * db 테이블 생성 함수
 */
void RankManager::createDatabase() {
	sqlite3 *pDB = NULL;
	char *errMsg;
	int result;

	result = sqlite3_open(dbfileName.c_str(), &pDB); //db 열기(없으면 생성)
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	std::string sqlStr;
	sqlStr =
			"create table IF NOT EXISTS RankTable(NAME nvarchar(32),STACK integer,SCORE integer)"; //테이블 생성 퀴리문
	result = sqlite3_exec(pDB, sqlStr.c_str(), NULL, NULL, &errMsg); //쿼리문 실행
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	sqlite3_close(pDB);	//db 닫기
}
RankManager* RankManager::rankManager;

/*
 * 테이블에 데이터 넣는 함수
 */
void RankManager::insertData(char* name, int stack, int score) {
	int dcount = 0;
	CCUserDefault *shared = CCUserDefault::sharedUserDefault(); //랭크에 몇명있는지 저장
	dcount = shared->getIntegerForKey("datacount");
	CCUserDefault::sharedUserDefault()->setIntegerForKey("datacount", ++dcount);
	CCUserDefault::sharedUserDefault()->flush();
	dataCount = dcount;
	sqlite3* pDB = NULL;
	char *errMsg;
	int result;
	int minscore;
	result = sqlite3_open(dbfileName.c_str(), &pDB);	 //db 열기
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	std::string sqlStr;
	sqlStr = CCString::createWithFormat( //데이터 삽입을 위한 쿼리문 생성
			"insert into RankTable(name,stack,score) values('%s',%d,%d)", name,
			stack, score)->getCString();
	result = sqlite3_exec(pDB, sqlStr.c_str(), NULL, NULL, &errMsg);//쿼리문 실행
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	sqlite3_close(pDB); //db 닫기
}
/*
 * 랭크에 몇명이 있는지 반환하는 함수
 */
int RankManager::getDataRowNum() {
	return dataCount;
}

/*
 * 정렬된 데이터 반환하는 함수
 */
void RankManager::selectData(GameResultScene::RankData *rdata) {
	sqlite3* pDB = NULL;
	char *errMsg;
	int result;
	int count = 0;
	result = sqlite3_open(dbfileName.c_str(), &pDB);	 //db 열기
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	std::string sqlStr;
	sqlStr = "select * from RankTable order by score desc"; //오름 차순으로 정렬 후
	sqlite3_stmt* statement;
	if (sqlite3_prepare_v2(pDB, sqlStr.c_str(), -1, &statement,
			NULL)==SQLITE_OK) {
		//////////////////////////////
	   // statement를 사용해서 한 행씩 데이터를 가져와서 rdata에 저장
		while (sqlite3_step(statement) == SQLITE_ROW) {
			if (count > 4)
				break;
			char* pname = (char*) sqlite3_column_text(statement, 0);
			int pstack = sqlite3_column_int(statement, 1);
			int pscore = sqlite3_column_int(statement, 2);
			strcpy(rdata[count].name, pname);
			rdata[count].stack = pstack;
			rdata[count].score = pscore;
			count++;
		}
	}
	sqlite3_finalize(statement);
	sqlite3_close(pDB);	//db 닫기
}
/*
 * N이하의 점수를 가진 정보들은 모두 삭제하는 함수
 */
void RankManager::deleteData(int minimum) {
	sqlite3* pDB = NULL;
	char *errMsg;
	int result;
	result = sqlite3_open(dbfileName.c_str(), &pDB);	 //db 열기
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	std::string sqlStr;
	sqlStr = sqlStr = CCString::createWithFormat(
			"delete from RankTable where score<=%d", minimum)->getCString();
	result = sqlite3_exec(pDB, sqlStr.c_str(), NULL, NULL, &errMsg);
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	sqlite3_close(pDB);	//db 닫기
}
RankManager::~RankManager() {
	// TODO Auto-generated destructor stub
}

} /* namespace com_solmi_happplus_RankManager */
