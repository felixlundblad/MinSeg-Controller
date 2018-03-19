package com.example.felix.btminseg.feature;

import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

public class GyroControl extends AppCompatActivity implements SensorEventListener{
    private TextView textView;
    private Button touchBtn;
    private SeekBar seekBar;
    private Sensor mySensor;
    private SensorManager SM;
    private int speed;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gyro_control);

        SM = (SensorManager) getSystemService(SENSOR_SERVICE);

        mySensor = SM.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        SM.registerListener(this, mySensor, SensorManager.SENSOR_DELAY_GAME);

        textView = findViewById(R.id.speedText);
        seekBar = findViewById(R.id.seekBar);

        touchBtn = findViewById(R.id.touchBtn);

        touchBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent startIntent = new Intent(getApplicationContext(), MainActivity.class);
                startActivity(startIntent);
            }
        });
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        speed = Math.round(-(sensorEvent.values[1])*15);
        if(speed > 100) speed = 100;
        else if(speed < -100) speed = -100;
        textView.setText("Speed: " + speed);
        seekBar.setProgress(speed/2 + 50);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }
}
