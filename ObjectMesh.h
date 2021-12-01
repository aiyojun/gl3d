#pragma once

#include <cstddef>

class ObjectMesh {
public:
	ObjectMesh gen();
	template<typename E, typename K, typename T>
	static ObjectMesh bind(ObjectMesh& obj, size_t size, const void* start_ptr);
	ObjectMesh ebo(size_t size, const void* start_ptr);
	void draw();
private:
	bool hasGen = false;
	bool eboOpened = false;
	size_t vboSize;
	size_t eboSize;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};