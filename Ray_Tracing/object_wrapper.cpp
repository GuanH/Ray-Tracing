#include"scene.h"

void object_wrapper::add(object* obj)
{
	bool trans = obj->material.transparent;
	obj->material.transparent = false;
	objects.push_back(obj);
	randomize_lock.push_back(false);
	if (trans)
	{
		obj->material.transparent = false;
		UpdateTransparent(objects.size() - 1, trans);
	}	
}

object* object_wrapper::operator[](size_t i)
{
	return objects[i];
}

void object_wrapper::randomize(size_t i)
{
	object* obj = (*this)[i];
	if (!randomize_lock[i])
	{
		obj->randomize();
	}	
}

void object_wrapper::SetRandomizeLock(size_t i, bool lock)
{
	randomize_lock[i] = lock;
}

bool object_wrapper::GetRandomizeLock(size_t i)
{
	return randomize_lock[i];
}

void object_wrapper::MakeSphereUpTo(int x, Scene* scene)
{
	for (size_t i = size(); i < x; i++)
	{
		add(new sphere{ float3{0,0,0}, scene });
	}
}

size_t object_wrapper::UpdateTransparent(size_t i, bool trans)
{
	if (objects[i]->material.transparent == trans) { return i; }
	if (trans)
	{
		object* ta = objects[transparent_object_size];
		objects[transparent_object_size] = objects[i];
		objects[i] = ta;
		objects[transparent_object_size]->material.transparent = true;

		bool tb = randomize_lock[transparent_object_size];
		randomize_lock[transparent_object_size] = randomize_lock[i];
		randomize_lock[i] = tb;

		return transparent_object_size++;
	}
	else
	{
		object* ta = objects[transparent_object_size - 1];
		objects[transparent_object_size - 1] = objects[i];
		objects[i] = ta;
		objects[transparent_object_size - 1]->material.transparent = false;

		bool tb = randomize_lock[transparent_object_size - 1];
		randomize_lock[transparent_object_size - 1] = randomize_lock[i];
		randomize_lock[i] = tb;

		return --transparent_object_size;
	}

}

void object_wrapper::Delete(size_t i)
{
	objects.erase(objects.begin() + i);
	randomize_lock.erase(randomize_lock.begin() + i);
}

void object_wrapper::Clone(size_t i)
{
	add(objects[i]->Clone());
}
