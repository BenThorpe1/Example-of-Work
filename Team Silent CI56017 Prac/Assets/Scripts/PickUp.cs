using UnityEngine;

/// <summary>
/// Repersent an gameobject that can be picked up
/// Ben Thorpe
/// </summary>
public class PickUp : MonoBehaviour
{
    protected BoxCollider boxCollider;
    protected Vector3 defaultColliderSize;

    /// <summary>
    /// Used to inilise varaibles
    /// </summary>
    protected virtual void Start()
    {
       boxCollider = gameObject.GetComponent<BoxCollider>();
        defaultColliderSize = boxCollider.size;

        GameObject balancer = GameObject.FindGameObjectWithTag(GlobalTags.BALANCE);
        if (balancer != null)
        {
            CollisionBalance deleget = balancer.GetComponent<CollisionBalance>();
            deleget.AddCollisionDelegate(ChangeCollision);
        }
    }

    /// <summary>
    /// Removes function form delegate
    /// </summary>
    protected virtual void OnDestroy()
    {
        GameObject balancer = GameObject.FindGameObjectWithTag(GlobalTags.BALANCE);
        if (balancer != null)
        {
            CollisionBalance deleget = balancer.GetComponent<CollisionBalance>();
            deleget.RemoveCollisionDelegate(ChangeCollision);
        }
    }

    /// <summary>
    /// Destroy the friut if player is in ranged
    /// </summary>
    public virtual void Touched()
    {
        //Chech if player is in range
        if (IsPlayerInRange())
        {
            //Inform the inventory manger to pick this item
            Inventory inventory = GameObject.FindGameObjectWithTag(GlobalTags.INVENTORY)
                .GetComponent<Inventory>();
            inventory.PickUpItem(gameObject.GetComponent<Item>());
            Destroy(gameObject);
        }
    }

    /// <summary>
    /// Check if player is in range of item
    /// </summary>
    /// <returns></returns>
    protected bool IsPlayerInRange()
    {

        PlayerController player = GameObject.FindGameObjectWithTag(GlobalTags.PLAYER)
                 .GetComponent<PlayerController>();

        //Chech if player is in range
        if (player != null)
        {
            if (player.InRange(transform))
            {
                return true;
            }
        }
        return false;
    }

    /// <summary>
    /// Change collider size
    /// </summary>
    /// <param name="a_value"></param>
    protected virtual void ChangeCollision(float a_value)
    {
        Vector3 newSize = defaultColliderSize * a_value;
        //Alter collider size, can't go smaller then default size
        if (newSize.sqrMagnitude < defaultColliderSize.sqrMagnitude)
        {
            boxCollider.size = defaultColliderSize;
        }
        else
        {
            boxCollider.size = newSize;
        }
    }

}
