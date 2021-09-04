using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Repersents a Clickable button for inventory
/// Ben Thorpe
/// </summary>
public class ItemUI : MonoBehaviour
{
    private InventoryUI inventoryUI;

    /// <summary>
    /// Used for inilization
    /// </summary>
    void Start()
    {
        inventoryUI = GameObject.FindGameObjectWithTag(GlobalTags.INVENTORY_UI)
            .GetComponent<InventoryUI>();
        Button button = gameObject.GetComponent<Button>();
        button.onClick.AddListener(InfromClick);
    }

    /// <summary>
    /// Infroms the Inventory UI this object has been clicked
    /// </summary>
    private void InfromClick()
    {
        inventoryUI.ActiveButton(gameObject);
    }
 
}
