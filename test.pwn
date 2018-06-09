#include "inventory.inc"

main() {
	new Item:tmp;

	AddItemToInventory(0, Item:0);
	RemoveItemFromInventory(0, 0);
	GetInventorySlotItem(0, 0, tmp);
	IsInventorySlotUsed(0, 0);
	IsPlayerInventoryFull(0);
	IsPlayerInventoryEmpty(0);
	InventoryFitsItemType(0, ItemType:0, bool:tmp);
	new slots;
	GetInventoryFreeSlots(0, slots);
	new player;
	GetItemInventoryPlayer(Item:0, player);
	new slot;
	GetItemPlayerInventorySlot(Item:0, slot);
	SetPlayerInventorySize(0, 0);
	new size;
	GetPlayerInventorySize(0, size);
}
