#pragma once
#include "hitable.h"
#include "material.h"


class sphere :public hittable {
public:
	sphere();
	sphere(vec3 cen, float r, material *m) :center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray&r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;

	vec3 center;
	float radius;
	material *mat_ptr;
};

bool sphere::hit(const ray&r, float tmin, float tmax, hit_record& rec)const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;

	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / (2.0*a);
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / (2.0*a);
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	/*判断小根和大根是否在范围内。首先判断小根是否在范围内。是：保存相关信息，然后直接返回；否，判断大根是否在范围内。也就是优先选小根，小根不行再考虑大根 */
	return false;

	
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
	box = aabb(center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}