#pragma once
#include "material.h"

class metal :public material {
public:
	metal(const vec3&a,float f) :albedo(a) {
		if (f < 1) fuzz = f; else fuzz = 1;
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const;
	/*scatter()获取反射光线和衰减系数*/

	vec3 albedo;/*保存衰减系数*/
	float fuzz;//模糊
};



vec3 reflect(const vec3& v, const vec3& n) {
	/*获取镜面反射的反射光线的方向向量。具体计算，后面解释*/
	vec3 vp;
	return v - 2 * dot(v, n)*n;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
	/*这里具体实现metal::scatter()。做两件事情：获取镜面反射的反射光线；获取材料的衰减系数。 */
	vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
	scattered = ray(rec.p, reflected+ fuzz * random_in_unit_sphere());
	attenuation = albedo;
	return (dot(scattered.direction(), rec.normal) > 0);

}