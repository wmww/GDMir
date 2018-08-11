#ifndef GDMIR_H
#define GDMIR_H

#include "reference.h"

class GDMir : public Reference {
    GDCLASS(GDMir, Reference);

    int count;

protected:
    static void _bind_methods();

public:
    void add(int value);
    void reset();
    int get_total() const;
    void start_mir_server();

    GDMir();
};

#endif // GDMIR_H
