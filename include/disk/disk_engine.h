#include <page.h>

#include <string>
class DiskEngine {
   public:
    DiskEngine();
    DiskEngine(std::string storage_path);

    Page* fetch();
    void flush();

   private:
    void init();
    std::string storage_path_;
};