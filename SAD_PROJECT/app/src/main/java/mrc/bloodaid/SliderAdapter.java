package mrc.bloodaid;



import android.content.Context;
import android.support.v4.view.PagerAdapter;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;



public class SliderAdapter extends PagerAdapter {

    Context context;
    LayoutInflater layoutInflator;

    public SliderAdapter(Context context)
    {
        this.context = context ;
    }

    public int[] slide_images = {R.drawable.main_icon1, R.drawable.maps_icon};

    public String slide_headings[] = {"WELCOME TO BLOOD AID"};

    public String slide_descriptions[] = {"Making blood donation easier"};


    @Override
    public int getCount() {
        return 2;
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        return view == (RelativeLayout) object ;
    }

    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        layoutInflator = (LayoutInflater) context.getSystemService(context.LAYOUT_INFLATER_SERVICE);
        View view = layoutInflator.inflate(R.layout.slide_layout, container, false);

        ImageView slideImageView = view.findViewById(R.id.slide_image);
        TextView slideHeading = view.findViewById(R.id.slide_heading);
        TextView slideDescription = view.findViewById(R.id.slide_description);

        slideImageView.setImageResource(slide_images[position]);
        slideHeading.setText(slide_headings[position]);
        slideDescription.setText(slide_descriptions[position]);

        container.addView(view);

        return view;
    }

    // TO REMOVE VIEWS AFTER THE LAST PAGE  NEXT IS CLICKED
    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        container.removeView((RelativeLayout)object);

    }

}