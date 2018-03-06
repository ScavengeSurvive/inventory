// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_inventory
	#undef _inc_inventory
#endif
// custom include-guard to ensure we don't duplicate
#if defined _inventory_included
	#endinput
#endif
#define _inventory_included


#include <a_samp>
#include <logger>
#include <item>
#include <container>
#include <YSI\y_timers>
#include <YSI\y_hooks>


// Maximum amount of item slots available in a player inventory.
#if !defined MAX_INVENTORY_SLOTS
	#define MAX_INVENTORY_SLOTS (4)
#endif

forward AddItemToInventory(playerid, itemid, call = 1);
/*
# Description
Adds specified item to players inventory and removes item from world.

# Parameters
- call: Determines whether or not to call OnItemAddToInventory.

# Returns
Returns 0 on success. If item doesn't fit, returns a positive integer
representing required item slots, otherwise -1 if itemid invalid, -2 if item is
already in container, -4 if OnItemAddToInventory returned non-zero, -5 if item
is in the inventory of another player, -6 if item is inside a container from the
SIF/Container module.
*/

forward RemoveItemFromInventory(playerid, slotid, call = 1);
/*
# Description
Removes the item from the specified slot if there is one.

# Parameters
- slotid: Inventory slot to remove from, not an itemid.
- call: If true, will call OnItemRemoveFromInventory.
*/

forward GetInventorySlotItem(playerid, slotid, &itemid);
/*
# Description
Returns the ID handle of the item stored in the specified slot.
*/

forward bool:IsInventorySlotUsed(playerid, slotid);
/*
# Description
Checks if the specified inventory slot contains an item.
*/

forward bool:IsPlayerInventoryFull(playerid);
/*
# Description
Checks if a players inventory is full.
*/

forward bool:IsPlayerInventoryEmpty(playerid);
/*
# Description
Checks if a players inventory is empty.
*/

forward InventoryFitsItemType(playerid, ItemType:itemtype, &bool:result);
/*
# Description
Checks if an item type will fit into a player's inventory.
*/

forward GetInventoryFreeSlots(playerid, &slots);
/*
# Description
Returns the amount of free slots in a player's inventory.
*/

forward GetItemInventoryPlayer(itemid, &playerid);
/*
# Description
Returns the ID of a player if itemid is stored in their inventory.
*/

forward GetItemPlayerInventorySlot(itemid, &slot);
/*
# Description
Returns inventory slot of an item if it's stored inside a player's inventory.
*/

forward SetPlayerInventorySize(playerid, size);
/*
# Description
Sets the maximum amount of slots a player's inventory has.
*/

forward GetPlayerInventorySize(playerid, &size);
/*
# Description
Returns the capacity of a player's inventory.
*/


// Events


forward OnItemAddToInventory(playerid, itemid);
/*
# Called
Before an item is added to a player's inventory by the function AddItemToInventory.

# Returns
1 to cancel item being added.
*/

forward OnItemAddedToInventory(playerid, itemid);
/*
# Called
After an item is added to a player's inventory by the function AddItemToInventory.
*/

forward OnItemRemoveFromInventory(playerid, slot);
/*
# Called
Before an item is removed from a player's inventory by the function RemoveItemFromInventory.

# Returns
1 to cancel item being removed.
*/

forward OnItemRemovedFromInventory(playerid, slot);
/*
# Called
After an item is removed from a player's inventory by the function RemoveItemFromInventory.
*/


static
	inv_Container[MAX_PLAYERS] = {INVALID_CONTAINER_ID, ...},
	inv_ContainerPlayer[MAX_CONTAINER] = {INVALID_PLAYER_ID, ...};


stock AddItemToInventory(playerid, itemid, call = 1) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	if(call) {
		if(CallLocalFunction("OnItemAddToInventory", "dd", playerid, itemid)) {
			return 2;
		}
	}

	new ret = AddItemToContainer(inv_Container[playerid], itemid, playerid, call);

	if(ret > 0) {
		return ret;
	}

	if(call) {
		CallLocalFunction("OnItemAddedToInventory", "dd", playerid, itemid);
	}

	return 0;
}

stock RemoveItemFromInventory(playerid, slotid, call = 1) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	if(call) {
		if(CallLocalFunction("OnItemRemoveFromInventory", "dd", playerid, slotid)) {
			return 0;
		}
	}

	new ret = RemoveItemFromContainer(inv_Container[playerid], slotid, playerid, call);

	if(ret > 0) {
		return ret;
	}

	if(call) {
		CallLocalFunction("OnItemRemovedFromInventory", "dd", playerid, slotid);
	}

	return 1;
}

stock GetInventorySlotItem(playerid, slotid, &itemid) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	return GetContainerSlotItem(inv_Container[playerid], slotid, itemid);
}

stock bool:IsInventorySlotUsed(playerid, slotid) {
	if(!IsPlayerConnected(playerid)) {
		return false;
	}

	return IsContainerSlotUsed(inv_Container[playerid], slotid);
}

stock bool:IsPlayerInventoryFull(playerid) {
	if(!IsPlayerConnected(playerid)) {
		return false;
	}

	return IsContainerFull(inv_Container[playerid]);
}

stock bool:IsPlayerInventoryEmpty(playerid) {
	if(!IsPlayerConnected(playerid)) {
		return false;
	}

	return IsContainerEmpty(inv_Container[playerid]);
}

stock InventoryFitsItemType(playerid, ItemType:itemtype, &bool:result) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	return ContainerFitsItemType(inv_Container[playerid], itemtype, result);
}

stock GetInventoryFreeSlots(playerid, &slots) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	return GetContainerFreeSlots(inv_Container[playerid], slots);
}

stock GetItemInventoryPlayer(itemid, &playerid) {
	if(!IsValidItem(itemid)) {
		return 1;
	}

	new containerid;

	new ret = GetItemContainer(itemid, containerid);

	if(ret != 0) {
		return ret;
	}

	playerid = inv_ContainerPlayer[containerid];
	return 0;
}

stock GetItemPlayerInventorySlot(itemid, &slot) {
	return GetItemContainerSlot(itemid, slot);
}

stock SetPlayerInventorySize(playerid, size) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	return SetContainerSize(inv_Container[playerid], size);
}

stock GetPlayerInventorySize(playerid, &size) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	return GetContainerSize(inv_Container[playerid], size);
}


/*==============================================================================

	Internal Functions and Hooks

==============================================================================*/


_inventory_playerSetup(playerid) {
	if(IsValidContainer(inv_Container[playerid])) {
		err("attempted to create inventory container for player and container already exists",
			_i("playerid", playerid),
			_i("containerid", inv_Container[playerid]));
	}

	new containerid = CreateContainer("Inventory", MAX_INVENTORY_SLOTS);

	if(!IsValidContainer(containerid)) {
		err("failed to create inventory container for player",
			_i("playerid", playerid),
			_i("containerid", containerid));
		
		return;
	}
	
	inv_Container[playerid] = containerid;
	inv_ContainerPlayer[containerid] = playerid;
}

timer _inventory_tearDown[1](playerid) {
	if(!IsValidContainer(inv_Container[playerid])) {
		err("attempted to delete inventory container for player and container does not exist",
			_i("playerid", playerid));
	}

	new ret = DestroyContainer(inv_Container[playerid]);

	if(ret != 0) {
		err("failed to destroy inventory container for player",
			_i("playerid", playerid),
			_i("return", ret));
	}

	inv_ContainerPlayer[inv_Container[playerid]] = INVALID_PLAYER_ID;
	inv_Container[playerid] = INVALID_CONTAINER_ID;
}


hook OnFilterScriptInit() {
	foreach(new i : Player) {
		_inventory_playerSetup(i);
	}
}

hook OnPlayerConnect(playerid) {
	_inventory_playerSetup(playerid);

	return;
}

hook OnPlayerDisconnect(playerid) {
	defer _inventory_tearDown(playerid);
}
