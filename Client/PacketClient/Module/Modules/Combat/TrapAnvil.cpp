#include "TrapAnvil.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"

TrapAnvil::TrapAnvil() : IModule(0x0, Category::COMBAT, "Automatically do a big collars to anvils") {
    this->registerIntSetting("range", &this->range, this->range, 3, 12);
    registerBoolSetting("onClick", &this->onClick, this->onClick);
}

TrapAnvil::~TrapAnvil() {
}

const char* TrapAnvil::getModuleName() {
    return "TrapAnvil";
}

static std::vector<C_Entity*> targetList15;

void findEntity1510(C_Entity* currentEntity15, bool isRegularEntity) {
    static auto TrapAnvilMod = moduleMgr->getModule<TrapAnvil>();

    if (currentEntity15 == nullptr)
        return;

    if (currentEntity15 == g_Data.getLocalPlayer())  // Skip Local player
        return;

    if (!g_Data.getLocalPlayer()->canAttack(currentEntity15, false))
        return;

    if (!g_Data.getLocalPlayer()->isAlive())
        return;

    if (!currentEntity15->isAlive())
        return;

    if (!Target::isValidTarget(currentEntity15))
        return;

    float dist = (*currentEntity15->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
    if (dist < TrapAnvilMod->range) {
        targetList15.push_back(currentEntity15);

        float dist = (*currentEntity15->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

        if (dist < TrapAnvilMod->range) {
            targetList15.push_back(currentEntity15);
        }
    }
}

bool TrapAnvil::tryTrapAnvil(vec3_t TrapAnvil) {
    TrapAnvil = TrapAnvil.floor();

    C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(TrapAnvil));
    C_BlockLegacy* blockLegacy = (block->blockLegacy);
    if (blockLegacy->material->isReplaceable) {
        vec3_ti blok(TrapAnvil);

        // Find neighbour
        static std::vector<vec3_ti*> checklist;
        if (checklist.empty()) {
            checklist.push_back(new vec3_ti(0, -1, 0));
            checklist.push_back(new vec3_ti(0, 1, 0));

            checklist.push_back(new vec3_ti(0, 0, -1));
            checklist.push_back(new vec3_ti(0, 0, 1));

            checklist.push_back(new vec3_ti(-1, 0, 0));
            checklist.push_back(new vec3_ti(1, 0, 0));
        }

        bool foundCandidate = false;
        int i = 0;
        for (auto current : checklist) {
            vec3_ti calc = blok.sub(*current);
            bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
            if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
                // Found a solid block to click
                foundCandidate = true;
                blok = calc;
                break;
            }
            i++;
        }
        if (foundCandidate) {
            g_Data.getCGameMode()->buildBlock(&blok, i);

            return true;
        }
    }
    return false;
}



void TrapAnvil::onTick(C_GameMode* gm) {
    if (blockSwap) {
        Odelay2++;
        
    }

    targetList15.clear();
    g_Data.forEachEntity(findEntity1510);

    int place = 1;

    if (onClick) {
        if (GameData::isRightClickDown()) {
            place = 0;
        } else {
            place = 1;
        }
    }

    if (!onClick) {
        place = 0;
    }

    if (!targetList15.empty()) {
        // Silent Code
        int slotab = 0;
        C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
        C_Inventory* inv = supplies->inventory;
        slotab = supplies->selectedHotbarSlot;
        for (int n = 0; n < 9; n++) {
            C_ItemStack* stack = inv->getItemStack(n);
            if (stack->item != nullptr) {
                if (stack->getItem()->itemId == 49) { // itemid == 146
                    supplies->selectedHotbarSlot = n;
                    break;
                }
            }
        }
        // ground level


        // above head level
        vec3_t blockAboveH1 = targetList15[0]->eyePos0;
        vec3_t blockAboveH0 = targetList15[0]->eyePos0;
        vec3_t blockAboveH2 = targetList15[0]->eyePos0;
        vec3_t blockAboveH3 = targetList15[0]->eyePos0;
        vec3_t blockAboveH4 = targetList15[0]->eyePos0;
        vec3_t blockAboveH5 = targetList15[0]->eyePos0;

        // ground level

        // H1 x + z above head leve
        blockAboveH1.y -= targetList15[0]->height - 1;
        blockAboveH1.x -= targetList15[0]->height - 1;
        blockAboveH1.z -= targetList15[0]->height - 2;


        blockAboveH1.y -= targetList15[0]->height - 2;
        blockAboveH1.x -= targetList15[0]->height - 1;
        blockAboveH1.z -= targetList15[0]->height - 2;

        blockAboveH2.y -= targetList15[0]->height - 3;
        blockAboveH2.x -= targetList15[0]->height - 1;
        blockAboveH2.z -= targetList15[0]->height - 2;

        blockAboveH3.y -= targetList15[0]->height - 4;
        blockAboveH3.x -= targetList15[0]->height - 1;
        blockAboveH3.z -= targetList15[0]->height - 2;

        blockAboveH4.y -= targetList15[0]->height - 5;
        blockAboveH4.x -= targetList15[0]->height - 1;
        blockAboveH4.z -= targetList15[0]->height - 2;

        blockAboveH5.y -= targetList15[0]->height - 5;
        blockAboveH5.x -= targetList15[0]->height - 2;
        blockAboveH5.z -= targetList15[0]->height - 2;

        // below Player B1 to B4

        if (place == 0) {
            // L1 to R4 are x + z sides ground level

            // H1 to H4 are above players head

            if (!tryTrapAnvil(blockAboveH1)) {
                blockAboveH1.x -= 0;
                blockAboveH1.z -= 0;
            }
            if (!tryTrapAnvil(blockAboveH0)) {
                blockAboveH1.x -= 0;
                blockAboveH1.z -= 0;
            }

            if (!tryTrapAnvil(blockAboveH2)) {
                blockAboveH2.x -= 0;
                blockAboveH2.z -= 0;
            }

            if (!tryTrapAnvil(blockAboveH3)) {
                blockAboveH3.x -= 0;
                blockAboveH3.z -= 0;
            }
            if (!tryTrapAnvil(blockAboveH4)) {
                blockAboveH4.x -= 0;
                blockAboveH4.z -= 0;
            }
            if (!tryTrapAnvil(blockAboveH5)) {
                blockAboveH5.x -= 0;
                blockAboveH5.z -= 0;
            }
        }
        supplies->selectedHotbarSlot = slotab;

       


        
    }
}



