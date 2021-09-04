using UnityEngine;
using UnityEngine.AI;
using System.Collections.Generic;
/// <summary>
/// Controllers the player
/// Ben Thorpe
/// </summary>
public class PlayerController : MonoBehaviour
{
    private CamerFollow camerFollow;
    //Start yPos, to ensure no changes
    private float yPos;
    //How close the player has to be to be in contack with items
    private const float CONTACT_RANGE = 3.0f;
    private NavMeshAgent agent;
    private List<Transform> collidingObjects;
    private bool isHiding = false;
    private Transform hidingBush;
    [SerializeField]
    private GameObject gameOverCanas;
    [SerializeField]
    private PlayerAttackRange attackRange;

    /// <summary>
    /// Use this for initialization	
    /// </summary>
    private void Start()
    {
        camerFollow = Camera.main.gameObject.GetComponent<CamerFollow>();
        yPos = transform.position.y;
        agent = GetComponent<NavMeshAgent>();
        //Stop rototaion
        agent.updateUpAxis = false;
        collidingObjects = new List<Transform>();
    }

    /// <summary>
    /// Ensure the camera follows the player
    /// </summary>
    private void Update()
    {
        //Move the camera along
        camerFollow.CenterCamera(transform.position);
    }

    /// <summary>
    /// Moves the character in the dirrection of the touch
    /// </summary>
    /// <param name="a_pos"></param>
    public void MoveOnClick(Vector3 a_pos)
    {
        //Calacule new position
        Vector3 target = new Vector3(a_pos.x, yPos, a_pos.z);
        //  Either follow mouse or nav mesh
        agent.SetDestination(target);
        //  transform.position = Vector3.MoveTowards(transform.position, target, movementSpeed * Time.deltaTime);

    }

    /// <summary>
    /// Chech if player is close to position
    /// </summary>
    /// <param name="a_pos"></param>
    /// <returns></returns>
    public bool InRange(Transform transform)
    {
        if (collidingObjects.Contains(transform))
        {
            return true;
        }
        return false;
    }

    /// <summary>
    /// Keep track of all colliding objects
    /// Checks if player is in bushes
    /// </summary>
    /// <param name="other"></param>
    private void OnTriggerEnter(Collider other)
    {
        collidingObjects.Add(other.transform);
        //Automaticall hides player in bush
        if (other.tag == GlobalTags.BUSH)
        {
            isHiding = true;
            hidingBush = other.transform;
        }

    }

    /// <summary>
    ///  Keep track of all colliding objects
    /// Checks if player moved out of the bushes
    /// </summary>
    /// <param name="other"></param>
    private void OnTriggerExit(Collider other)
    {
        collidingObjects.Remove(other.transform);
        //The player is no longer hiding
        if (other.transform == hidingBush)
        {
            isHiding = false;
            hidingBush = null;
        }
    }

    /// <summary>
    /// Determines if player is hidden
    /// </summary>
    /// <returns></returns>
    public bool CheckHidden()
    {
        return isHiding;
    }

    /// <summary>
    /// Kills the player
    /// </summary>
    /// <returns></returns>
    public void KillPlayer()
    {
        Debug.Log("Dead");
        Time.timeScale = 0;
        gameOverCanas.SetActive(true);
    }

    /// <summary>
    /// The player attacks an AI
    /// </summary>
    /// <returns></returns>
    public bool Attack()
    {
       Transform[] enemies = attackRange.GetEnemyInRange();
        if (enemies.Length == 0)
        {
            return false;
        }
        //Kill all enemies
        for(int i = 0; i < enemies.Length; i++)
        {
            enemies[i].GetComponent<Enemy>().KillEnemy();
        }
        return true;
    }
}

