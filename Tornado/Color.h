#pragma once
#include <cstdint>

struct Color
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 0;

	static const Color black;
	static const Color gray;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color yellow;
	static const Color cyan;
	static const Color pink;
	static const Color white;
};

const Color Color::black  = {   0,   0,   0, 255 };
const Color Color::gray   = { 128, 128, 128, 255 };
const Color Color::red    = { 255,   0,   0, 255 };
const Color Color::green  = {   0, 255,   0, 255 };
const Color Color::blue   = {   0,   0, 255, 255 };
const Color Color::yellow = { 255, 255,   0, 255 };
const Color Color::cyan   = {   0, 255, 255, 255 };
const Color Color::pink   = { 255,   0, 255, 255 };
const Color Color::white  = {  255, 255, 255, 255 };
