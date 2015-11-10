/*
 * CoordinationManager.cpp
 *
 *  Created on: 2014. 2. 3.
 *      Author: bsh
 */

#include "CoordinationManager.h"

USING_NS_CC;
namespace edu_kmu_coordination_solmiengine {

CoordinationManager::CoordinationManager() {
	 //////////////////////////////
	//(�̹��� id enum�� = key, �̹��� id = value)���� �ʱ�ȭ
	imageNameData[IMAGE_BACKGROUND] = "IMAGE_BACKGROUND";
	imageNameData[IMAGE_MAIN_BACKGROUND] = "IMAGE_MAIN_BACKGROUND";
	imageNameData[IMAGE_BACKGROUND_TABLE] = "IMAGE_BACKGROUND_TABLE";
	imageNameData[IMAGE_VERTICALLY_START_BUTTON] ="IMAGE_VERTICALLY_START_BUTTON";
	imageNameData[IMAGE_VERTICALLY_TUTORIAL_BUTTON] ="IMAGE_VERTICALLY_TUTORIAL_BUTTON";
	imageNameData[IMAGE_TITLE] = "IMAGE_TITLE";
	imageNameData[IMAGE_TUTORIAL] = "IMAGE_TUTORIAL";
	imageNameData[IMAGE_BACK_BOUTTON] = "IMAGE_BACK_BOUTTON";
	imageNameData[IMAGE_HAND] = "IMAGE_HAND";
	imageNameData[IMAGE_BLOCK] = "IMAGE_BLOCK";
	imageNameData[IMAGE_ACCURACY] = "IMAGE_ACCURACY";
	imageNameData[IMAGE_SCORE] = "IMAGE_SCORE";
	imageNameData[IMAGE_SCORENUMBER] = "IMAGE_SCORENUMBER";
	imageNameData[IMAGE_TIMEBAR] = "IMAGE_TIMEBAR";
	imageNameData[IMAGE_GAMEOVER] = "IMAGE_GAMEOVER";
	imageNameData[IMAGE_HORIZONTAL_BUTTON] = "IMAGE_HORIZONTAL_BUTTON";
	imageNameData[IMAGE_RESULT_WINDOW] = "IMAGE_RESULT_WINDOW";
	imageNameData[LABLE_NAME] = "LABLE_NAME";
	imageNameData[LABLE_BLOCKS] = "LABLE_BLOCKS";
	imageNameData[LABLE_SCORE] = "LABLE_SCORE";
	imageNameData[LABLE_FIRST_NAME] = "LABLE_FIRST_NAME";
	imageNameData[LABLE_SECOND_NAME] = "LABLE_SECOND_NAME";
	imageNameData[LABLE_THIRD_NAME] = "LABLE_THIRD_NAME";
	imageNameData[LABLE_FOURTH_NAME] = "LABLE_FOURTH_NAME";
	imageNameData[LABLE_FIFTH_NAME] = "LABLE_FIFTH_NAME";
	imageNameData[LABLE_FIRST_BLOCKS] = "LABLE_FIRST_BLOCKS";
	imageNameData[LABLE_SECOND_BLOCKS] = "LABLE_SECOND_BLOCKS";
	imageNameData[LABLE_THIRD_BLOCKS] = "LABLE_THIRD_BLOCKS";
	imageNameData[LABLE_FOURTH_BLOCKS] = "LABLE_FOURTH_BLOCKS";
	imageNameData[LABLE_FIFTH_BLOCKS] = "LABLE_FIFTH_BLOCKS";
	imageNameData[LABLE_FIRST_SCORE] = "LABLE_FIRST_SCORE";
	imageNameData[LABLE_SECOND_SCORE] = "LABLE_SECOND_SCORE";
	imageNameData[LABLE_THIRD_SCORE] = "LABLE_THIRD_SCORE";
	imageNameData[LABLE_FOURTH_SCORE] = "LABLE_FOURTH_SCORE";
	imageNameData[LABLE_FIFTH_SCORE] = "LABLE_FIFTH_SCORE";

	 //////////////////////////////
	//������ �����͸� �о� �´�
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("coordinationdata.txt");
	unsigned char* pBuffer = NULL;
	unsigned long bufferSize = 0;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r",
			&bufferSize);
	char pzxBuf[256];
	CCString* tempstr = CCString::createWithData(pBuffer, bufferSize);
	fullStr = tempstr->getCString();
	strSplit(); //slpit�Լ��� �ɰ� (�̹��� id = key, ��ǥ ����ü = value)�ʿ� ����
}
CoordinationManager* CoordinationManager::s_Coordinationmanager;

void CoordinationManager::strSplit()
{
	coordinatePoint tempPoint;
	std::string imageName;
	int nCount = 0;
	int nGetIndex = 0;

	std::string sTemp = "";

	std::string sCopy = fullStr;
	while (true) {

		if(nCount<3) //�̸�, x, y 3���� ���� �ε����� �����Ѵ�.
		{
			nGetIndex = sCopy.find(","); //","�� �������� split

			if (nGetIndex != -1) {
				sTemp = sCopy.substr(0, nGetIndex);
				switch(nCount)
				{
				case 0:	//ù��° �̸��� ���
					imageName = sTemp;
					break;
				case 1:	//�ι�° x��ǥ�� ���
					tempPoint.x = atof(sTemp.c_str());
					break;
				case 2:	//����° y��ǥ�ΰ�� �����ϰ� ���������� �ʿ� insert��
					nGetIndex = sCopy.find("\n");
					sTemp = sCopy.substr(0, nGetIndex);
					tempPoint.y =  atof(sTemp.c_str());
					coordinationData.insert(std::pair<std::string,coordinatePoint>(imageName,tempPoint));
					break;
				}
				sCopy = sCopy.substr(nGetIndex + 1); //","���� ���� �ٽ� ����
				++nCount;
			} else {	//�������� ���
				tempPoint.y =  atof(sCopy.c_str());
				coordinationData.insert(std::pair<std::string,coordinatePoint>(imageName,tempPoint));
				break;
			}
		}else
		{
			nCount=0;
		}
	}
}
CoordinationManager::~CoordinationManager() {

}

coordinatePoint CoordinationManager::getPos(int id){

	return coordinationData[(imageNameData[id]).c_str()];
}

} /* namespace edu_kmu_coordination_solmiengine */

