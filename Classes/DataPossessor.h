
#ifndef DATAPOSSESSOR_H_
#define DATAPOSSESSOR_H_

class DataPossessor {
	private:
	int message;
	int accx;
	int accy;
	int accz;
	int stress;
	int ECG;
	int HR;

	public:
	DataPossessor();
	void setAccData(int x,int y,int z);
	void setECG(int e);
	void setHR(int h);
	void setStress(int stress);

	int getAccx();
	int getAccy();
	int getAccz();
	int getECG();
	int getHR();
	int getStress();

	virtual ~DataPossessor();
};

#endif /* DATAPOSSESSOR_H_ */
