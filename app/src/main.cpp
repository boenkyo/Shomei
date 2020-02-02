#include <algorithm>
#include <cmath>
#include <cstdint>
#include <execution>
#include <iostream>
#include <vector>

#include <SDL.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vector_relational.hpp>

#include "vertex_buffer.hpp"

uint32_t to_color(const glm::vec4& vec) {
	return (uint32_t(vec.r * 255.0f) << 24) | (uint32_t(vec.g * 255.0f) << 16) |
	       (uint32_t(vec.b * 255.0f) << 8) | (uint32_t(vec.a * 255.0f));
}

glm::vec2 ndc_to_viewport(const glm::vec2 v, int w, int h) {
	return {
	   (v.x + 1.0f) * 0.5f * w,
	   (v.y + 1.0f) * 0.5f * h,
	};
}

int main(int argc, char* args[]) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
	   "Shomei", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

	int w;
	int h;
	SDL_GetWindowSize(window, &w, &h);

	SDL_Renderer* renderer =
	   SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* tex = SDL_CreateTexture(
	   renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, w, h);

	std::vector<std::uint32_t> pixels;
	pixels.resize(w * h);

	// clang-format off
   float vertices[] {
      // position          // color
      -0.5f, 0.0f,   1.0f, 0.8f, 0.0f,
       0.0f, 0.8f,   0.8f, 0.0f, 1.0f,
       0.5f, 0.2f,   1.0f, 0.8f, 0.0f,
   };
	// clang-format on
   std::size_t size = sizeof(vertices) / sizeof(float);

	sm::VertexBuffer<float> buffer(vertices, size);

	sm::Attribute pos_attribute;
	pos_attribute.dimension = sm::Dimension::Two;
	pos_attribute.index     = 0;
	pos_attribute.offset    = 0;
	pos_attribute.stride    = 5;
	buffer.def_attribute(pos_attribute);

	sm::Attribute color_attribute;
	color_attribute.dimension = sm::Dimension::Three;
	color_attribute.index     = 1;
	color_attribute.offset    = 2;
	color_attribute.stride    = 5;
	buffer.def_attribute(color_attribute);

	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) exit(0);
		}

      buffer.set_data(vertices, size);

		std::for_each(std::execution::seq,
		              buffer.vertex_indices().begin(),
		              buffer.vertex_indices().end(),
		              [&](auto& n) {
			              // vertex in attributes
			              sm::AttributePtr pos_ptr = buffer.attribute_ptr(0, n);
			              float* pos               = pos_ptr.data;

			              // vertex shader
			              glm::vec2 vertex_pos =
			                 ndc_to_viewport(glm::make_vec2(pos), w, h);
			              // end vertex shader

			              float* vertex_pos_ptr = glm::value_ptr(vertex_pos);
			              std::copy(&vertex_pos_ptr[0],
			                        &vertex_pos_ptr[vertex_pos.length()],
			                        &(buffer.data()[pos_ptr.index]));
		              });

		// "rasterize"
		std::for_each(
		   std::execution::seq,
		   buffer.vertex_indices().begin(),
		   buffer.vertex_indices().end(),
		   [&](auto& n) {
			   sm::AttributePtr pos_ptr = buffer.attribute_ptr(0, n);
			   float* pos               = pos_ptr.data;
			   glm::vec2 vertex_pos     = glm::make_vec2(pos);

			   sm::AttributePtr color_ptr = buffer.attribute_ptr(1, n);
			   float* color               = color_ptr.data;
			   glm::vec3 vertex_color     = glm::make_vec3(color);

			   if (n < 2) {

               sm::AttributePtr next_pos_ptr = buffer.attribute_ptr(0, n + 1);
               float* next_pos               = next_pos_ptr.data;
               glm::vec2 next_vertex_pos     = glm::make_vec2(next_pos);

				   auto& start = vertex_pos;
				   auto& end   = next_vertex_pos;

				   for (int x = int(start.x); x <= end.x; ++x) {
					   float t = (x - start.x) / float(end.x - start.x);
					   int y   = int(glm::mix(start.y, end.y, t));

					   pixels[x + y * w] = to_color(glm::vec4(vertex_color, 1.0f));
				   }
			   }
		   });

		SDL_UpdateTexture(tex, NULL, pixels.data(), w * sizeof(std::uint32_t));
		SDL_RenderCopy(renderer, tex, NULL, NULL);
		SDL_RenderPresent(renderer);

		std::fill(pixels.begin(), pixels.end(), to_color(glm::vec4(0.0f)));
	}

	return 0;
}