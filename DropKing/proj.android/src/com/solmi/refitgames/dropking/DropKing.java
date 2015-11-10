/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.solmi.refitgames.dropking;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.WindowManager;
import edu.kmu.sensor.SensorBroker;
import edu.kmu.sensor.SensorBroker.m_SensorEvent;



public class DropKing extends Cocos2dxActivity{
	public static final int DEVICE_CONNECT = 1;
	public static final int DEVICE_DISCONNECT = 2;

	private static Object activity; 			//�÷��̾� ������ static���� ���� �ʱ� ����
	public static boolean connected = false; 	//���� ����
	static Handler b_handler; 
	private String PlayerName = "JHON SILVER";	//����� �̸�
	private static SolmiBroker solmi;			//��� ��ü
	
	private String deviceName = "00:00:00:00:00:00"; //��� �ʱ� �� 
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		
		this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		activity = this.getContext(); 
		solmi = new SolmiBroker(getApplicationContext()); 	//������� ������ ���� ���ؽ�Ʈ ����
		solmi.setCallbackfunc(cocoCallback); 				//��Ⱚ�� �޾� �Ѱ��� �ݹ鰴ü ����
		b_handler = new Handler() {
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case DEVICE_CONNECT:
					connectBT();
					solmi.connect(1);
					break;
				case DEVICE_DISCONNECT:
					solmi.disconnect();
					break;
				}
			}
		};
	}
    /**
     * ������� ���� ��Ƽ��Ƽ ���� �Լ�
     */
    protected void connectBT() {
		Intent intent = new Intent(getApplicationContext(),FindDeviceActivity.class);
		startActivityForResult(intent,FindDeviceActivity.SET_DEVICE_ADDRESS);//��Ƽ��Ƽ�� �����鼭 ������� ����
	}
    
    /**
     * ���罺 ���� ��Ƽ��Ƽ�κ��� ���� ���� �����ϴ� �Լ�
     */
    @Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		if(requestCode ==  FindDeviceActivity.SET_DEVICE_ADDRESS){
			if (resultCode == FindDeviceActivity.RESULT_OK) {
				//RESULT_OK
				deviceName = data.getExtras().getString(FindDeviceActivity.EXTRA_DEVICE_ADDRESS);//��� ��������
				solmi.setDeviceName(deviceName);
				
			}	
		}
	}
    /**
     * ��Ƽ��Ƽ ��ü �Ѱ��ִ� �Լ�
     */
    public static Object cppCall_logsth(){ 
    	  return activity;
    	 }
    
    private native void getValue(SolmiBroker.SolmiEvent sevent); //Navtive �Լ� ����
  
    @Override
	protected void onDestroy() {
    	
    	solmi.disconnect();
		super.onDestroy();
		}

	public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// DropKing should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	return glSurfaceView;
    }
    
	SensorBroker.sensorcallback cocoCallback = new SensorBroker.sensorcallback() //sensorbroker�� sensorcallback�������̽��� ������ �� �ν��Ͻ�ȭ
	{
		@Override
		public void rcvedData(m_SensorEvent paramsolmi) { //������ ���� �� ȣ���
		SolmiBroker.SolmiEvent sevent = (SolmiBroker.SolmiEvent)paramsolmi; //m_SensorEvent�� ������ �ְ� �̰��� ��ӹ��� solmiEvent�� ĳ�����Ͽ� c++��
		getValue(sevent); //native �Լ� ȣ��
		}

		@Override
		public void successConnected() {//���� ������ ȣ�� ��
			connected = true;
		}
		
	};
	
	public String getPlayerName() //����� �̸� String���� C++��
	{
		return this.PlayerName;
	}
	
    public static void disConnect() //�������
    {
    //	solmi.saveLogfile();
    	 b_handler.sendEmptyMessage(DEVICE_DISCONNECT); //�ڵ鷯�� �޼��� ����
    }
	public static void connectDevice() { //�����ϱ�
		 b_handler.sendEmptyMessage(DEVICE_CONNECT);
		 
	}
	public static boolean getConnected() //������� Native call
	{
		return connected;
	}
    static {
        System.loadLibrary("cocos2dcpp");
    }     
}
