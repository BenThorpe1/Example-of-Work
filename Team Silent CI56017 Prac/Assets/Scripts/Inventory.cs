using UnityEngine;
using System.Collections.Generic;

/// <summary>
/// Managers player's inventory
/// Ben Thorpe
/// </summary>
public class Inventory : MonoBehaviour
{
    private InventoryUI inventoryUI;
    private List<Item> items;

    /// <summary>
    /// Used for inilization
    /// </summary>
    private void Start()
    {
        inventoryUI = GameObject.FindGameObjectWithTag(GlobalTags.INVENTORY_UI)
           .GetComponent<InventoryUI>();
        items = new List<Item>();
    }

    /// <summary>
    /// Add item to inventory and update UI elements
    /// </summary>
    /// <param name="a_item"></param>
    public void PickUpItem(Item a_item)
    {
        items.Add(a_item);
        inventoryUI.AddItem(a_item);
    }

    /// <summary>
    /// Active item effect
    /// </summary>
    /// <param name="a_item"></param>
    /// <returns></returns>
    public bool UseItem(Item a_item)
    {
        return a_item.ItemEffect();
    }
}

