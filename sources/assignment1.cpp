#include "assignment1.h"
#include <cmath>
#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#include "PointLight.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Phong.h"
#include "CellularStoneTexture.h"

void
a1_makeBunnyScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(128, 128);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(500);
    g_scene->addLight(light);

    Phong* mat = new Phong(shared_ptr<Material>(new CellularStoneTexture(0.25)), 0.2);
    Phong* mat2 = new Phong(1, 0, 0.2);
    //mat = mat2;
    mat->setPhong(10);
    TriangleMesh * bunny = new TriangleMesh;
    bunny->load("bunny_smooth.obj");
    
    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < bunny->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(bunny);
        t->setMaterial(mat); 
        g_scene->addObject(t);
    }
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(mat2); 
    g_scene->addObject(t);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
a1_makeTeapotScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(128, 128);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.8f, 0.8f, 1.f));
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 15, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setBlur(0.6, 10);
    light->setWattage(500);
    g_scene->addLight(light);
    g_scene->setAntiAliasing(1, 1);

    Phong* mat = new Phong(shared_ptr<Material>(new CellularStoneTexture(0.2f)), 0.2);
    mat->setKd(0.8);
    mat->setKs(0);
    mat->setPhong(100);
    Phong* mat2 = new Phong(shared_ptr<Material>(new CellularStoneTexture(1.f)), Vector3(0.f));
    mat2->setKd(0.8);
    mat2->setKs(0.2);
    mat2->setPhong(100);

    TriangleMesh * teapot = new TriangleMesh;
    teapot->load("teapot.obj");
    
    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < teapot->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(teapot);
        t->setMaterial(mat); 
        g_scene->addObject(t);
    }
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(mat2); 
    g_scene->addObject(t);

    Phong *transp = new Phong(1, 1, 0.0);
    transp->setKd(0.2);
    transp->setRefraction(1.3);
    transp->setTransparency(0.8);
    transp->setPhong(30);
    transp->setCastShadow(false);
    Sphere * sphere = new Sphere();
    sphere->setCenter(Vector3(-1.0, 1.4, 0));
    sphere->setRadius(0.8);
    sphere->setMaterial(transp);
    g_scene->addObject(sphere);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
a1_makeSphereScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(128, 128);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.0f));
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-10, 10, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(500);
    g_scene->addLight(light);
    g_scene->setBGColor(Vector3(0.8, 0.8, 1));

    Material* mat = new Phong(Vector3(1.0f), 0, 0);

    TriangleMesh * sphere = new TriangleMesh;
    sphere->load("sphere.obj");
    
    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < sphere->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(sphere);
        t->setMaterial(mat); 
        g_scene->addObject(t);
    }
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
a1_makeTextureScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(800, 800);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.8f, 0.8f, 1.0f));
    g_camera->setEye(Vector3(-1, 10, 10));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-6, 12, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(300);
    g_scene->addLight(light);

    Material* mat = new Phong(shared_ptr<Material>(new CellularStoneTexture()), 0.1);

	//Material* mat = new CellularStoneTexture(Vector3(.5f, .0f, .2f));

    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(mat); 
    g_scene->addObject(t);

	Sphere* sphere = new Sphere();
    sphere->setMaterial(mat);
	sphere->setCenter(Vector3(0, 0, 0));
	sphere->setRadius(2);
    g_scene->addObject(sphere);
    
	sphere = new Sphere();
    sphere->setMaterial(mat);
	sphere->setCenter(Vector3(4, 0, 0));
	sphere->setRadius(2);
    g_scene->addObject(sphere);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}
