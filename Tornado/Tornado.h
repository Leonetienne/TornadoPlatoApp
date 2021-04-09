#pragma once
#include "WorkerPool.h"
#include "ProjectionEngine.h"
#include "DrawingEngine.h"

class Tornado
{
public:
	Tornado(const Vector2i& renderTargetSize, std::size_t numRenderthreads);
	~Tornado();

	// Will initialize the rendering of a new frame.
	void BeginFrame();

	// Will register a RenderTriangle3D to be rendered.
	void RegisterRender(const RenderTriangle3D* tri);

	// Will execute the render
	void Render(const ProjectionProperties& projectionProperties, const Matrix4x4 worldMatrix);

	// Will return the pixel buffer with the rendered pixel data.
	const PixelBuffer<3>* GetPixelBuffer() const;

private:
	WorkerPool* workerPool;
	ProjectionEngine* projectionEngine;
	DrawingEngine* drawingEngine;
	PixelBuffer<3>* pixelBuffer;

	std::vector<const RenderTriangle3D*> registeredTriangles;
};
