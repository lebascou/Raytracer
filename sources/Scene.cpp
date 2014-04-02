#include "boost/threadpool.hpp"
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#include <boost/timer/timer.hpp>

Scene * g_scene = 0;

Scene::Scene()  : m_bgColor(0.f), m_global_photons(400000), m_caustics_photons(400000), m_samples(1), m_cutoffs(0), m_focus_length(-1.f), m_lens(0.25f) {}

void
Scene::openGL(Camera *cam)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam->drawGL();

    // draw objects
    for (size_t i = 0; i < m_objects.size(); ++i)
		m_objects[i]->renderGL();
	for (size_t i = 0; i < m_lights.size(); ++i)
	{
		glColor3f(1, 1, 0.2);
		glPushMatrix();
		glTranslatef(m_lights[i]->position().x, m_lights[i]->position().y, m_lights[i]->position().z);
		glutSolidSphere(1, 10, 10);
		glPopMatrix();
		glColor3f(1, 1, 1);
	}
    g_caustics_map->renderGL();

    glutSwapBuffers();
}

void Scene::sampleMap(Photon_map *map, int n_photons, float total_wattage)
{
    Lights::iterator lit;
    std::vector<float> refr_stack;
    int total = 0;
    for (lit = m_lights.begin(); lit != m_lights.end(); ++lit)
    {
        int photons = (*lit)->wattage() * n_photons / total_wattage;
        float power = (*lit)->wattage() / photons;
        for (int i = 0; i < photons; ++i)
        {
            if (total % 1000 == 0)
			{
                printf("%.2f%%\r", 100 * total / ((float) n_photons));
				fflush(stdout);
			}
            total++;
            Ray ray;
            do
            {
                ray.d.x = 2*randone(g_rng) - 1;
                ray.d.y = 2*randone(g_rng) - 1;
                ray.d.z = 2*randone(g_rng) - 1;
            } while (ray.d.x*ray.d.x + ray.d.y*ray.d.y + ray.d.z*ray.d.z > 1);
            ray.d.normalize();
            ray.o = (*lit)->position();
            refr_stack.clear();
            refr_stack.push_back(1.f);
            ray.refractionStack = &refr_stack;
            ray.refractionIndex = 0;
            HitInfo h;
            if (trace(h, ray))
                h.material->shadePhoton(ray, h, *this, power * (*lit)->color(), map);
        }
    }
    printf("100.00%\n");
    map->balance();
}

void
Scene::preCalc()
{
    Objects::iterator it;
    for (it = m_objects.begin(); it != m_objects.end(); it++)
    {
        Object* pObject = *it;
        pObject->preCalc();
    }
    Lights::iterator lit;
    float total_wattage = 0.f;
    for (lit = m_lights.begin(); lit != m_lights.end(); lit++)
    {
        PointLight* pLight = *lit;
        pLight->preCalc();
        total_wattage += pLight->wattage();
    }

    m_bvh.build(&m_objects);

    delete g_global_illum_map;
    g_global_illum_map = new Photon_map(m_global_photons);
    delete g_caustics_map;
    g_caustics_map = new Photon_map(m_caustics_photons);
    g_caustics_map->setCaustics(true);

    printf("Caustics map:\n");
    sampleMap(g_caustics_map, m_caustics_photons, total_wattage);
    printf("Global illum map:\n");
    sampleMap(g_global_illum_map, m_global_photons, total_wattage);
}

std::vector<Vector3 *> Scene::traceLine(Camera const *cam, Image const *img, int j) const
{
    std::vector<Vector3 *> results(img->width());
    Ray ray;
    HitInfo hitInfo;
    Vector3 shadeResult;
    std::vector<float> refr_stack;
    std::vector<Vector3> colors(m_samples);
    for (int i = 0; i < img->width(); ++i)
    {
		for (int k = 0; k < m_samples; ++k)
		{
			ray = cam->eyeRay(i + (m_samples == 1 ? 0.5f : randone(g_rng) - 0.5f), j + (m_samples == 1 ? 0.5f : randone(g_rng) - 0.5f), img->width(), img->height());
			if (m_focus_length >= 0.f) // if depth of field enabled
			{
				Vector3 focal_point = ray.o + m_focus_length * ray.d;
				Vector3 view_dir = cam->viewDir();
				Vector3 d_vec(randone(g_rng) - 0.5f, randone(g_rng) - 0.5f, randone(g_rng) - 0.5f);
				d_vec.normalize();
				d_vec = view_dir.cross(d_vec);
					
				ray.o += d_vec * randone(g_rng) * m_lens;
				ray.d = focal_point - ray.o;
				ray.d.normalize();
			}

			refr_stack.clear();
			refr_stack.push_back(1.f);
			ray.refractionStack = &refr_stack;
			ray.refractionIndex = 0;
			if (!results[i])
				results[i] = new Vector3();
			if (trace(hitInfo, ray))
			{
				colors[k] = hitInfo.material->shade(ray, hitInfo, *this);
			}
			else
			{
				colors[k] = bgColor();
			}
			*results[i] += colors[k];
		}
		if (m_cutoffs > 0)
		{
			std::vector<int> biggest(m_cutoffs, -1);
			for (int c = 0; c < m_cutoffs; ++c)
			{
				if (biggest[0] == -1 || colors[c].length2() > colors[biggest[0]].length2())
				{
					biggest[0] = c;
					for (int k = 1; k < m_cutoffs; ++k)
					{
						if (biggest[k] == -1 || colors[biggest[k - 1]].length2() > colors[biggest[k]].length2())
							std::swap(biggest[k - 1], biggest[k]);
						else
							break;
					}
				}
			}
			for (int k = 0; k < m_cutoffs; ++k)
				*results[i] -= colors[biggest[k]];
		}
		if (results[i])
			*results[i] /= m_samples - m_cutoffs;
    }
    return results;
}

void
Scene::raytraceImage(Camera *cam, Image *img)
{
    boost::timer::auto_cpu_timer t;
    boost::threadpool::pool threadpool(7);//nCpus() * 2);
    std::vector<boost::packaged_task<std::vector<Vector3 *> > * > tasks;
    std::vector<boost::unique_future<std::vector<Vector3 *> > * > lines;

    // loop over all pixels in the image
    {
        for (int j = 0; j < img->height(); ++j)
        {
            tasks.push_back(new boost::packaged_task<std::vector<Vector3 *> >(boost::bind(&Scene::traceLine, this, cam, img, j)));
            lines.push_back(new boost::unique_future<std::vector<Vector3 *> >(tasks.back()->get_future()));
            boost::threadpool::schedule(threadpool, boost::bind(&boost::packaged_task<std::vector<Vector3 *> >::operator(), tasks.back()));
        }
		printf("Rendering Progress: %.3f%%\r", 0.f);
        for (int j = 0; j < img->height(); ++j)
        {
            for (int i = 0; i < img->width(); ++i)
            {
                if (lines[j]->get()[i])
                {
                    img->setPixel(i, j, *lines[j]->get()[i]);
                    delete lines[j]->get()[i];
                }
            }
			img->drawScanline(j);
			if (j + 1 == img->height() || !lines[j + 1]->has_value())
				glFinish();
			printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
			fflush(stdout);
		}
    }
    printf("Rendering Progress: 100.000%\n");
    debug("done Raytracing!\n");
}

bool
Scene::trace(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
    return m_bvh.intersect(minHit, ray, tMin, tMax);
}

void
Scene::setDepthOfField(float focus_length, float lens)
{
	m_focus_length = focus_length;
	m_lens = lens;
}
