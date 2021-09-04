using UnityEngine;

/// <summary>
/// Repersent a useable and pickable item
/// Ben Thorpe
/// </summary>
public class Item : MonoBehaviour
{
    [SerializeField]
    private string itemName;
    [SerializeField]
    private Sprite itemSprite;
    [SerializeField]
    

   public virtual bool ItemEffect()
    {
        return true;
    }

    #region Getters and Setters
    public string GetName() { return itemName; }
    public Sprite GetSprite() { return itemSprite; }
    #endregion


}
