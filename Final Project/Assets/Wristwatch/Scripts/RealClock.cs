//This script uses the system time to position the hands of a clock. You may assign as many or few hands as you wish, but I recommend you use all 3!
//And of course you can use your own watch model, should work with any as long as the hands are
//all at 12:00 when their rotation is zeroed and their local Z axis is perpendicular to the clock face

using UnityEngine;
using System.Collections;
using System;

public class RealClock : MonoBehaviour {
	public Transform Hours;
	public Transform Minutes;
	public Transform Seconds;

	private float hour;
    private float minute;
    private float second;
    private float millisecond;

    // Use this for initialization
    void Start () {
		hour = System.DateTime.Now.Hour;
        minute = System.DateTime.Now.Minute;
        second = System.DateTime.Now.Second;
        millisecond = System.DateTime.Now.Millisecond;
    }
	
	// Update is called once per frame
	void Update () {
        //float hour = System.DateTime.Now.Hour;
        //float minute = System.DateTime.Now.Minute;
        //float second = System.DateTime.Now.Second;
        //float millisecond = System.DateTime.Now.Millisecond;

        //we want these hands to move smoothly, not jump every time the hour/minute changes.
        second += Time.deltaTime;
        minute += Time.deltaTime / 60f;
        hour += Time.deltaTime / 3600f;

        //The second hand is fine jumping, but if you want a smooth rotation on that just uncomment this code:  
        //second = second + millisecond / 1000f;


        if (Hours)
			Hours.localRotation = Quaternion.Euler (0, 0, hour / 12 * 360);

		if(Minutes)
			Minutes.localRotation = Quaternion.Euler (0, 0, minute / 60 * 360);

		if(Seconds)
			Seconds.localRotation = Quaternion.Euler (0, 0, second / 60 * 360);


	}

	public void SetTime(int hour, int minute, int second)
    {
        this.hour = hour;
        this.minute = minute;
        this.second = second;
        if (Hours)
            Hours.localRotation = Quaternion.Euler(0, 0, hour / 12f * 360f);
        if (Minutes)
            Minutes.localRotation = Quaternion.Euler(0, 0, minute / 60f * 360f);
        if (Seconds)
            Seconds.localRotation = Quaternion.Euler(0, 0, second / 60f * 360f);
    }

    public void ResetTime9AM()
    {
        SetTime(9, 0, 0);
    }

    public void ResetTime9AM8Seconds()
    {
        SetTime(9, 0, 8);
    }
}
