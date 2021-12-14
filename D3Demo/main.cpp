#include "../D3.h"
#define ALL_IMPL
#include "../smartfs.hpp"

#define MAIN_BE int main(int argc, char** argv) {
#define MAIN_EN return 0;}

MAIN_BE
{
    three3t model = D3::load3t(smartfs::find("cube.obj"));
    model.prepare();
}
MAIN_EN
