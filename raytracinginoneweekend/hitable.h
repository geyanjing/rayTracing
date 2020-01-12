#pragma once
#include "ray.h"
#include "aabb.h"


class material;
struct hit_record {
	float t;
	vec3 p;
	vec3 normal;
	material *mat_ptr;

	};
/*该结构体记录“撞点”处的信息：离光线起点的距离t、撞点的坐标向量p、撞点出的法向量normal。*/
class hittable {
	/*hitable这个类表示能够被光线撞上的任何物体。比如，球体*/
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;

	/*hit()在此被声明为虚函数，则hitable为抽象类。抽象类的子类中必须实现其虚函数*/

};