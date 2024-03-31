#include "public.h"
class Page {
   public:
    virtual unsigned int page_size() const = 0;
    virtual unsigned int data_size() const = 0;

    virtual PAGE_TYPE page_type() const = 0;
    virtual char* get_data() const = 0;
    page_t page_id{-1};
};

class TinyPage : public Page {
   public:
    unsigned int page_size() const override { return page_size_; }
    PAGE_TYPE page_type() const override { return type_; }
    unsigned int data_size() const override { return *(unsigned int*)data; }

   private:
    unsigned int page_size_ = TINY_PAGE_SIZE;
    PAGE_TYPE type_ = PAGE_TYPE::TINY_PAGE;
    char data[TINY_PAGE_SIZE + sizeof(unsigned int)];
};

class SmallPage : public Page {
   public:
    unsigned int page_size() const override { return page_size_; }
    PAGE_TYPE page_type() const override { return type_; }
    unsigned int data_size() const override { return *(page_header_t*)data; }
    
   public:
    char data[SMALL_PAGE_SIZE + sizeof(page_header_t)];

   private:
    unsigned int page_size_ = SMALL_PAGE_SIZE;
    PAGE_TYPE type_ = PAGE_TYPE::SMALL_PAGE;
};

class MidPage : public Page {
   public:
    unsigned int page_size() const override { return page_size_; }
    PAGE_TYPE page_type() const override { return type_; }
    unsigned int data_size() const override { return *(page_header_t*)data; }

   public:
    char data[MID_PAGE_SIZE + sizeof(page_header_t)];

   private:
    unsigned int page_size_ = MID_PAGE_SIZE;
    PAGE_TYPE type_ = PAGE_TYPE::MID_PAGE;
};

class LargePage : public Page {
   public:
    unsigned int page_size() const override { return page_size_; }
    PAGE_TYPE page_type() const override { return type_; }
    unsigned int data_size() const override { return *(page_header_t*)data; }

   public:
    char data[LARGE_PAGE_SIZE + sizeof(page_header_t)];

   private:
    unsigned int page_size_ = LARGE_PAGE_SIZE;
    PAGE_TYPE type_ = PAGE_TYPE::LARGE_PAGE;
};