/* register_types.cpp */

#include "register_types.h"
#include "core/class_db.h"
#include "gdmir.h"

void register_gdmir_types() {
    ClassDB::register_class<GDMir>();
}

void unregister_gdmir_types() {
    //nothing to do here
}
