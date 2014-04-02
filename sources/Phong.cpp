#include "Phong.h"

Phong::Phong(const Vector3 & kd, const Vector3 &ks, const Vector3 &ka)
{
	m_ka = ka;
	m_ks = ks; // old: m_sp
	m_kd = kd; // old: m_dp
	m_bm = 10.0f;
	m_tp = 0;
	m_phong = 100;
	m_refr = 1;
	m_indirect = true;
	m_texture_ka = shared_ptr<Material>(new Material());
	m_texture_ks = shared_ptr<Material>(new Material());
	m_texture_kd = shared_ptr<Material>(new Material());
	m_is_glossy = false;
	m_is_bump_noisy = false;
	m_bump_noise_height = 1;
	m_bump_noise_turbulence = 1;
}

Phong::Phong(shared_ptr<Material> texture_kd, shared_ptr<Material> texture_ks, const Vector3 &ka)
{
	m_ka = ka;
	m_ks = Vector3(0.f);
	m_kd = Vector3(1.f);
	m_bm = 10.0f;
	m_tp = 0;
	m_phong = 100;
	m_refr = 1;
	m_indirect = true;
	m_texture_kd = texture_kd;
	m_texture_ks = texture_ks;
	m_texture_ka = shared_ptr<Material>(new Material());
	m_is_glossy = false;
	m_is_bump_noisy = false;
	m_bump_noise_height = 1;
	m_bump_noise_turbulence = 1;
}

Phong::Phong(shared_ptr<Material> texture_kd, const Vector3 &ka)
{
	m_ka = ka;
	m_ks = Vector3(0.f);
	m_kd = Vector3(1.f);
	m_bm = 10.0f;
	m_tp = 0;
	m_phong = 100;
	m_refr = 1;
	m_indirect = true;
	m_texture_kd = texture_kd;
	m_texture_ks = shared_ptr<Material>(new Material());
	m_texture_ka = shared_ptr<Material>(new Material());
	m_is_glossy = false;
	m_is_bump_noisy = false;
	m_bump_noise_height = 1;
	m_bump_noise_turbulence = 1;
}

Phong::~Phong()
{
}

HitInfo Phong::bumpHit(HitInfo const &rhit) const
{
    HitInfo bm_hit = rhit;

	if (m_is_bump_noisy)
	{
		float noise_x = float(PerlinNoise::turbulence(Vector3(m_bump_noise_height * double(bm_hit.P.x), 
															  m_bump_noise_height * double(bm_hit.P.y),
															  m_bump_noise_height * double(bm_hit.P.z)), m_bump_noise_turbulence));
		float noise_y = float(PerlinNoise::turbulence(Vector3(m_bump_noise_height * double(bm_hit.P.y), 
															  m_bump_noise_height * double(bm_hit.P.z),
															  m_bump_noise_height * double(bm_hit.P.x)), m_bump_noise_turbulence));
		float noise_z = float(PerlinNoise::turbulence(Vector3(m_bump_noise_height * double(bm_hit.P.z), 
															  m_bump_noise_height * double(bm_hit.P.x),
															  m_bump_noise_height * double(bm_hit.P.y)), m_bump_noise_turbulence));
		bm_hit.N.x = (1.0f - m_bm) * bm_hit.N.x + m_bm * noise_x;
		bm_hit.N.y = (1.0f - m_bm) * bm_hit.N.y + m_bm * noise_y;
		bm_hit.N.z = (1.0f - m_bm) * bm_hit.N.z + m_bm * noise_z;
	}
	else if (m_texture_bump != NULL)
	{
		float me = m_texture_bump->getPixel(bm_hit.u, bm_hit.v, 0, 0).x;
		float north = m_texture_bump->getPixel(bm_hit.u, bm_hit.v, 0, 1).x;
		float south = m_texture_bump->getPixel(bm_hit.u, bm_hit.v, 0, -1).x;
		float east = m_texture_bump->getPixel(bm_hit.u, bm_hit.v, 1, 0).x;
		float west = m_texture_bump->getPixel(bm_hit.u, bm_hit.v, -1, 0).x;
		
		Vector3 pv = bm_hit.tangent;
		Vector3 pu = bm_hit.N.cross(pu);
		Vector3 offset = -100 * m_bm *(((north - me) - (south - me))*pv + ((east - me) - (west - me))*pu);
		bm_hit.N += offset;
	}
	bm_hit.N.normalize();		
    return bm_hit;
}

void Phong::shadePhoton(const Ray &ray, const HitInfo &rhit, const Scene &scene, Vector3 const &power, Photon_map *map) const
{
    HitInfo hit = bumpHit(rhit);
    Vector3 dcolor = m_kd * m_texture_kd->shade(ray, hit, scene);
    if (dcolor.max() > EPSILON && (!map->isCaustics() || ray.seen_specular))
    {
        Vector3 const &position = hit.P;
        Vector3 powe = power * dcolor;
        Vector3 const &dir = ray.d;
        map->store(powe.array(), position.array(), dir.array());
    }
    if (ray.iter > MAX_RAY_ITER) 
		return;
    Vector3 scolor = m_ks * m_texture_ks->shade(ray, hit, scene);
    Vector3 total_color = dcolor + scolor;
    float pr = total_color.max();
    float pd = pr * dcolor.sum() / total_color.sum();
    float ps = pr - pd;
    float pspec_refr = m_tp * ps;
    float pspec_refl = ps - pspec_refr;

    Vector3 *color = 0;
    float probability = -1;
    Ray r;
    r.refractionIndex = ray.refractionIndex;
    r.refractionStack = ray.refractionStack;
    r.iter = ray.iter + 1;
    r.o = hit.P;
    r.seen_specular = ray.seen_specular;
    Vector3 randvect(randone(g_rng), randone(g_rng), randone(g_rng));
    float russian_roulette = randone(g_rng);
    if (russian_roulette < pd)
    {
        if (map->isCaustics()) return;
        color = &dcolor;
        probability = pd;
        float theta = asinf((randone(g_rng)));
        float phi = 2 * PI * randone(g_rng);

        Vector3 const &yaxis = hit.N;
        Vector3 xaxis = yaxis.cross(randvect);
        xaxis.normalize();
        Vector3 zaxis = yaxis.cross(xaxis);
        zaxis.normalize();
        r.d = 0;
        r.d += sinf(theta) * cosf(phi) * xaxis;
        r.d += sinf(theta) * sinf(phi) * zaxis;
        r.d += cosf(theta) * yaxis;
        r.d.normalize();
    }
    else if (russian_roulette < pd + pspec_refl)
    {
        probability = pspec_refl;
        color = &scolor;
        if (m_is_glossy)
        {
            Vector3 R = ray.d - 2 * ray.d.dot(hit.N) * hit.N;

            Vector3 u = R.cross(randvect);
            u.normalize();
            Vector3 v = R.cross(u);
            v.normalize();

            float theta = acos(powf(1.0f - randone(g_rng), (1.0f / (m_phong + 1))));
            float phi = 2.0f * PI * randone(g_rng);

            r.d = 0;
            r.d += sinf(theta) * cosf(phi) * u;
            r.d += sinf(theta) * sinf(phi) * v;
            r.d += cosf(theta) * R;
        }
        else
            r.d = ray.d - 2*ray.d.dot(hit.N)*hit.N;
        r.seen_specular = true;
    }
    else if (russian_roulette < pr)
    {
        probability = pspec_refr;
        color = &scolor;
        float ratio = 1.0;
		if ((*ray.refractionStack)[ray.refractionIndex] == m_refr && m_refr != 1.0)
        {
            r.refractionIndex = ray.refractionIndex - 1;
            ratio = m_refr / (*r.refractionStack)[r.refractionIndex];
        }
        else if ((*ray.refractionStack)[ray.refractionIndex] != m_refr)
        {
            r.refractionIndex = ray.refractionIndex + 1;
            r.refractionStack->push_back(m_refr);
            ratio = (*ray.refractionStack)[ray.refractionIndex] / m_refr;
        }
        Vector3 w = -ray.d;
        float dDotN = w.dot(hit.N);
        if (dDotN < 0)
            dDotN = -dDotN;
        if (1 - ratio*ratio*(1 - dDotN*dDotN) < 0)
            return;
        r.d = -ratio * (w - dDotN*hit.N) - sqrtf(1 - ratio*ratio*(1 - dDotN*dDotN)) * hit.N;
        r.d.normalize();
        r.seen_specular = true;
    }
    else
        return;

    HitInfo h;
    if (scene.trace(h, r, EPSILON))
    {
        h.material->shadePhoton(r, h, scene, power * *color / probability, map);
    }
}

Vector3
Phong::shade(const Ray& ray, const HitInfo& rhit, const Scene& scene) const
{
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);
 	Vector3 randvect(randone(g_rng), randone(g_rng), randone(g_rng));
    const Lights *lightlist = scene.lights();
    
	HitInfo bm_hit = bumpHit(rhit);
    
    // loop over all of the lights
    Lights::const_iterator lightIter;
	Vector3 diffcolor = m_kd * m_texture_kd->shade(ray, bm_hit, scene);
    Vector3 speccolor = m_ks * m_texture_ks->shade(ray, bm_hit, scene);
    // float irrad[3];
    // g_global_illum_map->irradiance_estimate(irrad, bm_hit.P.array(), bm_hit.N.array(), 1.f, 100);
    // L += Vector3(irrad) * diffcolor;
	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        PointLight* pLight = *lightIter;

        float shadow_mul = 0.f;

        Vector3 tolight = pLight->position() - bm_hit.P;
        tolight.normalize();
        Vector3 xaxis = randvect.cross(tolight);
        xaxis.normalize();
        Vector3 yaxis = xaxis.cross(tolight);
        yaxis.normalize();
        float xdisk, ydisk;
        do
        {
            xdisk = 1 - 2*randone(g_rng);
            ydisk = 1 - 2*randone(g_rng);
        } while (xdisk*xdisk + ydisk*ydisk > 1);
        Vector3 posinlight = pLight->position() + xdisk*pLight->sphere()*xaxis + ydisk*pLight->sphere()*yaxis;
        Ray lightray;
        lightray.d = (posinlight - bm_hit.P);
        float raylength = lightray.d.length();
        float remaininglength = raylength;
        lightray.d /= raylength;
        lightray.o = bm_hit.P;
        while (true)
        {
            HitInfo lighthit;

            if (!scene.trace(lighthit, lightray, EPSILON, remaininglength))
            {
                shadow_mul = 1;
                break;
            }
            if (lighthit.material->castShadow())
                break;
            remaininglength -= lighthit.t;
            lightray.o = lighthit.P;
        }

        if (shadow_mul < EPSILON)
            continue;

        // Diffuse
        Vector3 result = pLight->color()*shadow_mul;

        // the inverse-squared falloff
        float falloff = raylength*raylength;

        // normalize the light direction

        float nDotL = dot(bm_hit.N, lightray.d);

        if (m_kd.max() > EPSILON)
        {
            L += std::max(0.0f, nDotL/falloff * pLight->wattage() / (4*PI)) * result * diffcolor;
        }
        if (m_ks.max() > EPSILON && nDotL > 0)
        {
            Vector3 refl = -lightray.d + 2*nDotL*bm_hit.N;
            L += std::max(0.f, powf((-ray.d).dot(refl), m_phong)) * result * speccolor * (1 - m_tp);
            //Vector3 half = -ray.d + hit.N;
            //half.normalize();
            //L += std::max(0.f, powf(half.dot(hit.N), 1000)) * result * speccolor * m_sp;
        }
    }

    float caustic[3] = {0, 0, 0};
    g_caustics_map->irradiance_estimate(caustic, bm_hit.P.array(), bm_hit.N.array(), 3.0f, 200); // TODO: customize these parameters
    Vector3 caustic_color(caustic);
    L += caustic_color;

	// Indirect diffuse sampling (method 2)
	if (m_indirect && m_kd.max() > EPSILON && ray.iter < MAX_RAY_ITER)
	{
		float theta = asinf((randone(g_rng)));
		float phi = 2 * PI * randone(g_rng);
		Vector3 const &yaxis = bm_hit.N;
		Vector3 xaxis = yaxis.cross(randvect);
		xaxis.normalize();
		Vector3 zaxis = yaxis.cross(xaxis);
		zaxis.normalize();
		Ray diffuse_ray;
        diffuse_ray.refractionIndex = ray.refractionIndex;
        diffuse_ray.refractionStack = ray.refractionStack;
		diffuse_ray.iter = ray.iter + 1;
		diffuse_ray.o = bm_hit.P;
		diffuse_ray.d = 0;
		diffuse_ray.d += sinf(theta) * cosf(phi) * xaxis;
		diffuse_ray.d += sinf(theta) * sinf(phi) * zaxis;
		diffuse_ray.d += cosf(theta) * yaxis;
        diffuse_ray.d.normalize();

		HitInfo diff_hit;
		Vector3 diff_res;
		if (scene.trace(diff_hit, diffuse_ray, EPSILON))
        {
            float irrad[3];
            g_global_illum_map->irradiance_estimate(irrad, diff_hit.P.array(), diff_hit.N.array(), 100.f, 50); // TODO: customize these parameters
            diff_res = Vector3(irrad);
        }
			// diff_res = diff_hit.material->shade(diffuse_ray, diff_hit, scene);
		else
            diff_res = scene.bgColor();
        L += (diffcolor * diff_res) / PI;
	}

    float roulette = randone(g_rng);

    if (roulette > m_tp && (1 - m_tp) * m_ks.max() > EPSILON && ray.iter < MAX_RAY_ITER)
    {
        Ray newray;
		newray.iter = ray.iter + 1;
		newray.refractionIndex = ray.refractionIndex;
		newray.refractionStack = ray.refractionStack;
		newray.o = bm_hit.P;

		if (m_is_glossy)
		{
			Vector3 R = ray.d - 2 * ray.d.dot(bm_hit.N) * bm_hit.N;

			Vector3 u = R.cross(randvect);
			u.normalize();
			Vector3 v = R.cross(u);
			v.normalize();

			float theta = acos(powf(1.0f - randone(g_rng), (1.0f / (m_phong + 1))));
			float phi = 2.0f * PI * randone(g_rng);
					
			newray.d = 0;
			newray.d += sinf(theta) * cosf(phi) * u;
			newray.d += sinf(theta) * sinf(phi) * v;
			newray.d += cosf(theta) * R;
		}
		else
			newray.d = ray.d - 2 * ray.d.dot(bm_hit.N) * bm_hit.N;

		newray.d.normalize();
        HitInfo minHit;
        if (scene.trace(minHit, newray, EPSILON))
        {
			L += minHit.material->shade(newray, minHit, scene) * speccolor;
        }
        else
        {
            L += scene.bgColor() * speccolor;
        }
    }

    if (roulette < m_tp && m_tp * m_ks.max() > EPSILON && ray.iter < MAX_RAY_ITER)
	{
		Ray newray;
        newray.refractionStack = ray.refractionStack;
        newray.refractionIndex = ray.refractionIndex;
        float ratio = 1.0;
		if ((*ray.refractionStack)[ray.refractionIndex] == m_refr && m_refr != 1.0)
        {
            newray.refractionIndex = ray.refractionIndex - 1;
            ratio = m_refr / (*newray.refractionStack)[newray.refractionIndex];
        }
        else if ((*ray.refractionStack)[ray.refractionIndex] != m_refr)
        {
            newray.refractionIndex = ray.refractionIndex + 1;
            newray.refractionStack->push_back(m_refr);
            ratio = (*ray.refractionStack)[ray.refractionIndex] / m_refr;
        }
        newray.o = bm_hit.P;
        Vector3 w = -ray.d;
        float dDotN = w.dot(bm_hit.N);
        if (dDotN < 0)
            dDotN = -dDotN;
		if (1 - ratio*ratio*(1 - dDotN*dDotN) >= 0)
		{
			newray.d = -ratio * (w - dDotN*bm_hit.N) - sqrtf(1 - ratio*ratio*(1 - dDotN*dDotN)) * bm_hit.N;
			newray.d.normalize();
			newray.iter = ray.iter + 1;
			HitInfo minHit;
			if (scene.trace(minHit, newray, EPSILON))
			{
				L += minHit.material->shade(newray, minHit, scene) * speccolor;
			}
			else
			{
				L += scene.bgColor() * m_tp * speccolor;
			}
		}
    }

	L += m_ka * m_texture_ka->shade(ray, bm_hit, scene);

    return L;
}
