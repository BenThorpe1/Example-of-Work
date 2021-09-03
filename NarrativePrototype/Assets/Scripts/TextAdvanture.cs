using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TextAdvanture : MonoBehaviour
{
    protected List<Node> conversation;
    protected int index = 0;
    protected const int MAX_OPTION = 4;
    protected float previousMorality;

    public Node GetNextLine()
    {
        return conversation[index];
    }
    public float GetOpionMorality(int a_i)
    {
        return previousMorality;
    }

    public virtual bool MoveStory(int a_i)
    {
        bool advanced = false;
        Node currentNode = conversation[index];
        if (currentNode.options[a_i] != 0)
        {
            advanced = true;
            previousMorality = currentNode.morality[a_i];
            index = currentNode.options[a_i];
        }
        return advanced;
    }
    
    protected Node CreateNode(string a_text, int a_1 = 0, int a_2 = 0, int a_3 = 0, int a_4 = 0
           , float a_m1 = 0, float a_m2 = 0, float a_m3 = 0, float a_m4 = 0,bool a_b =false)
    {
        Node first = new Node();
        first.text = a_text;
        first.options = new int[MAX_OPTION];
        first.morality = new float[MAX_OPTION];
        first.options[0] = a_1;
        first.options[1] = a_2;
        first.options[2] = a_3;
        first.options[3] = a_4;
        first.morality[0] = a_m1;
        first.morality[1] = a_m2;
        first.morality[2] = a_m3;
        first.morality[3] = a_m4;
        first.branchValue = a_b;
        return first;
    }

    public struct Node
    {
        public string text;
        public int[] options;
        public float[] morality;
        public bool branchValue;
    };

}
