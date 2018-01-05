#include "inventory.inc"

main() {
	new tmp;

	AddItemToInventory(0, 0);
	RemoveItemFromInventory(0, 0);
	GetInventorySlotItem(0, 0, tmp);
	IsInventorySlotUsed(0, 0);
	IsPlayerInventoryFull(0);
	IsPlayerInventoryEmpty(0);
	InventoryFitsItemType(0, ItemType:0, bool:tmp);
	GetInventoryFreeSlots(0, tmp);
	GetItemInventoryPlayer(0, tmp);
	GetItemPlayerInventorySlot(0, tmp);
	SetPlayerInventorySize(0, 0);
	GetPlayerInventorySize(0, tmp);
}
