#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#include "PointLight.h"
#include "Sphere.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Phong.h"
#include "MiroWindow.h"
#include "assignment1.h"
#include "assignment2.h"

boost::mt19937 g_rng;
//boost::uniform_01<float> randone;

void
makeSpiralScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
    g_camera->setEye(Vector3(-5, 2, 3));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(1000);
    g_scene->addLight(light);

    // create a spiral of spheres
    Material* mat = new Phong(Vector3(1.0f, 0.0f, 0.0f), 0, 0);
    const int maxI = 200;
    const float a = 0.15f;
    for (int i = 1; i < maxI; ++i)
    {
        float t = i/float(maxI);
        float theta = 4*PI*t;
        float r = a*theta;
        float x = r*cos(theta);
        float y = r*sin(theta);
        float z = 2*(2*PI*a - r);
        Sphere * sphere = new Sphere;
        sphere->setCenter(Vector3(x,y,z));
        sphere->setRadius(r/10);
        sphere->setMaterial(mat);
        g_scene->addObject(sphere);
    }
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
makeLorenzScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
    g_camera->setEye(Vector3(-5, 2, 150));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(0, 0, 55));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(1000);
    g_scene->addLight(light);

    // create a spiral of spheres
    Material* mat = new Phong(Vector3(1.0f, 0.0f, 0.0f), 0, 0);
    const int maxI = 10000;
    const float a = 0.15f;
    const float pi = 28;
    const float theta = 10;
    const float beta = 8.0/3.0;
    const float dt = 0.02;
    float x = 0, y = 1, z = 1.05;
    float t = 0.01;
    float t2;
    float minx = -10000, miny = -10000, minz = -10000;
    for (int i = 1; i < maxI; ++i)
    {
        t = i * dt;
        t2 = (i % 100) / 100.0;
        float theta2 = 4*PI*t;
        float r = a*theta2;
        x += theta * (y - x) * dt;
        y += (x * (pi - z) - y) * dt;
        z += (x*y - beta*z)*dt;
        Sphere * sphere = new Sphere;
        sphere->setCenter(Vector3(x,y,z));
        sphere->setRadius(0.005 * t);
        sphere->setMaterial(new Phong(Vector3((i % 100) / 100.0, 0.5 * (i % 30) / 30.0, ((i % 20) / 20.0)), 0, 0));
        g_scene->addObject(sphere);
        if (x > minx)
            minx = x;
        if (y > miny)
            miny = y;
        if (z > minz)
            minz = z;
    }
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}


int
main(int argc, char*argv[])
{
	srand(time(NULL));
    // create a scene
	makeAllInOneScene();

    MiroWindow miro(&argc, argv);
    miro.mainLoop();

    return 0; // never executed
}

