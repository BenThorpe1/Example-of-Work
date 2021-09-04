using UnityEngine;

/// <summary>
/// Handlers all touch input
/// Ben Thorpe
/// </summary>
public class TouchInputHandler : MonoBehaviour
{
   
    //Distance from camera to game level
    private const float RAY_DISTANCE = 90.0f;
    //The Y pos of cursors
    private const float Y_DISTANCE = 0.3f;
    [SerializeField]
    private LayerMask PLAYER_LAYER_MASK;
    [SerializeField]
    private LayerMask WALL_LAYER_MASK;
    //Only one touch is registered
    private int registerTouch;
    private Vector3 defaultCursorSize;
    private PlayerController playerController;
    [SerializeField]
    private GameObject cursor;

    /// <summary>
    /// Checks for correct platform
    /// </summary>
   private void Start()
    {
        #region  //Check for correct platform Game doesn't work without ISO or Android
#if UNITY_IOS || UNITY_ANDROID  
                 Debug.Log("You are using IOS or Andriod");
       
#elif UNITY_EDITOR
         if (UnityEditor.EditorApplication.isRemoteConnected)
        {
            Debug.Log("You are using IOS or Andriod");
        }
        else
        {
        Debug.Log("This project is tested using an ISO phone and should be played with eith ISO or android \n Unity Remove has a small delay and is not detected just yet, A following message is displayed with remote is conntecded");
}
#endif
        #endregion

        //Iniliaze variables
        registerTouch = -1;
        playerController = GameObject.FindGameObjectWithTag(GlobalTags.PLAYER)
            .GetComponent<PlayerController>();
        defaultCursorSize = cursor.transform.localScale;
        GameObject balancer = GameObject.FindGameObjectWithTag(GlobalTags.BALANCE);
        if (balancer != null)
        {
            CollisionBalance deleget = balancer.GetComponent<CollisionBalance>();
            deleget.AddCollisionDelegate(ChangeCursor);
        }
    }

    /// <summary>
    /// Removes the delage funtion
    /// </summary>
    private void OnDestroy()
    {
        GameObject balancer = GameObject.FindGameObjectWithTag(GlobalTags.BALANCE);
        if (balancer != null)
        {
            CollisionBalance deleget = balancer.GetComponent<CollisionBalance>();
            deleget.RemoveCollisionDelegate(ChangeCursor);
        }
    }

    /// <summary>
    /// Change the cursor size to be easily seen
    /// </summary>
    /// <param name="a_value"></param>
    public void ChangeCursor(float a_value)
    {
        cursor.transform.localScale = defaultCursorSize * a_value;
    }


    /// <summary>
    /// Main loop for physics updates 
    /// </summary>
    void FixedUpdate()
    {
        UpdateTouches();
    }

    /// <summary>
    ///Gets the touch inputs
    /// </summary>
    private void UpdateTouches()
    {
        //Chech which platform
#if UNITY_IOS || UNITY_ANDROID
        MobileCode();
        //Allow for click with mouse
        if (Input.GetMouseButton(0))
        {
            CheckCollision(Input.mousePosition);
        }
#elif UNITY_EDITOR

        if (UnityEditor.EditorApplication.isRemoteConnected)
        {
            Debug.Log("You are using IOS or Andriod");
            MobileCode();
        }
        //Allow for click with mouse
        if (Input.GetMouseButton(0))
        {
            CheckCollision(Input.mousePosition);
        }
#elif UNITY_STANDALONE_WIN

        //Allow for click with mouse
        if (Input.GetMouseButton(0))
        {
            CheckCollision(Input.mousePosition);
        }



#endif
    }

    /// <summary>
    /// Input code for mobile
    /// </summary>
    private void MobileCode()
    {
        //Get touches
        Touch[] touches = Input.touches;
        if (touches.Length > 0)
        {
            //Chech touch phase
            foreach (Touch touch in touches)
            {

                switch (touch.phase)
                {
                    case TouchPhase.Began:
                        {
                            //Update regiestered touch
                            if (registerTouch == -1)
                            {
                                registerTouch = touch.fingerId;
                            }
                            //Only one touch is registered
                            else if (touch.fingerId != registerTouch)
                            {
                                continue;
                            }

                            CheckCollision(touch.position);
                            break;
                        }
                    case TouchPhase.Moved:
                        {
                            if (touch.fingerId == registerTouch)
                            {
                                CheckCollision(touch.position);
                            }
                            break;
                        }
                    case TouchPhase.Stationary:
                        {
                            if (touch.fingerId == registerTouch)
                            {
                                CheckCollision(touch.position);
                            }
                            break;
                        }
                    case TouchPhase.Ended:
                        {
                            //Only one touch is registered
                            if (touch.fingerId == registerTouch)
                            {
                                registerTouch = FindFirstAvaliabeTouch(touches);
                            }
                            break;
                        }
                    default:
                        {
                            //Do nothing
                            break;
                        }

                }
            }

        }
    }

    /// <summary>
    /// Chech if touch collise with anything
    /// </summary>
    /// <param name="a_pos"></param>
    private void CheckCollision(Vector2 a_pos)
    {
        //Create phyics ray
        Ray ray = Camera.main.ScreenPointToRay(a_pos);
        RaycastHit hit;
        if (Physics.Raycast(ray, out hit, RAY_DISTANCE, ~(PLAYER_LAYER_MASK | WALL_LAYER_MASK)))
        {
            cursor.transform.position = new Vector3(hit.point.x, Y_DISTANCE, hit.point.z);
            //Determine which object was touched
            switch (hit.transform.tag)
            {
                case GlobalTags.LEVEL:
                    {
                        playerController.MoveOnClick(hit.point);
                        break;
                    }
                case GlobalTags.PICK_UP:
                    {
                        playerController.MoveOnClick(hit.point);
                        hit.transform.gameObject.GetComponent<PickUp>().Touched();
                        break;
                    }
                case GlobalTags.BUSH:
                    {
                        playerController.MoveOnClick(hit.point);
                        break;
                    }
                default:
                    {
                        //Do nothing
                        break;
                    }
            }
        }
    }


    /// <summary>
    /// Finds the first touch that is avaliable
    /// </summary>
    /// <param name="a_touches"></param>
    /// <returns></returns>
    private int FindFirstAvaliabeTouch(Touch[] a_touches)
    {
        foreach (Touch touch in a_touches)
        {
            if (touch.phase != TouchPhase.Canceled && touch.phase != TouchPhase.Ended)
            {
                return touch.fingerId;
            }
        }
        return -1;
    }
}
