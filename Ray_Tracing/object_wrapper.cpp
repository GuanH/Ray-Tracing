#include"scene.h"

void object_wrapper::add(object* obj)
{
	bool trans = obj->material.transparent;
	obj->material.transparent = false;
	objects.push_back(obj);
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
	objects[i]->randomize();
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

		return transparent_object_size++;
	}
	else
	{
		object* ta = objects[transparent_object_size - 1];
		objects[transparent_object_size - 1] = objects[i];
		objects[i] = ta;
		objects[transparent_object_size - 1]->material.transparent = false;


		return --transparent_object_size;
	}

}

void object_wrapper::Delete(size_t i)
{
	delete objects[i];
	objects.erase(objects.begin() + i);
}

void object_wrapper::Clone(size_t i)
{
	add(objects[i]->Clone());
}
