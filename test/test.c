#include <ccore/window.h>
#include <ccore/display.h>
#include <ccore/opengl.h>

#include <stdio.h>
#include <gl/GL.h>

#include <lodepng/lodepng.h>
#include <convexHull/convexHull.h>

#define TITLE "Convex Hull test case\n"
#define IMAGEDIR "test/source.png"

unsigned char *pixels;
GLuint texture;

void loadImage()
{
	unsigned int width, height;

	// Load image from file using lodepng
	lodepng_decode32_file(&pixels, &width, &height, IMAGEDIR);
	printf("\tLoaded test image %dx%d\n", width, height);

	// Initialize openGL texture
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);	glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);	glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);	glVertex2f(1.0f, 1.0f);
	glEnd();
}

int run()
{
	// Poll events
	while(ccWindowEventPoll()) {
		ccEvent event = ccWindowEventGet();

		switch(event.type) {
		case CC_EVENT_KEY_DOWN:
			switch(event.keyCode) {
			case CC_KEY_ESCAPE:
				return 0;
				break;
			}
			break;
		case CC_EVENT_WINDOW_QUIT:
			return 0;
			break;
		}
	}

	// Render
	render();
	ccGLBuffersSwap();

	return 1;
}

int main(int argc, char **argv)
{
	printf(TITLE);

	// Fetch displays prior to window creation
	ccDisplayInitialize();

	// Create window
	ccWindowCreate((ccRect){ 0, 0, 512, 512 }, TITLE, CC_WINDOW_FLAG_NORESIZE);
	ccWindowSetCentered();

	// Bind openGL context
	ccGLContextBind();

	// Configure opengl
	glClearColor(0.3f, 0.3f, 0.8f, 1.0f);

	// Initialize
	loadImage();

	while(run());

	return 0;
}