#pragma once
#include "PixelBuffer.h"
#include "WorkerPool.h"
#include "InterRenderTriangle.h"
#include "../Eule/Rect.h"
#include "LightingEngine.h"

namespace TorGL
{
	/** Engine to draw InterRenderTriangle's to the screen.
	* These IRDs have to be completely processed (e.g. projected and clipped)
	*/
	class DrawingEngine
	{
	public:
		DrawingEngine(PixelBuffer<3>* renderTarget, WorkerPool* workerPool);
		~DrawingEngine();

		//! Will initialize the new drawing sequence
		void BeginBatch(std::size_t reservesize_triangles = 0);

		//! Will register an InterRenderTriangle to be drawn
		void RegisterInterRenderTriangle(const InterRenderTriangle* tri);

		//! Faster way to register a lot of InterRenderTriangle's at once using std::move. This consumes the original vector.
		void HardsetInterRenderTriangles(std::vector<const InterRenderTriangle*>&& triangles);

		//! Will create and start the drawing tasks and allocate resources accordingly.  
		//! Will freeze the main (calling) thread, until the drawing has been finished.
		void Draw();

	private:
		//! Will calculate cached values that only need to be calculated once ber InterRenderTriangle.
		//! Call before running its compute task!!
		void CalculateRenderingRelatedCaches_IRD(const InterRenderTriangle* ird);

		//! Will distribute drawing tasks based on a triangles screen size
		void CreateTasks();

		//! Will execute the tasks
		void ComputeTasks();

		//! Main drawing method for the tasks
		void Thread_Draw(const InterRenderTriangle* ird, const Eule::Rect& bounds);

		//! Will draw a single pixel
		void Thread_PixelShader(const InterRenderTriangle* ird, uint8_t* pixelBase, const Vector2d& pixelPosition, std::array<double, 5>* berp_cache, double z);

		WorkerPool* workerPool;
		PixelBuffer<3>* renderTarget;

		double* zBuffer;
		std::size_t numPixels;
		std::vector<const InterRenderTriangle*> registeredTriangles;
	};
}
