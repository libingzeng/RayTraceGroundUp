#ifndef GLOBALTRACE_H
#define GLOBALTRACE_H

#include "Tracer.h"


class GlobalTrace: public Tracer {
	public:

		GlobalTrace(void);

		GlobalTrace(World* _worldPtr);

		virtual RGBColor
		trace_ray(const Ray ray, const int depth) const;

		virtual RGBColor
		trace_ray(const Ray ray, double& tmin, const int depth) const;
};

#endif // GLOBALTRACE_H
