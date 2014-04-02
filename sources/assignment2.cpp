#include "assignment2.h"
#include <cmath>
#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"

#include "PointLight.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Phong.h"
#include "CellularStoneTexture.h"
#include "Box.h"
#include "Sphere.h"
#include "Texture.h"
#include "Box.h"

// local helper function declarations
namespace
{
void addMeshTrianglesToScene(TriangleMesh * mesh, Material * material);
inline Matrix4x4 translate(float x, float y, float z);
inline Matrix4x4 scale(float x, float y, float z);
inline Matrix4x4 rotate(float angle, float x, float y, float z);
} // namespace

void makeAllInOneScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 440);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.10f, 0.39f, 0.99f));
    g_camera->setEye(Vector3(10.5, 7.20, 0.59));
    g_camera->setLookAt(Vector3(-2.f, 3.2f, 0.f));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

	g_scene->setSamples(512);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(0, 10.0, 0));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(500);
	light->setBlur(1.0, 0);
    g_scene->addLight(light);

	Phong* material = new Phong(0.0f, 0.85f, Vector3(0.1, 0.1, 0.15));
	material->setGlossy(false);
	material->setPhong(10000);
	Sphere *sphere = new Sphere();
	sphere->setMaterial(material);
	sphere->setRadius(1.2);
	sphere->setCenter(Vector3(-2.f, 1.2f, 0.f));
	g_scene->addObject(sphere);
	
	material = new Phong(0.0f, 0.85f, Vector3(0.1, 0.1, 0.15));
	material->setTransparency(0.80);
	material->setRefraction(1.5);
	material->setGlossy(false);
	material->setPhong(10000);
	sphere = new Sphere();
	sphere->setMaterial(material);
	sphere->setRadius(0.9);
	sphere->setCenter(Vector3(-5.f, 3.2f, 0.f));
	g_scene->addObject(sphere);


	material = new Phong(0.0f, 0.85f, Vector3(0.1, 0.15, 0.1));
	material->setPhong(180);
	material->setGlossy(true);
	sphere = new Sphere();
	sphere->setMaterial(material);
	sphere->setRadius(0.6);
	sphere->setCenter(Vector3(0.9f, 6.5f, -2.61f));
	g_scene->addObject(sphere);

	Matrix4x4 xform;
	xform.setIdentity();
	xform *= translate(-12, 5.5, -1.5);
	xform *= rotate(-90, 0, 1, 0);
	xform *= scale(0.8, 0.8, 0.8);

	material = new Phong(Vector3(0.60, 0.40, 0.40), 0.40f, 0);
	material->setGlossy(false);
	material->setPhong(10000);
	TriangleMesh *bunny = new TriangleMesh;
	bunny->load("bunny.obj", xform);
	addMeshTrianglesToScene(bunny, material);

	material = new Phong(1, 0, 0.2);
    TriangleMesh *mesh = new TriangleMesh;
    mesh->load("sponza.obj");
    addMeshTrianglesToScene(mesh, material);

	xform.setIdentity();
	xform *= translate(0, 0.8, 0);
    xform *= scale(10, 10, 10);

	material = new Phong(1, 0, 0.2);
    TriangleMesh *ocean = new TriangleMesh;
    ocean->load("Ocean.obj", xform);
    addMeshTrianglesToScene(ocean, material);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeCornellScene()
{
	g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(640, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(7.28, 2.58, 1));
    g_camera->setLookAt(Vector3(0.05, 1.6f, -0.8f));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    g_scene->setSamples(16);
    g_scene->setCutoffs(0);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(0.5, 3.99, -0.5));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(100);
    g_scene->addLight(light);

    Phong* material = new Phong(0.9, 0, 0);
    TriangleMesh * teapot = new TriangleMesh;
    teapot->load("cornell_2.obj");
    addMeshTrianglesToScene(teapot, material);

	material = new Phong(0.0f, 0.85f, 0.1f);
	material->setTransparency(0.4);
	material->setRefraction(1.5);

	Sphere *sphere = new Sphere();
	sphere->setMaterial(material);
	sphere->setRadius(1.0f);
	sphere->setCenter(Vector3(-0.7f, 1.0f, 0.7f));
	g_scene->addObject(sphere);

	sphere = new Sphere();
	sphere->setMaterial(material);
	sphere->setRadius(1.0f);
	sphere->setCenter(Vector3(1.3f, 1.0f, -1.8f));
	g_scene->addObject(sphere);


	// make the top triangle
	TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-5, 4.05, -5));
    floor->setV2(Vector3(  0, 4.05,  5));
    floor->setV3(Vector3( 5, 4.05, -5));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
	material = new Phong(0.0, 0, 1);
	material->setKs(0.13);
	material->setIndirectLighting(false);
    t->setMaterial(material); 
    g_scene->addObject(t);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeNoiseBumpScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(800, 300);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.10f, 0.39f, 0.99f));
    g_camera->setEye(Vector3(0, 4.5, 1));
    g_camera->setLookAt(Vector3(0, 1, 1));
    g_camera->setUp(Vector3(0, 0, 1));
    g_camera->setFOV(45);

	g_scene->setSamples(256);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(1500);
	light->setBlur(1.5f, 0);
    g_scene->addLight(light);

	light = new PointLight;
    light->setPosition(Vector3(-10, 10, -10));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(1500);
	light->setBlur(1.5f, 0);
    g_scene->addLight(light);

    Phong* material = new Phong(0.0, 0.7, 0.2);
	material->setTransparency(1.0);
	material->setRefraction(1.33);
	material->setBumpNoisy(8, 4);
	material->setBm(0.40);
	Sphere* sphere = new Sphere();
	sphere->setRadius(1);
	sphere->setCenter(Vector3(-2.3, 1, 1));
	sphere->setMaterial(material);
	g_scene->addObject(sphere);

	material = new Phong(0.0, 0.7, 0.2);
	material->setTransparency(1.0);
	material->setRefraction(1.33);
	material->setBumpNoisy(8, 4);
	material->setBm(0.20);
	sphere = new Sphere();
	sphere->setRadius(1);
	sphere->setCenter(Vector3(0, 1, 1));
	sphere->setMaterial(material);
	g_scene->addObject(sphere);

	material = new Phong(0.0, 0.7, 0.2);
	material->setTransparency(1.0);
	material->setRefraction(1.33);
	sphere = new Sphere();
	sphere->setRadius(1);
	sphere->setCenter(Vector3(2.3, 1, 1));
	sphere->setMaterial(material);
	g_scene->addObject(sphere);
 

    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-50, -5, -50));
    floor->setV2(Vector3(  0, -5,  50));
    floor->setV3(Vector3( 50, -5, -50));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
	material = new Phong(shared_ptr<Material>(new CellularStoneTexture(0.25)), 0.2);
	material->setKs(0.13);
	material->setIndirectLighting(false);
    t->setMaterial(material); 
    g_scene->addObject(t);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
makeGlossyScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.10f, 0.39f, 0.99f));
    g_camera->setEye(Vector3(8, 1.5, 1));
    g_camera->setLookAt(Vector3(0, 2.5, -1));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

	g_scene->setSamples(256);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(0, 10.0, 0));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(300);
    g_scene->addLight(light);

	
	Phong* material = new Phong(0.0f, 0.85f, Vector3(0.1, 0.1, 0.15));
	material->setGlossy(false);
	material->setPhong(10000);
	Sphere *sphere = new Sphere();
	sphere->setMaterial(material);
	sphere->setRadius(0.9);
	sphere->setCenter(Vector3(0.f, 0.5f, -2.f));
	g_scene->addObject(sphere);
	
	material = new Phong(0.0f, 0.85f, Vector3(0.1, 0.15, 0.1));
	material->setPhong(170);
	material->setGlossy(true);
	sphere = new Sphere();
	sphere->setMaterial(material);
	sphere->setRadius(0.9);
	sphere->setCenter(Vector3(0.f, 0.5f, 0.f));
	g_scene->addObject(sphere);


    material = new Phong(0.0f, 0.85f, Vector3(0.15, 0.1, 0.1));
	material->setPhong(40);
	material->setGlossy(true);
	sphere = new Sphere();
	sphere->setMaterial(material);
	sphere->setRadius(0.9);
	sphere->setCenter(Vector3(0.f, 0.5f, 2.f));
	g_scene->addObject(sphere);

	material = new Phong(1, 0, 0.2);
    TriangleMesh *mesh = new TriangleMesh;
    mesh->load("sponza.obj");
    addMeshTrianglesToScene(mesh, material);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
makeDepthScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.10f, 0.39f, 0.99f));
    g_camera->setEye(Vector3(0.08, 3.27, 5.61));
    g_camera->setLookAt(Vector3(0, 1, 2));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

	g_scene->setSamples(256);
	g_scene->setDepthOfField(5.f, 0.25f);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(1800);
	light->setBlur(1.5f, 0);
    g_scene->addLight(light);

    Phong* material = new Phong(0.0, 0, 0.2);
	material->setKs(0.5);
	Sphere* sphere = new Sphere();
	sphere->setRadius(1);
	sphere->setCenter(Vector3(0, 1, 1));
	sphere->setMaterial(material);
	g_scene->addObject(sphere);

	material = new Phong(Vector3(0.2, 0.2, 0.7), 0, 0.2);
	material->setRefraction(1.4);
    material->setTransparency(0.7);
	material->setKs(0.3);
	sphere = new Sphere();
	sphere->setRadius(1);
	sphere->setCenter(Vector3(-3, 1, -2));
	sphere->setMaterial(material);
	g_scene->addObject(sphere);

	material = new Phong(Vector3(0.9, 0.1, 0.2), 0, 0.2);
	material->setKs(0.2);
	sphere = new Sphere();
	sphere->setRadius(1);
	sphere->setCenter(Vector3(3, 1, -6));
	sphere->setMaterial(material);
	g_scene->addObject(sphere);
 
	material = new Phong(Vector3(0.2, 0.2, 0.7), 0, 0.2);
	material->setRefraction(1.4);
    material->setTransparency(0.7);
	material->setKs(0.8);
	sphere = new Sphere();
	sphere->setRadius(1);
	sphere->setCenter(Vector3(3, 1, 6));
	sphere->setMaterial(material);
	g_scene->addObject(sphere);

    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-20, 0, -20));
    floor->setV2(Vector3(  0, 0,  20));
    floor->setV3(Vector3( 20, 0, -20));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
	material = new Phong(shared_ptr<Material>(new CellularStoneTexture(0.25)), 0.2);
	material->setKs(0.13);
	material->setIndirectLighting(false);
    t->setMaterial(material); 
    g_scene->addObject(t);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void 
makeFinalScene()
{
	g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(1, 1, 1));
	//g_scene->setBGColor(Vector3(0., 0., 0.));
    g_camera->setEye(Vector3(8.625, 23.551, 12));
    g_camera->setLookAt(Vector3(4, 12, -60));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    g_scene->setSamples(24);
    g_scene->setCutoffs(0);

    // point light in the cave
    PointLight * light = new PointLight;
    light->setPosition(Vector3(6, 22, -60));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(2000);
    g_scene->addLight(light);

	Sphere *sphere = new Sphere();
	sphere->setRadius(5);
    Phong *transp = new Phong(0.0, 1.0, 0.0);
    transp->setRefraction(1.3);
    transp->setTransparency(0.8);
	sphere->setMaterial(transp);
	sphere->setCenter(Vector3(4, 6, -60));
	g_scene->addObject(sphere);

	sphere = new Sphere();
	sphere->setRadius(4);
	sphere->setMaterial(transp);
	sphere->setCenter(Vector3(16, 6, -60));
	g_scene->addObject(sphere);

	sphere = new Sphere();
	sphere->setRadius(4);
	sphere->setMaterial(transp);
	sphere->setCenter(Vector3(-8, 6, -60));
	g_scene->addObject(sphere);

    Phong* material = new Phong(1, 0.f, 0.4);
	material->setIndirectLighting(false);

	Matrix4x4 mscale;
	mscale.setIdentity();
    mscale *= scale(0.1, 0.1, 0.1);

    TriangleMesh *object = new TriangleMesh;
    object->load("cave_hd.obj");
    addMeshTrianglesToScene(object, material);

	// lights in the cave
	float light_watt = 1200;
	float x_1 = 2;
	float x_2 = 12.5;
	float y_1 = 15;
	float y_2 = 24;

	light = new PointLight;
    light->setPosition(Vector3(x_1, y_2, 125.5));
    light->setColor(Vector3(0.3377f, 0.3019f, 0.0627f));
    light->setWattage(light_watt * 0.1);
    g_scene->addLight(light);

	light = new PointLight;
    light->setPosition(Vector3(x_1, y_2, 53));
    light->setColor(Vector3(0.8156f, 0.7843f, 0.7294f));
    light->setWattage(light_watt * 0.25);
    g_scene->addLight(light);

	light = new PointLight;
    light->setPosition(Vector3(x_1, y_2, -19));
    light->setColor(Vector3(0.7921f, 0.7137f, 0.3960f));
    light->setWattage(light_watt * 0.25);
    g_scene->addLight(light);

	light = new PointLight;
    light->setPosition(Vector3(x_1, y_2, -91));
    light->setColor(Vector3(0.6627f, 0.8313f, 0.9254f));
    light->setWattage(light_watt * 0.25);
    g_scene->addLight(light);

	light = new PointLight;
    light->setPosition(Vector3(x_1, y_2, -163));
    light->setColor(Vector3(0.8470f, 0.9960f, 0.9607f));
    light->setWattage(light_watt * 0.55);
    g_scene->addLight(light);

    g_scene->preCalc();
}

void
makeSpecialScene()
{
	g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 3, 6));
    g_camera->setLookAt(Vector3(3, 2.5, -1.5));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    g_scene->setSamples(100);
    g_scene->setCutoffs(0);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(2.7, 2.5, 5.7));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(500);
    light->setBlur(0, 0);
    g_scene->addLight(light);

    Phong* material = new Phong(0.9, 0, 0);
    TriangleMesh * teapot = new TriangleMesh;
    teapot->load("cornell_box.obj");
    addMeshTrianglesToScene(teapot, material);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
makeBoxScene()
{
	g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 3, 6));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(700);
    g_scene->addLight(light);

    
	Material* material = new Phong(shared_ptr<Material>(new CellularStoneTexture(0.25)), 0.2);
	TriangleMesh * floor = new TriangleMesh;
    
	floor->createSingleTriangle();
    floor->setV1(Vector3(-10, 0, -10));
    floor->setV2(Vector3(  0, 0,  10));
    floor->setV3(Vector3( 10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(material); 
    g_scene->addObject(t);

	Box *b = new Box(Vector3(0.f), Vector3(2));
	b->setMaterial(material);
	g_scene->addObject(b);
	
	g_scene->preCalc();
}

void
makeTeapotScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 3, 6));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(700);
    g_scene->addLight(light);

    Material* material = new Phong(1, 0, 0.2);
    TriangleMesh * teapot = new TriangleMesh;
    teapot->load("teapot.obj");
    addMeshTrianglesToScene(teapot, material);
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-10, 0, -10));
    floor->setV2(Vector3(  0, 0,  10));
    floor->setV3(Vector3( 10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(material); 
    g_scene->addObject(t);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}


void
makeBunny1Scene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
	g_scene->setBGColor(Vector3(0.10f, 0.39f, 0.99f));
    g_camera->setEye(Vector3(0, 5, 15));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);
    g_scene->setSamples(100);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(3, 5, 2));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(800);
    g_scene->addLight(light);


    Material* material = new Phong(1, 0, 0.2);
    TriangleMesh * bunny = new TriangleMesh;
    bunny->load("Ocean.obj");
    addMeshTrianglesToScene(bunny, material);

    //Sphere *sphere = new Sphere();
    //sphere->setCenter(Vector3(0, 3, 0));
    //sphere->setRadius(2);
    //Phong *sphmat = new Phong(0, Vector3(1, 1, 1), 0);
    //sphmat->setRefraction(1.3);
    //sphmat->setTransparency(1);
    //sphere->setMaterial(sphmat);
    //g_scene->addObject(sphere);
    
    // create the floor triangle
    //TriangleMesh * floor = new TriangleMesh;
    //floor->createSingleTriangle();
    //floor->setV1(Vector3(-100, 0, -100));
    //floor->setV2(Vector3(   0, 0,  100));
    //floor->setV3(Vector3( 100, 0, -100));
    //floor->setN1(Vector3(0, 1, 0));
    //floor->setN2(Vector3(0, 1, 0));
    //floor->setN3(Vector3(0, 1, 0));
    //
    //Triangle* t = new Triangle;
    //t->setIndex(0);
    //t->setMesh(floor);
    //t->setMaterial(material); 
    //g_scene->addObject(t);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}



void
makeBunny20Scene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 5, 15));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(4, 6, 1));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(300);
    g_scene->addLight(light);

    TriangleMesh * mesh;
    Phong* material = new Phong(shared_ptr<Material>(new CellularStoneTexture(0.25)), 0);
    material->setIndirectLighting(false);
    Matrix4x4 xform;
    Matrix4x4 xform2;
    xform2 *= rotate(110, 0, 1, 0);
    xform2 *= scale(.6, 1, 1.1);


    // bunny 1
    xform.setIdentity();
    xform *= scale(0.3, 2.0, 0.7);
    xform *= translate(-1, .4, .3);
    xform *= rotate(25, .3, .1, .6);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 2
    xform.setIdentity();
    xform *= scale(.6, 1.2, .9);
    xform *= translate(7.6, .8, .6);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 3
    xform.setIdentity();
    xform *= translate(.7, 0, -2);
    xform *= rotate(120, 0, .6, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 4
    xform.setIdentity();
    xform *= translate(3.6, 3, -1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 5
    xform.setIdentity();
    xform *= translate(-2.4, 2, 3);
    xform *= scale(1, .8, 2);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 6
    xform.setIdentity();
    xform *= translate(5.5, -.5, 1);
    xform *= scale(1, 2, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 7
    xform.setIdentity();
    xform *= rotate(15, 0, 0, 1);
    xform *= translate(-4, -.5, -6);
    xform *= scale(1, 2, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 8
    xform.setIdentity();
    xform *= rotate(60, 0, 1, 0);
    xform *= translate(5, .1, 3);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 9
    xform.setIdentity();
    xform *= translate(-3, .4, 6);
    xform *= rotate(-30, 0, 1, 0);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 10
    xform.setIdentity();
    xform *= translate(3, 0.5, -2);
    xform *= rotate(180, 0, 1, 0);
    xform *= scale(1.5, 1.5, 1.5);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 11
    xform = xform2;
    xform *= scale(0.3, 2.0, 0.7);
    xform *= translate(-1, .4, .3);
    xform *= rotate(25, .3, .1, .6);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 12
    xform = xform2;
    xform *= scale(.6, 1.2, .9);
    xform *= translate(7.6, .8, .6);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 13
    xform = xform2;
    xform *= translate(.7, 0, -2);
    xform *= rotate(120, 0, .6, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 14
    xform = xform2;
    xform *= translate(3.6, 3, -1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 15
    xform = xform2;
    xform *= translate(-2.4, 2, 3);
    xform *= scale(1, .8, 2);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 16
    xform = xform2;
    xform *= translate(5.5, -.5, 1);
    xform *= scale(1, 2, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 17
    xform = xform2;
    xform *= rotate(15, 0, 0, 1);
    xform *= translate(-4, -.5, -6);
    xform *= scale(1, 2, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 18
    xform = xform2;
    xform *= rotate(60, 0, 1, 0);
    xform *= translate(5, .1, 3);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 19
    xform = xform2;
    xform *= translate(-3, .4, 6);
    xform *= rotate(-30, 0, 1, 0);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 20
    xform = xform2;
    xform *= translate(3, 0.5, -2);
    xform *= rotate(180, 0, 1, 0);
    xform *= scale(1.5, 1.5, 1.5);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);


    // create the floor triangle
    mesh = new TriangleMesh;
    mesh->createSingleTriangle();
    mesh->setV1(Vector3(-100, 0, -100));
    mesh->setV2(Vector3(   0, 0,  100));
    mesh->setV3(Vector3( 100, 0, -100));
    mesh->setN1(Vector3(0, 1, 0));
    mesh->setN2(Vector3(0, 1, 0));
    mesh->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(mesh);
    Phong *triangle_mat = new Phong(1, 0, 0.2);
    triangle_mat->setIndirectLighting(false);
    t->setMaterial(triangle_mat); 
    g_scene->addObject(t);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}


void
makeSponzaScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_scene->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(8, 1.5, 1));
    g_camera->setLookAt(Vector3(0, 2.5, -1));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(55);
    g_scene->setAntiAliasing(4, 4);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(0, 10.0, 0));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(200);
    g_scene->addLight(light);

    Material* material = new Phong(1, 0, 0.2);
    TriangleMesh * mesh = new TriangleMesh;
    mesh->load("sponza.obj");
    addMeshTrianglesToScene(mesh, material);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}


// local helper function definitions
namespace
{

void
addMeshTrianglesToScene(TriangleMesh * mesh, Material * material)
{
    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < mesh->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(mesh);
		Material *mat = mesh->getMaterialForId(i);
		if (mat == NULL)
			mat = material;
        t->setMaterial(mat); 
        g_scene->addObject(t);
    }
}


inline Matrix4x4
translate(float x, float y, float z)
{
    Matrix4x4 m;
    m.setColumn4(Vector4(x, y, z, 1));
    return m;
}


inline Matrix4x4
scale(float x, float y, float z)
{
    Matrix4x4 m;
    m.m11 = x;
    m.m22 = y;
    m.m33 = z;
    return m;
}

// angle is in degrees
inline Matrix4x4
rotate(float angle, float x, float y, float z)
{
    float rad = angle*(PI/180.);
    
    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;
    float c = cos(rad);
    float cinv = 1-c;
    float s = sin(rad);
    float xy = x*y;
    float xz = x*z;
    float yz = y*z;
    float xs = x*s;
    float ys = y*s;
    float zs = z*s;
    float xzcinv = xz*cinv;
    float xycinv = xy*cinv;
    float yzcinv = yz*cinv;
    
    Matrix4x4 m;
    m.set(x2 + c*(1-x2), xy*cinv+zs, xzcinv - ys, 0,
          xycinv - zs, y2 + c*(1-y2), yzcinv + xs, 0,
          xzcinv + ys, yzcinv - xs, z2 + c*(1-z2), 0,
          0, 0, 0, 1);
    return m;
}

} // namespace


