#pragma once
struct PerformanceMetric {
    double totalFrameTime;
    double uptateHooks;
    double render;
    double platorender_beginFrame;
    double platorender_resolveCameraSpaceVertices;
    double platorender_registerTornadoObjects;
    double tornadorender_beginFrame;
    double tornadorender_render;
    double tornadorender_render_perspectiveProjection;
    double tornadorender_render_cullBackfaces;
    double tornadorender_render_drawTriangles;
    double sdlDraw;
};

