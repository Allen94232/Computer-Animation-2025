#include "simulation/kinematics.h"

#include <iostream>
#include "Eigen/Dense"
#include "acclaim/bone.h"
#include "util/helper.h"

namespace kinematics {

void forwardSolver(const acclaim::Posture& posture, acclaim::Bone* bone) {
    // TODO#1: Forward Kinematic
    // Hint:
    // - Traverse the skeleton tree from root to leaves.
    // - Compute each bone's global rotation and global position.
    // - Use local rotation (from posture) and bone hierarchy (parent rotation, offset, etc).
    // - Remember to update both bone->start_position and bone->end_position.
    // - Use bone->rotation to store global rotation (after combining parent, local, etc).

    if (bone == nullptr) return;

    if (bone->parent == nullptr) {
        // root bone

        bone->rotation = bone->rot_parent_current * util::rotateDegreeZYX(posture.bone_rotations[bone->idx]);

        bone->start_position = posture.bone_translations[bone->idx];
        bone->start_position[3] = 1.0;
    } 
    else
    {
        // child bone
        bone->rotation = bone->parent->rotation * bone->rot_parent_current * util::rotateDegreeZYX(posture.bone_rotations[bone->idx]);

        bone->start_position = bone->parent->end_position;
        bone->start_position[3] = 1.0;
    }

    // calculate end position
    Eigen::Vector4d dir_normalized = bone->dir.normalized();
    dir_normalized[3] = 0.0;
    bone->end_position = bone->start_position + bone->rotation * (dir_normalized * bone->length);

    // DFS
    if (bone->child != nullptr)
    {
        forwardSolver(posture, bone->child);
    }
    if (bone->sibling != nullptr) 
    {
        forwardSolver(posture, bone->sibling);
    }
 }

Eigen::VectorXd pseudoInverseLinearSolver(const Eigen::Matrix4Xd& Jacobian, const Eigen::Vector4d& target) {
    Eigen::VectorXd deltatheta;
    // TODO#2: Inverse linear solver (find x which min(| jacobian * x - target |))
    // Hint:
    //   1. Linear algebra - least squares solution
    //   2. https://en.wikipedia.org/wiki/Moore%E2%80%93Penrose_inverse#Construction
    // Note:
    //   1. SVD or other pseudo-inverse method is useful
    //   2. Some of them have some limitation, if you use that method you should check it.

    if (Jacobian.cols() == 0) {
        deltatheta = Eigen::VectorXd::Zero(Jacobian.rows());
        return deltatheta;
    }

    // Use SVD to solve least squares (pseudo-inverse)
    Eigen::JacobiSVD<Eigen::Matrix4Xd> svd(Jacobian, Eigen::ComputeThinU | Eigen::ComputeThinV);
    deltatheta = svd.solve(target);

    return deltatheta;
}

/**
 * @brief Perform inverse kinematics (IK)
 *
 * @param target_pos The position where `end_bone` will move to.
 * @param obs_pos The position where the obstacle is at
 * @param obsActive Whether the obstacle is active or not
 * @param start_bone This bone is the last bone you can move while doing IK
 * @param end_bone This bone will try to reach `target_pos`
 * @param posture The original AMC motion's reference, you need to modify this
 *
 * @return True if IK is stable (HW2 bonus)
 */
bool inverseJacobianIKSolver(const Eigen::Vector4d& target_pos, const Eigen::Vector4d& obs_pos, bool obsActive,
                             acclaim::Bone* start_bone, acclaim::Bone* end_bone, acclaim::Posture& posture) {
    constexpr int max_iteration = 1000;
    constexpr double epsilon = 1E-3;
    constexpr double step = 0.1;
    constexpr double obsAvoidThreshold = 1.01;  // if bone is within 1 unit from obstacle

    // Since bone stores in bones[i] that i == bone->idx, we can use bone - bone->idx to find bones[0] which is the
    // root.
    acclaim::Bone* root_bone = start_bone - start_bone->idx;

    // TODO#3:
    // Perform inverse kinematics (IK)
    // HINTs will tell you what should do in that area.
    // Of course you can ignore it (Any code below this line) and write your own code.
    acclaim::Posture original_posture(posture);

    size_t bone_num = 0;
    std::vector<acclaim::Bone*> boneList;
    acclaim::Bone* current = end_bone;
    // TODO#3-1:
    // Calculate number of bones need to move to perform IK, store in `bone_num`
    // (a.k.a. how may bones from end_bone to its parent than to start_bone (include both side))
    // Store the bones need to move to perform IK into boneList
    // Hint:
    //   1. Traverse from end_bone to start_bone is easier than start to end (since there is only 1 parent)
    //   2. If start bone is not reachable from end. Go to root first.
    // Note:
    //   1. Both start and end should be in the list

    while (current != nullptr) {
        boneList.push_back(current);
        if (current == start_bone) {
            break;
        }
        current = current->parent;
    }

    if (boneList.back() != start_bone) {
        current = boneList.back();
        while (current->parent != nullptr) {
            current = current->parent;
            boneList.push_back(current);
        }
    }

    bone_num = boneList.size();

    for (int iter = 0; iter < max_iteration; ++iter) {
        forwardSolver(posture, root_bone);
        Eigen::Vector4d desiredVector = target_pos - end_bone->end_position;

        if (desiredVector.head<3>().norm() < epsilon) {
            break;
        }
        // TODO#3-2 (compute jacobian)
        //   1. Compute arm vectors
        //   2. Compute jacobian columns, store in `Jacobian`
        // Hint:
        //   1. You should not put rotation in jacobian if it doesn't have that DoF.
        //   2. jacobian.col(/* some column index */) = /* jacobian column */

        Eigen::Matrix4Xd Jacobian(4, 3 * bone_num);
        Jacobian.setZero();

        for (rsize_t i = 0; i < bone_num; ++i) {
            acclaim::Bone* bone = boneList[i];

            Eigen::Vector4d bone_pos = bone->start_position;
            Eigen::Vector4d bone_to_effector = end_bone->end_position - bone_pos;

            Eigen::Matrix3d rotation = bone->rotation.linear();
            
            if (bone->dofrx) 
            {
                Eigen::Vector3d axis = rotation * Eigen::Vector3d::UnitX();
                Eigen::Vector3d J = axis.cross(bone_to_effector.head<3>());
                Jacobian.block<3, 1>(0, i * 3) = J;
            }

            if (bone->dofry) {
                Eigen::Vector3d axis = rotation * Eigen::Vector3d::UnitY();
                Eigen::Vector3d J = axis.cross(bone_to_effector.head<3>());
                Jacobian.block<3, 1>(0, i * 3 + 1) = J;
            }

            if (bone->dofrz) 
            {
                Eigen::Vector3d axis = rotation * Eigen::Vector3d::UnitZ();
                Eigen::Vector3d J = axis.cross(bone_to_effector.head<3>());
                Jacobian.block<3, 1>(0, i * 3 + 2) = J;
            }

        }
        // TODO#3-3 (obstacle avoidance)
        //  1. Iterate through all bones in `boneList`.
        //  2. Compute the center of each bone (average of start and end positions).
        //  3. Calculate the vector from obstacle center to bone center.
        //  4. If distance is below threshold, compute repulsive vector.
        //  5. Add this repulsive vector to `desiredVector`.
        // Hint:
        // - Use a constant threshold distance to determine proximity.
        // - The repulsive vector should point away from the obstacle.
        // - Use `.head<3>().norm()` to compute 3D distance from a 4D vector.
        // - Normalize the repulsive vector and scale it based on how close it is.
        if (obsActive) {
            const double cubeHalfSize = 0.25;
            const double threshold = obsAvoidThreshold;

            for (size_t i = 0; i < bone_num; ++i) {
                Eigen::Vector4d bone_center = (boneList[i]->start_position + boneList[i]->end_position) / 2.0;
                Eigen::Vector3d obsDiff = (bone_center - obs_pos).head<3>();

                Eigen::Vector3d clampedDiff = obsDiff;

                clampedDiff.x() = std::max(std::min(clampedDiff.x(), cubeHalfSize), -cubeHalfSize);
                clampedDiff.y() = std::max(std::min(clampedDiff.y(), cubeHalfSize), -cubeHalfSize);
                clampedDiff.z() = std::max(std::min(clampedDiff.z(), cubeHalfSize), -cubeHalfSize);

                Eigen::Vector3d nearestSurface = obs_pos.head<3>() + clampedDiff;
                Eigen::Vector3d boneToSurface = bone_center.head<3>() - nearestSurface;

                double distance = boneToSurface.norm();

                if (distance < threshold) {
                    Eigen::Vector3d repulse = boneToSurface.normalized() * (threshold - distance);
                    desiredVector.head<3>() += repulse;
                }
            }
        }

        Eigen::VectorXd deltatheta = step * pseudoInverseLinearSolver(Jacobian, desiredVector);
        // TODO#3-4 (update rotation)
        //   Update `posture.bone_rotation` (in euler angle / degrees) using deltaTheta
        // Hint:
        //   1. You can ignore rotation limit of the bone.
        // Bonus:
        //   1. You cannot ignore rotation limit of the bone.

        double M_PI = 3.14159265358979323846;

        for (size_t i = 0; i < bone_num; ++i) {
            if (boneList[i]->dofrx)
                posture.bone_rotations[boneList[i]->idx].x() += deltatheta(i * 3 + 0) * 180.0 / M_PI;
            if (boneList[i]->dofry)
                posture.bone_rotations[boneList[i]->idx].y() += deltatheta(i * 3 + 1) * 180.0 / M_PI;
            if (boneList[i]->dofrz)
                posture.bone_rotations[boneList[i]->idx].z() += deltatheta(i * 3 + 2) * 180.0 / M_PI;
        }

        for (size_t i = 0; i < bone_num; ++i) {
            posture.bone_rotations[boneList[i]->idx].x() = std::clamp(
                posture.bone_rotations[boneList[i]->idx].x(), (double)boneList[i]->rxmin, (double)boneList[i]->rxmax);
            posture.bone_rotations[boneList[i]->idx].y() = std::clamp(
                posture.bone_rotations[boneList[i]->idx].y(), (double)boneList[i]->rymin, (double)boneList[i]->rymax);
            posture.bone_rotations[boneList[i]->idx].z() = std::clamp(
                posture.bone_rotations[boneList[i]->idx].z(), (double)boneList[i]->rzmin, (double)boneList[i]->rzmax);
        }
    }
    // TODO#3-5
    // Return whether IK is stable
    // i.e. whether the ball is reachable
    // Hint:
    //      1. comment out the line here and return whether the IK is stable or not
    //      2. if the ball is reachable,  swinging its hand in air

    forwardSolver(posture, root_bone);
    double final_error = (target_pos - end_bone->end_position).head<3>().norm();

    if (final_error < epsilon) {
        return true;
    } else {
        posture = original_posture;
        forwardSolver(posture, root_bone);
        return false;
    }
}

}  // namespace kinematics
