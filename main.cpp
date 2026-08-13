#include <mod/amlmod.h>
#include <mod/logger.h>
#include "menu_dex.h"

#include <cstring>

MYMOD(net.kiraweb.gtasa.cheats, KiraCheats GTASA, 1.1.0, KiraWeb)
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

static bool CallGameCheat(const char* symbol)
{
    if(!pGame || !symbol) return false;

    uintptr_t addr = aml->GetSym(pGame, symbol);
    if(!addr)
    {
        logger->Error("Cheat symbol not found: %s", symbol);
        return false;
    }

    logger->Info("Calling cheat %s at " PTRFMT, symbol, addr);

    using CheatFn = void(*)();
    reinterpret_cast<CheatFn>(addr)();

    return true;
}

static bool RevealFullMap211311()
{
    const char* ver = aml->GetAppVersionName();
    if(!ver || std::strstr(ver, "2.11.311") == nullptr)
    {
        logger->Error(
            "Map reveal patch refused: expected GTA 2.11.311, got %s",
            ver ? ver : "(unknown)"
        );
        return false;
    }

    // GTA SA Android 2.11.311:
    // CTheZones::ZonesVisited[100]
    // CTheZones::ZonesRevealed
    std::memset(reinterpret_cast<void*>(pGame + 0xA59D7A), 1, 100);
    *reinterpret_cast<uint32_t*>(pGame + 0xA59DE0) = 100;

    logger->Info("Full radar map revealed for GTA 2.11.311.");
    return true;
}

enum CheatId
{
    CHEAT_HEALTH_ARMOR_MONEY = 1,
    CHEAT_INFINITE_HEALTH,
    CHEAT_NEVER_WANTED,
    CHEAT_CLEAR_WANTED,
    CHEAT_MAX_STAMINA,
    CHEAT_MAX_DRIVING,
    CHEAT_MAX_WEAPONS,

    CHEAT_WEAPON_SET_1 = 20,
    CHEAT_WEAPON_SET_2,
    CHEAT_WEAPON_SET_3,
    CHEAT_PARACHUTE,
    CHEAT_JETPACK,

    CHEAT_RHINO = 40,
    CHEAT_HUNTER,
    CHEAT_HYDRA,
    CHEAT_QUAD,
    CHEAT_DOZER,
    CHEAT_STUNT_PLANE,
    CHEAT_MONSTER,
    CHEAT_VORTEX,
    CHEAT_TANKER,
    CHEAT_TRASHMASTER,
    CHEAT_STOCK_CAR_1,
    CHEAT_STOCK_CAR_2,
    CHEAT_STOCK_CAR_3,
    CHEAT_STOCK_CAR_4,

    CHEAT_SUNNY = 70,
    CHEAT_EXTRA_SUNNY,
    CHEAT_CLOUDY,
    CHEAT_RAIN,
    CHEAT_FOG,

    CHEAT_FAST_TIME = 90,
    CHEAT_SLOW_TIME,
    CHEAT_MIDNIGHT,
    CHEAT_DUSK,

    CHEAT_REVEAL_MAP = 110
};

static jboolean JNICALL NativeRunCheat(
    JNIEnv* env,
    jobject thiz,
    jint id
)
{
    (void)env;
    (void)thiz;

    bool ok = false;

    switch(id)
    {
        case CHEAT_HEALTH_ARMOR_MONEY:
            ok = CallGameCheat("_ZN6CCheat22MoneyArmourHealthCheatEv");
            break;

        case CHEAT_INFINITE_HEALTH:
            ok = CallGameCheat("_ZN6CCheat11HealthCheatEv");
            break;

        case CHEAT_NEVER_WANTED:
            ok = CallGameCheat("_ZN6CCheat14NotWantedCheatEv");
            break;

        case CHEAT_CLEAR_WANTED:
            ok = CallGameCheat("_ZN6CCheat20WantedLevelDownCheatEv");
            break;

        case CHEAT_MAX_STAMINA:
            ok = CallGameCheat("_ZN6CCheat12StaminaCheatEv");
            break;

        case CHEAT_MAX_DRIVING:
            ok = CallGameCheat("_ZN6CCheat18VehicleSkillsCheatEv");
            break;

        case CHEAT_MAX_WEAPONS:
            ok = CallGameCheat("_ZN6CCheat17WeaponSkillsCheatEv");
            break;

        case CHEAT_WEAPON_SET_1:
            ok = CallGameCheat("_ZN6CCheat12WeaponCheat1Ev");
            break;

        case CHEAT_WEAPON_SET_2:
            ok = CallGameCheat("_ZN6CCheat12WeaponCheat2Ev");
            break;

        case CHEAT_WEAPON_SET_3:
            ok = CallGameCheat("_ZN6CCheat12WeaponCheat3Ev");
            break;

        case CHEAT_PARACHUTE:
            ok = CallGameCheat("_ZN6CCheat14ParachuteCheatEv");
            break;

        case CHEAT_JETPACK:
            ok = CallGameCheat("_ZN6CCheat12JetpackCheatEv");
            break;

        case CHEAT_RHINO:
            ok = CallGameCheat("_ZN6CCheat9TankCheatEv");
            break;

        case CHEAT_HUNTER:
            ok = CallGameCheat("_ZN6CCheat11ApacheCheatEv");
            break;

        case CHEAT_HYDRA:
            ok = CallGameCheat("_ZN6CCheat11FlyboyCheatEv");
            break;

        case CHEAT_QUAD:
            ok = CallGameCheat("_ZN6CCheat9QuadCheatEv");
            break;

        case CHEAT_DOZER:
            ok = CallGameCheat("_ZN6CCheat10DozerCheatEv");
            break;

        case CHEAT_STUNT_PLANE:
            ok = CallGameCheat("_ZN6CCheat15StuntPlaneCheatEv");
            break;

        case CHEAT_MONSTER:
            ok = CallGameCheat("_ZN6CCheat17MonsterTruckCheatEv");
            break;

        case CHEAT_VORTEX:
            ok = CallGameCheat("_ZN6CCheat11VortexCheatEv");
            break;

        case CHEAT_TANKER:
            ok = CallGameCheat("_ZN6CCheat11TankerCheatEv");
            break;

        case CHEAT_TRASHMASTER:
            ok = CallGameCheat("_ZN6CCheat16TrashmasterCheatEv");
            break;

        case CHEAT_STOCK_CAR_1:
            ok = CallGameCheat("_ZN6CCheat13StockCarCheatEv");
            break;

        case CHEAT_STOCK_CAR_2:
            ok = CallGameCheat("_ZN6CCheat14StockCar2CheatEv");
            break;

        case CHEAT_STOCK_CAR_3:
            ok = CallGameCheat("_ZN6CCheat14StockCar3CheatEv");
            break;

        case CHEAT_STOCK_CAR_4:
            ok = CallGameCheat("_ZN6CCheat14StockCar4CheatEv");
            break;

        case CHEAT_SUNNY:
            ok = CallGameCheat("_ZN6CCheat17SunnyWeatherCheatEv");
            break;

        case CHEAT_EXTRA_SUNNY:
            ok = CallGameCheat("_ZN6CCheat22ExtraSunnyWeatherCheatEv");
            break;

        case CHEAT_CLOUDY:
            ok = CallGameCheat("_ZN6CCheat18CloudyWeatherCheatEv");
            break;

        case CHEAT_RAIN:
            ok = CallGameCheat("_ZN6CCheat17RainyWeatherCheatEv");
            break;

        case CHEAT_FOG:
            ok = CallGameCheat("_ZN6CCheat17FoggyWeatherCheatEv");
            break;

        case CHEAT_FAST_TIME:
            ok = CallGameCheat("_ZN6CCheat13FastTimeCheatEv");
            break;

        case CHEAT_SLOW_TIME:
            ok = CallGameCheat("_ZN6CCheat13SlowTimeCheatEv");
            break;

        case CHEAT_MIDNIGHT:
            ok = CallGameCheat("_ZN6CCheat13MidnightCheatEv");
            break;

        case CHEAT_DUSK:
            ok = CallGameCheat("_ZN6CCheat9DuskCheatEv");
            break;

        case CHEAT_REVEAL_MAP:
            ok = RevealFullMap211311();
            break;

        default:
            logger->Error("Unknown KiraCheats id: %d", id);
            break;
    }

    logger->Info("Cheat id %d result: %s", id, ok ? "OK" : "FAILED");
    return ok ? JNI_TRUE : JNI_FALSE;
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
        loadedClassObject = env->CallObjectMethod(
            loader,
            loadClass,
            menuClassName
        );
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

static bool RegisterMenuNatives(JNIEnv* env, jclass menuClass)
{
    JNINativeMethod methods[] = {
        {
            const_cast<char*>("runNativeCheat"),
            const_cast<char*>("(I)Z"),
            reinterpret_cast<void*>(NativeRunCheat)
        }
    };

    if(env->RegisterNatives(menuClass, methods, 1) != JNI_OK)
    {
        ClearJavaException(env, "RegisterNatives");
        logger->Error("RegisterNatives for KiraMenu failed.");
        return false;
    }

    logger->Info("KiraMenu native cheat bridge registered.");
    return true;
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

    if(!RegisterMenuNatives(env, menuClass))
    {
        env->DeleteLocalRef(menuClass);
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
    logger->Info("KiraCheats v1.1.0 direct ARM64 loaded.");

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
