using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AddParent : MonoBehaviour
{
    public void SetParent(Transform ParentTransform)
    {
        // Check if the parent transform is not null
        if (ParentTransform != null)
        {
            Vector3 worldPos = transform.position;
            Quaternion worldRot = transform.rotation;

            // Set the parent of the current object's transform
            this.transform.SetParent(ParentTransform, false);

            Debug.Log("Parent set to: " + ParentTransform.name);
            Debug.Log("Position: " + transform.position);
            Debug.Log("Rotation: " + transform.rotation);
            // Log the world position and rotation before changing the parent
            Debug.Log("World Position: " + worldPos);
            Debug.Log("World Rotation: " + worldRot);

            // Reset the local position and rotation to maintain the world position and rotation
            transform.position = worldPos - ParentTransform.position;
            transform.rotation = worldRot * Quaternion.Inverse(ParentTransform.rotation);
        }
        else
        {
            Debug.LogWarning("Parent Transform is null. Cannot set parent.");
        }
    }

    public void SetParent2Null()
    {
        // Set the parent of the current object's transform to null
        this.transform.SetParent(null, false);
        Debug.Log("Parent set to null.");

        this.transform.position = Vector3.zero;
        this.transform.rotation = Quaternion.identity;
        this.transform.localScale = Vector3.one;
    }
}
