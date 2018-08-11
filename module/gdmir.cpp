#include "gdmir.h"

void GDMir::add(int value) {

    count += value;
}

void GDMir::reset() {

    count = 0;
}

int GDMir::get_total() const {

    return count;
}

void GDMir::_bind_methods() {

    ClassDB::bind_method(D_METHOD("add", "value"), &GDMir::add);
    ClassDB::bind_method(D_METHOD("reset"), &GDMir::reset);
    ClassDB::bind_method(D_METHOD("get_total"), &GDMir::get_total);
}

GDMir::GDMir() {
    count = 0;
}

