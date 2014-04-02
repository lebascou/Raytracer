#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/* This is the photon
 * The power is not compressed so the
 * size is 28 bytes
*/
//**********************
typedef struct Photon {
//**********************
  float pos[3];                 // photon position
  short plane;                  // splitting plane for kd-tree
  unsigned char theta, phi;     // incoming direction
  float power[3];               // photon power (uncompressed)
} Photon;


/* This structure is used only to locate the
 * nearest photons
*/
//******************************
typedef struct NearestPhotons {
//******************************
  int max;
  int found;
  int got_heap;
  float pos[3];
  float *dist2;
  const Photon **index;
} NearestPhotons;


/* This is the Photon_map class
 */
//*****************
class Photon_map {
//*****************
public:
  Photon_map( int max_phot );
  ~Photon_map();

  void store(
    const float power[3],          // photon power
    const float pos[3],            // photon position
    const float dir[3] );          // photon direction

  void scale_photon_power(
    const float scale );           // 1/(number of emitted photons)

  void balance(void);              // balance the kd-tree (before use!)

  void irradiance_estimate(
    float irrad[3],                // returned irradiance
    const float pos[3],            // surface position
    const float normal[3],         // surface normal at pos
    const float max_dist,          // max distance to look for photons
    const int nphotons ) const;    // number of photons to use

  void locate_photons(
    NearestPhotons *const np,      // np is used to locate the photons
    const int index ) const;       // call with index = 1

  void photon_dir(
    float *dir,                    // direction of photon (returned)
    const Photon *p ) const;       // the photon

  void renderGL() const;

  bool isCaustics() const {return m_caustics;}
  void setCaustics(bool c) {m_caustics = c;}

private:

  void balance_segment(
    Photon **pbal,
    Photon **porg,
    const int index,
    const int start,
    const int end );

  void median_split(
    Photon **p,
    const int start,
    const int end,
    const int median,
    const int axis );
  
  Photon *photons;

  int stored_photons;
  int half_stored_photons;
  int max_photons;
  int prev_scale;

  float costheta[256];
  float sintheta[256];
  float cosphi[256];
  float sinphi[256];
  
  float bbox_min[3];		// use bbox_min;
  float bbox_max[3];		// use bbox_max;
  bool m_caustics;
};

// #include <vector>
// #include <stdint.h>
// #include <queue>
// #include <boost/function.hpp>
// #include "Vector3.h"
// #include "Miro.h"

// struct Photon
// {
// 	Vector3 position;
// 	Vector3 power;
// 	Vector3 dir;
// };

// class Photon_map
// {
// public:
// 	Photon_map() : m_root(0) {}

// 	void addPhoton(Photon const &ph) {m_photons.push_back(new Photon(ph));}

// 	void build();

// 	void kNearest(Vector3 const &v, int k, std::vector<Photon *> &result) const;

// 	void renderGL() const;

// 	~Photon_map() {delete m_root;}

// private:
// 	struct PhotonNode
// 	{
// 		Photon *photon;
// 		PhotonNode *left;

// 		PhotonNode() : photon(0), left(0) {}

// 		~PhotonNode()
// 		{
// 			delete photon;
// 			if (left)
// 			{
// 				delete[] (PhotonNode*)((uintptr_t) left & ~3);
// 			}
// 		}
// 	};

// 	typedef std::priority_queue<Photon *, std::vector<Photon *>, boost::function<bool (Photon *, Photon *)> > Heap;
// 	void tryInsertPhoton(Vector3 const &p, int k, Photon *photon, Heap &result) const;
// 	void recKNearest(PhotonNode *node, Vector3 const &v, int k, Heap &result) const;
// 	void recursiveBuild(PhotonNode *node, int axis, std::vector<Photon*>::iterator start, std::vector<Photon*>::iterator end);

// 	std::vector<Photon *> m_photons;
// 	PhotonNode *m_root;
// };
