package mrc.bloodaid;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

public class SearchActivity extends AppCompatActivity {

    private Spinner bgps;
    private Button submitButton;
    private EditText editText;
    String blgps[] = {"A+", "A-", "B+", "B-", "O+", "O-", "AB+", "AB-"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_search);
        submitButton = findViewById(R.id.search_submit);
        //editText = findViewById(R.id.search_name);



        bgps = findViewById(R.id.search_blood_group_spinner);
        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, blgps);
        arrayAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        bgps.setAdapter(arrayAdapter);

        submitButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                    Intent resultIntent = new Intent(SearchActivity.this, ResultActivity.class);
                    resultIntent.putExtra("bloodgp", bgps.getSelectedItem().toString());
                if (! bgps.getSelectedItem().toString().isEmpty()) {
                    startActivity(resultIntent);
                } else {
                    Toast.makeText(SearchActivity.this, "Please Select Blood Group", Toast.LENGTH_SHORT).show();
                }

            }
        });
    }
}
