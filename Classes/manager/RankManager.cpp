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
	dbfileName = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath(); //��밡���� �н� ���
	dbfileName = dbfileName + "rankdb.sqlite"; //�����̸� ���ϱ�
	dataCount = 0;
	this->createDatabase();
}
/*
 * db ���̺� ���� �Լ�
 */
void RankManager::createDatabase() {
	sqlite3 *pDB = NULL;
	char *errMsg;
	int result;

	result = sqlite3_open(dbfileName.c_str(), &pDB); //db ����(������ ����)
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	std::string sqlStr;
	sqlStr =
			"create table IF NOT EXISTS RankTable(NAME nvarchar(32),STACK integer,SCORE integer)"; //���̺� ���� ������
	result = sqlite3_exec(pDB, sqlStr.c_str(), NULL, NULL, &errMsg); //������ ����
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	sqlite3_close(pDB);	//db �ݱ�
}
RankManager* RankManager::rankManager;

/*
 * ���̺� ������ �ִ� �Լ�
 */
void RankManager::insertData(char* name, int stack, int score) {
	int dcount = 0;
	CCUserDefault *shared = CCUserDefault::sharedUserDefault(); //��ũ�� ����ִ��� ����
	dcount = shared->getIntegerForKey("datacount");
	CCUserDefault::sharedUserDefault()->setIntegerForKey("datacount", ++dcount);
	CCUserDefault::sharedUserDefault()->flush();
	dataCount = dcount;
	sqlite3* pDB = NULL;
	char *errMsg;
	int result;
	int minscore;
	result = sqlite3_open(dbfileName.c_str(), &pDB);	 //db ����
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	std::string sqlStr;
	sqlStr = CCString::createWithFormat( //������ ������ ���� ������ ����
			"insert into RankTable(name,stack,score) values('%s',%d,%d)", name,
			stack, score)->getCString();
	result = sqlite3_exec(pDB, sqlStr.c_str(), NULL, NULL, &errMsg);//������ ����
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	sqlite3_close(pDB); //db �ݱ�
}
/*
 * ��ũ�� ����� �ִ��� ��ȯ�ϴ� �Լ�
 */
int RankManager::getDataRowNum() {
	return dataCount;
}

/*
 * ���ĵ� ������ ��ȯ�ϴ� �Լ�
 */
void RankManager::selectData(GameResultScene::RankData *rdata) {
	sqlite3* pDB = NULL;
	char *errMsg;
	int result;
	int count = 0;
	result = sqlite3_open(dbfileName.c_str(), &pDB);	 //db ����
	if (result != SQLITE_OK) {
		CCLog("Open Error : Code:%d Msg:%s", result, errMsg);
	}
	std::string sqlStr;
	sqlStr = "select * from RankTable order by score desc"; //���� �������� ���� ��
	sqlite3_stmt* statement;
	if (sqlite3_prepare_v2(pDB, sqlStr.c_str(), -1, &statement,
			NULL)==SQLITE_OK) {
		//////////////////////////////
	   // statement�� ����ؼ� �� �྿ �����͸� �����ͼ� rdata�� ����
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
	sqlite3_close(pDB);	//db �ݱ�
}
/*
 * N������ ������ ���� �������� ��� �����ϴ� �Լ�
 */
void RankManager::deleteData(int minimum) {
	sqlite3* pDB = NULL;
	char *errMsg;
	int result;
	result = sqlite3_open(dbfileName.c_str(), &pDB);	 //db ����
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
	sqlite3_close(pDB);	//db �ݱ�
}
RankManager::~RankManager() {
	// TODO Auto-generated destructor stub
}

} /* namespace com_solmi_happplus_RankManager */
