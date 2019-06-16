package mrc.bloodaid;

import android.app.Application;

import com.google.firebase.database.FirebaseDatabase;



public class BloodAid extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        FirebaseDatabase.getInstance().setPersistenceEnabled(true);
    }
}
