package com.qyh.cerestest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    float[] array1 = new float[3];
    float[] array2 = new float[3];
    float[] array3 = new float[3];

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        array1[0] = 2;
        array1[1] = 4;
        array1[2] = 6;

        array2[0] = 1;
        array2[1] = 3;
        array2[2] = 5;

        array3 = test(array1, array2);
        String text = array3[0]+" "+array3[1]+" "+array3[2];
        Log.d("MainActivity", text);
        tv.setText(text);

        String ceres = testCeres();
        Log.d("MainActivity", ceres);
        tv.setText(ceres);
    }

    public native String stringFromJNI();
    public native float[] test(float[] array1, float[] array2);
    public native String testCeres();
}
