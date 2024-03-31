#include <atomic>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <queue>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <vector>

#include "../public.h"
#include "spdlog/spdlog.h"
namespace fs = std::filesystem;
class PageidDisaptcher {
   public:
    PageidDisaptcher(std::string storage_path) : storage_path_(storage_path) {
        init();
    }
    ~PageidDisaptcher() {
        close(allocated_fd_);
        close(free_fd_);
    }

    auto allocate_id(int num) -> std::vector<page_t> {
        auto free_size = read_header(free_fd_);
        std::vector<page_t> res;
        int bufsize = num * sizeof(page_t);
        std::vector<char> buf(bufsize);
        int start = std::max(free_size - num, 0);
        if (start >= 0) {  // 先从free_id里面分配
            lseek(free_fd_, sizeof(page_header_t) + start * sizeof(page_t),
                  SEEK_SET);
            int n = read(free_fd_, buf.data(), bufsize);
            if (n != bufsize) {
                throw std::runtime_error(
                    "Failed to read the expected number of bytes");
            }
            page_t* p = reinterpret_cast<page_t*>(buf.data());
            for (int i = 0; i < num; ++i) {
                res.emplace_back(p[i]);
            }
            ftruncate(free_fd_, sizeof(page_header_t) + sizeof(page_t) * start);
        }
        // todo 从正常分配器分配

        return res;
    }

    auto return_id(std::vector<page_header_t> ids) -> bool {}

   public:
    auto load_header(int fd) -> page_header_t const {
        page_header_t header_size = sizeof(page_header_t);
        char buf[header_size];
        if (lseek(fd, 0, SEEK_SET) < 0) {
            perror("lseek error");
            exit(1);
        }
        int n = read(fd, buf, header_size);
        if (n < 0) {
            perror("read error"), exit(1);
        }
        if (lseek(fd, 0, SEEK_SET) < 0) {
            perror("lseek error");
            exit(1);
        }
        decltype(header_size) header{};
        if (n != header_size) {
            memcpy(buf, &header, header_size);
            if (write(fd, buf, header_size) < 0) {
                perror("write error");
                exit(0);
            }
        } else {
            memcpy(&header, buf, header_size);
        }
        return header;
    }
    auto init() -> void {
        if (!fs::exists(storage_path_)) {
            try {
                if (fs::create_directories(storage_path_)) {
                    spdlog::info("[{}] Directory created successfully.",
                                 storage_path_);
                }
            } catch (fs::filesystem_error& e) {
                spdlog::error("{}", e.what());
                return;
            }
        }
        fs::path dir = fs::absolute(storage_path_);
        fs::path allocated_file = dir / fs::path(allocated_id_file_);
        fs::path free_file = dir / fs::path(free_id_file_);
        allocated_fd_ =
            open(allocated_file.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        free_fd_ = open(free_file.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (allocated_fd_ < 0 || free_fd_ < 0) {
            perror("allocated_fd_ or free_fd_ open error");
            exit(1);
        }
        cache_allocatedid_num_ = load_header(allocated_fd_);
        cache_freeid_num_ = load_header(free_fd_);
    }

   private:
    auto read_header(int fd) -> page_header_t {
        std::shared_lock<std::shared_mutex> lock(s_page_idMtx_);
        if (fd != allocated_fd_ || fd != free_fd_) {
            return -1;
        }
        page_header_t res =
            (fd == allocated_fd_ ? cache_allocatedid_num_ : cache_freeid_num_);
        lock.unlock();
        return res;
    }
    auto next_page_id() -> page_t {
        page_t t = ++current_id_;

        return t;
    }
    auto write_header(int fdType, std::vector<page_header_t> ids) -> bool {
        std::unique_lock<std::shared_mutex> lock(s_page_idMtx_);
        int* cache_ptr{};
        auto n = read_header(fdType);
        if (n < 0) return false;
        if (fdType == allocated_fd_) {
            cache_ptr = &allocated_fd_;
        } else if (fdType == free_fd_) {
            cache_ptr = &free_fd_;
        } else {
            perror("fdType error");
            return false;
        }
        lseek(fdType, n, SEEK_SET);
        for (const auto id : ids) {
            if (write(fdType, &id, sizeof(id)) < 0) {
                perror("pageid write error");
                return false;
            }
            (*cache_ptr)++;
        }
        return true;
    }

   private:
    page_header_t cache_allocatedid_num_{};
    page_header_t cache_freeid_num_{};
    std::string storage_path_;
    const std::string allocated_id_file_ = "allocted_.id";
    const std::string free_id_file_ = "free_.id";
    int allocated_fd_{};
    int free_fd_{};
    std::shared_mutex s_page_idMtx_{};
    std::mutex operator_mtx_{};
    std::atomic<page_t> current_id_;
};