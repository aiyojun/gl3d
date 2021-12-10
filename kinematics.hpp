#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <map>

class TF {
public:
    TF();
    float iTime();
private:
    long beginTime;
};

class RigidBody {
public:
    std::string uid;
    glm::vec3& position, eular, base;
    RigidBody(glm::vec3& base, glm::vec3& pos, glm::vec3& rot);
    glm::mat4 iState();
    static glm::mat4 state(const glm::vec3& pos, const glm::vec3& rot);
    static glm::mat4 state(const glm::vec3& base, const glm::vec3& pos, const glm::vec3& rot);
};

class Joint {
public:
    explicit Joint(RigidBody rigidBody);
    explicit Joint(std::string uid, RigidBody rigidBody);
    std::string uid;
    RigidBody rigidBody;
    std::vector<Joint *> children;
    Joint* parent;
private:
    static std::string GenUID();
};

class Topology {
public:
    void add(RigidBody rigidBody);
    void add(const std::string& uid, RigidBody rigidBody);
    glm::mat4 getMatrix(const std::string& uid);
private:
    std::vector<Joint*> getLines(const std::string& uid);
    std::map<std::string, Joint*> cache;
    Joint* root;
};

//////////////////////////////////////////////////////////////////////

#if defined(ALL_IMPL)

#include "basic_ops.hpp"

TF::TF(): beginTime(jlib::GetSystemCurrentMs()) {}

float TF::iTime() 
{ return (float) (((double) (jlib::GetSystemCurrentMs() - beginTime)) / 1000000); }

//////////////////////////////////////////////////////////////////////

RigidBody::RigidBody(glm::vec3& base, glm::vec3& pos, glm::vec3& rot): base(base), position(pos), eular(rot) {}

glm::mat4 RigidBody::state(const glm::vec3& pos, const glm::vec3& rot)
{
    glm::mat4 transform(1.0f);
    transform = glm::rotate(transform, glm::radians(rot.x), glm::vec3(1.f, 0.f, 0.f));
    transform = glm::rotate(transform, glm::radians(rot.y), glm::vec3(0.f, 1.f, 0.f));
    transform = glm::rotate(transform, glm::radians(rot.z), glm::vec3(0.f, 0.f, 1.f));
    transform = glm::translate(transform, pos);
    return transform;
}

glm::mat4 RigidBody::state(const glm::vec3& base, const glm::vec3& pos, const glm::vec3& rot)
{
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, base);
    transform = glm::translate(transform, pos);
    transform = glm::rotate(transform, glm::radians(rot.x), glm::vec3(1.f, 0.f, 0.f));
    transform = glm::rotate(transform, glm::radians(rot.y), glm::vec3(0.f, 1.f, 0.f));
    transform = glm::rotate(transform, glm::radians(rot.z), glm::vec3(0.f, 0.f, 1.f));
    transform = glm::translate(transform, base * glm::vec3(-1));
    return transform;
}

glm::mat4 RigidBody::iState()
{ return state(base, position, eular); }

//////////////////////////////////////////////////////////////////////

Joint::Joint(RigidBody rigidBody): uid(rigidBody.uid), rigidBody(rigidBody), parent(nullptr), children()
{

}

Joint::Joint(std::string uid, RigidBody rigidBody): uid(std::move(uid)), rigidBody(rigidBody), parent(nullptr), children()
{

}

std::string Joint::GenUID()
{
    return std::to_string(jlib::GetSystemCurrentMs());
}

//////////////////////////////////////////////////////////////////////

void Topology::add(RigidBody rigidBody)
{
    cache.clear();
    auto *joint = new Joint(rigidBody);
    cache[joint->uid] = joint;
    root = joint;
}

void Topology::add(const std::string& uid, RigidBody rigidBody)
{
    if (cache.find(uid) != cache.end()) {
        std::cout << "[Topology] cannot find (" << uid << ")!" << std::endl;
        return;
    }
    auto* joint = new Joint(rigidBody);
    joint->parent = cache[uid];
    cache[uid]->children.emplace_back(joint);
}

glm::mat4 Topology::getMatrix(const std::string& uid)
{
    std::vector<Joint*> parents = getLines(uid);
    glm::mat4 accumulate(1.f);
    for (auto & parent : parents)
        accumulate = accumulate * parent->rigidBody.iState();
    return accumulate;
}

std::vector<Joint*> Topology::getLines(const std::string& uid)
{

    return {};
}

#endif