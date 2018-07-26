#include "World.h"

int main(){
    World w;        // construct a default world object
    w.build();
    w.camera_ptr->render_scene(w);
    return 0;
}
