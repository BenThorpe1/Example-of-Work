using UnityEngine;

/// <summary>
/// Alters all collision boxes when zoom changes
/// Ben Thorpe
/// </summary>
public class CollisionBalance : MonoBehaviour
{
    //Delegate function
    public delegate void ChangeCollision(float a_value);
    ChangeCollision changeCollision;

    /// <summary>
    /// Inform all box collider holders to alter
    /// </summary>
    /// <param name="a_value"></param>
    public void AlterCollision(float a_value)
    {
        if (changeCollision != null)
        {
            changeCollision(a_value);
        }
    }

    /// <summary>
    /// Adds function to be changed to change collision
    /// </summary>
    /// <param name="a_function"></param>
    public void AddCollisionDelegate(ChangeCollision a_function)
    {
        changeCollision += a_function;
    }

    /// <summary>
    /// Remove function to the change collision delegate
    /// </summary>
    /// <param name="a_function"></param>
    public void RemoveCollisionDelegate(ChangeCollision a_function)
    {
        changeCollision -= a_function;
    }
}

