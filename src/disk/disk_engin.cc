#include "disk/disk_engine.h"
DiskEngine::DiskEngine(){
    init();
}
DiskEngine::DiskEngine(std::string storage_path)
    : storage_path_(storage_path) {
        init();
    }
void DiskEngine::init() {

}