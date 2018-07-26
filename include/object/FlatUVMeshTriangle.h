#ifndef FLATUVMESHTRIANGLE_H
#define FLATUVMESHTRIANGLE_H

#include "FlatMeshTriangle.h"


class FlatUVMeshTriangle : public FlatMeshTriangle
{
    public:
		FlatUVMeshTriangle(void);

		FlatUVMeshTriangle(Mesh* _meshPtr, const int i0, const int i1, const int i2);

		FlatUVMeshTriangle(const FlatUVMeshTriangle& fmt);

		virtual
		~FlatUVMeshTriangle(void);

		virtual	bool
		hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

		virtual	bool
		shadow_hit(const Ray& ray, double& tmin) const;
};

#endif // FLATUVMESHTRIANGLE_H
