#ifndef INDEX_H
#define INDEX_H

class Index {
    private:
        int index;
    public:
        Index(int initial_index) {
            index = initial_index;
        }
        Index(){
            index = 0;
        }
        int get_index(){return index;}
        void first_increment_index(){++index;}
        void first_subtract_index(){--index;}
        void second_increment_index(){index++;}
        void second_subtract_index(){index--;}
        void reset_index(){index = 0;}
}

#endif
