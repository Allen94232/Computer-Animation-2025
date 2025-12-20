using ReadyPlayerMe.Core;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FacialExpressionController : MonoBehaviour
{
    public SkinnedMeshRenderer skinnedMeshRenderer;

    void Start()
    {
        // Check if the SkinnedMeshRenderer is found
        if (skinnedMeshRenderer == null)
        {
            Debug.LogError("SkinnedMeshRenderer not found in the avatar's GameObject.");
            return;
        }
    }

    private IEnumerator AnimateBlendShape(string shapeName, float targetWeight, float duration)
    {
        int index = skinnedMeshRenderer.sharedMesh.GetBlendShapeIndex(shapeName);
        if (index < 0)
        {
            Debug.LogWarning($"BlendShape '{shapeName}' not found.");
            yield break;
        }

        float startWeight = skinnedMeshRenderer.GetBlendShapeWeight(index);
        float elapsed = 0f;

        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            float t = elapsed / duration;
            float newWeight = Mathf.Lerp(startWeight, targetWeight, t);
            skinnedMeshRenderer.SetBlendShapeWeight(index, newWeight);
            yield return null;
        }

        // Ensure it ends at the target
        skinnedMeshRenderer.SetBlendShapeWeight(index, targetWeight);
    }

    public void ResetAllBlendShapes(float transitionTime = 1f)
    {
        Mesh mesh = skinnedMeshRenderer.sharedMesh;
        for (int i = 0; i < mesh.blendShapeCount; i++)
        {
            string shapeName = mesh.GetBlendShapeName(i);
            StartCoroutine(AnimateBlendShape(shapeName, 0f, transitionTime));
        }
    }

    public void SetConfusedExpression(float transitionTime = 0.5f)
    {
        // Check if the SkinnedMeshRenderer is found
        if (skinnedMeshRenderer == null)
        {
            Debug.LogError("SkinnedMeshRenderer not found in the avatar's GameObject.");
            return;
        }

        // Set the blend shape weight for the confused expression
        StartCoroutine(AnimateBlendShape("browDownLeft", 1f, transitionTime));
        StartCoroutine(AnimateBlendShape("browDownRight", 1f, transitionTime));
        StartCoroutine(AnimateBlendShape("browOuterUpLeft", 1f, transitionTime));
        StartCoroutine(AnimateBlendShape("browOuterUpRight", 1f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthFrownLeft", 0.5f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthFrownRight", 0.5f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthStretchLeft", 0.5f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthStretchRight", 0.5f, transitionTime));
    }

    public void SetAnxietyExpression(float transitionTime = 0.5f)
    {
        // Check if the SkinnedMeshRenderer is found
        if (skinnedMeshRenderer == null)
        {
            Debug.LogError("SkinnedMeshRenderer not found in the avatar's GameObject.");
            return;
        }
        // Set the blend shape weight for the anxiety expression
        StartCoroutine(AnimateBlendShape("browDownLeft", 1f, transitionTime));
        StartCoroutine(AnimateBlendShape("browDownRight", 1f, transitionTime));
        StartCoroutine(AnimateBlendShape("browOuterUpLeft", 1f, transitionTime));
        StartCoroutine(AnimateBlendShape("browOuterUpRight", 1f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthFrownLeft", 0.5f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthFrownRight", 0.5f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthStretchLeft", 0.7f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthStretchRight", 0.7f, transitionTime));
        StartCoroutine(AnimateBlendShape("noseSneerLeft", 0.7f, transitionTime));
        StartCoroutine(AnimateBlendShape("noseSneerRight", 0.7f, transitionTime));
    }

    public void SetHappyExpression(float transitionTime = 0.5f)
    {
        // Check if the SkinnedMeshRenderer is found
        if (skinnedMeshRenderer == null)
        {
            Debug.LogError("SkinnedMeshRenderer not found in the avatar's GameObject.");
            return;
        }
        // Set the blend shape weight for the happy expression
        StartCoroutine(AnimateBlendShape("mouthSmile", 0.7f, transitionTime));
        StartCoroutine(AnimateBlendShape("cheekSquintLeft", 0.7f, transitionTime));
        StartCoroutine(AnimateBlendShape("cheekSquintRight", 0.7f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthDimpleLeft", 0.7f, transitionTime));
        StartCoroutine(AnimateBlendShape("mouthDimpleRight", 0.7f, transitionTime));
    }
}
