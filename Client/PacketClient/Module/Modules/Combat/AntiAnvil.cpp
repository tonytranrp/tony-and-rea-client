#include "AntiAnvil.h"
#include "../ClickGuiMod.h"
#include "../Exploit/Phase.h"
#include <chrono>
#include "../Exploit/AutoButton.h"


AntiAnvil::AntiAnvil() : IModule(0x0, Category::PLAYER, "Auto break anvil if we are in an anvil!!!: DEV by tony : (                                                                        ") {
    registerFloatSetting("Height", &height, 3.0f, 1.0f, 10.0f);
    // Other settings...
}

const char* AntiAnvil::getModuleName() {
    return ("AntiAnvil");
}

bool tryPlace1(vec3_t blkPlacement) {
    blkPlacement = blkPlacement.floor();

    C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
    C_BlockLegacy* blockLegacy = block->blockLegacy;
    if (blockLegacy->material->isReplaceable) {
        vec3_ti blok(blkPlacement);
        int i = 0;

        static const std::vector<vec3_ti> checklist = {
            //vec3_ti(1, -1, 0),
            vec3_ti(0, -1, 0),
            vec3_ti(0, 1, 0),
            vec3_ti(0, 0, -1),
            vec3_ti(0, 0, 1),
            vec3_ti(-1, 0, 0),
            vec3_ti(1, 0, 0)
        };

        bool foundCandidate = false;

        for (const auto& current : checklist) {
            vec3_ti calc = blok.sub(current);
            bool Y = (g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy)->material->isReplaceable;
            if (!(g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy)->material->isReplaceable) {
                // Found a solid block to click
                foundCandidate = true;
                blok = calc;
                break;
            }
            i++;
        }

        if (foundCandidate) {
            // Remove the delay by placing the button directly without delay
            while (true) {
                g_Data.getCGameMode()->buildBlock(&blok, i); // if it breaks, then idk
                if (!(g_Data.getLocalPlayer()->region->getBlock(blok)->blockLegacy)->material->isReplaceable) {
                    break; // Break the loop once the button is successfully placed
                }
            }
            return true;
        }
    }

    return false;
}


void AntiAnvil::findPickaxeLOL3() {
    C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
    C_Inventory* inv = supplies->inventory;
    float damage = 0;
    int slot = supplies->selectedHotbarSlot;
    for (int n = 0; n < 9; n++) {
        C_ItemStack* stack = inv->getItemStack(n);
        if (stack->item != nullptr) {
            if (stack->getItem()->isPickaxe()) {
                float currentDamage = stack->getAttackingDamageWithEnchants();
                if (currentDamage > damage) {
                    damage = currentDamage;
                    slot = n;
                }
            }
        }
    }
    supplies->selectedHotbarSlot = slot;
}

bool placeBlockBelow1(const vec3_t& blockPos) {
    vec3_t blockPosBelow1(blockPos.x - 0, blockPos.y - 1, blockPos.z - 0);
    vec3_t blockPosBelow(blockPos.x - 0, blockPos.y - 2, blockPos.z - 0);
    if (tryPlace1(blockPosBelow1))
        return true;
    if (tryPlace1(blockPosBelow))
        return true;
    return false;
}


void AntiAnvil::onEnable() {
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr) return;
}

bool isFallingBlock(C_Block* block) {
    // Modify this function based on your specific requirements to determine if the block is falling
    // For example, you can check if the block has a certain behavior or properties indicating it's falling
    // Return true if the block is falling, false otherwise

    // Placeholder logic, assuming block 145 is considered falling
    int blockID = (int)block->toLegacy()->blockId;
    return (blockID == 145);
}

bool AntiAnvil::isFallingAnvilAbovePlayer(C_Player* plr) {
    if (plr == nullptr) return false;

    vec3_ti blockpos3 = plr->getPos()->floor();
    blockpos3.y = (int)blockpos3.y - 1;

    // Check for anvils falling above the player's head
    for (int yOffset = 0; yOffset <= (int)height; yOffset++) {  // Use the height value from the slider
        vec3_ti blockposAbove = blockpos3;
        blockposAbove.y += yOffset;

        C_Block* blockAbove = g_Data.getLocalPlayer()->region->getBlock(blockposAbove);
        int blockAboveID = (int)blockAbove->toLegacy()->blockId;

        if (blockAboveID == 145 && isFallingBlock(blockAbove)) {
            return true;
        }
    }

    return false;
}

bool isItemAbovePlayer(C_Player* plr, int itemID) {
    // Get the player's position
    vec3_t playerPos = plr->getPos()->floor();

    // Check the block above the player's head
    vec3_t blockAbovePos = vec3_t(playerPos.x, playerPos.y + 1, playerPos.z);
    C_Block* blockAbovePlayer = g_Data.getLocalPlayer()->region->getBlock(blockAbovePos);

    // Check if the block above has the specified item ID
    if (blockAbovePlayer->toLegacy()->blockId == itemID) {
        return true;  // Item ID detected above the player's head
    }

    return false;  // Item ID not detected
}

bool getObby1() {
    int slotab = 0;
    C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
    C_Inventory* inv = supplies->inventory;
    slotab = supplies->selectedHotbarSlot;
    for (int n = 0; n < 9; n++) {
        C_ItemStack* stack = inv->getItemStack(n);
        if (stack->item != nullptr) {
            if (stack->getItem()->itemId == 77) { // itemid == 146
                supplies->selectedHotbarSlot = n;
                break;
            }
        }
    }

    // place code
    supplies->selectedHotbarSlot = slotab;

    return false;  // Button not found
}

void AntiAnvil::onPlayerTick(C_Player* plr) {
    if (plr == nullptr) return;

    C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
    int slottd2 = supplies->selectedHotbarSlot;

    bool fallingAnvilsDetected = isFallingAnvilAbovePlayer(plr);
    bool item12Detected = isItemAbovePlayer(plr, 12);  // Check if item ID 12 (sand) is detected above the player's head

    // Check if another player is within a radius of 2
    auto* players = g_Data.getClientInstance()->getLevel()->getPlayer();
    if (players == nullptr) return;  // Ensure players is not null

    for (C_Player* player : *players) {
        if (player == g_Data.getLocalPlayer()) continue;  // Skip the local player

        if (player->getPos()->dist(*g_Data.getLocalPlayer()->getPos()) <= 2.0f) {
            // Another player is within the radius, turn on the button module
            auto buttonModule = moduleMgr->getModule<AutoButton>();
            if (buttonModule != nullptr && !buttonModule->isEnabled()) {
                buttonModule->toggle();
            }
            break;  // No need to continue checking other players
        }
    }

    // Turn on Phase module if falling anvils or item ID 12 are detected
    auto phaseModule = moduleMgr->getModule<Phase>();
    auto buttonModule = moduleMgr->getModule<AutoButton>();

    if (fallingAnvilsDetected || item12Detected) {
        caidelljthe = true;
        findPickaxeLOL3();
        supplies->selectedHotbarSlot = slottd2;

        // Check if there is already a button beneath the player's feet
        vec3_t playerFeetPos = plr->getPos()->floor();
        vec3_t buttonPos = vec3_t(playerFeetPos.x, playerFeetPos.y - 1, playerFeetPos.z);
        C_Block* blockBelowPlayer = g_Data.getLocalPlayer()->region->getBlock(buttonPos);

        if (blockBelowPlayer->toLegacy()->blockId != 77) {
            // Button is not present, get obsidian (item ID 77) instead
            bool buttonObtained = getObby1();

            // Place the button immediately under the player's feet if obtained successfully
            if (buttonObtained) {

                // Silent Code
                int slotab = 0;
                C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
                C_Inventory* inv = supplies->inventory;
                slotab = supplies->selectedHotbarSlot;
                for (int n = 0; n < 9; n++) {
                    C_ItemStack* stack = inv->getItemStack(n);
                    if (stack->item != nullptr) {
                        if (stack->getItem()->itemId == 77) { // itemid == 146
                            supplies->selectedHotbarSlot = n;
                            break;
                        }
                    }
                }
                placeBlockBelow1(playerFeetPos);
                // place code
                supplies->selectedHotbarSlot = slotab;

            }
        }

        // Turn on the Phase module if it's not enabled
        if (phaseModule != nullptr && !phaseModule->isEnabled()) {
            phaseModule->toggle();
        }
        if (buttonModule != nullptr && !buttonModule->isEnabled()) {
            buttonModule->toggle();
        }
    }
    else {
        // No falling anvils or item ID 12 detected, turn off the Phase module if it's enabled
        if (phaseModule != nullptr && phaseModule->isEnabled()) {
            phaseModule->toggle();
        }
        if (buttonModule != nullptr && buttonModule->isEnabled()) {
            buttonModule->toggle();
        }
    }
}



void AntiAnvil::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr) return;

    if (GameData::canUseMoveKeys()) {
        if (!moduleMgr->getModule<ClickGuiMod>()->isEnabled()) {
            if (caidelljthe) {
                DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
                DrawUtils::drawBox(vec3_t(blockpos3.toVec3t().floor()), vec3_t(blockpos3.toFloatVector().floor().add(1.f)), 0.5f, false);
                caidelljthe = false;
            }
        }
    }
}
