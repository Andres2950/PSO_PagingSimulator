#ifndef PAGE_H
#define PAGE_H

#include <bits/types/idtype_t.h>
#include <iostream>
#include <string.h>

class Page {
    public:
        int id;
        int m_addr;
        int l_addr;
        int d_addr;
        int is_loaded;
        int timestamp;
        int load_t;
        int mark;
        static inline int id_counter = 1;

    Page(int m_addr, int l_addr, int d_addr){
        this->m_addr = m_addr;
        this->l_addr = l_addr;
        this->d_addr = d_addr;
        mark = 0;
        //id = id_counter++;
    }
    Page(int id) {
        this->id = id;
        this->m_addr = -1;
        this->l_addr = -1;
        this->d_addr = -1;
        mark = 0;
    }
    Page(){
        this->m_addr = -1;
        this->l_addr = -1;
        this->d_addr = -1;
        this->id = -1; 
        mark = 0;
        //id = id_counter++;
    }

    bool operator==(const Page& otro) const {
        return id == otro.id;
    }
    friend std::ostream& operator<<(std::ostream& os, const Page& page) {
        os << "Page(" << page.id << ", mark=" << page.mark << ")";
        return os;
    }
};

#endif
