package com.example.sripath.lab11;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //The code to initialize Bundle and get the value of counter it was set.


        Bundle bundle = null;//Todo 1: define the Bundle bundle which will be used to get the "Value".
        if(bundle!=null){
            int initialCounterValue =0;//Todo 2: get the "Value" from the bundle.
            EditText valueEditText = (EditText) findViewById(R.id.valueEditText);
            valueEditText.setText(String.valueOf(initialCounterValue));
        }

        //The code for the working of countButton.
        final Button countButton = (Button) findViewById(R.id.countButton);
        countButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //On clicking the button the value of valueEditText should increase by 1.
                //Todo 3: Take the current value of valueEditText, increment it by 1 and set the new number in the valueEditText.
            }
        });

        //The code for the working of countButton.
        final Button setButton = (Button) findViewById(R.id.setButton);
        setButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //On clicking the button the SetCounter activity should be called.
                //Todo 4: call the setCounter activity by using Intent.
            }
        });


    }
}
