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
 * 좌표 구조체
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
	static CoordinationManager* getCoordinationManager()	//for 싱글톤
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
	static CoordinationManager* s_Coordinationmanager;	//for 싱글톤
	typedef std::map<std::string, coordinatePoint> coordiText;	//이미지 id = key, 좌표 구조체 = value
	typedef std::map<int, std::string> imageName;				//이미지 id enum값 = key, 이미지 id = value
	coordiText coordinationData;
	imageName imageNameData;
	std::string fullStr;	//파일 풀 데이터
	void strSplit();		//파일 데이터를 split하는 함수
	CoordinationManager();
};

} /* namespace edu_kmu_coordination_solmiengine */
#endif /* COORDINATIONMANAGER_H_ */
