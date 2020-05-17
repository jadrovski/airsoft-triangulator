#ifndef NEPOPAL_MEM_H_
#define NEPOPAL_MEM_H_

namespace MEM {
    void writeULong(int address, unsigned long value);
    unsigned long readULong(int address);
}

#endif
