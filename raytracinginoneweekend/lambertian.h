#pragma once
#include "material.h"
#include "texture.h"

class lambertian :public material {
public:
	lambertian(texture *a):albedo(a){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const;
	/*scatter()获取反射光线和衰减系数*/

	texture *albedo;/*保存衰减系数*/
};

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3((rand() % (100) / (float)(100)),
			(rand() % (100) / (float)(100)),
			(rand() % (100) / (float)(100)))
			- vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;

}

bool lambertian::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
	/*这里具体实现lambertian::scatter()。做两件事情：获取漫反射的反射光线；获取材料的衰减系数。 */
	/*vec3 target = rec.p + rec.normal + random_in_unit_sphere();
	scattered = ray(rec.p, target - rec.p,r_in.time());
	attenuation = albedo;*/

	vec3 target = rec.p + rec.normal + random_in_unit_sphere();
	scattered = ray(rec.p, target - rec.p);
	attenuation = albedo->value(0, 0, rec.p);
	return true;

}