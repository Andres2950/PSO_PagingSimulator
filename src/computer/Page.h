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
        int load_t;
        int mark;

    Page(int id, int m_addr, int l_addr, int d_addr){
        this->m_addr = m_addr;
        this->l_addr = l_addr;
        this->d_addr = d_addr;
        this->id = id; 
    }
    Page(){
        this->m_addr = -1;
        this->l_addr = -1;
        this->d_addr = -1;
        this->id = -1; 
    }

    bool operator==(const Page& otro) const {
        return id == otro.id;
    }
    friend std::ostream& operator<<(std::ostream& os, const Page& page) {
        os << "Page(" << page.id << ")";
        return os;
    }
};

#endif
