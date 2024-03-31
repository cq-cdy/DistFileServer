#include <filesystem>

#include "disk/disk_engine.h"
#include "spdlog/spdlog.h"
namespace fs = std::filesystem;
DiskEngine::DiskEngine():storage_path_("./dfs") { init(); }
DiskEngine::DiskEngine(std::string storage_path) : storage_path_(storage_path) {
    init();
}
void DiskEngine::init() {
    fs::path absPath = fs::absolute(this->storage_path_);
    if (!fs::exists(absPath)) {
        try {
            if (fs::create_directories(absPath)) {
                spdlog::info("[{}] Directory created successfully.",absPath);
            } 
        } catch (fs::filesystem_error& e) {
            spdlog::error("{}", e.what());
            return ;
        }
    } 
}