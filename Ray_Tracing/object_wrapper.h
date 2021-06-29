#pragma once
#include"object.h"
class object;
class Scene;
class object_wrapper
{
public:
	void add(object* obj);
	object* operator[](size_t i);
	void randomize(size_t i);
	void SetRandomizeLock(size_t i, bool lock);
	bool GetRandomizeLock(size_t i);
	void MakeSphereUpTo(int x, Scene* scene);
	size_t UpdateTransparent(size_t i, bool trans);
	void Delete(size_t i);
	void Clone(size_t i);
	const size_t size() const { return objects.size(); }
private:
	std::vector<object*> objects;
	int transparent_object_size = 0;
	std::vector<bool> randomize_lock;
};