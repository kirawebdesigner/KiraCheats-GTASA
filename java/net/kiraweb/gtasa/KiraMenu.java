kipackage net.kiraweb.gtasa;

import android.app.Activity;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.KeyCharacterMap;
import android.view.KeyEvent;
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
        return (int) (value * activity.getResources().getDisplayMetrics().density + 0.5f);
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

    private Button cheatButton(final String title, final String code) {
        Button button = new Button(activity);
        button.setText(title);
        button.setAllCaps(false);
        button.setTextColor(Color.WHITE);
        button.setTextSize(14f);
        button.setGravity(Gravity.CENTER_VERTICAL | Gravity.START);
        button.setPadding(dp(14), 0, dp(14), 0);
        button.setFocusable(false);
        button.setBackground(rounded(Color.rgb(44, 47, 58), 10));

        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, dp(48));
        lp.setMargins(dp(8), dp(4), dp(8), dp(4));
        button.setLayoutParams(lp);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCheat(title, code);
            }
        });

        return button;
    }

    private Button infoButton(final String title, final String message) {
        Button button = new Button(activity);
        button.setText(title);
        button.setAllCaps(false);
        button.setTextColor(Color.WHITE);
        button.setTextSize(14f);
        button.setGravity(Gravity.CENTER_VERTICAL | Gravity.START);
        button.setPadding(dp(14), 0, dp(14), 0);
        button.setFocusable(false);
        button.setBackground(rounded(Color.rgb(52, 48, 70), 10));

        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, dp(48));
        lp.setMargins(dp(8), dp(4), dp(8), dp(4));
        button.setLayoutParams(lp);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toast(message);
            }
        });

        return button;
    }

    private void sendCheat(final String title, final String code) {
        if (activity == null || activity.isFinishing()) return;

        // Hide the card briefly so the game can regain key focus.
        menu.setVisibility(View.GONE);

        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                try {
                    View decor = activity.getWindow().getDecorView();
                    decor.setFocusableInTouchMode(true);
                    decor.requestFocus();

                    KeyCharacterMap keyMap =
                            KeyCharacterMap.load(KeyCharacterMap.VIRTUAL_KEYBOARD);
                    KeyEvent[] events = keyMap.getEvents(code.toCharArray());

                    if (events == null || events.length == 0) {
                        toast("Could not create game key events for " + title);
                        return;
                    }

                    for (KeyEvent event : events) {
                        activity.dispatchKeyEvent(event);
                    }

                    toast(title + " sent");
                } catch (Throwable error) {
                    toast("KiraCheats input error");
                }
            }
        }, 120);
    }

    private void toast(String message) {
        Toast.makeText(activity, message, Toast.LENGTH_SHORT).show();
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
        bubble.setBackground(rounded(Color.rgb(112, 70, 255), 26));

        FrameLayout.LayoutParams bubbleParams =
                new FrameLayout.LayoutParams(dp(52), dp(52));
        bubbleParams.gravity = Gravity.TOP | Gravity.START;
        bubbleParams.leftMargin = dp(14);
        bubbleParams.topMargin = dp(22);
        decor.addView(bubble, bubbleParams);

        LinearLayout content = new LinearLayout(activity);
        content.setOrientation(LinearLayout.VERTICAL);
        content.setPadding(dp(6), dp(8), dp(6), dp(12));
        content.setBackground(
                rounded(Color.argb(238, 22, 23, 30), 16));

        TextView title = new TextView(activity);
        title.setText("KiraCheats  •  GTA SA ARM64");
        title.setTextColor(Color.WHITE);
        title.setTextSize(18f);
        title.setGravity(Gravity.CENTER_VERTICAL);
        title.setPadding(dp(12), dp(10), dp(12), dp(8));
        content.addView(
                title,
                new LinearLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        dp(48)));

        TextView subtitle = new TextView(activity);
        subtitle.setText(
                "Tap an option. The menu hides while the game receives the cheat.");
        subtitle.setTextColor(Color.rgb(170, 174, 190));
        subtitle.setTextSize(11f);
        subtitle.setPadding(dp(12), 0, dp(12), dp(10));
        content.addView(subtitle);

        content.addView(section("Player"));
        content.addView(cheatButton(
                "Health + armor + money", "HESOYAM"));
        content.addView(cheatButton(
                "Infinite health", "BAGUVIX"));
        content.addView(cheatButton(
                "Never wanted", "AEZAKMI"));
        content.addView(cheatButton(
                "Clear wanted level", "ASNAEB"));
        content.addView(cheatButton(
                "Infinite oxygen", "CVWKXAM"));
        content.addView(cheatButton(
                "Never hungry", "AEDUWNV"));
        content.addView(cheatButton(
                "Maximum respect", "OGXSDAG"));
        content.addView(cheatButton(
                "Maximum driving skill", "VQIMAHA"));

        content.addView(section("Weather"));
        content.addView(cheatButton(
                "Sunny", "AFZLLQLL"));
        content.addView(cheatButton(
                "Very sunny", "ICIKPYH"));
        content.addView(cheatButton(
                "Cloudy", "ALNSFMZO"));
        content.addView(cheatButton(
                "Rain", "AUIFRVQS"));
        content.addView(cheatButton(
                "Fog", "CFVFGMJ"));

        content.addView(section("Time + world"));
        content.addView(cheatButton(
                "Faster clock", "YSOHNUL"));
        content.addView(cheatButton(
                "Faster gameplay", "PPGWJHT"));
        content.addView(cheatButton(
                "Slower gameplay", "LIYOAAY"));
        content.addView(cheatButton(
                "Always midnight", "XJVSNAJ"));
        content.addView(cheatButton(
                "Orange evening sky", "OFVIAC"));
        content.addView(cheatButton(
                "Reduced traffic", "THGLOJ"));
        content.addView(cheatButton(
                "Green traffic lights", "ZEIIVG"));

        content.addView(section("Map"));
        content.addView(infoButton(
                "All cities unlocked",
                "Load your installed 100% save in slot 1 for the full map."));

        menu = new ScrollView(activity);
        menu.setTag("KiraCheatsMenu");
        menu.setFillViewport(true);
        menu.setVerticalScrollBarEnabled(false);
        menu.setElevation(dp(12));
        menu.addView(content);
        menu.setVisibility(View.GONE);

        int screenHeight =
                activity.getResources().getDisplayMetrics().heightPixels;
        int maxHeight =
                Math.max(dp(300), screenHeight - dp(40));

        FrameLayout.LayoutParams menuParams =
                new FrameLayout.LayoutParams(dp(330), maxHeight);
        menuParams.gravity = Gravity.TOP | Gravity.START;
        menuParams.leftMargin = dp(76);
        menuParams.topMargin = dp(18);
        decor.addView(menu, menuParams);

        bubble.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                menu.setVisibility(
                        menu.getVisibility() == View.VISIBLE
                                ? View.GONE
                                : View.VISIBLE);
            }
        });

        toast("KiraCheats loaded — tap K");
    }
}
