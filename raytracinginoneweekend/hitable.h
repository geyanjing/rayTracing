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
/*�ýṹ���¼��ײ�㡱������Ϣ����������ľ���t��ײ�����������p��ײ����ķ�����normal��*/
class hittable {
	/*hitable������ʾ�ܹ�������ײ�ϵ��κ����塣���磬����*/
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;

	/*hit()�ڴ˱�����Ϊ�麯������hitableΪ�����ࡣ������������б���ʵ�����麯��*/

};