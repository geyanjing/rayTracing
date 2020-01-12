#pragma once
#include "material.h"
#include "metal.h"


float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}


class dielectric : public material
{
public:
	dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;
	float ref_idx;
};

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	/*�ú�������������ߵķ������������㷽������24.1.1��˵���ġ�ni_over_ntΪ������ʵ�����ָ����������ʵ�����ָ���ı�ֵ*/
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminat = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminat > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminat);
		return true;
	}
	else
		/*�������������С���㣬˵��������ߵķ���������ʵ������û��������ߣ�������ȫ���䡣���ԣ�������ߺ���return false*/
		return false;
}

bool dielectric::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
	vec3 outward_normal;
	vec3 reflected = reflect(r_in.direction(), rec.normal);
	float ni_over_nt;
	/* ni_over_ntΪ������ʵ�����ָ����������ʵ�����ָ���ı�ֵ*/
	attenuation = vec3(1.0, 1.0, 1.0);
	/*���ʵ�˥������Ϊ��1��1��1�����ǹ��߲�˥��*/
	vec3 refracted;

	float reflect_prob;
	float cosine;

	if (dot(r_in.direction(), rec.normal) > 0) {
		/*���ߵķ�����������ķ������ĵ�˴����㣬˵�������Ǵ������ڲ�������������ԣ�����ʱ�ķ���������ķ����������෴��ע�⣬ref_idx��ָ���ܽ��ʵ�����ָ���͹�����ʵ�����ָ���ı�ֵ����ʱ��������ǹ��ܽ��ʣ���������ǹ�����ʣ�����ni_over_nt=ref_idx*/
		outward_normal = -rec.normal;
		ni_over_nt = ref_idx;

		cosine = ref_idx * dot(r_in.direction(), rec.normal)/ r_in.direction().length();
	}

	else {
		/*���ߵķ�����������ķ������ĵ��bu�����㣬˵�������Ǵӿ������������������ԣ�����ʱ�ķ���������ķ���������ͬ��ע�⣬ref_idx��ָ���ܽ��ʵ�����ָ���͹�����ʵ�����ָ���ı�ֵ����ʱ��������ǹ�����ʣ���������ǹ��ܽ��ʣ�����ni_over_nt=1.0/ref_idx*/
		outward_normal = rec.normal;
		ni_over_nt = 1.0 / ref_idx;

		cosine = -dot(r_in.direction(), rec.normal)/ r_in.direction().length();
	}
	if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
		reflect_prob = schlick(cosine, ref_idx);
	}
	else {
		reflect_prob = 1.0;
	}

	if ((rand() % (100) / (float)(100)) < reflect_prob) {
		scattered = ray(rec.p, reflected);
	}
	else {
		scattered = ray(rec.p, refracted);
	}
	return true;

}