#ifndef COORDINATIONMANAGER_H_
#define COORDINATIONMANAGER_H_

#define STANDRAD_WIDTH 720
#define STANDRAD_HEIGHT 1280

#include "C:\cocos2d-x-2.2.3\cocos2d-x-2.2.3\projects\DropKing\Classes/ResourcesID.h"
#include <map>
#include <string>
#include<stdlib.h>
#include "cocos2d.h"
using namespace __gnu_cxx;

/*
 * ��ǥ ����ü
 */
struct coordinatePoint{
	float x;
	float y;
	coordinatePoint(){}
	coordinatePoint(int p_x,int p_y):x(p_x),y(p_y){}
};

namespace edu_kmu_coordination_solmiengine {

class CoordinationManager {
public:
	static CoordinationManager* getCoordinationManager()	//for �̱���
	{
			if(!s_Coordinationmanager)
			{
				s_Coordinationmanager = new CoordinationManager();
			}
			return s_Coordinationmanager;
	}
	virtual ~CoordinationManager();
	coordinatePoint getPos(int id);
private:
	static CoordinationManager* s_Coordinationmanager;	//for �̱���
	typedef std::map<std::string, coordinatePoint> coordiText;	//�̹��� id = key, ��ǥ ����ü = value
	typedef std::map<int, std::string> imageName;				//�̹��� id enum�� = key, �̹��� id = value
	coordiText coordinationData;
	imageName imageNameData;
	std::string fullStr;	//���� Ǯ ������
	void strSplit();		//���� �����͸� split�ϴ� �Լ�
	CoordinationManager();
};

} /* namespace edu_kmu_coordination_solmiengine */
#endif /* COORDINATIONMANAGER_H_ */
