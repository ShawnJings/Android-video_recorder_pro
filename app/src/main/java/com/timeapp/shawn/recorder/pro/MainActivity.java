package com.timeapp.shawn.recorder.pro;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {

	static {
		System.loadLibrary("songstudio");
	}
	private Button forward_video_recorder;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		forward_video_recorder = (Button) findViewById(R.id.forward_video_recorder);
		forward_video_recorder.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, CommonRecordPublisherActivity.class);
				startActivity(intent);
			}
		});
	}
}
