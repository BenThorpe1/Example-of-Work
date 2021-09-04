using UnityEngine;

/// <summary>
/// Repersent a fruit that the player can interactive with to earn points
/// Ben Thorpe
/// </summary>
public class Friut : PickUp
{
    [SerializeField]
    private float score;

    /// <summary>
    /// Destroy the friut if player is in ranged
    /// </summary>
    public override void Touched()
    {
        if (IsPlayerInRange())
        {
            GameObject.FindGameObjectWithTag(GlobalTags.SCORE_MANAGER)
                .GetComponent<ScoreManager>().AddScore(score);
            Destroy(gameObject);
        }
    }


}