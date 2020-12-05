package com.example.sripath.lab11;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class SetCounter extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_set_counter);

        //The code when the setButton is clicked.
        //Todo 1: define the setButton and create its setOnClickListener. inside it past the following code:-
        /**
         * On clicking the button the value of counterValueText should be sent to the MainActivity and it should be called.
         * EditText counterValueText = (EditText) findViewById(R.id.counterValueText);
         * int number=Integer.parseInt(counterValueText.getText().toString());
         * Intent activityChangeIntent = new Intent(SetCounter.this,MainActivity.class);
         * //Todo 2: use putExtra("Value",number); to pass values to the MainActivity.
         * startActivity(activityChangeIntent);
         */
    }
}
