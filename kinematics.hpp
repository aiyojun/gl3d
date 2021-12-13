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
    // glm::vec3& position, eular, base;
    const glm::vec3& position;
    const glm::vec3& eular;
    const glm::vec3& base;
    const glm::vec3& ending;
    // RigidBody(glm::vec3& base, glm::vec3& pos, glm::vec3& rot);
    // RigidBody(const std::string& s, glm::vec3& base, glm::vec3& pos, glm::vec3& rot);
    glm::mat4 iState();
    // static glm::mat4 state(const glm::vec3& pos, const glm::vec3& rot);
    // static glm::mat4 state(const glm::vec3& base, const glm::vec3& pos, const glm::vec3& rot);
    RigidBody(const std::string& s, const glm::vec3& base, const glm::vec3& ending, const glm::vec3& pos, const glm::vec3& rot);
    // static glm::mat4 state(glm::vec3* base, glm::vec3* pos, glm::vec3* rot);
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

class Joint_t {
public:
    float length;
    glm::vec3 pivot; // rotation point
    glm::vec3 angle; // eular
    glm::vec3 keptPivot;
    explicit Joint_t(const std::string& u, const glm::vec3& p, float l, const glm::vec3& a);
    std::string getUID();
    // void setPivot(const glm::vec3& p);
    void updatePivot(const glm::mat4& t);
    void setEular(const glm::vec3& e);
    void setParentPtr(Joint_t *p);
    Joint_t* getParentPtr();
    void addChildPtr(Joint_t* j);
    const std::vector<Joint_t*>& getChildrenPtr();
    glm::mat4 getMatrix();
private:
    const std::string uid;
    Joint_t* parentPtr;
    std::vector<Joint_t*> childrenPtr;
};
#if defined(ALL_IMPL)
#include "glm_helper.h"
Joint_t::Joint_t(const std::string& u, const glm::vec3& p, float l, const glm::vec3& a)
: uid(u), length(l), pivot(p), keptPivot(p), angle(a), parentPtr(nullptr), childrenPtr() {}
inline std::string Joint_t::getUID() {return uid;}
// inline void Joint_t::setPivot(const glm::vec3& p) {pivot = p;}
inline void Joint_t::updatePivot(const glm::mat4& t) 
{pivot = glm::vec3(t * glm::vec4(keptPivot, 1.f));}
inline void Joint_t::setEular(const glm::vec3& e) {angle = e;}
inline void Joint_t::setParentPtr(Joint_t *p) {parentPtr = p;}
inline Joint_t* Joint_t::getParentPtr() {return parentPtr;}
inline void Joint_t::addChildPtr(Joint_t* j) {childrenPtr.emplace_back(j);}
inline const std::vector<Joint_t*>& Joint_t::getChildrenPtr() {return childrenPtr;}
glm::mat4 Joint_t::getMatrix()
{
    glm::mat4 transform(1.f);
    transform = glm::translate(transform, pivot);
    transform = glm::rotate(transform, glm::radians(angle.x), glm::vec3(1.f, 0.f, 0.f));
    transform = glm::rotate(transform, glm::radians(angle.y), glm::vec3(0.f, 1.f, 0.f));
    transform = glm::rotate(transform, glm::radians(angle.z), glm::vec3(0.f, 0.f, 1.f));
    transform = glm::translate(transform, glm::vec3(-pivot.x, -pivot.y, -pivot.z));
    return transform;
}
#endif

class Topology {
public:
    void add(RigidBody rigidBody);
    void add(const std::string& uid, RigidBody rigidBody);
    glm::mat4 getMatrix(const std::string& uid);
    std::vector<Joint*> getLines(const std::string& uid);
    void print();
    void print(Joint* j);
private:
    std::map<std::string, Joint*> cache;
    Joint* root;
};

class Topology_t {
public:
    static Topology_t GenHuman();
    void add(Joint_t* j);
    void add(const std::string& u, Joint_t* j);
    std::vector<Joint_t*> getChain(const std::string& u);
    void clear();
    /** OpenGL */
    std::vector<glm::mat4> getMatrices(const std::vector<glm::vec3>& eulars);
private:
    Joint_t* root = nullptr;
    std::vector<std::string> uids;
    std::map<std::string, Joint_t*> cache;
    void print(Joint_t* ptr); /** Keep this for debugging. */
};
#if defined(ALL_IMPL)
Topology_t Topology_t::GenHuman()
{
    std::vector<std::string> uids = {
        "head", "body", 
        "left_arm", "left_fore_arm", 
        "right_arm", "right_fore_arm", 
        "left_leg", "left_small_leg", 
        "right_leg", "right_small_leg"
    };
    std::vector<Joint_t*> human = {
        new Joint_t(uids[0], glm::vec3( 0.f, 14.f, 0.f),   2, glm::vec3(0.f)),
        new Joint_t(uids[1], glm::vec3( 0.f, 12.f, 0.f),   5, glm::vec3(0.f)),
        new Joint_t(uids[2], glm::vec3(-3.f, 12.f, 0.f),   3, glm::vec3(0.f)),
        new Joint_t(uids[3], glm::vec3(-6.f, 12.f, 0.f),   3, glm::vec3(0.f)),
        new Joint_t(uids[4], glm::vec3( 3.f, 12.f, 0.f),   3, glm::vec3(0.f)),
        new Joint_t(uids[5], glm::vec3( 6.f, 12.f, 0.f),   3, glm::vec3(0.f)),
        new Joint_t(uids[6], glm::vec3( 0.f,  7.f, 0.f), 3.5, glm::vec3(0.f)),
        new Joint_t(uids[7], glm::vec3(-1.f,  4.f, 0.f), 4.5, glm::vec3(0.f)),
        new Joint_t(uids[8], glm::vec3( 0.f,  7.f, 0.f), 3.5, glm::vec3(0.f)),
        new Joint_t(uids[9], glm::vec3( 1.f,  4.f, 0.f), 4.5, glm::vec3(0.f)),
    };
    Topology_t topo;
    topo.add(human[0]);          // add head
    topo.add(uids[0], human[1]); // add body
    topo.add(uids[0], human[2]); // add left arm
    topo.add(uids[2], human[3]); // add left fore arm
    topo.add(uids[0], human[4]); // add right arm
    topo.add(uids[4], human[5]); // add right fore arm
    topo.add(uids[1], human[6]); // add left leg
    topo.add(uids[6], human[7]); // add left small leg
    topo.add(uids[1], human[8]); // add right leg
    topo.add(uids[8], human[9]); // add right small leg
    topo.print(topo.root);
    return topo;
}
void Topology_t::add(Joint_t* j)
{
    clear();
    root = j;
    cache[j->getUID()] = j;
    uids.emplace_back(j->getUID());
}
void Topology_t::add(const std::string& u, Joint_t* j)
{
    if (cache.find(u) == cache.end()) 
        return;
    j->setParentPtr(cache[u]);
    cache[u]->addChildPtr(j);
    cache[j->getUID()] = j;
    uids.emplace_back(j->getUID());
}
std::vector<Joint_t*> Topology_t::getChain(const std::string& u)
{
    if (cache.find(u) == cache.end())
        return {};
    std::vector<Joint_t*> _r;
    Joint_t* joint = cache[u];
    do {
        _r.emplace_back(joint);
        joint = joint->getParentPtr();
    } while (joint != nullptr);
    std::reverse(_r.begin(), _r.end());
    return _r;
}
void Topology_t::clear()
{
    for (auto& every : cache) 
        delete every.second;
    uids.clear();
    cache.clear();
    root = nullptr;
}
std::vector<glm::mat4> Topology_t::getMatrices(const std::vector<glm::vec3>& eulars)
{
    std::vector<glm::mat4> _r;
    for (unsigned int i = 0; i < eulars.size(); i++) 
        cache[uids[i]]->setEular(eulars[i]);
    for (unsigned int index = 0; index < uids.size(); index++) {
        std::string uid = uids[index];
        std::vector<Joint_t*> chain = getChain(uid);
        /** Mark the next line! 'transform' is core matrix. */
        glm::mat4 transform(1.f);  /** The matrix is the final 
        transform matrix for current rigid body.            */
        for (auto& joint_ptr : chain) 
            transform = transform * joint_ptr->getMatrix();
        if (!chain.back()->getChildrenPtr().empty()) {
            /** Execute updating. The step is very important! */
            for (auto& child : chain.back()->getChildrenPtr()) 
                child->updatePivot(transform);
        }
        _r.emplace_back(transform);
    }
    return _r;
}
void Topology_t::print(Joint_t* ptr)
{
    std::string s;
    if (ptr->getParentPtr() != nullptr) {
        s = "(" + ptr->getParentPtr()->getUID() + ")";
    }
    std::cout 
        << "[Topology] uid: " 
        << ptr->getUID() 
        << (ptr->getParentPtr() != nullptr ? s : "") 
        << std::endl;
    if (!ptr->getChildrenPtr().empty()) {
        for (auto& child : ptr->getChildrenPtr()) {
            print(child);
        }
    }
}
#endif

//////////////////////////////////////////////////////////////////////

#if defined(ALL_IMPL)

#include "basic_ops.hpp"

TF::TF(): beginTime(jlib::GetSystemCurrentMs()) {}

float TF::iTime() 
{ return (float) (((double) (jlib::GetSystemCurrentMs() - beginTime)) / 1000000); }

//////////////////////////////////////////////////////////////////////

RigidBody::RigidBody(const std::string& s, const glm::vec3& base, const glm::vec3& ending, const glm::vec3& pos, const glm::vec3& rot)
: uid(s), base(base), ending(ending), position(pos), eular(rot) {}

glm::mat4 RigidBody::iState()
{ 
    glm::mat4 transform(1.0f);
    // std::cout << uid << "|base : " << std::setprecision(4) << base.x << "," << std::setprecision(4) << base.y << "," << std::setprecision(4) << base.z 
    // << "; pos : " << std::setprecision(4) << position.x << "," << std::setprecision(4) << position.y << "," << std::setprecision(4) << position.z 
    // << "; rot : " << std::setprecision(4) << eular.x << "," << std::setprecision(4) << eular.y << "," << std::setprecision(4) << eular.z 
    // << "\n";
    // transform = glm::translate(transform, glm::vec3(position.x, position.y, position.z));
    transform = glm::translate(transform, glm::vec3(base.x, base.y, base.z));
    transform = glm::rotate(transform, glm::radians(eular.x), glm::vec3(1.f, 0.f, 0.f));
    transform = glm::rotate(transform, glm::radians(eular.y), glm::vec3(0.f, 1.f, 0.f));
    transform = glm::rotate(transform, glm::radians(eular.z), glm::vec3(0.f, 0.f, 1.f));
    transform = glm::translate(transform, glm::vec3(- base.x, - base.y, - base.z));
    return transform;
    // return state(base, position, eular); 
}

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
    joint->uid = rigidBody.uid;
    cache[joint->uid] = joint;
    root = joint;
}

void Topology::add(const std::string& uid, RigidBody rigidBody)
{
    if (cache.find(uid) == cache.end()) {
        std::cout << "[Topology] cannot find (" << uid << ")!" << std::endl;
        for (auto& kv : cache) {
            std::cout << "  >> " << kv.first << std::endl;
        }
        exit(1);
        return;
    }
    auto* joint = new Joint(rigidBody);
    joint->parent = cache[uid];
    cache[uid]->children.emplace_back(joint);
    cache[joint->uid] = joint;
}

glm::mat4 Topology::getMatrix(const std::string& uid)
{
    std::vector<Joint*> parents = getLines(uid);
    glm::mat4 accumulate(1.f);
    for (auto & parent : parents)
        accumulate = parent->rigidBody.iState() * accumulate;
    return accumulate;
}

std::vector<Joint*> Topology::getLines(const std::string& uid)
{
    std::vector<Joint*> _r;
    Joint* joint = cache[uid];
    do {
        _r.emplace_back(joint);
        joint = joint->parent;
    } while (joint != nullptr);
    std::reverse(_r.begin(), _r.end());
    return _r;
}

 void Topology::print()
 {
     print(root);
 }

void Topology::print(Joint* ptr)
{
    std::string s;
    if (ptr->parent != nullptr) {s = "(" + ptr->parent->uid + ")";}
    std::cout << "[Topology] uid: " << ptr->uid << (ptr->parent != nullptr ? s : "") << std::endl;
    if (!ptr->children.empty()) {
        for (auto& child : ptr->children) {
            print(child);
        }
    }
}

#endif