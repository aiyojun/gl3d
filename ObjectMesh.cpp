#include "ObjectMesh.h"
#include "templates.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

template<size_t INDEX, size_t OFFSET, size_t ALL, typename... Args>
struct Attr;
template<size_t INDEX, size_t OFFSET, size_t ALL, typename T>
struct Attr<INDEX, OFFSET, ALL, T> { static void enable() {
	glEnableVertexAttribArray(INDEX);
	glVertexAttribPointer(INDEX, 3, GL_FLOAT, GL_FALSE, ALL, (void*) OFFSET); // TODO: 
}};
template<size_t INDEX, size_t OFFSET, size_t ALL, typename T, typename... Args>
struct Attr<INDEX, OFFSET, ALL, T, Args...> { static void enable() {
	glEnableVertexAttribArray(INDEX);
	glVertexAttribPointer(INDEX, 3, GL_FLOAT, GL_FALSE, ALL, (void*) OFFSET); // TODO: 
	Attr<INDEX + 1, OFFSET + sizeof(T), ALL, Args...>::enable();}
};
struct AttrWapper { template<typename... Args> static void enable() { Attr<0, 0, TypeSize<Args...>::value, Args...>::enable(); } };

// -----------------------------------------------------------------------------------

ObjectMesh ObjectMesh::gen() {
	hasGen = true;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	return *this;
}

template<typename E>
ObjectMesh ObjectMesh::bind(ObjectMesh& obj, size_t size, const void* start_ptr) {
	if (!obj.hasGen) obj.gen();
	vboSize = size;
	glBindVertexArray(obj.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
	glBufferData(GL_ARRAY_BUFFER, size * Types<E, K, T>::size(), start_ptr, GL_STATIC_DRAW);
	AttrWapper::enable<E>();
	return obj;
}

template<typename E, typename K>
ObjectMesh ObjectMesh::bind(ObjectMesh& obj, size_t size, const void* start_ptr) {
	if (!obj.hasGen) obj.gen();
	vboSize = size;
	glBindVertexArray(obj.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
	glBufferData(GL_ARRAY_BUFFER, size * Types<E, K, T>::size(), start_ptr, GL_STATIC_DRAW);
	AttrWapper::enable<E, K>();
	glEnableVertexAttribArray(0);
	return obj;
}

template<typename E, typename K, typename T>
ObjectMesh ObjectMesh::bind(ObjectMesh& obj, size_t size, const void* start_ptr) {
	if (!obj.hasGen) obj.gen();
	obj.vboSize = size;
	glBindVertexArray(obj.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
	glBufferData(GL_ARRAY_BUFFER, size * Types<E, K, T>::size(), start_ptr, GL_STATIC_DRAW);
	AttrWapper::enable<E, K, T>();
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return obj;
}

ObjectMesh ObjectMesh::ebo(size_t size, const void* start_ptr) {
	eboSize = size;
	eboOpened = true;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), start_ptr, GL_STATIC_DRAW);
	return *this;
}

void draw() {
    glBindVertexArray(VAO);
    if (eboOpened) {
    	glDrawElements(GL_TRIANGLES, eboSize, GL_UNSIGNED_INT, 0);
    } else {
    	glDrawElements(GL_TRIANGLES, vboSize, GL_UNSIGNED_INT, 0);
    	 glDrawArrays(GL_TRIANGLES, 0, vboSize);
    }
    glBindVertexArray(0);
}