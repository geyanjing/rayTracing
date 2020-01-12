#pragma once
#include "hitable.h"

class hitable_list :public hittable {
public:
	hitable_list() {};
	hitable_list(hittable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const;
	bool bounding_box(float t0, float t1, aabb& box) const;
	/*����ʵ�ָ�����麯�����ڴ˳�������������sphere.cpp�о���ʵ��*/

hittable **list;
int list_size;

};/*hitable_list�������ܹ�������ײ����������б�/����*/

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {//�˴����þ��������hit�����������sphere,�����sphere��hit����
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	/*�����ж��б������������Ƿ񱻹���ײ����ÿ���ж�һ�������б�ײ������ײ����Ϣ������hit_record�ṹ���С����ǿ��Կ���rec�ǿ��ܱ�д��εģ����ձ����ֵ�Ǻ�һ�ε�ֵ��Ҳ����������Ч��ֵ�Ǻ�һ�ε�ֵ��Ҳ������۲��������������Чײ�㣨����Чײ�㡱�����ڵ������壬��ɸѡ��һ���ֲ���Чײ�㣻���ڶ�����壬�����е���������Եľֲ���Чײ��ɸѡ������һ��������Чײ�㣩����Ϊ����������������ײ���˶��ٸ�����������ٸ�ײ�㣬�����ܿ�����ֻ�������������ײ��*/
/*�����ǰײ���ڷ�Χ�ڣ��򽫵�ǰײ��ľ�������Ϊ��Χ�����ֵ��Ҳ���Ǻ���ֻ���Ǳȸ�ײ�������ײ�㡣�����ǣ��ҵ���ײ����Խ��Խ���ģ������ҵ������ײ�㡣*/

	
	return hit_anything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb& box) const {
	if (list_size < 1) return false;
	aabb temp_box;
	bool first_true = list[0]->bounding_box(t0, t1, temp_box);
	if (!first_true)
		return false;
	else
		box = temp_box;
	for (int i = 1; i < list_size; i++) {
		if (list[i]->bounding_box(t0, t1, temp_box)) {
			box = surrounding_box(box, temp_box);
		}
		else
			return false;
	}
	return true;
}