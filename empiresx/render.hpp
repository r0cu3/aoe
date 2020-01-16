#pragma once

#include <memory>

struct SDL_Window;

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

namespace genie {

class Render {
protected:
	SDL_Window* handle;

	Render(SDL_Window* handle) : handle() {}
public:
	virtual void clear() = 0;
	virtual void paint() = 0;
};

class SimpleRender : public Render {
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> handle;
public:
	SimpleRender(SDL_Window* handle, Uint32 flags, int index = -1);

	SDL_Renderer* canvas() { return handle.get(); }

	void clear() override {
		SDL_RenderClear(handle.get());
	}

	void color(SDL_Color c) {
		SDL_SetRenderDrawColor(handle.get(), c.r, c.g, c.b, c.a);
	}

	void paint() override {
		SDL_RenderPresent(handle.get());
	}
};

class GLRender : public Render {
	SDL_GLContext ctx;
public:
	GLRender(SDL_Window* handle, Uint32 flags);
	~GLRender();

	SDL_GLContext context() { return ctx; }

	void clear() override {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void paint() override {
		SDL_GL_SwapWindow(handle);
	}
};

class Surface final {
	std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> handle;
public:
	Surface(const char* fname);
	Surface(SDL_Surface* handle) : handle(handle, &SDL_FreeSurface) {}

	SDL_Surface* data() { return handle.get(); }
};

class Text;

class Texture final {
	friend Text;

	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> handle;
public:
	int width, height;

	Texture(SimpleRender& r, Surface& s) : handle(NULL, &SDL_DestroyTexture) {
		reset(r, s.data());
	}

	Texture(SimpleRender& r, SDL_Surface* s, bool close = false);
	Texture(int width, int height, SDL_Texture* handle);

	SDL_Texture* data() { return handle.get(); }

	void paint(SimpleRender& r, int x, int y);
private:
	void reset(SimpleRender& r, SDL_Surface* surf);
};

}
