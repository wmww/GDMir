#include "gdmir.h"
#include <thread>

int mir_server_main(int argc, char const* argv[]);

void GDMir::add(int value) {

    count += value;
}

void GDMir::reset() {

    count = 0;
}

int GDMir::get_total() const {

    return count;
}

void GDMir::start_mir_server() {
    new std::thread{[](){
        mir_server_main(0, nullptr);
    }};
}

void GDMir::_bind_methods() {

    ClassDB::bind_method(D_METHOD("add", "value"), &GDMir::add);
    ClassDB::bind_method(D_METHOD("reset"), &GDMir::reset);
    ClassDB::bind_method(D_METHOD("get_total"), &GDMir::get_total);
    ClassDB::bind_method(D_METHOD("start_mir_server"), &GDMir::start_mir_server);
}

GDMir::GDMir() {
    count = 0;
}

