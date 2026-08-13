#include <mod/amlmod.h>
#include <mod/logger.h>
#include "menu_dex.h"

MYMOD(net.kiraweb.gtasa.cheats, KiraCheats GTASA, 1.0.0, KiraWeb)
NEEDGAME(com.rockstargames.gtasa)

uintptr_t pGame = 0;

static void StartKiraMenu(void*)
{
    JNIEnv* env = aml->GetJNIEnvironment();
    jobject activity = aml->GetCurrentActivity();

    if(!env || !activity)
    {
        logger->Error("Could not get current Android Activity.");
        return;
    }

    jobject menuObj = aml->InjectSmaliDEX(
        kKiraMenuDex,
        kKiraMenuDexLen,
        "net.kiraweb.gtasa.KiraMenu"
    );

    if(!menuObj)
    {
        logger->Error("Failed to inject KiraMenu DEX.");
        return;
    }

    jclass menuClass = env->GetObjectClass(menuObj);
    if(!menuClass)
    {
        logger->Error("Failed to get KiraMenu class.");
        return;
    }

    jmethodID initMethod = env->GetMethodID(
        menuClass,
        "init",
        "(Landroid/app/Activity;)V"
    );

    if(!initMethod)
    {
        if(env->ExceptionCheck()) env->ExceptionClear();
        env->DeleteLocalRef(menuClass);
        logger->Error("KiraMenu.init(Activity) not found.");
        return;
    }

    env->CallVoidMethod(menuObj, initMethod, activity);

    if(env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        logger->Error("KiraMenu.init(Activity) threw an exception.");
    }
    else
    {
        logger->Info("KiraCheats menu UI started.");
    }

    env->DeleteLocalRef(menuClass);
}

extern "C" void OnModLoad()
{
    logger->SetTag("KiraCheats");
    logger->Info("KiraCheats v1 ARM64 loaded.");

    pGame = aml->GetLib("libGame.so");

    if(pGame)
        logger->Info("GTASA libGame.so found at " PTRFMT, pGame);
    else
        logger->Error("GTASA libGame.so was NOT found.");

    if(!aml->PushToJavaUIThread(StartKiraMenu, nullptr))
    {
        logger->Error("Could not queue KiraMenu on Android UI thread.");
    }
}
