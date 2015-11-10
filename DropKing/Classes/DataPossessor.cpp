/*
 * DataPossessor.cpp
 *
 *  Created on: 2014. 2. 10.
 *       Author: bsh
 */

#include "DataPossessor.h"

DataPossessor::DataPossessor() {


}

void DataPossessor::setAccData(int x,int y,int z)
{
	this->accx = x;
	this->accy = y;
	this->accz = z;
}
	void DataPossessor::setECG(int e)
	{
		this->ECG = e;
	}
	void DataPossessor::setHR(int h)
	{
		this->HR = h;
	}
	void DataPossessor::setStress(int stress)
	{
		this->stress = stress;
	}
	int DataPossessor::getAccx()
	{
		return this->accx;
	}
	int DataPossessor::getAccy()
	{
		return this->accy;
	}
	int DataPossessor::getAccz()
	{
		return this->accz;
	}
	int DataPossessor::getECG()
	{
		return this->ECG;
	}
	int DataPossessor::getHR()
	{
		return this->HR;
	}
	int DataPossessor::getStress()
	{
		return this->stress;
	}

DataPossessor::~DataPossessor() {
	 message = 0;
			 accx= 0;
			 accy= 0;
			 accz= 0;
			 stress= 0;
			 ECG= 0;
			 HR= 0;
}

