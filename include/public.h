const static unsigned int TINY_FILE_SIZE = 1 << 12;   // 4KB
const static unsigned int SMALL_FILE_SIZE = 1 << 22;  // 4MB
const static unsigned int MID_FILE_SIZE = 1 << 27;    // 128MB

const static unsigned int TINY_PAGE_SIZE = 1 << 12;     // 4 KB
const static unsigned int SMALL_PAGE_SIZE = 1 << 14;    // 16 KB
const static unsigned int MID_PAGE_SIZE = 1 << 17;      // 128 KB
const static unsigned int LARGE_PAGE_SIZE = 1 << 19;    // 512 KB
enum class FILE_TYPE {TINY_FILE = 0,SMALL_FILE,MID_FILE,LARGE_FILE};
enum class PAGE_TYPE {TINY_PAGE = 10,SMALL_PAGE,MID_PAGE,LARGE_PAGE};
