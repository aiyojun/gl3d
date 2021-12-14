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

class Joint_t {
public:
    float length;
    glm::vec3 pivot, euler, keptPivot;
    explicit Joint_t(const std::string& u, const glm::vec3& p, float l, const glm::vec3& a);
    std::string getUID();
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

//////////////////////////////////////////////////////////////////////

#if defined(ALL_IMPL)

#include "glm_helper.h"
#include "basic_ops.hpp"

TF::TF(): beginTime(jlib::GetSystemCurrentMs()) {}

float TF::iTime() 
{ return (float) (((double) (jlib::GetSystemCurrentMs() - beginTime)) / 1000000); }

//////////////////////////////////////////////////////////////////////

Joint_t::Joint_t(const std::string& u, const glm::vec3& p, float l, const glm::vec3& a)
: uid(u), length(l), pivot(p), keptPivot(p), euler(a), parentPtr(nullptr), childrenPtr() {}
inline std::string Joint_t::getUID() {return uid;}
inline void Joint_t::updatePivot(const glm::mat4& t) 
{pivot = glm::vec3(t * glm::vec4(keptPivot, 1.f));}
inline void Joint_t::setEular(const glm::vec3& e) {euler = e;}
inline void Joint_t::setParentPtr(Joint_t *p) {parentPtr = p;}
inline Joint_t* Joint_t::getParentPtr() {return parentPtr;}
inline void Joint_t::addChildPtr(Joint_t* j) {childrenPtr.emplace_back(j);}
inline const std::vector<Joint_t*>& Joint_t::getChildrenPtr() {return childrenPtr;}
glm::mat4 Joint_t::getMatrix()
{
    glm::mat4 transform(1.f);
    transform = glm::translate(transform, pivot);
    transform = glm::rotate(transform, glm::radians(euler.x), glm::vec3(1.f, 0.f, 0.f));
    transform = glm::rotate(transform, glm::radians(euler.y), glm::vec3(0.f, 1.f, 0.f));
    transform = glm::rotate(transform, glm::radians(euler.z), glm::vec3(0.f, 0.f, 1.f));
    transform = glm::translate(transform, glm::vec3(-pivot.x, -pivot.y, -pivot.z));
    return transform;
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////
#endif