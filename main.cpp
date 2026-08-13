#include <mod/amlmod.h>
#include <mod/logger.h>

MYMOD(net.kiraweb.gtasa.test, Kira GTASA Test, 0.1.0, KiraWeb)
NEEDGAME(com.rockstargames.gtasa)

uintptr_t pGame = 0;

extern "C" void OnModLoad()
{
    logger->SetTag("KiraTest");
    logger->Info("Kira ARM64 AML test mod loaded!");

    pGame = aml->GetLib("libGame.so");

    if (pGame)
        logger->Info("GTASA libGame.so found at " PTRFMT, pGame);
    else
        logger->Error("GTASA libGame.so was NOT found.");
}
