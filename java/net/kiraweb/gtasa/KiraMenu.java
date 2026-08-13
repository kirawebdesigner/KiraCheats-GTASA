package net.kiraweb.gtasa;

import android.app.Activity;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

public final class KiraMenu {
    private Activity activity;
    private Button bubble;
    private ScrollView menu;
    private Handler handler;

    private native boolean runNativeCheat(int id);

    public KiraMenu() {}

    public void init(final Activity activity) {
        this.activity = activity;
        this.handler = new Handler(Looper.getMainLooper());

        if (Looper.myLooper() == Looper.getMainLooper()) {
            buildMenu();
        } else {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    buildMenu();
                }
            });
        }
    }

    private int dp(float value) {
        return (int) (
                value *
                activity.getResources().getDisplayMetrics().density +
                0.5f
        );
    }

    private GradientDrawable rounded(int color, float radiusDp) {
        GradientDrawable drawable = new GradientDrawable();
        drawable.setColor(color);
        drawable.setCornerRadius(dp(radiusDp));
        return drawable;
    }

    private TextView section(String text) {
        TextView view = new TextView(activity);
        view.setText(text);
        view.setTextColor(Color.rgb(185, 190, 205));
        view.setTextSize(12f);
        view.setAllCaps(true);
        view.setPadding(dp(12), dp(12), dp(12), dp(6));
        return view;
    }

    private Button cheatButton(final String title, final int id) {
        Button button = new Button(activity);
        button.setText(title);
        button.setAllCaps(false);
        button.setTextColor(Color.WHITE);
        button.setTextSize(14f);
        button.setGravity(Gravity.CENTER_VERTICAL | Gravity.START);
        button.setPadding(dp(14), 0, dp(14), 0);
        button.setFocusable(false);
        button.setBackground(
                rounded(Color.rgb(44, 47, 58), 10)
        );

        LinearLayout.LayoutParams lp =
                new LinearLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        dp(48)
                );
        lp.setMargins(dp(8), dp(4), dp(8), dp(4));
        button.setLayoutParams(lp);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                executeCheat(title, id);
            }
        });

        return button;
    }

    private void executeCheat(String title, int id) {
        try {
            boolean ok = runNativeCheat(id);
            toast(ok ? title + " ✓" : title + " failed");
        } catch (Throwable error) {
            toast(title + " error");
        }
    }

    private void toast(String message) {
        Toast.makeText(
                activity,
                message,
                Toast.LENGTH_SHORT
        ).show();
    }

    private void buildMenu() {
        if (activity == null || activity.isFinishing()) return;

        final ViewGroup decor =
                (ViewGroup) activity.getWindow().getDecorView();

        if (decor.findViewWithTag("KiraCheatsBubble") != null) return;

        bubble = new Button(activity);
        bubble.setTag("KiraCheatsBubble");
        bubble.setText("K");
        bubble.setAllCaps(false);
        bubble.setTextColor(Color.WHITE);
        bubble.setTextSize(18f);
        bubble.setPadding(0, 0, 0, 0);
        bubble.setFocusable(false);
        bubble.setElevation(dp(8));
        bubble.setBackground(
                rounded(Color.rgb(112, 70, 255), 26)
        );

        FrameLayout.LayoutParams bubbleParams =
                new FrameLayout.LayoutParams(
                        dp(52),
                        dp(52)
                );
        bubbleParams.gravity = Gravity.CENTER_VERTICAL | Gravity.START;
        bubbleParams.leftMargin = dp(12);
        decor.addView(bubble, bubbleParams);

        LinearLayout content = new LinearLayout(activity);
        content.setOrientation(LinearLayout.VERTICAL);
        content.setPadding(dp(6), dp(8), dp(6), dp(12));
        content.setBackground(
                rounded(Color.argb(238, 22, 23, 30), 16)
        );

        TextView title = new TextView(activity);
        title.setText("KiraCheats  •  GTA SA 2.11.311");
        title.setTextColor(Color.WHITE);
        title.setTextSize(18f);
        title.setGravity(Gravity.CENTER_VERTICAL);
        title.setPadding(dp(12), dp(10), dp(12), dp(8));
        content.addView(
                title,
                new LinearLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        dp(48)
                )
        );

        TextView subtitle = new TextView(activity);
        subtitle.setText(
                "Direct native GTA cheats — no keyboard injection"
        );
        subtitle.setTextColor(Color.rgb(170, 174, 190));
        subtitle.setTextSize(11f);
        subtitle.setPadding(dp(12), 0, dp(12), dp(10));
        content.addView(subtitle);

        content.addView(section("Player"));
        content.addView(cheatButton(
                "Health + armor + $250K", 1));
        content.addView(cheatButton(
                "Infinite health", 2));
        content.addView(cheatButton(
                "Never wanted", 3));
        content.addView(cheatButton(
                "Clear wanted level", 4));
        content.addView(cheatButton(
                "Maximum stamina", 5));
        content.addView(cheatButton(
                "Maximum driving skills", 6));
        content.addView(cheatButton(
                "Maximum weapon skills", 7));

        content.addView(section("Weapons + gear"));
        content.addView(cheatButton(
                "Weapon set 1", 20));
        content.addView(cheatButton(
                "Weapon set 2", 21));
        content.addView(cheatButton(
                "Weapon set 3", 22));
        content.addView(cheatButton(
                "Parachute", 23));
        content.addView(cheatButton(
                "Jetpack", 24));

        content.addView(section("Vehicles"));
        content.addView(cheatButton(
                "Spawn Rhino", 40));
        content.addView(cheatButton(
                "Spawn Hunter", 41));
        content.addView(cheatButton(
                "Spawn Hydra", 42));
        content.addView(cheatButton(
                "Spawn Quad", 43));
        content.addView(cheatButton(
                "Spawn Dozer", 44));
        content.addView(cheatButton(
                "Spawn Stunt Plane", 45));
        content.addView(cheatButton(
                "Spawn Monster Truck", 46));
        content.addView(cheatButton(
                "Spawn Vortex", 47));
        content.addView(cheatButton(
                "Spawn Tanker", 48));
        content.addView(cheatButton(
                "Spawn Trashmaster", 49));
        content.addView(cheatButton(
                "Spawn Stock Car 1", 50));
        content.addView(cheatButton(
                "Spawn Stock Car 2", 51));
        content.addView(cheatButton(
                "Spawn Stock Car 3", 52));
        content.addView(cheatButton(
                "Spawn Stock Car 4", 53));

        content.addView(section("Weather"));
        content.addView(cheatButton(
                "Sunny", 70));
        content.addView(cheatButton(
                "Extra sunny", 71));
        content.addView(cheatButton(
                "Cloudy", 72));
        content.addView(cheatButton(
                "Rain", 73));
        content.addView(cheatButton(
                "Fog", 74));

        content.addView(section("Time"));
        content.addView(cheatButton(
                "Faster time", 90));
        content.addView(cheatButton(
                "Slower time", 91));
        content.addView(cheatButton(
                "Midnight", 92));
        content.addView(cheatButton(
                "Dusk / orange sky", 93));

        content.addView(section("Map"));
        content.addView(cheatButton(
                "Reveal full radar map", 110));

        menu = new ScrollView(activity);
        menu.setTag("KiraCheatsMenu");
        menu.setFillViewport(true);
        menu.setVerticalScrollBarEnabled(false);
        menu.setElevation(dp(12));
        menu.addView(content);
        menu.setVisibility(View.GONE);

        int screenHeight =
                activity.getResources()
                        .getDisplayMetrics()
                        .heightPixels;

        int maxHeight =
                Math.max(dp(300), screenHeight - dp(36));

        FrameLayout.LayoutParams menuParams =
                new FrameLayout.LayoutParams(
                        dp(350),
                        maxHeight
                );

        menuParams.gravity =
                Gravity.CENTER_VERTICAL | Gravity.START;
        menuParams.leftMargin = dp(76);

        decor.addView(menu, menuParams);

        bubble.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                menu.setVisibility(
                        menu.getVisibility() == View.VISIBLE
                                ? View.GONE
                                : View.VISIBLE
                );
            }
        });

        toast("KiraCheats v1.1 loaded");
    }
}
