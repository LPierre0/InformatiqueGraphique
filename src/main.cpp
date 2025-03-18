#include "core/Object.h"
#include "core/Application.h"



int main(){
    Application app(1600, 1200, "SHADOW MAPPING");


    app.render_shadow_map();

    return 1;
}

