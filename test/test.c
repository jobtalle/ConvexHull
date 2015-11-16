#include <ccore/window.h>
#include <ccore/display.h>
#include <ccore/opengl.h>

#include <stdio.h>

#include <convexHull/convexHull.h>

#define TITLE "Convext Hull test case\n"

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

	while(run());

	return 0;
}