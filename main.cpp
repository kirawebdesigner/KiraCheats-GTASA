#include <mod/amlmod.h>
#include <mod/logger.h>
#include "menu_dex.h"

MYMOD(net.kiraweb.gtasa.cheats, KiraCheats GTASA, 1.0.1, KiraWeb)
NEEDGAME(com.rockstargames.gtasa)

uintptr_t pGame = 0;

static void ClearJavaException(JNIEnv* env, const char* where)
{
    if(env && env->ExceptionCheck())
    {
        logger->Error("JNI exception at %s", where);
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

static jobject LoadMenuDexInMemory(JNIEnv* env, jobject activity)
{
    if(!env || !activity) return nullptr;

    jclass activityClass = env->GetObjectClass(activity);
    if(!activityClass)
    {
        ClearJavaException(env, "GetObjectClass(activity)");
        return nullptr;
    }

    jmethodID getClassLoader = env->GetMethodID(
        activityClass,
        "getClassLoader",
        "()Ljava/lang/ClassLoader;"
    );

    if(!getClassLoader)
    {
        ClearJavaException(env, "Activity.getClassLoader");
        env->DeleteLocalRef(activityClass);
        return nullptr;
    }

    jobject parentLoader = env->CallObjectMethod(activity, getClassLoader);
    if(!parentLoader || env->ExceptionCheck())
    {
        ClearJavaException(env, "getClassLoader call");
        env->DeleteLocalRef(activityClass);
        return nullptr;
    }

    jclass loaderClass = env->FindClass("dalvik/system/InMemoryDexClassLoader");
    if(!loaderClass)
    {
        ClearJavaException(env, "FindClass InMemoryDexClassLoader");
        env->DeleteLocalRef(parentLoader);
        env->DeleteLocalRef(activityClass);
        return nullptr;
    }

    jmethodID loaderCtor = env->GetMethodID(
        loaderClass,
        "<init>",
        "(Ljava/nio/ByteBuffer;Ljava/lang/ClassLoader;)V"
    );

    if(!loaderCtor)
    {
        ClearJavaException(env, "InMemoryDexClassLoader ctor");
        env->DeleteLocalRef(loaderClass);
        env->DeleteLocalRef(parentLoader);
        env->DeleteLocalRef(activityClass);
        return nullptr;
    }

    jobject dexBuffer = env->NewDirectByteBuffer(
        const_cast<uint8_t*>(kKiraMenuDex),
        static_cast<jlong>(kKiraMenuDexLen)
    );

    if(!dexBuffer)
    {
        ClearJavaException(env, "NewDirectByteBuffer");
        env->DeleteLocalRef(loaderClass);
        env->DeleteLocalRef(parentLoader);
        env->DeleteLocalRef(activityClass);
        return nullptr;
    }

    jobject loader = env->NewObject(
        loaderClass,
        loaderCtor,
        dexBuffer,
        parentLoader
    );

    if(!loader || env->ExceptionCheck())
    {
        ClearJavaException(env, "Create InMemoryDexClassLoader");
        env->DeleteLocalRef(dexBuffer);
        env->DeleteLocalRef(loaderClass);
        env->DeleteLocalRef(parentLoader);
        env->DeleteLocalRef(activityClass);
        return nullptr;
    }

    jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
    if(!classLoaderClass)
    {
        ClearJavaException(env, "FindClass ClassLoader");
        env->DeleteLocalRef(loader);
        env->DeleteLocalRef(dexBuffer);
        env->DeleteLocalRef(loaderClass);
        env->DeleteLocalRef(parentLoader);
        env->DeleteLocalRef(activityClass);
        return nullptr;
    }

    jmethodID loadClass = env->GetMethodID(
        classLoaderClass,
        "loadClass",
        "(Ljava/lang/String;)Ljava/lang/Class;"
    );

    jstring menuClassName = env->NewStringUTF("net.kiraweb.gtasa.KiraMenu");

    jobject loadedClassObject = nullptr;
    if(loadClass && menuClassName)
    {
        loadedClassObject = env->CallObjectMethod(loader, loadClass, menuClassName);
    }

    if(!loadedClassObject || env->ExceptionCheck())
    {
        ClearJavaException(env, "loadClass KiraMenu");
        if(menuClassName) env->DeleteLocalRef(menuClassName);
        env->DeleteLocalRef(classLoaderClass);
        env->DeleteLocalRef(loader);
        env->DeleteLocalRef(dexBuffer);
        env->DeleteLocalRef(loaderClass);
        env->DeleteLocalRef(parentLoader);
        env->DeleteLocalRef(activityClass);
        return nullptr;
    }

    jclass menuClass = static_cast<jclass>(loadedClassObject);
    jmethodID menuCtor = env->GetMethodID(menuClass, "<init>", "()V");

    jobject menuObject = nullptr;
    if(menuCtor)
    {
        menuObject = env->NewObject(menuClass, menuCtor);
    }

    if(!menuObject || env->ExceptionCheck())
    {
        ClearJavaException(env, "Create KiraMenu instance");
        menuObject = nullptr;
    }

    if(menuClassName) env->DeleteLocalRef(menuClassName);
    env->DeleteLocalRef(classLoaderClass);
    env->DeleteLocalRef(loader);
    env->DeleteLocalRef(dexBuffer);
    env->DeleteLocalRef(loaderClass);
    env->DeleteLocalRef(parentLoader);
    env->DeleteLocalRef(activityClass);

    return menuObject;
}

static void StartKiraMenu(void*)
{
    JNIEnv* env = aml->GetJNIEnvironment();
    jobject activity = aml->GetCurrentActivity();

    if(!env || !activity)
    {
        logger->Error("Could not get current Android Activity.");
        return;
    }

    logger->Info("Android API level: %d", aml->GetAndroidVersion());

    jobject menuObj = nullptr;

    if(aml->GetAndroidVersion() >= 26)
    {
        logger->Info("Loading KiraMenu with InMemoryDexClassLoader.");
        menuObj = LoadMenuDexInMemory(env, activity);
    }
    else
    {
        logger->Info("Using AML legacy DEX loader.");
        menuObj = aml->InjectSmaliDEX(
            kKiraMenuDex,
            kKiraMenuDexLen,
            "net.kiraweb.gtasa.KiraMenu"
        );
    }

    if(!menuObj)
    {
        logger->Error("Failed to load KiraMenu DEX.");
        return;
    }

    jclass menuClass = env->GetObjectClass(menuObj);
    if(!menuClass)
    {
        ClearJavaException(env, "GetObjectClass KiraMenu");
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
        ClearJavaException(env, "KiraMenu.init lookup");
        env->DeleteLocalRef(menuClass);
        logger->Error("KiraMenu.init(Activity) not found.");
        return;
    }

    env->CallVoidMethod(menuObj, initMethod, activity);

    if(env->ExceptionCheck())
    {
        ClearJavaException(env, "KiraMenu.init call");
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
    logger->Info("KiraCheats v1.0.1 ARM64 loaded.");

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
