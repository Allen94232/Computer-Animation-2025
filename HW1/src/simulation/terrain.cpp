#include "terrain.h"

#include <stdexcept>

#include "../util/helper.h"
#include <iostream>

namespace simulation {
// Factory
std::unique_ptr<Terrain> TerrainFactory::CreateTerrain(TerrainType type) {
    switch (type) {
        case simulation::TerrainType::Plane:
            return std::make_unique<PlaneTerrain>();
        case simulation::TerrainType::Elevator:
            return std::make_unique<ElevatorTerrain>();

        default:
            throw std::invalid_argument("TerrainFactory::CreateTerrain : invalid TerrainType");
            break;
    }
    return nullptr;
}
// Terrain

Eigen::Matrix4f Terrain::getModelMatrix() { return modelMatrix; }

float Terrain::getMass() const { return mass; }

Eigen::Vector3f Terrain::getPosition() const { return position; }

Eigen::Vector3f Terrain::getVelocity() const { return velocity; }

Eigen::Vector3f Terrain::getAcceleration() const { return force / mass; }

Eigen::Vector3f Terrain::getForce() const { return force; }

void Terrain::setMass(const float _mass) { mass = _mass; }

void Terrain::setPosition(const Eigen::Vector3f& _position) { 
    modelMatrix = util::translate(_position - position) * modelMatrix;
    position = _position;
}

void Terrain::setVelocity(const Eigen::Vector3f& _velocity) { velocity = _velocity; }

void Terrain::setAcceleration(const Eigen::Vector3f& _acceleration) { force = _acceleration * mass; }

void Terrain::setForce(const Eigen::Vector3f& _force) { force = _force; }

void Terrain::addPosition(const Eigen::Vector3f& _position) { 
    position += _position;
    modelMatrix = util::translate(_position) * modelMatrix;
}

void Terrain::addVelocity(const Eigen::Vector3f& _velocity) { velocity += _velocity; }

void Terrain::addAcceleration(const Eigen::Vector3f& _acceleration) { force += _acceleration * mass; }

void Terrain::addForce(const Eigen::Vector3f& _force) { force += _force; }

// Note:
// You should update each particles' velocity (base on the equation in
// slide) and force (contact force : resist + friction) in handleCollision function

// PlaneTerrain //

PlaneTerrain::PlaneTerrain() { reset(); }

void PlaneTerrain::reset() { 
    modelMatrix = util::translate(0.0f, position[1], 0.0f) * util::rotateDegree(0, 0, -20) * util::scale(30, 1, 30);
}


TerrainType PlaneTerrain::getType() { return TerrainType::Plane; }

void PlaneTerrain::handleCollision(const float delta_T, Jelly& jelly) {
    constexpr float eEPSILON = 0.01f;
    constexpr float coefResist = 0.8f;
    constexpr float coefFriction = 0.5f;
    // TODO#3-1: Handle collision when a particle collide with the plane terrain.
    //   If collision happens:
    //      1. Directly update particles' velocity
    //      2. Apply contact force to particles when needed
    // Note:
    //   1. There are `jelly.getParticleNum()` particles.
    //   2. See TODOs in `Jelly::computeInternalForce` and functions in `particle.h` if you don't know how to access
    //   data.
    //   3. The plane spans 30x30 units in the XZ plane and is rotated -20 degrees around the Z-axis.
    // Hint:
    //   1. Review "particles.pptx" from p.14 - p.19
    //   1. Use a.norm() to get length of a.
    //   2. Use a.normalize() to normalize a inplace.
    //          a.normalized() will create a new vector.
    //   3. Use a.dot(b) to get dot product of a and b.

    float M_PI = 3.14159265358979323846;

    float angleRad = 20.0f * M_PI / 180.0f;
    Eigen::Vector3f normal(sin(angleRad), cos(angleRad), 0.0f);
    float d = 0.0f;

    for (int i = 0; i < jelly.getParticleNum(); i++) {
        Particle& particle = jelly.getParticle(i);
        Eigen::Vector3f pos = particle.getPosition();
        Eigen::Vector3f vel = particle.getVelocity();

        float distance = normal.dot(pos) + d;

        if (distance < eEPSILON) {
            float vDotN = vel.dot(normal);
            if (vDotN < 0) {
                Eigen::Vector3f newVel = vel - (1 + coefResist) * vDotN * normal;
                particle.setVelocity(newVel);
            }

            if (vel.norm() < eEPSILON) {
                Eigen::Vector3f resistForce = -particle.getMass() * coefFriction * vel;
                particle.addForce(resistForce);
            }
        }
    }
}

ElevatorTerrain::ElevatorTerrain() { 
    reset();
}

void ElevatorTerrain::reset() {
    modelMatrix = util::translate(0.0f, 1.0f, 0.0f) * util::rotateDegree(0, 0, 0) * util::scale(5, 1, 5);
    position = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
    velocity = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
}

TerrainType ElevatorTerrain::getType() { return TerrainType::Elevator; }

void ElevatorTerrain::handleCollision(const float delta_T, Jelly& jelly) {
    constexpr float eEPSILON = 0.01f;
    constexpr float coefResist = 0.8f;
    constexpr float coefFriction = 0.5f;

    // TODO#3-2: Implement the collision handling between the jelly and the elevator
    //   If collision happens:
    //      1. Directly update particles' velocity
    //      2. Apply contact force to particles when needed
    // Note:
    //   1. There are `jelly.getParticleNum()` particles.
    //   2. See TODOs in `Jelly::computeInternalForce` and functions in `particle.h` if you don't know how to access
    //   data.
    //   3. The elevator plane spans 5x5 units in the XZ plane.
    // Hint:
    //   1. Review "particles.pptx" from p.14 - p.19
    //   1. Use a.norm() to get length of a.
    //   2. Use a.normalize() to normalize a inplace.
    //          a.normalized() will create a new vector.
    //   3. Use a.dot(b) to get dot product of a and b.

    // get elevator position
    Eigen::Vector3f elevatorPos = getPosition();

    Eigen::Vector3f elevatorNormal(0, 1, 0); 

    for (int i = 0; i < jelly.getParticleNum(); i++) {
        Particle& particle = jelly.getParticle(i);
        Eigen::Vector3f pos = particle.getPosition();
        Eigen::Vector3f vel = particle.getVelocity();

        if (pos[1] < elevatorPos[1] + eEPSILON) {
            pos[1] = elevatorPos[1] + eEPSILON;
            particle.setPosition(pos);

            if (vel[1] < 0) {
                vel[1] = -vel[1] * coefResist;
                particle.setVelocity(vel);
            }

            if (vel.norm() < eEPSILON) {
                Eigen::Vector3f resistForce = -particle.getMass() * coefFriction * vel;
                particle.addForce(resistForce);
            }
        }
    }

}

}  // namespace simulation
