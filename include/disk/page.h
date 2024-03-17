#include "public.h"
class Page {
   public:
    virtual unsigned int page_size() const = 0;
  
    virtual PAGE_TYPE page_type() const = 0;
    virtual char* get_data() const = 0;
};
class TinyPage:public Page{
    public:
        unsigned int page_size() const {
            return page_size_;
        }
    private:
        unsigned int page_size_ = TINY_PAGE_SIZE;
        PAGE_TYPE type_ = PAGE_TYPE::TINY_PAGE;
        char data[TINY_PAGE_SIZE];
};