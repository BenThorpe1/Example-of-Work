using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

/// <summary>
/// Repersents the base enemy class
/// Ben Thorpe
/// </summary>
public class Enemy : MonoBehaviour
{

    private ENEMY_STATE state;
    [SerializeField]
    private Pathing pathing;
    private NavMeshAgent agent;
    private Vector3 lastPlayerPos;
    //How close to the last player position the AI will travel to
    private const float DISTANCE_TO_PLAYER = 2f;

    /// <summary>
    /// Used to inilize
    /// </summary>
    protected virtual void Start()
    {
        agent = GetComponent<NavMeshAgent>();
        state = new ENEMY_STATE();
        state = ENEMY_STATE.PARTOL;
        agent.SetDestination(pathing.getNextPoint());
        //Stop rototaion
          agent.updateUpAxis = false;
       // agent.updateRotation = false;
    }

    /// <summary>
    /// Main logic loop
    /// </summary>
    protected virtual void Update()
    {
        switch (state)
        {
            case ENEMY_STATE.PARTOL:
                {
                    Partol();
                    break;
                }
            case ENEMY_STATE.ATTACKING:
                {
                    Attack();
                    break;
                }
            default:
                {
                    //Do nothing
                    break;
                }
        }
    }

    /// <summary>
    /// The AI is seeking and attacking player
    /// </summary>
    protected virtual void Attack()
    {
    //Check if AI is at players last position
    if (agent.remainingDistance <DISTANCE_TO_PLAYER)
        {
            state = ENEMY_STATE.PARTOL;
        }
    }

    /// <summary>
    /// Sets the AI to move along set path
    /// </summary>
    protected virtual void Partol()
    {
        if (agent.remainingDistance <= agent.stoppingDistance)
        {
            //Get new position
            agent.SetDestination(pathing.getNextPoint());
        }
    }

    /// <summary>
    /// Draws the partol line when selected
    /// </summary>
    public void OnDrawGizmosSelected()
    {
        if (pathing != null)
        {
            pathing.DrawPath();
        }
    }

    /// <summary>
    /// Inform the AI of the player's position
    /// </summary>
    /// <param name="a_pos"></param>
    public void SpotPlayer(Vector3 a_pos)
    {
        lastPlayerPos = a_pos;
        if (state != ENEMY_STATE.ATTACKING)
        {
            state = ENEMY_STATE.ATTACKING;
        }
        agent.SetDestination(lastPlayerPos);
    }

    /// <summary>
    /// Infrom the AI that the player is in attack range
    /// </summary>
    public void PlayerInRange()
    {
        PlayerController player = GameObject.FindGameObjectWithTag(GlobalTags.PLAYER).GetComponent<PlayerController>();
        player.KillPlayer();
    }

    /// <summary>
    /// Detory this AI
    /// </summary>
    public void KillEnemy()
    {
        Destroy(gameObject);
    }

    /// <summary>
    /// The state of the AI
    /// </summary>
    protected enum ENEMY_STATE
    {
        //AI is moving along the set path
        PARTOL,
        //AI is attacking the player
        ATTACKING
    };


}
