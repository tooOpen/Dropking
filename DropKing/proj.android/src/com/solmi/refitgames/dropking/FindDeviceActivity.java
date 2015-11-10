package com.solmi.refitgames.dropking;

import com.dropking.game.R;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class FindDeviceActivity extends Activity{
	
	public static final String EXTRA_DEVICE_ADDRESS = "device_address";				
	//result code
	public static final int RESULT_OK = 201;											
	public static final int RESULT_CANCEL = 200;
	public static final int REQUEST_ENABLE_BT = 202;
	
	public static final int SET_DEVICE_ADDRESS = 100;
	
	
	
	private BluetoothAdapter mBtAdapter;	
	private ArrayAdapter<String> mNewDeviceArrayAdapter; //����Ʈ�� ���� ����� �ؽ�Ʈ
	
	private TextView g_tvDev;//��⿷�� �ؽ�Ʈ �� ���� �� ��� �̸� ���
	private TextView g_tvTitle; //�˻����Դϴ�
	private ImageView g_ivDev;
	private ProgressBar g_progressSearch;
	


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_bluetooth_search);
		
		setResult(Activity.RESULT_CANCELED);
		
		g_tvDev = (TextView)findViewById(R.id.tv_selectedDev); 
		g_ivDev = (ImageView)findViewById(R.id.iv_selectedDev);
		g_progressSearch = (ProgressBar)findViewById(R.id.progress_measure);
		g_tvTitle = (TextView)findViewById(R.id.title_new_devices);
		LinearLayout scanButton = (LinearLayout)findViewById(R.id.button_scan); //�ٽ� �˻��ϱ� ��ư
		scanButton.setOnClickListener(new View.OnClickListener() {		
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(mBtAdapter.isEnabled()){ //������� �����ϸ�
					doDiscover(); //������� Ž�� �Լ�
					searchTimer(11); //���α׷����� �׸���
				}
				else{
					Toast.makeText(getApplicationContext(),getResources().getString(R.string.tryBTActive),Toast.LENGTH_LONG).show();
				}
				
				
			}
		});
		
		
		
		//initial 
		mNewDeviceArrayAdapter = new ArrayAdapter<String>(this,R.layout.dev_name);//����Ʈ �信 ���� �ؽ�Ʈ��
	
		//find and set up the Listview for new
		ListView newDeviceArrayAdapter = (ListView)findViewById(R.id.new_devices);
		newDeviceArrayAdapter.setAdapter(mNewDeviceArrayAdapter); //
		newDeviceArrayAdapter.setOnItemClickListener(mDeviceClickListener); //�����Ͽ��� �� �ݹ� ��ü ���
		
		//Register for broadcast
		IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND); //������� ��ε�ĳ��Ʈ ���ù� ���
		this.registerReceiver(mReceiver, filter);
		
		filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
		this.registerReceiver(mReceiver, filter);
		
		filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
		this.registerReceiver(mReceiver, filter);
				
		mBtAdapter = BluetoothAdapter.getDefaultAdapter();
		
		if(mBtAdapter.isEnabled()){		//��������� ���� ������
			doDiscover();	//�ٷ� ��ġ�� �˻��ϰ�			
		}
		else{	//��������� ����������			
			mBtAdapter.enable();	//��ġ�� Ȱ��ȭ �ϰ� ��ٸ��� ��ε�ĳ��Ʈ ���ù����� �˻��� ����
			/*
			Handler handler = new Handler();
			handler.postDelayed(new Runnable(){
				public void run() {
					// TODO Auto-generated method stub
					//��� �Ŀ� ��ġ �˻��� ���� �Ѵ�.
					doDiscover();
				}					
			}, 500);
			*/
			
		}
		
		/**
		 * 2014.01.13 On/Off ����ġ ��� ����
		if(mBtAdapter.isEnabled()){
			gSWonoff.setChecked(true);
			doDiscover();
			searchTimer(11);
		}
		else{
			gSWonoff.setChecked(false);
		}
		**/
	}


	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		if(mBtAdapter != null){
			mBtAdapter.cancelDiscovery();
		}
		//Unregister
		this.unregisterReceiver(mReceiver);
	}
	

	public void doDiscover(){		
		g_tvTitle.setText(getResources().getString(R.string.searchDevice));
		if(mNewDeviceArrayAdapter.getCount() >= 0) //ã���������� 
			mNewDeviceArrayAdapter.clear(); //����
    	    	
    	if(mBtAdapter.isDiscovering()){ //�˻����̸�
    		mBtAdapter.cancelDiscovery();//���� �� 
    	}
    	
    	mBtAdapter.startDiscovery(); // �ٽ� Ž��
		searchTimer(11);	//���α׷����ٸ� �׸���
	}
	
	
	private OnItemClickListener mDeviceClickListener = new OnItemClickListener() //����Ʈ �� Ŭ����
	{
		public void onItemClick(AdapterView<?> av, View v, int arg2, //���� ����
				long arg3) {
			// TODO Auto-generated method stub
			mBtAdapter.cancelDiscovery(); //�˻� ���ڤ�
				
			String info = ((TextView)v).getText().toString();
			
			if(info.length() != 0)
			{
				Intent intent = new Intent();
				String address = info.substring(info.length() - 17);
				String dev = info.substring(0,info.length() - 17);
				intent.putExtra(EXTRA_DEVICE_ADDRESS, address);		//����Ʈ�� ��Ⱚ�ֱ�	
				setResult(RESULT_OK, intent);//result ����
				g_tvDev.setText(dev);
			}
			else
				setResult(RESULT_CANCEL);
			
			finish();	 //�� ����		
		}		
	};

	private void nofindDev(){
		setResult(RESULT_CANCEL);//���� �̤Ѥ�
	}
	private final BroadcastReceiver mReceiver = new BroadcastReceiver()//��ε�ĳ��Ʈ ���ù�
	{
		@Override
		public void onReceive(Context context, Intent intent) { //���ú� �Ͽ��� ��.
			// TODO Auto-generated method stub
			String action = intent.getAction(); //�׼� �ް�
			
			if(BluetoothDevice.ACTION_FOUND.equals(action)){ //ã���� ���� ����
				
				BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				if(device.getName() != null){
					mNewDeviceArrayAdapter.add(device.getName() + "\n" + device.getAddress());
				}
						
				
			}else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)){//�˻� ���� ����
				setProgressBarIndeterminateVisibility(false); //���α׷��� �� �Ⱥ��̰�
				g_tvTitle.setText(getResources().getString(R.string.searchedDevice));//��ã��
				if(mNewDeviceArrayAdapter.getCount() == 0){						
					nofindDev(); //�˻����ж�� result�� ����
				}
			}			
			else if(BluetoothAdapter.ACTION_STATE_CHANGED.equals(action)){//��������� ������ ������ ��.
				int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.STATE_OFF);	//�⺻������ �����ִٰ� ����
				
				if(state == BluetoothAdapter.STATE_ON){	//�� �� �Ǿ�����
					doDiscover();
				}
			}
		}
		
	};
	
	
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if(event.getKeyCode() == event.KEYCODE_BACK){ //�ڷΰ��� ���� ���гֱ�
			setResult(RESULT_CANCEL);
		}
		
		return super.onKeyDown(keyCode, event);
	}
	
	
	private CountDownTimer g_timer;
	private int g_nVal = 0;
	protected void searchTimer(int sec) {
		g_nVal = 0;
		g_timer = new CountDownTimer(sec * 1000, 100) {//11�ʵ��� 0.1�ʸ���
			@Override
			public void onFinish() {				
				g_progressSearch.setProgress(1000);
			}

			@Override
			public void onTick(long millisUntilFinished) {
				g_progressSearch.setProgress(g_nVal++);
			}

		};
		g_timer.start();
	}

}
