using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Text;

/// <summary>
/// Displays the text and get input
/// </summary>
public class TextManager : MonoBehaviour
{
    [SerializeField]
    private Text displayTest;
    private string log;
    [SerializeField]
    private TextAdvanture conversation;
    private int lineCount;
    private const int MAX_LINE = 4;
    [SerializeField]
    private bool isMorality;
    private float moralityValue = 0;
    [SerializeField]
    private Slider moralityDisplay;

    public float GetMoralityValue() { return moralityValue; }

    /// <summary>
    /// Used to inilize
    /// </summary>
    void Start()
    {
        AddLine(GetNextLine());
    }

    /// <summary>
    /// Check for input
    /// </summary>
    void Update()
    {
        //One
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            AddLine("1");
            ImpletedOption(1);
        }
        //Two
        if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            AddLine("2");
            ImpletedOption(2);

        }
        //Three
        if (Input.GetKeyDown(KeyCode.Alpha3))
        {
            AddLine("3");
            ImpletedOption(3);

        }
        //Four
        if (Input.GetKeyDown(KeyCode.Alpha4))
        {
            AddLine("4");
            ImpletedOption(4);
        }
    }

    private string GetNextLine()
    {
       TextAdvanture.Node nextNode = conversation.GetNextLine();
             return nextNode.text;
    }

    /// <summary>
    /// Adds a new line to the text;
    /// </summary>
    /// <param name="a_value"></param>
    private void AddLine(string a_value)
    {
         
        StringBuilder sb = new StringBuilder(log);
       sb.AppendLine(a_value);
        lineCount++;
        //Remove oldest line from log
        if (lineCount >= MAX_LINE)
        {       
            sb.Remove(0, sb.ToString().IndexOf("\n") +1);
            lineCount--;
        }
        log = sb.ToString();
        displayTest.text = log;
    }

    private void ImpletedOption(int a_i)
    {
        if (conversation.MoveStory(a_i - 1))
        {
            if (isMorality)
            {
                UpdateMorality(conversation.GetOpionMorality(a_i-1));

            }
            AddLine(GetNextLine());
        }
    }

    private void UpdateMorality(float a_i)
    {
        moralityValue += a_i;
        moralityDisplay.value = moralityValue;
    }
}
