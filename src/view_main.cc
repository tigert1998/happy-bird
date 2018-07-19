#include "opengl_view.h"
#include "viewmodel.h"
#include "model.h"

int main(void){
	Model model;
	ViewModel viewmodel(model);
	OpenGLViewConfig config(600, 800);
	OpenGLView view(viewmodel, config);
	view.Run();
}