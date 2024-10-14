#pragma once
#include "WorkerPool.h"
#include "BackfaceCullingEngine.h"
#include "ProjectionEngine.h"
#include "DrawingEngine.h"
#include "LightingEngine.h"

namespace TorGL
{
	/** The main class to interface with the rendering engine.
	* You basically don't have to interact with anything else but a few data structures to get things going!
	*/
	class Tornado
	{
	public:
		Tornado(const Vector2i& renderTargetSize, std::size_t numRenderthreads, double globalIllumination = 0);
		~Tornado();

		//! Will initialize the rendering of a new frame.
		void BeginFrame();

		//! Will register a RenderTriangle3D to be rendered.
		void RegisterRender(const RenderTriangle3D* tri);

		//! Will register a RenderLightSource to be rendered
		void RegisterRender(const RenderLightSource* lightSource);

		//! Will execute the render
		void Render(const ProjectionProperties& projectionProperties, const Matrix4x4 worldMatrix);

		//! Will return the pixel buffer with the rendered pixel data.
		const PixelBuffer<3>* GetPixelBuffer() const;

	private:
		WorkerPool* workerPool;
		BackfaceCullingEngine* backfaceCullingEngine;
		ProjectionEngine* projectionEngine;
		DrawingEngine* drawingEngine;
		PixelBuffer<3>* pixelBuffer;

		std::vector<const RenderTriangle3D*> registeredTriangles;
		std::vector<const RenderLightSource*> registeredLightsources;

        // Benchmarking stuff
        #ifdef _BENCHMARK_CONTEXT
        double _benchmark_perspectiveProjectionTime;
        double _benchmark_cullBackfacesTime;
        double _benchmark_drawTrianglesTime;

        public:
        const inline double _benchmark_GetPerspectiveProjectionTime() {
            return _benchmark_perspectiveProjectionTime;
        }
        const inline double _benchmark_GetCullBackfacesTime() {
            return _benchmark_cullBackfacesTime;
        }
        const inline double _benchmark_GetDrawTrianglesTime() {
            return _benchmark_drawTrianglesTime;
        }
        private:
        #endif
	};
}
