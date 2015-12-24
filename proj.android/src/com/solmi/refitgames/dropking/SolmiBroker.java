package com.solmi.refitgames.dropking;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;
import java.util.Locale;
import java.util.Queue;
import java.util.Timer;
import java.util.TimerTask;

import android.bluetooth.BluetoothAdapter;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Debug;
import android.os.Environment;
import android.os.IBinder;
import android.os.Looper;
import android.os.RemoteException;
import android.util.Log;

import com.solmi.bluetoothservice.BluetoothService;
import com.solmi.protocol.rev3.SHC_M1;
import com.solmi.protocol.rev3.SHC_M1.SHC_Data;

import edu.kmu.sensor.SensorBroker;

public class SolmiBroker implements SensorBroker {

	private int _dataAcquisitionMode; 				//원하는 데이터 타입
	private int _gravityRange; 						//중력 정도
	private int _runTime; 							//연결 시간
	private Context cp; 							//블루투스 연결을 위한 
	private SolmiEvent sevent;						//값 전달 객체
	private String _g_DevName = ""; 				//디바이스 번호
	private SensorBroker.sensorcallback cocallback; //콜백 객체
	private boolean isplay = false; 				//게임중인지
	private boolean isSetted = false; 				//세팅이 완료되었는지
	private boolean isBinded = false; 				//bind가 완료되었는지
	private Timer btime; 							//지속적 연결 체크를 위한 타이머
	private Queue<String> gSaveQue; 				//로그저장을 위한 큐
	protected BluetoothService g_BluetoothService; 

	protected ServiceConnection g_BluetoothServiceConn = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			g_BluetoothService = ((BluetoothService.BluetoothServiceBinder) service)
					.getService();
			g_BluetoothService.registerCallback(mBTCallback);
			isBinded = true;
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			g_BluetoothService = null;
		}
		
	};

	private BluetoothAdapter mBtAdapter; 

	/***
	 * 생성자
	 */
	public SolmiBroker(Context c) {
		gSaveQue = new LinkedList<String>();
		sevent = new SolmiEvent();
		this._g_DevName = "00:00:00:00:00:00";
		cp = c;
		bindService(); //바인드하기
	}
	
	public void setDeviceName(String deviceName) //디바이스 번호 받기
	{
		this._g_DevName = deviceName;
	}
	/*
	 * 연결종료함수
	 */
	public void disconnect() {
		try {

			g_BluetoothService.stopDeviceMode();
		} catch (RemoteException e) {
			e.printStackTrace();
		}

		if (g_BluetoothServiceConn != null) {
			cp.unbindService(g_BluetoothServiceConn);
			g_BluetoothServiceConn = null;
		}
		synchronized (gSaveQue) {
			gSaveQue.clear();
		}
	}

	/*
	 * 블루투스 켜기
	 */
	public void turnOnbluetooth() { 
		mBtAdapter = BluetoothAdapter.getDefaultAdapter();
		if (mBtAdapter.isEnabled()) {
		} else {
			mBtAdapter.enable();
		}

	}
	/*
	 * 기기와 연결을 하는 함수 중복정의
	 */
	public boolean connect(int dataAcquisition) {               
		if (dataAcquisition == SensorBroker.DATA_ACC
				|| dataAcquisition == SensorBroker.DATA_ECG
				|| dataAcquisition == SensorBroker.DATA_STRESS
				|| dataAcquisition == SensorBroker.DATA_HR) {
		} else
			return false;
		this._dataAcquisitionMode = dataAcquisition; //데이터 타입 받기
		this._runTime = SensorBroker.RUNTIME_CONTINUE; //연결 시간 설정
		this._gravityRange = SensorBroker.ACC_DEGREE_2G; //중력 정도
		connectsolmi(); //본격 연결 시작
		return true;
	}

	public boolean connect(int dataAcquisition, int runTime) {
		if (dataAcquisition == SensorBroker.DATA_ACC
				|| dataAcquisition == SensorBroker.DATA_ECG
				|| dataAcquisition == SensorBroker.DATA_STRESS
				|| dataAcquisition == SensorBroker.DATA_HR) {
		} else
			return false;
		this._dataAcquisitionMode = dataAcquisition;
		this._runTime = runTime;
		this._gravityRange = SensorBroker.ACC_DEGREE_2G;
		connectsolmi();
		return true;
	}

	public boolean connect(int dataAcquisition, int runTime, int gDgree) {
		if (dataAcquisition == SensorBroker.DATA_ACC
				|| dataAcquisition == SensorBroker.DATA_ECG
				|| dataAcquisition == SensorBroker.DATA_STRESS
				|| dataAcquisition == SensorBroker.DATA_HR) {
		} else
			return false;
		this._dataAcquisitionMode = dataAcquisition;
		this._runTime = runTime;
		this._gravityRange = gDgree;
		connectsolmi();
		return true;
	}

	/*
	 * 기기와 블루투스 연결
	 */
	class ConnectTask extends TimerTask {
		public void run() {
			Looper.prepare();
			try {
				g_BluetoothService.BTconnect(_g_DevName);
			} catch (RemoteException e) {
				e.printStackTrace();
			}
		}
	}

	/*
	 * 기기와 연결시간 설정
	 */
	class RuntimeTask extends TimerTask {
		public void run() {
			try {
				g_BluetoothService.setRunningTime(_runTime);
			} catch (RemoteException e) {
			
				e.printStackTrace();
			}
		}
	}
	
	/*
	 * 기기의 중력 정도 설정
	 */
	class GravityDegreeTask extends TimerTask {
		public void run() {
			int command = 0;

			switch (_gravityRange) {
			case 2:
				command = BluetoothService.ACC_RANGE_2G;
				break;
			case 4:
				command = BluetoothService.ACC_RANGE_4G;
				break;
			case 8:
				command = BluetoothService.ACC_RANGE_8G;
				break;
			case 16:
				command = BluetoothService.ACC_RANGE_16G;
				break;
			default:
				command = BluetoothService.ACC_RANGE_2G;
				break;
			}

			try {
				g_BluetoothService.setAccelerometer(command);
			} catch (RemoteException e) {
				
				e.printStackTrace();
			}

		}
	}
	
	class DataInfoTask extends TimerTask {
		public void run() {

			try {
				g_BluetoothService.getDeviceInfo();
			} catch (RemoteException e) {
				
				e.printStackTrace();
			}

		}
	}

	/*
	 * 어떤 종류의 값을 얻을 것인지 선택 ex) 가속도,스트레스,심박,심전도
	 */
	class AcquisitionTask extends TimerTask {
		public void run() {

			try {
				switch (_dataAcquisitionMode) {

				case SensorBroker.DATA_HR:
					g_BluetoothService.startHrStressMode();
					sevent.message = SensorBroker.DATA_HR;
					break;
				case SensorBroker.DATA_STRESS:
					g_BluetoothService.startHrECGMode();
					sevent.message = SensorBroker.DATA_STRESS;
					break;
				case SensorBroker.DATA_ACC:
					g_BluetoothService.startACCMode();
					sevent.message = SensorBroker.DATA_ACC;
					break;
				case SensorBroker.DATA_ECG:
					g_BluetoothService.startECGACCMode();
					sevent.message = SensorBroker.DATA_ECG;
					break;
				default:
					break;
				}
			} catch (RemoteException e) {
			
				e.printStackTrace();
			}

		}
	}
	
	/*
	 * 기기와 연결이 완료되었나 체크
	 */
	class Check extends TimerTask {
		public void run() {
			if (g_BluetoothService.getState() == BluetoothService.STATE_CONNECTED) {
				isplay = true;	
				
			}
			checkSetted();
			Looper.loop();
		}
	}
	
	class NameCheck extends TimerTask {
		public void run() {
			Looper.prepare();
			if (g_BluetoothService.getState() == BluetoothService.STATE_CONNECTED) {
				isplay = true;	
				
			}
			checkSetted();
			Looper.loop();
		}
	}
	

	private void connectsolmi() {

		Timer connectTimer = new Timer(true); //동시에 일어나면 안되게 때문에 텀을 줌

		TimerTask runtask = new RuntimeTask();
		TimerTask acquitask = new AcquisitionTask();
		TimerTask gravitytask = new GravityDegreeTask();
		TimerTask infotask = new DataInfoTask();
		TimerTask connecttask = new ConnectTask();
		TimerTask connectcheck = new Check();
		TimerTask devicenamecheck = new NameCheck();
		
		if(isBinded) //바인드가 완료되었을 때
		{
			if(_g_DevName!="00:00:00:00:00:00") //기기 번호가 들어왔을때
			{
			connectTimer.schedule(connecttask, 0);
			connectTimer.schedule(runtask, 3000);
			connectTimer.schedule(gravitytask, 3500);
			connectTimer.schedule(infotask, 4000);
			connectTimer.schedule(acquitask, 4500);
			connectTimer.schedule(connectcheck, 5000);
			}else
			{
				connectTimer.schedule(devicenamecheck, 0); 
			}
		}else
		{
			TimerTask second = new TimerTask() { 
				@Override
				public void run() {
					connectsolmi();
				}
			};
			if (btime != null) {
				btime.cancel();
				btime = null;
			}
			btime = new Timer();
			btime.schedule(second, 2000); //2초마다 바인드가 되었는지 계속 확인
		}
	}

	private int gECG, gAccX, gAccY, gAccZ, HR;
	private float stress;
	
	public void checkSetted() //연결 확인
	{
			TimerTask second = new TimerTask() {
				@Override
				public void run() {
					if(!isSetted) //데이터가 들어왔다면 정상적으로 연결이 되었다고 인정.
					{
						connectsolmi(); //재연결
						btime.cancel();
						btime = null;
						
					}else
					{
						isSetted = false;
						cocallback.successConnected();
						checkSetted(); //연결이 완료되었다면 체크만 계속
					}
				}
			};
			if (btime != null) {
				btime.cancel();
				btime = null;
			}
			btime = new Timer();
			btime.schedule(second, 2000);//2후 확인
	}
	
	protected BluetoothService.BTCallback mBTCallback = new BluetoothService.BTCallback() {//라이브러리에 콜백 재정의 후 인스턴스화

		@Override
		public void dataUpdated(byte[] values) {
		

		}

		@Override
		public void rcvedData(SHC_Data shcData) { //값 받기
			
			isSetted = true; //값이 들어왔으므로 데이터가 정상처리됨.
			switch (shcData.packetType) {
			
			case SHC_M1.PKT_HEADERDATA_ECGACC: {
				SimpleDateFormat sdf_ms = new SimpleDateFormat(
						"[HH:mm:ss.SSS]  ", Locale.KOREA);
				String ts_ms = sdf_ms.format(new Date());
				if (_dataAcquisitionMode == SensorBroker.DATA_STRESS) {
					stress = shcData.daqHeader.Stress;
					sevent.fparam4 = stress;
				} else if (_dataAcquisitionMode == SensorBroker.DATA_HR) {
					HR = shcData.daqHeader.Hr;
					sevent.iparam6 = HR;
				}
				
				if (isplay) {
					String msg = "";
					if (_dataAcquisitionMode == SensorBroker.DATA_HR) {
						msg = ts_ms + String.format(",%03d\n", HR);
					} else if (_dataAcquisitionMode == SensorBroker.DATA_STRESS) {
						msg = ts_ms + String.format(",%03d\n", stress);
					}
					gSaveQue.add(msg);
				}
				if (cocallback != null)
					cocallback.rcvedData(sevent);// call callbackfunction for send data
			}
				break;

			case SHC_M1.PKT_RAWDATA_ECGACC: {
				SimpleDateFormat sdf_ms = new SimpleDateFormat(
						"[HH:mm:ss.SSS]  ", Locale.KOREA);
				String ts_ms = sdf_ms.format(new Date());
				gECG = (shcData.rawData.Ecg_0 - 212);
				gAccX = shcData.rawData.Acc_X;
				gAccY = shcData.rawData.Acc_Y;
				gAccZ = shcData.rawData.Acc_Z;

				if (_dataAcquisitionMode == SensorBroker.DATA_ACC) {
					sevent.iparam1 = gAccX;
					sevent.iparam2 = gAccY;
					sevent.iparam3 = gAccZ;
				} else if (_dataAcquisitionMode == SensorBroker.DATA_ECG) {
					sevent.iparam6 = HR;
				}
				
				if (isplay) {
					String msg = "";
					if (_dataAcquisitionMode == SensorBroker.DATA_ACC) {
						msg = ts_ms	+ String.format(",%03d,%03d,%03d\n", gAccX,	gAccY, gAccZ);
					} else if (_dataAcquisitionMode == SensorBroker.DATA_ECG) {
						msg = ts_ms + String.format(",%03d\n", gECG);
					}
					gSaveQue.add(msg);
				}
				if (cocallback != null)
					cocallback.rcvedData(sevent);// call callbackfunction for
			}
				break;
			}

		}

		@Override
		public void BTConnected() {
			

		}

		@Override
		public void ConnectionFail() {
			

		}
	};

	public void setRuntime(int runTime) {
		this._runTime = runTime;
		try {
			g_BluetoothService.setRunningTime(_runTime);
		} catch (RemoteException e) {
			
			e.printStackTrace();
		}
	}
	
	/*
	 * 기기로 부터 받을 데이터 정하기
	 */
	public void setDataAcquisition(int dataAcqui) {
		this._dataAcquisitionMode = dataAcqui;
		try {
			switch (_dataAcquisitionMode) {

			case SensorBroker.DATA_HR:
				g_BluetoothService.startHrStressMode();
				sevent.message = SensorBroker.DATA_HR;
				break;
			case SensorBroker.DATA_STRESS:
				g_BluetoothService.startHrECGMode();
				sevent.message = SensorBroker.DATA_STRESS;
				break;
			case SensorBroker.DATA_ACC:
				g_BluetoothService.startACCMode();
				sevent.message = SensorBroker.DATA_ACC;
				break;
			case SensorBroker.DATA_ECG:
				g_BluetoothService.startECGACCMode();
				sevent.message = SensorBroker.DATA_ECG;
				break;
			default:
				break;
			}
		} catch (RemoteException e) {
		
			e.printStackTrace();
		}
	}

	/*
	 * 중력 정도 설정
	 */
	public void setGravityDegree(int gravityDegree) {
		this._gravityRange = gravityDegree;
		int command = 0;

		switch (_gravityRange) {
		case 2:
			command = BluetoothService.ACC_RANGE_2G;
			break;
		case 4:
			command = BluetoothService.ACC_RANGE_4G;
			break;
		case 8:
			command = BluetoothService.ACC_RANGE_8G;
			break;
		case 16:
			command = BluetoothService.ACC_RANGE_16G;
			break;
		default:
			command = BluetoothService.ACC_RANGE_2G;
			break;
		}

		try {
			g_BluetoothService.setAccelerometer(command);
		} catch (RemoteException e) {
			
			e.printStackTrace();
		}
	}

	/*
	 * 현재 기기를 잡고있는 방향 구하기
	 */
	public int getMainAxis() {
		int temp;
		int temp2;
		int[] axisNameArray = { 1, 2, 3 };
		int[] axisArray = { gAccX, gAccY, gAccZ };
		int i;
		int j;
		int gap1, gap2;
		for (i = 1; i < 3; i++) {
			temp = axisArray[i];
			temp2 = axisNameArray[i];
			j = i;
			while ((j > 0) && (axisArray[j - 1] < temp)) {
				axisArray[j] = axisArray[j - 1];
				axisNameArray[j] = axisNameArray[j - 1];
				j--;
			}
			axisArray[j] = temp;
			axisNameArray[j] = temp2;
		}
		gap1 = axisArray[0] - 500;
		gap2 = axisArray[2] - 500;
		if (Math.abs(gap1) > Math.abs(gap2)) {
			if (gap1 > 0)					//값이 양수일때는 위로 음수일때는 아래로 잡고 있는것
				return axisNameArray[0];
			else
				return -axisNameArray[0];
		} else {
			if (gap2 > 0)
				return axisNameArray[2];
			else
				return -axisNameArray[2];
		}
	}

	
	/**
	 * 가속도 x,y,z = int 1,2,3 스트레스 flaot 4 ECG int 5 HR int 6
	 */
	public class SolmiEvent extends m_SensorEvent { 

		public int getDataType() {
			return this.message;
		}

		public int getECGData() {
			return this.iparam5;
		}

		public int getACCDataX() {
			return this.iparam1;
		}

		public int getACCDataY() {
			return this.iparam2;
		}

		public int getACCDataZ() {
			return this.iparam3;
		}

		public int getStressData() {
			return this.iparam4;
		}

		public int getHRData() {
			return this.iparam6;
		}
	}

	/*
	 * 콜백 객체 등록
	 */
	public void setCallbackfunc(SensorBroker.sensorcallback cococallback) {
		this.cocallback = cococallback;
	}
	
	protected void bindService() {
		if (g_BluetoothServiceConn != null) {
			Intent intent = new Intent(cp, BluetoothService.class);
			cp.bindService(intent, g_BluetoothServiceConn, cp.BIND_AUTO_CREATE
					+ cp.BIND_DEBUG_UNBIND);
		}
	}
	// no--------------------------------------
	// interface-------------------------------------
	
	

	
	public void insertFlag(String flagstr)
	{
		gSaveQue.add(flagstr);
	}

	public void saveLogfile() { 
		
		if (gSaveQue.isEmpty() == true)
		{
			
			return;
		}
		isplay = false;
		insertFlag("end is nigh");
		String gFileName = "";
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd_HHmm",
				Locale.KOREA);
		String ts = sdf.format(new Date());
		gFileName = ts + ".csv";
		
		String saveMsg = "";
		
		try {
			File ext = Environment.getExternalStorageDirectory();
			final File dir = new File(ext.getAbsolutePath()+"/Happ+Simulator");
			if(!dir.exists()){
				if (android.os.Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED)){
					dir.mkdirs();
				}
			}			
			while (gSaveQue.isEmpty() == false) {
				saveMsg = gSaveQue.remove();
				BufferedWriter bwfile = new BufferedWriter(new FileWriter(
						dir.getAbsolutePath() + "/" + gFileName, true));
				bwfile.write(saveMsg);
				bwfile.close();
			}
			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/***************************** *****************************************/
	

}
