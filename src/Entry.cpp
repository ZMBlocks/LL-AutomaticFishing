#include "Entry.h"
#include <ll/api/memory/Hook.h>
#include <ll/api/mod/RegisterHelper.h>
#include <mc/network/packet/InventorySlotPacket.h>
#include <mc/world/actor/FishingHook.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/item/Item.h>

namespace AutomaticFishing {

LL_TYPE_INSTANCE_HOOK(FishingHookedHook, HookPriority::Normal, FishingHook, &FishingHook::_serverHooked, bool) {
    auto result = origin();
    if (result && !mTimeUntilHooked) {
        if (auto* player = this->getPlayerOwner()) {
            auto& item = const_cast<ItemStack&>(player->getSelectedItem());
            item.getItem()->use(item, *player);
            if (!item.isNull()) item.getItem()->use(item, *player);
            InventorySlotPacket(ContainerID::Inventory, player->getSelectedItemSlot(), item).sendTo(*player);
        }
    }
    return result;
}

Entry& Entry::getInstance() {
    static Entry instance;
    return instance;
}

bool Entry::load() { return true; }

bool Entry::enable() {
    FishingHookedHook::hook();
    return true;
}

bool Entry::disable() {
    FishingHookedHook::unhook();
    return true;
}

bool Entry::unload() { return true; }

} // namespace AutomaticFishing

LL_REGISTER_MOD(AutomaticFishing::Entry, AutomaticFishing::Entry::getInstance());