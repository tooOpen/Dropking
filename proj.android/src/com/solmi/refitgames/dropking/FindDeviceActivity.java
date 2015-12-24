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
	private ArrayAdapter<String> mNewDeviceArrayAdapter; //리스트에 먹힐 어댑터 텍스트
	
	private TextView g_tvDev;//기기옆에 텍스트 뷰 선택 시 기기 이름 띄움
	private TextView g_tvTitle; //검색중입니다
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
		LinearLayout scanButton = (LinearLayout)findViewById(R.id.button_scan); //다시 검색하기 버튼
		scanButton.setOnClickListener(new View.OnClickListener() {		
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(mBtAdapter.isEnabled()){ //블루투스 가능하면
					doDiscover(); //블루투스 탐색 함수
					searchTimer(11); //프로그레스바 그리기
				}
				else{
					Toast.makeText(getApplicationContext(),getResources().getString(R.string.tryBTActive),Toast.LENGTH_LONG).show();
				}
				
				
			}
		});
		
		
		
		//initial 
		mNewDeviceArrayAdapter = new ArrayAdapter<String>(this,R.layout.dev_name);//리스트 뷰에 먹힘 텍스트뷰
	
		//find and set up the Listview for new
		ListView newDeviceArrayAdapter = (ListView)findViewById(R.id.new_devices);
		newDeviceArrayAdapter.setAdapter(mNewDeviceArrayAdapter); //
		newDeviceArrayAdapter.setOnItemClickListener(mDeviceClickListener); //선택하였을 시 콜백 객체 등록
		
		//Register for broadcast
		IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND); //블루투스 브로드캐스트 리시버 등록
		this.registerReceiver(mReceiver, filter);
		
		filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
		this.registerReceiver(mReceiver, filter);
		
		filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
		this.registerReceiver(mReceiver, filter);
				
		mBtAdapter = BluetoothAdapter.getDefaultAdapter();
		
		if(mBtAdapter.isEnabled()){		//블루투스가 켜져 있으면
			doDiscover();	//바로 장치를 검색하고			
		}
		else{	//블루투스가 꺼져있으면			
			mBtAdapter.enable();	//장치를 활성화 하고 기다리면 브로드캐스트 리시버에서 검색을 시작
			/*
			Handler handler = new Handler();
			handler.postDelayed(new Runnable(){
				public void run() {
					// TODO Auto-generated method stub
					//잠시 후에 장치 검색을 시작 한다.
					doDiscover();
				}					
			}, 500);
			*/
			
		}
		
		/**
		 * 2014.01.13 On/Off 스위치 사용 안함
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
		if(mNewDeviceArrayAdapter.getCount() >= 0) //찾은게있으면 
			mNewDeviceArrayAdapter.clear(); //비우고
    	    	
    	if(mBtAdapter.isDiscovering()){ //검색중이면
    		mBtAdapter.cancelDiscovery();//중지 후 
    	}
    	
    	mBtAdapter.startDiscovery(); // 다시 탐색
		searchTimer(11);	//프로그래스바를 그린다
	}
	
	
	private OnItemClickListener mDeviceClickListener = new OnItemClickListener() //리스트 뷰 클릭시
	{
		public void onItemClick(AdapterView<?> av, View v, int arg2, //여기 들어옴
				long arg3) {
			// TODO Auto-generated method stub
			mBtAdapter.cancelDiscovery(); //검색 중자ㅣ
				
			String info = ((TextView)v).getText().toString();
			
			if(info.length() != 0)
			{
				Intent intent = new Intent();
				String address = info.substring(info.length() - 17);
				String dev = info.substring(0,info.length() - 17);
				intent.putExtra(EXTRA_DEVICE_ADDRESS, address);		//인텐트에 기기값넣기	
				setResult(RESULT_OK, intent);//result 설정
				g_tvDev.setText(dev);
			}
			else
				setResult(RESULT_CANCEL);
			
			finish();	 //앱 종료		
		}		
	};

	private void nofindDev(){
		setResult(RESULT_CANCEL);//실패 ㅜㅡㅜ
	}
	private final BroadcastReceiver mReceiver = new BroadcastReceiver()//브로드캐스트 리시버
	{
		@Override
		public void onReceive(Context context, Intent intent) { //리시브 하였을 때.
			// TODO Auto-generated method stub
			String action = intent.getAction(); //액션 받고
			
			if(BluetoothDevice.ACTION_FOUND.equals(action)){ //찾았을 때는 여기
				
				BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				if(device.getName() != null){
					mNewDeviceArrayAdapter.add(device.getName() + "\n" + device.getAddress());
				}
						
				
			}else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)){//검색 끝은 여기
				setProgressBarIndeterminateVisibility(false); //프로그레스 바 안보이게
				g_tvTitle.setText(getResources().getString(R.string.searchedDevice));//못찾음
				if(mNewDeviceArrayAdapter.getCount() == 0){						
					nofindDev(); //검색실패라고 result에 넣음
				}
			}			
			else if(BluetoothAdapter.ACTION_STATE_CHANGED.equals(action)){//블루투스가 꺼졌다 켜졌을 때.
				int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.STATE_OFF);	//기본적으로 꺼져있다고 보고
				
				if(state == BluetoothAdapter.STATE_ON){	//턴 온 되었을때
					doDiscover();
				}
			}
		}
		
	};
	
	
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if(event.getKeyCode() == event.KEYCODE_BACK){ //뒤로가기 전에 실패넣기
			setResult(RESULT_CANCEL);
		}
		
		return super.onKeyDown(keyCode, event);
	}
	
	
	private CountDownTimer g_timer;
	private int g_nVal = 0;
	protected void searchTimer(int sec) {
		g_nVal = 0;
		g_timer = new CountDownTimer(sec * 1000, 100) {//11초동안 0.1초마다
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
