using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Controllers the players inventory UI
/// Ben Thorpe
/// </summary>
public class InventoryUI : MonoBehaviour
{
    //Inventory manager
    private Inventory inventory;
    //Button UI for item
    [SerializeField]
    private GameObject uiPrefab;
    private List<ItemUI> displyItems;

    /// <summary>
    /// Used for iniliszeation
    /// </summary>
    private void Start()
    {
        inventory = GameObject.FindGameObjectWithTag(GlobalTags.INVENTORY)
            .GetComponent<Inventory>();
        displyItems = new List<ItemUI>();
    }

    /// <summary>
    /// Added a new item UI element
    /// </summary>
    /// <param name="a_item"></param>
    public void AddItem(Item a_item)
    {
        ItemUI newItemUI = new ItemUI();
        newItemUI.item = a_item;
        newItemUI.display = CreateUIObject(a_item.GetSprite());
        displyItems.Add(newItemUI);
    }

    /// <summary>
    /// Creates a new UI object
    /// </summary>
    /// <param name="a_sprite"></param>
    /// <returns></returns>
    private GameObject CreateUIObject(Sprite a_sprite)
    {
        GameObject newSrpite = Instantiate(uiPrefab);
        newSrpite.transform.SetParent(transform);
        newSrpite.GetComponent<Image>().sprite = a_sprite;
        return newSrpite;
    }

    /// <summary>
    /// The button has been activated
    /// </summary>
    /// <param name="a_gameObject"></param>
    public void ActiveButton(GameObject a_gameObject)
    {
     //Find with item was clicked
     foreach (ItemUI currentItem in displyItems)
        {
            if (currentItem.display == a_gameObject)
            {
                //Item was used
                if (inventory.UseItem(currentItem.item))
                {
                    displyItems.Remove(currentItem);
                    Destroy(currentItem.display);
                    break;
                }
            }
        }
    }

    /// <summary>
    /// Repersent an item UI element
    /// </summary>
    struct ItemUI
    {
        public Item item;
        public GameObject display;
    }

}
